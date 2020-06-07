#include <stdio.h>
#include <stdlib.h> 
#include <lcom/lab5.h>
#include <lcom/xpm.h>
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include "i8042.h"
#include "menu.h"
#include "mouse.h"
#include "video.h"


#include "buttons/jogar.h"
#include "buttons/highscores.h"
#include "buttons/opcoes.h"
#include "buttons/sair.h"

#include "buttons/dificuldade.h"
#include "buttons/cores.h"

#include "buttons/cursor.h"

#include "buttons/facil.h"
#include "buttons/medio.h"
#include "buttons/dificil.h"
#include "buttons/extreme.h"

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
#include "small/apos.h"

 // extern uint32_t scancode;
  extern uint8_t *video_mem;
  extern uint8_t *aux ;
  extern vbe_mode_info_t data;
  uint32_t byte;
  int counter=0;
  extern uint32_t background;
  int16_t x_move = 400, y_move = 300;
  extern int diff;
  int clicka=0;

 int cores_menu()
 {

   
     enum xpm_image_type type = XPM_8_8_8;
  xpm_image_t img;


  uint8_t *sprite = NULL,*defa = NULL;
  sprite = xpm_load(cursor_xpm, type, &img);
uint32_t *l;
     defa=malloc(data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
  //aux=malloc(data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8); 



  int xa = x_move, ya = y_move;

int click=0;

 for (int j = 0; j < data.YResolution; j++) { 
    for (int i = 0; i < data.XResolution; i++) {
        print_pixel(i,j,background);
    }
    }


    vg_draw_rectangle(250,25,300,100,0xFFFFFF);
    vg_draw_rectangle(250,175,300,100,0xFFFFFF);
    vg_draw_rectangle(250,325,300,100,0xFFFFFF);
    vg_draw_rectangle(250,475,300,100,0xFFFFFF);
      for (int j = 4; j < 96; j++) { 
    for (int i = 4; i < 296; i++) {


        if (print_pixel(250 + i, 25 + j, 0x8c2d19) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
         
          //vg_exit();
        
        // return 1;
        }    

        if (print_pixel(250 + i, 175 + j, 0x00FF00) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
         // vg_exit();
        
        // return 1;
        }    


        if (print_pixel(250 + i, 325 + j, 0xFFFFFF-0x8c2d19) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
         // vg_exit();
          //return 1;
        } 
        
        if (print_pixel(250 + i, 475 + j, 0) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
          return 1;
        }  
      }
    }

memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
memcpy(defa,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
  while (1) {
    



    memcpy(aux,defa,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

    for (int i = 0; i < img.width; i++) {
      for (int j = 0; j < img.height; j++) {
        l = (uint32_t *)&(sprite[(j * img.width + i)*((data.BitsPerPixel + 7) / 8)]);
        /*  if(*l==0)
        *l=background; */
        if(sprite[(j * img.width + i)*((data.BitsPerPixel + 7) / 8)]!=0x0)
        {
        if (print_pixel(x_move + i, y_move + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
         // return 1;

        }
        }
      }
    }


    if(click==1  && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=25 && (y_move+10)<=125) )
    {
      background=0x8c2d19;
      return 0;
      
    }


      if(click==1  && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=175 && (y_move+10)<=275) )
    {
      background=0x00ff00;
      return 0;
      
    }


      if(click==1 && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=325 && (y_move+10)<=425) )
    {
      background=0xffffff-0x8c2d19;
      return 0;
      
    }
        if(click==1  && (x_move-16>=250 && x_move-16<=550) && (y_move+10>=475 && y_move+10<=575) )
        {
          background=0;
          return 0;
    
        }
    

    memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8); 


    xa = x_move;
    ya = y_move;

/*       sys_inb(IN_BUF, &scancode);
          if (scancode & O_BUFFER_FULL) {

            sys_inb(I_O_BUFFER, &scancode);
        } */
        
      
        mouse_int(&x_move, &y_move,&click);

    
 

    if(x_move+48>=data.XResolution)
    x_move=data.XResolution-48;
    if(x_move<0)
    x_move=0;
    if(y_move+48>=data.YResolution)
    y_move=data.YResolution-48;
    if(y_move<0)
    y_move=0;

  }

      
    


   return 0;
 }

 int dificuldade_menu()
 {


   //uint8_t def = video_mem[0];

  enum xpm_image_type type = XPM_8_8_8;
  xpm_image_t img,tile1 ,tile2, tile3, tile4/* ,tile5 */ ;

  uint8_t *sprite = NULL , *jogsprite = NULL  ,*defa = NULL , *sairsprite = NULL, *highsprite = NULL, *optsprite = NULL;
  defa=malloc(data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);




  sprite = xpm_load(cursor_xpm, type, &img);
  jogsprite = xpm_load(facil_button,type,&tile1);
  highsprite = xpm_load(dificil_button, type, &tile3);
  optsprite = xpm_load(medio_button, type, &tile4);
  sairsprite = xpm_load(extreme_button, type, &tile2); 





  int xa = x_move, ya = y_move;

    uint32_t *l=NULL/* ,*g = NUL */ ;
int click=0;

 for (int j = 0; j < data.YResolution; j++) { 
    for (int i = 0; i < data.XResolution; i++) {
        print_pixel(i,j,background);
    }
    }




      for (int j = 0; j < tile3.height; j++) { 
    for (int i = 0; i < tile3.width; i++) {

        l = (uint32_t *)&(jogsprite[(j * tile1.width + i)*((data.BitsPerPixel + 7) / 8)]);
        if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 25 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
        
         return 1;
        }    
        l = (uint32_t *)&(optsprite[(j * tile4.width + i)*((data.BitsPerPixel + 7) / 8)]);
         if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 175 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
        
         return 1;
        }     
        l = (uint32_t *)&(highsprite[(j * tile3.width + i)*((data.BitsPerPixel + 7) / 8)]);
         if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 325 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
          return 1;
        } 
        l = (uint32_t *)&(sairsprite[(j * tile2.width + i)*((data.BitsPerPixel + 7) / 8)]);
         if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 475 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
          return 1;
        }
      }
    }

memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
memcpy(defa,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
  while (1) {
    



    memcpy(aux,defa,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

    for (int i = 0; i < img.width; i++) {
      for (int j = 0; j < img.height; j++) {
        l = (uint32_t *)&(sprite[(j * img.width + i)*((data.BitsPerPixel + 7) / 8)]);
        /*  if(*l==0)
        *l=background; */
        if(sprite[(j * img.width + i)*((data.BitsPerPixel + 7) / 8)]!=0x0)
        {
        if (print_pixel(x_move + i, y_move + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          return 1;

        }
        }
      }
    }


    if(click==1 && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=25 && (y_move+10)<=125) )
    {
      diff=1;
      return 0;
    }


      if(click==1  && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=175 && (y_move+10)<=275) )
    {
       diff=2;
      return 0;
      
      
    }


    if(click==1 && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=325 && (y_move+10)<=425) )
    {
      diff=3;
      return 0;
    }

        if(click==1  && (x_move-16>=250 && x_move-16<=550) && (y_move+10>=475 && y_move+10<=575) )
    {
      diff=4;
      return 0;
    }
    
memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8); 


    xa = x_move;
    ya = y_move;

/*       sys_inb(IN_BUF, &scancode);
          if (scancode & O_BUFFER_FULL) {

            sys_inb(I_O_BUFFER, &scancode);
        } */
        
        
        mouse_int(&x_move, &y_move,&click);

    
 

    if(x_move+48>=data.XResolution)
    x_move=data.XResolution-48;
    if(x_move<0)
    x_move=0;
    if(y_move+48>=data.YResolution)
    y_move=data.YResolution-48;
    if(y_move<0)
    y_move=0;

  }



 
  vg_exit();
  return 0;
}
 


int opcoes_menu()
 {

  enum xpm_image_type type = XPM_8_8_8;
  xpm_image_t img,tile1 ,/* tile2, */ tile3, tile4/* ,tile5 */ ;


  uint8_t *sprite = NULL , *jogsprite = NULL  ,*defa = NULL , /* *sairsprite = NULL, */ *highsprite = NULL, *optsprite = NULL;
   defa=malloc(data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

INICIOO:
;
  sprite = xpm_load(cursor_xpm, type, &img);
  jogsprite = xpm_load(dificuldade_button,type,&tile1);
  highsprite = xpm_load(sair_button, type, &tile3);
  optsprite = xpm_load(cor_button, type, &tile4);
  //sairsprite = xpm_load(sair_button, type, &tile2); 

  //aux=malloc(data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8); 





  
  int xa = x_move, ya = y_move;

    uint32_t *l=NULL/* ,*g = NUL */ ;
int click=0;


 for (int j = 0; j < data.YResolution; j++) { 
    for (int i = 0; i < data.XResolution; i++) {
        print_pixel(i,j,background);
    }
    }



      for (int j = 0; j < tile3.height; j++) { 
    for (int i = 0; i < tile3.width; i++) {

        l = (uint32_t *)&(jogsprite[(j * tile1.width + i)*((data.BitsPerPixel + 7) / 8)]);
         if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 25 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          tickdelay(100);
          vg_exit();
        
         return 1;
        }    
        l = (uint32_t *)&(optsprite[(j * tile4.width + i)*((data.BitsPerPixel + 7) / 8)]);
         if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 175 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
        
         return 1;
        }     
        l = (uint32_t *)&(highsprite[(j * tile3.width + i)*((data.BitsPerPixel + 7) / 8)]);
         if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 325 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
          return 1;
        } 
       
        if (print_pixel(250 + i, 475 + j, background) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
          return 1;
        } 
      }
    }

memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
memcpy(defa,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
  while (1) {
    



    memcpy(aux,defa,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

    for (int i = 0; i < img.width; i++) {
      for (int j = 0; j < img.height; j++) {
        l = (uint32_t *)&(sprite[(j * img.width + i)*((data.BitsPerPixel + 7) / 8)]);
        /*  if(*l==0)
        *l=background; */
        if(sprite[(j * img.width + i)*((data.BitsPerPixel + 7) / 8)]!=0x0)
        {
        if (print_pixel(x_move + i, y_move + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          return 1;

        }
        }
      }
    }


    if(click==1  && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=25 && (y_move+10)<=125) )
    {
      
      dificuldade_menu();
      goto INICIOO;
      
    }


      if(click==1  && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=175 && (y_move+10)<=275) )
    {
     
      cores_menu();
      goto INICIOO;
      
    }



        if(click==1 && (x_move-16>=250 && x_move-16<=550) && (y_move+10>=325 && y_move+10<=425) )
    
      return 0;
    

    memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8); 


    xa = x_move;
    ya = y_move;

/*       sys_inb(IN_BUF, &scancode);
          if (scancode & O_BUFFER_FULL) {

            sys_inb(I_O_BUFFER, &scancode);
        } */
        
      
        mouse_int(&x_move, &y_move,&click);

    
 

    if(x_move+48>=data.XResolution)
    x_move=data.XResolution-48;
    if(x_move<0)
    x_move=0;
    if(y_move+48>=data.YResolution)
    y_move=data.YResolution-48;
    if(y_move<0)
    y_move=0;


  }

      return 0;
    }








  int menu_init(int *option)
  {

  //uint8_t def = video_mem[0];
  uint8_t bit_no = IRQ_MOUSE;
  enum xpm_image_type type = XPM_8_8_8;
  xpm_image_t img,tile1 ,tile2, tile3, tile4/* ,tile5 */ ;

  uint8_t *sprite = NULL , *jogsprite = NULL  ,*defa = NULL , *sairsprite = NULL, *highsprite = NULL, *optsprite = NULL;
  defa=malloc(data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
  aux=malloc(data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
  kbc_write(0xEA);
  kbc_write(0xF4);

  if (subscribe_mouse_int(&bit_no) != OK) { // 1
    printf("subscribe_mouse_int failed");
    return 1;
  }

INICIO:
;
  sprite = xpm_load(cursor_xpm, type, &img);
  jogsprite = xpm_load(jogartile,type,&tile1);
  highsprite = xpm_load(highscores_button, type, &tile3);
  optsprite = xpm_load(opcoes_button, type, &tile4);
  sairsprite = xpm_load(sair_button, type, &tile2); 





  int xa = x_move, ya = y_move;

    uint32_t *l=NULL/* ,*g = NUL */ ;
int click=0;

 for (int j = 0; j < data.YResolution; j++) { 
    for (int i = 0; i < data.XResolution; i++) {
        print_pixel(i,j,background);
    }
    }




      for (int j = 0; j < tile3.height; j++) { 
    for (int i = 0; i < tile3.width; i++) {

        l = (uint32_t *)&(jogsprite[(j * tile1.width + i)*((data.BitsPerPixel + 7) / 8)]);
        if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 25 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
        
         return 1;
        }    
        l = (uint32_t *)&(optsprite[(j * tile4.width + i)*((data.BitsPerPixel + 7) / 8)]);
         if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 175 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
        
         return 1;
        }     
        l = (uint32_t *)&(highsprite[(j * tile3.width + i)*((data.BitsPerPixel + 7) / 8)]);
         if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 325 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
          return 1;
        } 
        l = (uint32_t *)&(sairsprite[(j * tile2.width + i)*((data.BitsPerPixel + 7) / 8)]);
         if(*l==0)
        *l=background;
        if (print_pixel(250 + i, 475 + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          vg_exit();
          return 1;
        }
      }
    }

memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
memcpy(defa,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
  while (1) {
    



    memcpy(aux,defa,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

    for (int i = 0; i < img.width; i++) {
      for (int j = 0; j < img.height; j++) {
        l = (uint32_t *)&(sprite[(j * img.width + i)*((data.BitsPerPixel + 7) / 8)]);
        /*  if(*l==0)
        *l=background; */
        if(sprite[(j * img.width + i)*((data.BitsPerPixel + 7) / 8)]!=0x0)
        {
        if (print_pixel(x_move + i, y_move + j, *l) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          return 1;

        }
        }
      }
    }


    if(click==1  && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=25 && (y_move+10)<=125) )
    {
      *option=jogar;
      

      if (unsubscribe_mouse_int() != OK) 
      {
       printf("unsbscribe_mouse_int failed");
       return 1;
      } 

     kbc_write(0xF5);
     kbc_write(0xF6); 
     
      //vg_exit();
      return 0;
    }


      if(click==1  && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=175 && (y_move+10)<=275) )
    {
      
      opcoes_menu();
      goto INICIO;
      
      
    }


    if(click==1 && ((x_move-16)>=250 && (x_move-16)<=550) && ((y_move+10)>=325 && (y_move+10)<=425) )
    {
      *option=highscores;
      

      if (unsubscribe_mouse_int() != OK) 
      {
       printf("unsbscribe_mouse_int failed");
       return 1;
      } 

     kbc_write(0xF5);
     kbc_write(0xF6); 
      //vg_exit();
      return 0;
    }

        if(click==1  && (x_move-16>=250 && x_move-16<=550) && (y_move+10>=475 && y_move+10<=575) )
    {
      *option=sair;

      if (unsubscribe_mouse_int() != OK) 
      {
       printf("unsbscribe_mouse_int failed");
       return 1;
      } 

     kbc_write(0xF5);
     kbc_write(0xF6); 
      vg_exit();
      return 0;
    }
    
memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8); 


    xa = x_move;
    ya = y_move;

/*       sys_inb(IN_BUF, &scancode);
          if (scancode & O_BUFFER_FULL) {

            sys_inb(I_O_BUFFER, &scancode);
        } */
        
      
        mouse_int(&x_move, &y_move,&click);

    
 

    if(x_move+48>=data.XResolution)
    x_move=data.XResolution-48;
    if(x_move<0)
    x_move=0;
    if(y_move+48>=data.YResolution)
    y_move=data.YResolution-48;
    if(y_move<0)
    y_move=0;
    

  }



 
  vg_exit();
  return 0;
}


 
 
    




 


