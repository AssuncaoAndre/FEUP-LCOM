#include "keyboard.h"

#include <lcom/lcf.h>

#include <minix/sysutil.h>
#include <stdint.h>

#include "i8042.h"
#include "i8254.h"

#include <lcom/lab3.h>
#include <stdbool.h>

extern uint32_t scancode;
static int hook_id = 1;
extern uint32_t cnt;
  extern int flag;

void(kbc_ih)() {
  uint8_t scan_arr[2] = {0, 0};
 
#ifdef LAB3
    cnt++;
#endif
    sys_inb(IN_BUF, &scancode);
    if (scancode & O_BUFFER_FULL) {
#ifdef LAB3
      cnt++;
#endif
      sys_inb(I_O_BUFFER, &scancode);
              if (scancode != TWO_BYTES && flag == 0) {
                scan_arr[0] = scancode;
               // kbd_print_scancode(!(scancode & PAR_ERR), 1, scan_arr);
              }

              else if (scancode == TWO_BYTES) {
                scan_arr[0] = scancode;
                flag = 1;
              }

              else if (flag == 1) {
                scan_arr[1] = scancode;
               // kbd_print_scancode(!(scancode & PAR_ERR), 2, scan_arr);
                flag = 0;
              }
    }
  
}

int kbd_subscribe_int(uint8_t *bit_no) {
  *bit_no = hook_id;

  if (sys_irqsetpolicy(IRQ_KB, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id) != OK) {
    printf("ERROR kbd subscribe failed");
    return 1;
  }

  return 0;
}

int kbd_unsubscribe_int() {
  if (sys_irqrmpolicy(&hook_id) != OK) {
    printf("ERROR kbd unsubscribe failed");
    return 1;
  }

  return 0;
}
