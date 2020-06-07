#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t cont=0;
int hook_id;


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  uint8_t conf;
  timer_get_conf(timer,&conf);
  uint32_t tudo;
  uint8_t msb,lsb;
  tudo=(TIMER_FREQ/freq);
  util_get_MSB(tudo,&msb);
  util_get_LSB(tudo,&lsb);
  
  //printf("%x\n%x\n%x\n%d\n",tudo,msb,lsb,timer);


  if(timer==0)
  {
    conf=(conf&0xF);
    conf = (conf  | TIMER_SEL0 | TIMER_LSB_MSB);
  if(sys_outb(TIMER_CTRL, conf)!=0) return 1;
  if(sys_outb(TIMER_0, lsb)!=0) return 1;
  if(sys_outb(TIMER_0, msb)!=0) return 1;
  }

  if(timer==1)
  {
    conf=(conf&0xF);
    conf = (conf | TIMER_SEL1 | TIMER_LSB_MSB);
  if(sys_outb(TIMER_CTRL, conf)!=0) return 1;
  if(sys_outb(TIMER_1, lsb)!=0) return 1;
  if(sys_outb(TIMER_1, msb)!=0) return 1;
  }

  if(timer==2)
  {
    conf=(conf&0xF);
    conf = (conf | TIMER_SEL2 | TIMER_LSB_MSB);
  if(sys_outb(TIMER_CTRL, conf)!=0) return 1;
  if(sys_outb(TIMER_2, lsb)!=0) return 1;
  if(sys_outb(TIMER_2, msb)!=0) return 1;
  }
  return 0;

}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
 /*  int policy;
  sys_irqrmpolicy(&policy); */
  hook_id=*bit_no;
  if(sys_irqsetpolicy(IRQ_TIMER,IRQ_REENABLE,&hook_id)!=0) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
// printf("%s is not yet implemented!\n", __func__);

  if(sys_irqrmpolicy(&hook_id)!=0) return 1;

  return 0;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  cont=cont+1;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */

  uint8_t rbc = (TIMER_RB_CMD|TIMER_RB_SEL(4)|TIMER_RB_SEL(timer));
  if(sys_outb(TIMER_CTRL,rbc)!=0) return 1;

  
  if(timer==0)
  return (util_sys_inb(TIMER_0,st));
  if(timer==1)
  return (util_sys_inb(TIMER_1,st));
  if(timer==2)
  return (util_sys_inb(TIMER_2,st));

  return 1;

  //printf("%s is not yet implemented!\n", __func__);

  
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  union timer_status_field_val val;
  

  if(field==tsf_all)
  {
    val.byte=st; 
    
  }
 if(field==tsf_initial)
  {
    //printf("%d\n",tsf_initial);

  if(((st&0x30)>>4)==0)
  val.in_mode=INVAL_val;

  if(((st&0x30)>>4)==1)
  val.in_mode=LSB_only;

  if(((st&0x30)>>4)==2)
  val.in_mode=MSB_only;

  if(((st&0x30)>>4)==3)
  val.in_mode=MSB_after_LSB;
  }
  

  if(field==tsf_mode)
  {
    //printf("%x\n",st);
    val.count_mode=((st&0xE)>>1);

    /* printf("%x\n",val.count_mode);
    val.count_mode=(val.count_mode>>1);
    printf("%x\n",val.count_mode); */
  //val.count_mode=(st&0x1C)/4;

 if (val.count_mode==6)
 val.count_mode=2;

 if (val.count_mode==7)
 val.count_mode=3;


  }

  if(field==tsf_base ) //BCD
  {
    //printf("%d\n",tsf_base);
    if((st&BIT(0))==1)
    val.bcd=true;

    else val.bcd=false;
  }


 return( timer_print_config(timer, field, val));
  //printf("%s is not yet implemented!\n", __func__);

  //return 0;
}
