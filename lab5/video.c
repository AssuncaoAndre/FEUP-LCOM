#include "math.h"
#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "i8254.h"
#include "video.h"

#include <machine/int86.h>

uint8_t *video_mem;
vbe_mode_info_t data;

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

int(timer_int)(uint8_t time, uint8_t *flag) {
  /* To be implemented by the students */
  uint32_t scancode = 0;
  if (time < 0) {
    printf("INVALID TIME!");
    return 1;
  }
  int cont = 0;
  int ipc_status;
  message msg;
  uint8_t r;

  while (cont < time) { /* You may want to use a different condition */
    /* Get a request message. */

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(1)) {
            sys_inb(IN_BUF, &scancode);
            if (scancode & O_BUFFER_FULL)
              sys_inb(I_O_BUFFER, &scancode);
            kbc_ih();

            if (scancode == EXIT_CODE) {
              *flag = 1;
              return 0;
            }
          }
          if (msg.m_notify.interrupts & BIT(0)) { /* subscribed interrupt */

            cont++;
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
  get_mode_info(mode, &data); //vai ser substituido

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
  memcpy(video_mem + (x + data.XResolution * y) * ((data.BitsPerPixel + 7) / 8), &color, ((data.BitsPerPixel + 7) / 8));
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
