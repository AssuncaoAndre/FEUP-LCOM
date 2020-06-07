#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"
//#include "timer.c"
extern uint32_t cont;


int main(int argc, char *argv[]) {
  
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  /* To be implemented by the students */
  uint8_t st;
  if(timer_get_conf(timer,&st)==1) return 1;
  if(timer_display_conf(timer,st,field)==1) return 1;

  return 0;
 // printf("%s is not yet implemented!\n", __func__);

}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  if(freq<MIN_FREQ || freq>TIMER_FREQ)
  {
    printf("OUT OF RANGE FREQUENCY\n");
    return 1;
  }
  return(timer_set_frequency(timer,freq));
  //printf("%s is not yet implemented!\n", __func__);

}

int(timer_test_int)(uint8_t time) {
  /* To be implemented by the students */

  if(time<0) 
  {
    printf("INVALID TIME!");
  return 1;
  }

  int ipc_status;
  message msg;
  uint8_t r;
  uint8_t bit_no = 0;
  timer_subscribe_int(&bit_no);
  while( (cont/DEFAULT_FREQ)<time )
 { /* You may want to use a different condition */
     /* Get a request message. */
     // printf("1\n");
     //printf("cont:%d, time:%d\n\n",cont/60,time);
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) 
     { 
        printf("driver_receive failed with: %d", r);
       continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts  & BIT(0)) { /* subscribed interrupt */
                   timer_int_handler();
                   if(cont%DEFAULT_FREQ==0)
                   timer_print_elapsed_time();
                   //printf("cont:%d\n",cont);
               }
                //printf("msg:%x\n",msg.m_notify.interrupts);
                break;
            default:
                //printf("2\n");
                break; /* no other notifications expected: do nothing */	
        }
    } else {
     // printf("3\n"); /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
 }
 if(timer_unsubscribe_int()!=0) return 1;


  //printf("%s is not yet implemented!\n", __func__);

  return 0;
}
//dois primeiros bits sao 1, (TIMER_RB_CNTRL|BIT(5))
//SYS_OUTB para controlo->0x43, Config timer 0, dizer que queremos escrever para o controlo, e depois ler a parte corresponder
//g9
