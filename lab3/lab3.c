#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include "i8254.h"
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"
//#include <timer.c>


/* #define IRQ_KB 1
#define DELAY_US 20000
#define EXIT_CODE 0x81
#define I_O_BUFFER 0x60 */

uint32_t cnt=0;
 uint32_t scancode;
int flag=0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}



int(kbd_test_scan)() {
  /* To be completed by the students */
  int ipc_status;
  uint8_t r, bit_no = 0;
  message msg;
  int hook_id = bit_no;
/*   uint8_t scan_arr[2] = {0, 0};

  int flag = 0; */

  printf("%d\n",
         sys_irqsetpolicy(IRQ_KB, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id));
  /* sys_outb(0x64, 0x20);
  sys_inb(0x64, &scancode);
  printf("%x\n", scancode);
  if (scancode & 0x10) {
    scancode = (scancode & 0xEF);
  }

  printf("%x\n", scancode);
  sys_outb(0x64, 0x60);
  sys_outb(0x64, scancode); */

  printf("%x\n", scancode);
  //kbc_ih();
  printf("\n");
  while (scancode !=
         EXIT_CODE)
  { /* You may want to use a different condition */
    /* Get a request message. */
    // printf("1\n");
    // printf("cont:%d, time:%d\n\n",cont/60,time);
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & BIT(0)) { /* subscribed interrupt */

          kbc_ih();
          }

        
        // printf("msg:%x\n",msg.m_notify.interrupts);
        break;
      default:
        // printf("2\n");
        break; /* no other notifications expected: do nothing */
      }
    } else {
      // printf("3\n"); /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  kbd_print_no_sysinb(cnt);
  if (sys_irqrmpolicy(&hook_id) != 0)
    return 1;

  return 0;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  uint32_t key=0;
  uint8_t scan_arr[2] = {0, 0};
  //int flag = 0;
  //uint32_t cnt=0;

  while(scancode != EXIT_CODE)
  {
    tickdelay(micros_to_ticks(DELAY_US));
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
            kbd_print_scancode(!(scancode & PAR_ERR), 1, scan_arr);
          }

          else if (scancode == TWO_BYTES) {
            scan_arr[0] = scancode;
            flag = 1;
          }

          else if (flag == 1) {
            scan_arr[1] = scancode;
            kbd_print_scancode(!(scancode & PAR_ERR), 2, scan_arr);
            flag = 0;
          }
        }
  }
  sys_outb(IN_BUF, READ_CMD);        
  sys_outb(IN_BUF, I_O_BUFFER);

  #ifdef LAB3 
    cnt++; 
  #endif

  sys_inb(I_O_BUFFER,&key); 

  sys_outb(IN_BUF,I_O_BUFFER);        //enable interrupts again
  sys_outb(I_O_BUFFER,  key |  BIT(0));  

  kbd_print_no_sysinb(cnt);
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
/*   uint8_t scan_arr[2] = {0, 0};
  int flag=0; */
   int ipc_status;
  message msg;
  uint8_t r;
  uint8_t bit_no_timer = 0, bit_no_kbd = 1;
  int hook_id_kbd=bit_no_kbd, hook_id_timer=bit_no_timer;
  int cont=0;//cnt=0;
  
  sys_irqsetpolicy(IRQ_TIMER,IRQ_REENABLE,&hook_id_timer);
  sys_irqsetpolicy(IRQ_KB, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd);
  kbc_ih();
  while( (cont/DEFAULT_FREQ)<n && scancode !=EXIT_CODE) //se não atingir o tempo de espera nem a tecla for ESC
 { 
   /* You may want to use a different condition */
     /* Get a request message. */
     // printf("1\n");
     //printf("cont:%d, time:%d\n\n",cont/60,time);
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) 
     { 
        printf("driver_receive failed with: %d", r);
       continue;
    }
    if (is_ipc_notify(ipc_status)) { 
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 				
                if (msg.m_notify.interrupts  & BIT(bit_no_timer)) { 
                   cont++;
                   if(cont%DEFAULT_FREQ==0)
                   timer_print_elapsed_time();
                   //printf("cont:%d\n",cont);
               }
               if (msg.m_notify.interrupts  & BIT(bit_no_kbd)) 
               { 
                    
                  kbc_ih();
                   cont=0;
               }
                //printf("msg:%x\n",msg.m_notify.interrupts);
                break;
            default:
                //printf("2\n");
                break; /* no other notifications expected: do nothing */	
        }
    } 
     // printf("3\n"); /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    
 
 }
  kbd_print_no_sysinb(cnt);
  
  if(sys_irqrmpolicy(&hook_id_timer)!=0) return 1;
  if(sys_irqrmpolicy(&hook_id_kbd)!=0) return 1;
  
    
    return 0;


  //printf("%s is not yet implemented!\n", __func__);

 
}
