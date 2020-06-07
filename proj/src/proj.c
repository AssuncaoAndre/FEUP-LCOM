// IMPORTANT: you must include the following line in all your C files

/* para os gráficos, usar dois buffers, um para armazernar cada operação e o
video_mem para escrever tudo ao mesmo tempo, é mais rápido

char video_buffer= (char*) c*l*h*bpp;
memcpy(video_mem,video_buffer,c*l*h*bpp); */
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <lcom/lab5.h>

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "list.h"
#include "video.h"
//#include "write.h"


#include "i8042.h"
#include "i8254.h"

#include "makecodes.h"
//#include "mouse.h"
#include "menu.h"



//#include "words.txt"
#define DEF_TEMP 60

int diff=3;
uint8_t *aux ;
uint8_t *video_mem;
vbe_mode_info_t data;
uint32_t background=0;
char error_str[100]={""};

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


void wait_valid_rtc(uint32_t* hour,uint32_t*min,uint32_t*seg,uint32_t*day,uint32_t*month,uint32_t*year) 
{
uint32_t regA=0;
   do 
  {

    sys_outb(0x70/* RTC_ADDR_REG */, 0x0A/* RTC_REG_A */);
    sys_inb(0x71/* RTC_DATA_REG */, &regA);
 
  } 
  while ( regA & BIT(7)/* RTC_UIP */); 
  //printf("reg:%X\n",regA);

  sys_outb(0x70,0x0);
  sys_inb(0x71,seg);

  sys_outb(0x70,0x2);
  sys_inb(0x71,min);

  sys_outb(0x70,0x4);
  sys_inb(0x71,hour);

  sys_outb(0x70,0x7);
  sys_inb(0x71,day);

  sys_outb(0x70,0x8);
  sys_inb(0x71,month);

  sys_outb(0x70,0x9);
  sys_inb(0x71,year);

  
}

int pontuacao (char *pal)
{
  int pont=0;
  int comp=strlen(pal);
  for(int i=0;i<comp;i++)
  {
    pont=pont+char_to_pont(pal[i]);
  }
  pont=pont*comp;
  return pont;
}



uint32_t cnt = 0;
uint32_t scancode;
int flag = 0;

int(proj_main_loop)(int argc, char *argv[]) {

    if (video_init(0x115) != 0) {
    printf("video init error\n");
    vg_exit();
    return 1;
  }

 
int p;
uint32_t key=0;
MENU: 
;
  int ipc_status;
  uint8_t r, bit_no_kbd=1, bit_no_timer = 0;
  int hook_id_kbd = bit_no_kbd;
  int hook_id_timer = bit_no_timer;
  message msg;
 int option=0;

  if(menu_init(&option)!=0)
  {

  strcpy(error_str,"menu_init error");
  goto FIM;
  }

 //vg_exit(); 

 for(int xt=0;xt<data.XResolution;xt++)
{
  for(int yt=0;yt<data.YResolution;yt++)
  {
    if(print_pixel(xt,yt,background)!=0)
      {

      strcpy(error_str,"print_pixel error");
       goto FIM;
      }
  }
}
memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8); 

if(option==sair)
{
  goto FIM;
}






        if( sys_irqsetpolicy(IRQ_KB, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd)!=0)
          {
           printf("Erro 2º kbd\n");
           vg_exit();
           return 1;
         } 

         if(sys_irqsetpolicy(IRQ_TIMER,IRQ_REENABLE,&hook_id_timer)!=0)
         {
           printf("Erro 2º timer\n");
           vg_exit();
           return 1;
         }
if(option==highscores)
{

  goto FILE_READ;
}



    
  char palavra[100], falta[100], letras[26] = {0}, lixo;
  double n;

  int i = 0, contagem=0, tamanho = 0, tent = 0, l = 0, c = 0, igual = 0, lost=0, pont=0;
  double cont = 0;
  FILE *f;

 while(1)
{ 
  char temp[]={"escolhendo palavra"};
  v_sprintf(275,30,temp,1);
  memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
 srand(time(NULL));
  n = rand() % 466324 + 0;
   f = fopen("/home/lcom/labs/proj/src/words.txt", "r");

  if (f == NULL)
        {
         strcpy(error_str,"error opening file. Check path");
         goto FIM;
        }
    

while(cont<n)
{

     fscanf(f, "%c", &lixo);
    if (lixo == '\n')
      cont++; 
}
cont=0;

   
  fgets(palavra, 100, f);
  printf("palavra:%s\n",palavra);
  fclose(f); 
  pont=0;
  pont=pontuacao(palavra);
if (pont<=facil && diff==1)
break;
if (pont>facil &&  pont<=medio && diff==2)
break;
if (pont>medio && pont<=dificil && diff==3)
break;
if (pont>dificil && diff==4)
break;


}
for(int xt=0;xt<data.XResolution;xt++)
{
  for(int yt=0;yt<data.YResolution;yt++)
  {
    if(print_pixel(xt,yt,background)!=0)
      {
        strcpy(error_str,"print_pixel error");
        goto FIM;
      }
    

  }
}
memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
  if (palavra[0] > 64 && palavra[0] < 91)
    palavra[0] = palavra[0] + 32;
      char testeee[100];/* ={"teste bros\n ez"};
  v_sprintf(30,30,testeee,1) ; 
  memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8); */

  printf("\t \t the hangman game\n");
  strcpy(testeee,"the hangman game");
  v_sprintf(300,30,testeee,1) ;
  //memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

  printf("instructions\n");
  strcpy(testeee,"the hangman game");
  
  v_sprintf(30,70,testeee,1) ;
  //memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

  printf("-try to guess the hidden word\n");
  strcpy(testeee,"-try to guess the hidden word");
  v_sprintf(30,90,testeee,1) ;
  //memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

  printf("-only six tries\n");
  strcpy(testeee,"-only six tries");
  v_sprintf(30,110,testeee,1) ;
  //memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

  printf("-write only a letter at once\n");
  strcpy(testeee,"-write only a letter at once");
  v_sprintf(30,130,testeee,1) ;
 memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
 

  while (i < 100) {
    if (palavra[i] == '\n' || palavra[i] == '\0' || palavra[i] == 32) {
      palavra[i] = 0;
      falta[i]=0;
      break;
    }
/*     if (palavra[i] > 64 && palavra[i] < 91)
      palavra[i] = palavra[i] + 32; */

    falta[i] = '~';
    if (palavra[i] == '-')
      falta[i] = '-';
    if (palavra[i] == 44)
      falta[i] = 44;
    if (palavra[i] == 39)
      falta[i] = 39;

    i++;
    tamanho++;
  }
  printf("\n");
  i = 0;



  while (tent <= 6) 
  {
    if(forca_draw(tent)!=0)
      {
       strcpy(error_str,"forca_draw error");
       goto FIM;
      }
    if (tent == 6) 
    {
      printf("   ----\n");
      printf("   |  |\n");
      printf("   o  |\n");
      printf("  \\|/ |\n");
      printf("  / \\|\n");
      
      
    }
    if (tent == 5) 
    {
      printf("   ----\n");
      printf("   |  |\n");
      printf("   o  |\n");
      printf("  \\|/ |\n");
      printf("  /   |\n");


    }

    if (tent == 4) 
    {
      printf("   ----\n");
      printf("   |  |\n");
      printf("   o  |\n");
      printf("  \\|/ |\n");
      printf("      |\n");


    
    }

    if (tent == 3) 
    {
      printf("   ----\n");
      printf("   |  |\n");
      printf("   o  |\n");
      printf("  \\|  |\n");
      printf("      |\n");


   

    }
    if (tent == 2) 
    {
      printf("   ----\n");
      printf("   |  |\n");
      printf("   o  |\n");
      printf("   |  |\n");
      printf("      |\n");


    }

    if (tent == 1) 
    {
      printf("   ----\n");
      printf("   |  |\n");
      printf("   o  |\n");
      printf("      |\n");
      printf("      |\n");


      

    }
    if (tent == 0) 
    {
      printf("   ----\n");
      printf("      |\n");
      printf("      |\n");
      printf("      |\n");
      printf("      |\n");


     
    }
  if (tent==6)
  break;
    printf("\n");

    i = 0;
    while (i < tamanho) {
      printf("%c", falta[i]);
      i++;
    }
    //falta[tamanho]=0;
      strcpy(testeee,"-write only a letter at once");
    v_sprintf(30,360,falta,1) ;
memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

    printf("\n");

    for(int xt=30;xt<160+30;xt++)
      {
         for(int yt=450;yt<450+13;yt++)
          {
            if(print_pixel(xt,yt,background)!=0)
              {
                strcpy(error_str,"print_pixel error");
                goto FIM;
             }
          }
      }
          memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
    

    printf("Erros: %d\n", tent);
    if(tent==0)
    strcpy(testeee,"erros: 0");
    if(tent==1)
    strcpy(testeee,"erros: 1");
    if(tent==2)
    strcpy(testeee,"erros: 2");
    if(tent==3)
    strcpy(testeee,"erros: 3");
    if(tent==4)
    strcpy(testeee,"erros: 4");
    if(tent==5)
    strcpy(testeee,"erros: 5");
    if(tent==6)
    strcpy(testeee,"erros: 6");
    v_sprintf(30,450,testeee,1) ;
    memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);


    printf("Letters already used:");
    strcpy(testeee,"letters already used:");
    v_sprintf(30,400,testeee,1) ;
    memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
    i = 0;

    while (1)
    {
      if (letras[i] == '\0')
        break;
      printf("%c ", letras[i]);
      i++;
    }
  
     v_sprintf(270,400,letras,1) ;
   memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

    int flaga = 0;
    int temp=0;
    printf("\n");

    printf("Digite uma letra\n");
    strcpy(testeee,"digite uma letra");
    v_sprintf(80,500,testeee,1) ;
    memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);


/*
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--------------------------------------------INTERRUPTS------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
*/

REPEAT:    while (flaga == 0 && temp/DEFAULT_FREQ < DEF_TEMP) { /* You may want to use a different condition */
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

           if (msg.m_notify.interrupts  & BIT(bit_no_timer)) 
                { 
                   temp++;
               }

          if (msg.m_notify.interrupts & BIT(bit_no_kbd)) 
          { 
            kbc_ih();
            temp=0;

           
            if ((scancode & 0x80)== 0)
            {
           
              flaga = 1;
            break;

            }
            /* if(scancode==EXIT_CODE ) //caso se queira acabar o jogo a meio, poupava tempo a dar debugging
            goto FIM; */

          }



          break;
        default:

          break; /* no other notifications expected: do nothing */
        }
      } else {
      }
    }



    if(temp/60==DEF_TEMP)
    {
      tent=6;
      lost=1;
      break;
    }
    flaga=0;
    lixo=make_to_char(scancode);
    printf("LETRA:%c\n",lixo);
     
    
      for(int xt=30;xt<420+30;xt++)
      {
         for(int yt=530;yt<550+13;yt++)
          {
            if(print_pixel(xt,yt,background)!=0)
              {
              strcpy(error_str,"print_pixel error");
              goto FIM;
             }
          }
      }

    if(lixo==-1)
    {
      printf("That is not a letter, try again\n");
        strcpy(testeee,"that isn't a letter    try again");
        v_sprintf(30,530,testeee,1) ;
        memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
    goto REPEAT;
    }
    
          memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
    
    flaga = 0;
    i = 0;



    while (1) {

      if (letras[i] == '\0')
        break;

      if (lixo == letras[i] || lixo == letras[i] - 32)
      {
        printf("Letter already used!\n");
        strcpy(testeee,"letter already used");
        v_sprintf(50,550,testeee,1);
        memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
        printf("Write it again\n");
        goto REPEAT;

        lixo=make_to_char(scancode);
        if(lixo==-1)
        {
          printf("Not a valid letter, repeat\n");
           goto REPEAT;
        }
        i = 0;
      }
      i++;

    }
    letras[contagem] = lixo;
    contagem++;
    i = 0;
    while (i <= tamanho) {
      if (lixo == palavra[i]) {
        falta[i] = lixo;
        c = 1;
      }
      i++;
    }
    if (c == 0)
      tent++;
    l++;
    for(int qwerty=0;qwerty<20;qwerty++)
    printf("\n");
    if (tent == 6)
      break;
    c = 0;
    i = 0;
    igual = 0;

    while (i < tamanho) {
      if (falta[i] == palavra[i])
        ;
      else
        igual = 1;
      i++;
    }
    if (igual == 0)
      break;
  } 


        for(int xt=0;xt<800;xt++)
        {
         for(int yt=350;yt<600;yt++)
          {
            print_pixel(xt,yt,background);
          }
        }

      if(tent==6)
      {
      forca_draw(6);
         
    
       printf("PERDESTE!!!! AHAHAHAH LOSER!!\n");
        strcpy(testeee,"ahahah loser");
       v_sprintf(50,400,testeee,1); 
        

      }
      if(lost==1)
      {
       strcpy(testeee,"ainda por cima por tempo");
       v_sprintf(225,400,testeee,1); 
      printf("AINDA POR CIMA POR TEMPO, QUE GRANDE NABO!!\n");
      }

  if (tent < 6) 
  {
     strcpy(testeee,"what a pro! congrats for the victory");
      v_sprintf(225,400,testeee,1); 
    printf("Temos aqui um pro!! Congrats for the victory\n");

uint32_t hour,min,seg,day,month,year;
uint32_t houra,mina,sega,daya,montha,yeara;
wait_valid_rtc(&hour,&min,&seg,&day,&month,&year);
printf("%X:%X:%X %X/%X/%X",hour,min,seg,day,month,year);

memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

FILE_READ:
;
    FILE *fi;
lista* lst=lista_nova();
int score=0;
int h=0;
int flagl=0;
l_elemento *guess;

//printf("%p\n",lst);
fi=fopen("/home/lcom/labs/proj/src/best.txt","r");
//printf("%p\n",fi);
if(fi==NULL)
printf("error opening file\n");
while(fscanf(fi,"%d",&score)!=EOF && h<10)
{
  fscanf(fi,"%d",&houra);
  fscanf(fi,":%d",&mina);
  fscanf(fi,":%d",&sega); 
  fscanf(fi," %d",&daya);
  fscanf(fi,"/%d",&montha);
  fscanf(fi,"/%d",&yeara);
  if(pont>score && flagl==0 && option!=highscores)
  {
    flagl=1;
    guess=lista_insere(lst,seg,min,hour,day,month,year,pont,NULL);
    printf("inseriu guess: %d %X:%X:%X %X/%X/%X\n",guess->dif,guess->hour,guess->min,guess->seg,guess->day,guess->month,guess->year);
    printf("inseriu lst\n"); 
    h++;
    printf("Parabens! Um novo highscore!!");
        strcpy(testeee,"new highscore!!!");
      v_sprintf(275,420,testeee,1);
  }
  lista_insere(lst,sega,mina,houra,daya,montha,yeara,score,NULL);
  printf("inseriu: %d %X:%X:%X %X/%X/%X\n",score,hour,min,seg,day,month,year);
  
  h++;

}
fclose(fi);
printf("h:%d\npotion:%d\n\n",h,option); //h é 10 wtf
if (h<10 && option!=highscores && flagl==0)
{
  flagl=1;
  guess=lista_insere(lst,seg,min,hour,day,month,year,pont,NULL);
      printf("inseriu guess: %d %X:%X:%X %X/%X/%X\n",guess->dif,guess->hour,guess->min,guess->seg,guess->day,guess->month,guess->year);
    printf("inseriu lst\n"); 
    printf("Parabens! Um novo highscore!!");
        strcpy(testeee,"new highscore!!!");
      v_sprintf(275,420,testeee,1);
}

memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

  
 l_elemento *ele = NULL;
 ele=lst->inicio;
if(option==highscores)
{
  
  int posi=1;
  strcpy(testeee,"0");
  printf("highscores\n");
  sprintf(testeee,"highscores");
  v_sprintf(30,30,testeee,1);
  memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

  while(ele!=NULL)
{

  sprintf(testeee,"%d %d %d:%d:%d %d-%d-%d\n",posi,ele->dif,ele->hour,ele->min,ele->seg,ele->day,ele->month,ele->year);
  v_sprintf(30,(posi+1)*20+30,testeee,1);
 //memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
  posi++;
 
  ele=ele->proximo;
}
//memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);
}

if(flagl==1 && option!=highscores)
{
fi=fopen("/home/lcom/labs/proj/src/best.txt","wb+");
if(fi==NULL)
{
printf("error opening file2\n");
goto FIM;
}

while(ele !=NULL)
{
  if(ele==guess)
  fprintf(fi,"%d %X:%X:%X %X/%X/%X\n",ele->dif,ele->hour,ele->min,ele->seg,ele->day,ele->month,ele->year);
  else fprintf(fi,"%d %d:%d:%d %d/%d/%d\n",ele->dif,ele->hour,ele->min,ele->seg,ele->day,ele->month,ele->year);
  //printf("escreveu no file %d %X:%X:%X %X/%X/%X\n",ele->dif,ele->hour,ele->min,ele->seg,ele->day,ele->month,ele->year);
  //printf("nao escreveu no file %d %d:%d:%d %d/%d/%d\n",ele->dif,ele->hour,ele->min,ele->seg,ele->day,ele->month,ele->year);
  ele=ele->proximo;
}
fclose(fi);


lista_apaga(lst);
  }
  }

if(option==highscores)
{
  sprintf(testeee,"prima esc para voltar para o menu");
  v_sprintf(250,400,testeee,1);
  memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);

  p=300;

  while(p>0)
  {
    p--;

    sys_inb(IN_BUF, &scancode);
          if (scancode & O_BUFFER_FULL) {

            sys_inb(I_O_BUFFER, &scancode);
        }
        else break;
  }

  while (scancode!=EXIT_CODE) { 
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & BIT(bit_no_kbd)) { /* subscribed interrupt */
            kbc_ih();

          }

          break;
        default:

          break; 
        }
      } else {
      }
    }



  goto MENU;
}

  i = 0;
  printf("The word is.....\n");
  while (i <= tamanho) {

    printf("%c", palavra[i]);
    i++;
  }
  printf("\n");
  printf("Dificuldade:%d\n",pont); 
  sprintf(testeee,"the word is: %s",palavra);
  //strcpy(testeee,"the word is");
  v_sprintf(175,500,testeee,1);
  sprintf(testeee,"dificuldade: %d",pont);
  v_sprintf(175,520,testeee,1);

  //v_sprintf(11*13,500,palavra,1);
  strcpy(testeee,"para voltar para o menu prima esc");
  v_sprintf(230,550,testeee,1);

  memcpy(video_mem,aux,data.XResolution*data.YResolution*(data.BitsPerPixel+7)/8);




p=300;

  while(p>0)
  {
    p--;
    //tickdelay(micros_to_ticks(DELAY_US));

    sys_inb(IN_BUF, &scancode);
          if (scancode & O_BUFFER_FULL) {

            sys_inb(I_O_BUFFER, &scancode);

        }
        else break;
  }

while (scancode!=EXIT_CODE) { /* You may want to use a different condition */
      /* Get a request message. */
      // printf("1\n");
      // printf("cont:%d, time:%d\n\n",cont/60,time);
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & BIT(bit_no_kbd)) { /* subscribed interrupt */
            kbc_ih();

          }



          break;
        default:

          break; /* no other notifications expected: do nothing */
        }
      } else {
      }
    }



goto MENU;






  //kbd_print_no_sysinb(cnt);
  FIM:
 ; 
   if (sys_irqrmpolicy(&hook_id_kbd) != 0)
    {
      vg_exit();
       return 1;
    }
    if(sys_irqrmpolicy(&hook_id_timer)!=0) 
    {
      vg_exit();
    return 1;
    }
  printf("\n");


 key=0;
 p=300;

  while(p>0)
  {
    p--;

    sys_inb(IN_BUF, &scancode);
          if (scancode & O_BUFFER_FULL) {

            sys_inb(I_O_BUFFER, &scancode);

        }
  }
  sys_outb(IN_BUF, READ_CMD);        
  sys_outb(IN_BUF, I_O_BUFFER);

  

  sys_inb(I_O_BUFFER,&key); 

  sys_outb(IN_BUF,I_O_BUFFER);        
  sys_outb(I_O_BUFFER,  key |  BIT(0));  
  free(aux);
  free(video_mem);
  vg_exit();
  printf("%s",error_str); 
  return 0;
}
