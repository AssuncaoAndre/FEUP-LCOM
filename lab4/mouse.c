#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "i8254.h"
#include "mouse.h"

int mouse_hook_id;
extern uint32_t byte;
bool kbc_ih_failed = false, end = false;
unsigned int mousebyte = 0;
static uint32_t arr[3];
uint32_t status;
struct mouse_ev gesture;
state st = INIT;
static int x = 0, y = 0;

int(subscribe_mouse_int)(uint8_t *bit_no) {

  mouse_hook_id = IRQ_MOUSE;
  *bit_no = mouse_hook_id;

  if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) {
    printf("subscribe_mouse_int failed");
    return 1;
  }
  return 0;
}

int unsubscribe_mouse_int() {

  if (sys_irqdisable(&mouse_hook_id) != OK) {
    printf("ERRO , sys_irqdisable falhou\n");
    return -1;
  }
  if (sys_irqrmpolicy(&mouse_hook_id) != OK) {
    printf("ERRO , sys_irqrmpolicy falhou\n");
    return -1;
  }
  return 0;
}

int(mouse_enable)() {

  if (kbc_write(ENABLE_DR) != 0) {
    printf("failed to enable mouse");
    return 1;
  }

  return 0;
}

int(mouse_disable)() {

  if (kbc_write(DISABLE_DR) != 0) {
    printf("failed to disable mouse");
    return 1;
  }

  return 0;
}

int mouse_disable_data_reporting() {
  (mouse_disable)();

  if (kbc_write(SETDEFAULT) != 0) {
    return 1;
  }

  (mouse_enable)();
  return 0;
}

int mouse_enable_data_rep() {
  (mouse_disable)();

  if (kbc_write(SET_STREAM_MODE) != 0) {
    return 1;
  }

  (mouse_enable)();

  return 0;
}

void(mouse_ih)(void) {

  if (sys_inb(I_O_BUFFER, &byte) != OK) {
    kbc_ih_failed = true;
    /* printf("ERROU\n"); */
    return;
  }

  if (byte & O_BUFFER_FULL) {
    if (byte & AUX) {

      arr[mousebyte] = byte;
      mousebyte++;
      if (mousebyte == 3)
        mousebyte = 0;
      if (((A2 & arr[0])) == 0) {
        kbc_ih_failed = true;
        return;
      }

      kbc_ih_failed = false;
    }
  }

  return;
}

int(kbc_write)(uint32_t cmd) {

  uint32_t stat = 0, check;
  int tries = 3;

  while (tries) {

    sys_outb(STAT_REG, INIT_CMD);

    if (sys_inb(STAT_REG, &stat) != 0) {
      tries--;
      continue;
    }

    if ((stat & IBF) == 0)
      sys_outb(I_O_BUFFER, cmd);

    sys_inb(I_O_BUFFER, &check);

    if (check == NACK) {
      tries--;
      continue;
    }

    else if (check == ERROR) {
      printf("ERRO\n");
      return 1;
    }

    else
      return 0;
  }

  return 1;
}

void packet_processing(struct packet *packet) {

  if (packet->bytes[0] & L_BUTTON)
    packet->lb = true;
  else
    packet->lb = false;

  if (packet->bytes[0] & M_BUTTON)
    packet->mb = true;
  else
    packet->mb = false;

  if (packet->bytes[0] & R_BUTTON)
    packet->rb = true;
  else
    packet->rb = false;

  if (packet->bytes[0] & X_OVFL)
    packet->x_ov = true;
  else
    packet->x_ov = false;

  packet->delta_x =
    (-BIT(8) * ((packet->bytes[0] & 0x10) / 16) + (packet->bytes[1] & 0xFF));

  if (packet->bytes[0] & Y_OVFL)
    packet->y_ov = true;
  else
    packet->y_ov = false;
  packet->delta_y = (-BIT(8) * ((packet->bytes[0] & 0x20) / 32) + (packet->bytes[2] & 0xFF));
}

void check_i_v(struct mouse_ev gesture, uint8_t x_len, uint8_t tolerance) {

  if (st == INIT) {
    if (gesture.type == LB_PRESSED) {
      //   gesture.delta_x = 0;
      //   gesture.delta_y = 0;
      st = UP_RIGHT;
    }
    else if (gesture.type == LB_RELEASED)
      st = INIT;
    else
      st = INIT;
  }

  else if (st == UP_RIGHT) {
    if (gesture.type == MOUSE_MOV) {
      st = UP_RIGHT;
      x += gesture.delta_x;
      y += gesture.delta_y;
    }
    else if (gesture.type == LB_RELEASED && gesture.delta_x >= x_len)
      st = HOLD;
    else if (gesture.type == RB_PRESSED || (gesture.delta_x < -tolerance) || (gesture.delta_y < -tolerance)) {
      st = INIT;
      x = 0;
      y = 0;
    }
  }

  else if (st == HOLD) {
    if (gesture.type == RB_PRESSED) {
      st = DOWN_RIGHT;
      //   gesture.delta_x = 0;
      //   gesture.delta_y = 0;
    }
    else if (gesture.type == MOUSE_MOV)
      st = HOLD;
    else if (gesture.type < x_len) {
      st = INIT;
      x = 0;
      y = 0;
    }
    else if (gesture.type == LB_PRESSED) {
      st = UP_RIGHT;
      x = 0;
      y = 0;
    }
    else if ((abs(gesture.delta_x) > tolerance) || (abs(gesture.delta_y) > tolerance)) {
      st = INIT;
      x = 0;
      y = 0;
    }
    else
      st = INIT;
  }

  else if (st == DOWN_RIGHT) {
    if (gesture.type == MOUSE_MOV) {
      st = DOWN_RIGHT;
      x += gesture.delta_x;
      y += gesture.delta_y;
    }
    else if (gesture.type == LB_PRESSED || (gesture.delta_x < -tolerance) || (gesture.delta_y > tolerance)) {
      st = UP_RIGHT;
      x = 0;
      y = 0;
    }
    else if (gesture.type == RB_RELEASED && gesture.delta_x >= x_len)
      st = END;
    else
      st = INIT;
  }

  else if (st == END) {
    if (x < x_len || abs(y) <= x) {
      st = INIT;
      x = 0;
      y = 0;
    }
    else
      st = INIT;
  }
}
