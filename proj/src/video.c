#include "math.h"
#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "i8254.h"
#include "video.h"
#include "mouse.h"

#include <machine/int86.h>

#include "small/a.h"
#include "small/b.h"
#include "small/c.h"
#include "small/d.h"
#include "small/e.h"
#include "small/f.h"
#include "small/g.h"
#include "small/h.h"
#include "small/i.h"
#include "small/j.h"
#include "small/k.h"
#include "small/l.h"
#include "small/m.h"
#include "small/n.h"
#include "small/o.h"
#include "small/p.h"
#include "small/q.h"
#include "small/r.h"
#include "small/s.h"
#include "small/t.h"
#include "small/u.h"
#include "small/v.h"
#include "small/w.h"
#include "small/x.h"
#include "small/y.h"
#include "small/z.h"
#include "small/-.h"
#include "small/~.h"
#include "small/doispontos.h"
#include "small/exclamacao.h"
#include "small/apos.h"

#include "small/1forca.h"
#include "small/2head.h"
#include "small/3body.h"
#include "small/4firstarm.h"
#include "small/5secondarm.h"
#include "small/6firstleg.h"
#include "small/7gameover.h"

#include "small/0.h"
#include "small/1.h"
#include "small/2.h"
#include "small/3.h"
#include "small/4.h"
#include "small/5.h"
#include "small/6.h"
#include "small/7.h"
#include "small/8.h"
#include "small/9.h"


#define IRQ_MOUSE 12

extern uint32_t background;
extern uint32_t scancode;
extern uint32_t byte;
extern uint8_t *video_mem;
extern uint8_t *aux;
extern vbe_mode_info_t data;
extern int counter;
extern int clicka;


int get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p) {
  mmap_t map;
  reg86_t r;
  phys_bytes buf;

  if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) //alloc memory needed
  {
    printf("Erro em lm_alloc\n"); //reboot minix if there is problem or do retrys but may not solve
    return -1;
  }

  memset(&r, 0, sizeof(reg86_t));

  buf = map.phys;
  r.ax = 0x4F01;       // VBE get mode info (0x4F01)
  r.es = PB2BASE(buf); // PB2BASE Is a macro for computing the base of a segment, a 16-bit value, given a 32-bit linear address;
  r.di = PB2OFF(buf);  // PB2OFF Is a macro for computing the offset with respect to the base of a segment, a 16-bit value, given a 32-bit linear address;
  r.cx = mode;
  r.intno = 0x10;

  if (sys_int86(&r) != OK) { // call BIOS
    lm_free(&map);           //free memory
    return -1;
  }

  if (r.ah == 01 || r.ah == 02 || r.ah == 03) { // (return status)
    lm_free(&map);                              //free memory
    return -1;
  }

  memcpy(vmi_p, map.virt, map.size);
  lm_free(&map); //free memory
  return 0;
}

int(mouse_int)(/* uint8_t *flag, */ int16_t *x, int16_t *y, int *click) {
  /* To be implemented by the students */
  //uint32_t scancode = 0;
  struct packet read;

  //int cont = 0;
  int ipc_status;
  message msg;
  uint8_t r;


 // while (cont < time) { /* You may want to use a different condition */
    /* Get a request message. */

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      return 0;
     // continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
        
           if (msg.m_notify.interrupts & BIT(IRQ_MOUSE)) {
            
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
              *x=*x+read.delta_x;
              *y=*y-read.delta_y;
/*               if(read.mb)
              *click=3;
              
              else if(read.rb)
              *click=2;

              else */ if(read.lb)
              {

                if(clicka==0)
                *click=1;
                else 
                *click=0;
                clicka=1;
              }
                else 
                {
                  clicka=0;
                  *click=0;
                }
              mouse_print_packet(&read);
              counter = 0;
            }
          }

          else if (msg.m_notify.interrupts & BIT(1))
          {
                sys_inb(IN_BUF, &scancode);
                 if (scancode & O_BUFFER_FULL) 
                    sys_inb(I_O_BUFFER, &scancode);
          }
          

          

        /*   if (msg.m_notify.interrupts & BIT(0)) {

            cont++;
          } */

          

          

          return 0;
        default:
          return 0; /* no other notifications expected: do nothing */
      }
    }
    else {
      /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
 // }

  return 0;
}



int video_init(uint16_t mode) {
  struct minix_mem_range m; /*physical memory range*/
  int t;
  m.mr_base = 0;
  m.mr_limit = 1024 * 1024;
  if (OK != (t = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &m)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", t);
  //lm_init(1);
  get_mode_info(mode, &data); 

  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ax = 0x4F02;         // VBE call, function 02 -- set VBE mode
  r.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
  r.intno = 0x10;

  struct minix_mem_range mr;
  unsigned int vram_base = data.PhysBasePtr; /*VRAM’s physical addresss*/
  unsigned int vram_size;                    /*VRAM’s size, but you can usethe frame-buffer size, instead*/
                                             /*frame-buffer VM address*/
                                             /*Allow memory mapping*/
  vram_size = data.XResolution * data.YResolution * (data.BitsPerPixel + 7) / 8;
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (t = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", t); /*Map memory*/

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn’t map video memory");

  if (sys_int86(&r) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int print_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= data.XResolution)
    return 0;
  memcpy(aux + (x + data.XResolution * y) * ((data.BitsPerPixel + 7) / 8), &color, ((data.BitsPerPixel + 7) / 8));
  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  int i;
  for (i = 0; i < len; i++) {
    print_pixel(x + i, y, color);
    printf("printh:%d,%d\n", x + i, y);
  }
  return 0;
}

int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  int i;
  for (i = 0; i < len; i++) {
    print_pixel(x, y + i, color);
    printf("printv:%d,%d\n", x, y + i);
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++) {

    for (int j = 0; j < width; j++) {

      print_pixel(x + j, y + i, color);
    }
  }

  return 0;
}



int v_sprintf(int wid, int hei, char *str, int tam)
{
  enum xpm_image_type type = XPM_8_8_8;
  xpm_image_t img;
  uint8_t *sprite = NULL;
  int cont=0;
  printf("%s\n",str);
  
  while(str[cont]!='\0')
  {
    if(str[cont]=='a')
  sprite = xpm_load(a, type, &img);

    if(str[cont]=='b')
  sprite = xpm_load(b, type, &img);

    if(str[cont]=='c')
  sprite = xpm_load(c, type, &img);

    if(str[cont]=='d')
  sprite = xpm_load(d, type, &img);

    if(str[cont]=='e')
  sprite = xpm_load(e, type, &img);

    if(str[cont]=='f')
  sprite = xpm_load(f, type, &img);

    if(str[cont]=='g')
  sprite = xpm_load(g, type, &img);

    if(str[cont]=='h')
  sprite = xpm_load(h, type, &img);

    if(str[cont]=='i')
  sprite = xpm_load(i, type, &img);

    if(str[cont]=='j')
  sprite = xpm_load(j, type, &img);

    if(str[cont]=='k')
  sprite = xpm_load(k, type, &img);

    if(str[cont]=='l')
  sprite = xpm_load(l, type, &img);

    if(str[cont]=='m')
  sprite = xpm_load(m, type, &img);

    if(str[cont]=='n')
  sprite = xpm_load(n, type, &img);

    if(str[cont]=='o')
  sprite = xpm_load(o, type, &img);

    if(str[cont]=='p')
  sprite = xpm_load(p, type, &img);

    if(str[cont]=='q')
  sprite = xpm_load(q, type, &img);

    if(str[cont]=='r')
  sprite = xpm_load(r, type, &img);

    if(str[cont]=='s')
  sprite = xpm_load(s, type, &img);

    if(str[cont]=='t')
  sprite = xpm_load(t, type, &img);

    if(str[cont]=='u')
  sprite = xpm_load(u, type, &img);

      if(str[cont]=='v')
  sprite = xpm_load(v, type, &img);

      if(str[cont]=='w')
  sprite = xpm_load(w, type, &img);

      if(str[cont]=='x')
  sprite = xpm_load(x, type, &img);

      if(str[cont]=='y')
  sprite = xpm_load(y, type, &img);

      if(str[cont]=='z')
  sprite = xpm_load(z, type, &img);

      if(str[cont]=='~')
  sprite = xpm_load(til, type, &img);

      if(str[cont]=='-')
  sprite = xpm_load(hifen, type, &img);

      if(str[cont]=='\'')
  sprite = xpm_load(apos, type, &img);

      if(str[cont]==':')
  sprite = xpm_load(doisp, type, &img);

      if(str[cont]=='!')
  sprite = xpm_load(excla, type, &img);

     if(str[cont]=='0')
  sprite = xpm_load(n0, type, &img);

      if(str[cont]=='1')
  sprite = xpm_load(n1, type, &img);

      if(str[cont]=='2')
  sprite = xpm_load(n2, type, &img);

      if(str[cont]=='3')
  sprite = xpm_load(n3, type, &img);

      if(str[cont]=='4')
  sprite = xpm_load(n4, type, &img);

      if(str[cont]=='5')
  sprite = xpm_load(n5, type, &img);

      if(str[cont]=='6')
  sprite = xpm_load(n6, type, &img);

      if(str[cont]=='7')
  sprite = xpm_load(n7, type, &img);


      if(str[cont]=='8')
  sprite = xpm_load(n8, type, &img);

      if(str[cont]=='9')
  sprite = xpm_load(n9, type, &img);

      if(str[cont]=='\n')
      {

      wid=0;
      hei=hei+26;
      }

  
uint32_t *l=NULL;
if(str[cont]!=' ' && str[cont]!='\n')
{
  for(int conty=0;conty<img.height;conty++)
  {
    for(int contx=0;contx<img.width;contx++)
    {
      l = (uint32_t *)&(sprite[(conty * img.width + contx)*((data.BitsPerPixel + 7) / 8)]);
      if(*l==0)
      *l=background;
        if (print_pixel(wid + contx, hei + conty, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
        
         return 1;
        }     
    }
  }
}
if(str[cont]!='\n')
  wid=wid+img.width;
  //hei=hei+img.height;
cont++;
  }



  return 0;
}

int forca_draw(int tentativa)
{
  enum xpm_image_type type = XPM_8_8_8;
  xpm_image_t img;
  uint8_t *sprite = NULL;
  uint32_t *l=NULL;
  printf("tentativa: %d\n",tentativa);
  if(tentativa==0)
  sprite = xpm_load(forca0, type, &img);

  if(tentativa==1)
  sprite = xpm_load(forca1, type, &img);

  if(tentativa==2)
  sprite = xpm_load(forca2, type, &img);

  if(tentativa==3)
  sprite = xpm_load(forca3, type, &img);

  if(tentativa==4)
  sprite = xpm_load(forca4, type, &img);

  if(tentativa==5)
  sprite = xpm_load(forca5, type, &img);

  if(tentativa==6)
  sprite = xpm_load(forca6, type, &img);

 
  int wid=50, hei=150;

  for(int conty=0;conty<img.height;conty++)
  {
    for(int contx=0;contx<img.width;contx++)
    {
      l = (uint32_t *)&(sprite[(conty * img.width + contx)*((data.BitsPerPixel + 7) / 8)]);
      if(*l==0)
      *l=background;
        if (print_pixel(wid + contx, hei + conty, *l) != 0) {
          printf("forca_draw :: print_pixel error\n");
          vg_exit();
        
         return 1;
        }     
    }
  }

  return 0;
}

