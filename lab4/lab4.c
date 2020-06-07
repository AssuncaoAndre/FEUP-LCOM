// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "i8042.h"
#include "i8254.h"
#include "mouse.h"

int mouse_hook_id;
struct packet p_packet;
uint32_t byte;
unsigned int timer_counter = 0;

extern bool kbc_ih_failed;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  /* To be completed */
  int ipc_status;
  message msg;
  uint8_t bit_no;
  uint32_t r, irq_set = BIT(IRQ_MOUSE), cont = 0;
  /* static */ uint16_t counter = 0;
  struct packet read;

/*   if (mouse_enable_data_rep() != OK) {
    return 1;
  } */
  kbc_write(0xEA);
  kbc_write(0xF4);

  if (subscribe_mouse_int(&bit_no) != OK) { // 1
    printf("subscribe_mouse_int failed");
    return 1;
  }

  while (cont < cnt) { /* You may want to use a different condition */
    /* Get a request message. */
    // tickdelay(micros_to_ticks(DELAY_US));

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */

      switch (_ENDPOINT_P(msg.m_source)) {

        case HARDWARE: /* hardware interrupt notification */

          if (msg.m_notify.interrupts & irq_set) {

            mouse_ih();

            if (counter == 0) {
              read.bytes[0] = byte;
              counter++;
            }

            else if (counter == 1) {
              read.bytes[1] = byte;
              counter++;
            }

            else if (counter == 2) {
              read.bytes[2] = byte;
              packet_processing(&read);
              mouse_print_packet(&read);
              counter = 0;
              cont++;
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else {
      /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (unsubscribe_mouse_int() != OK) {
    printf("unsbscribe_mouse_int failed");
    return 1;
  }
/* 
  if (mouse_disable_data_reporting() != OK) {
    return 1;
  } */

  kbc_write(0xF5);
   kbc_write(0xF6);


  return 0;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  /* To be completed */

  int cont = 0;
  uint16_t counter = 0;
  struct packet read;

  if (kbc_write(DISABLE_DR) != OK) {
    return 1;
  }

  if (kbc_write(SET_REMOTE_MODE) != OK) {
    return 1;
  }

  if (period < 0) {
    printf("period must be greater than 0\n");
    return -1;
  }

  while (cnt > 0) {

    if (kbc_write(READ_DATA) != OK)
      return 1;
      cont=0;
    while (cont < 3) {

      mouse_ih();

      if (counter == 0) {
        read.bytes[0] = byte;
        counter++;
        cont++;
        continue;
      }

      else if (counter == 1) {
        read.bytes[1] = byte;
        counter++;
        cont++;
        continue;
      }

      else if (counter == 2) {
        read.bytes[2] = byte;
        cont++;
        counter = 0;
      }

      else if (counter > 2)
        return -1;
    }

    packet_processing(&read);
    mouse_print_packet(&read);
    tickdelay(micros_to_ticks(period * 1000));
    cnt--;
  }

  if (sys_outb(STAT_REG, MOUSE_W_CMD) != OK)
    return 1;

  if (sys_outb(I_O_BUFFER, minix_get_dflt_kbc_cmd_byte()) != OK) {
    return 1;
  }

  if (kbc_write(SET_STREAM_MODE) != OK) {
    return 1;
  }
  if (kbc_write(SETDEFAULT) != OK) {
    return 1;
  }

  return 0;
}

void(timer_int_handler)() {}

int(mouse_test_async)(uint8_t idle_time) {
  /* To be completed */
  uint8_t bit_no_timer = 0;
  int hook_id_timer;
  hook_id_timer = bit_no_timer;
  int ipc_status;
  message msg;
  uint8_t bit_no_mouse = 12;
  uint32_t r, irq_set = BIT(IRQ_MOUSE), cont = 0;
  /* static */ uint16_t counter = 0;
  struct packet read;

 /*  if (mouse_enable_data_reporting() != OK) { // 3  //tentar não usar esta função!!!!!!
    printf("mouse_enable failed");
    return 1;
  } */
  kbc_write(0xEA);
  kbc_write(0xF4);
  if (subscribe_mouse_int(&bit_no_mouse) != OK) { // 1
    printf("subscribe_mouse_int failed");
    return 1;
  }

  if (sys_irqsetpolicy(IRQ_TIMER, IRQ_REENABLE, &hook_id_timer) != 0)
    return 1;

  while ((cont / DEFAULT_FREQ) < idle_time) { /* You may want to use a different condition */
    /* Get a request message. */

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */

          if (msg.m_notify.interrupts & BIT(0)) { /* subscribed interrupt */
            timer_int_handler();

            cont++;

            if (cont % DEFAULT_FREQ == 0)
              timer_print_elapsed_time();
          }

          if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();
            if (counter == 0) {
              read.bytes[0] = byte;
              counter++;
            }

            else if (counter == 1) {
              read.bytes[1] = byte;
              counter++;
            }

            else if (counter == 2) {
              read.bytes[2] = byte;
              packet_processing(&read);
              mouse_print_packet(&read);
              counter = 0;
            }

            cont = 0;
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else {
      //* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (sys_irqrmpolicy(&hook_id_timer) != 0)
    return 1;

  if (unsubscribe_mouse_int() != OK) {
    printf("unsbscribe_mouse_int failed");
    return 1;
  }

  if (mouse_disable_data_reporting() != OK) {
    return 1;
  }
  return 0;
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  /* To be completed */

  uint8_t irq_set;
  int ipc_status, r;
  message msg;
  struct packet read,prev;
  static uint16_t counter = 0;
  int flag=0, y1=0,y2=0;
  if (x_len < 0 || tolerance < 0) {
    return 1;
  }

/*   if (mouse_enable_data_reporting() != OK) { // 3  //tentar não usar esta função!!!!!!
    printf("mouse_enable failed");
    return 1;
  } */
 kbc_write(0xEA);
  kbc_write(0xF4);
  if (subscribe_mouse_int(&irq_set) != OK) {
    return 1;
  }

  while (byte != END) { 
    
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                  
          if (msg.m_notify.interrupts & BIT(irq_set)) { 

            mouse_ih();

            if (counter == 0) {
              if (status & BIT(3)) {
                prev.bytes[0]=read.bytes[0];
                read.bytes[0] = byte;
                counter++;
              }
              continue;
            }

            else if (counter == 1) {
              prev.bytes[1]=read.bytes[1];
              read.bytes[1] = byte;
              counter++;
              continue;
            }

            else if (counter == 2) {
              prev.bytes[2]=read.bytes[2];
              read.bytes[2] = byte;
              packet_processing(&prev);
              packet_processing(&read);
              mouse_print_packet(&read);
              if(read.lb && !read.mb && !read.rb && flag==0 && read.delta_y<0)
              {
                y1=y1-read.delta_y;
                flag=1;
              }
              
              if (flag==1 && read.lb && !read.mb && !read.rb && read.delta_y)
              {
                y1=y1-read.delta_y;
                if (!read.delta_x>(-tolerance+x_len))
                  return 1;
                
              }

              if(flag==1 && !read.lb && !read.mb && read.rb && read.delta_y>0)
              {
                y2=y2+read.delta_y;
                if( !read.delta_x>(-tolerance+x_len))
                  return 1;
              }
              if(y2>=y1 && y1!=0)
              break;
              
              //////////////////////
             // check_i_v(*mouse_detect_event(&read), x_len, tolerance);
              counter = 0;
            }

            else if (counter > 2)
              return -1;
          }
          break;
        default:
          break; 
      }
    }
    else { 
           
    }
  }

  if (unsubscribe_mouse_int() != OK) {
    return 1;
  }

  if (mouse_disable_data_reporting() != OK) {
    return 1;
  }

  return 0;
}
