// IMPORTANT: you must include the following line in all your C files
#include "math.h"
#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include <machine/int86.h>

uint8_t *video_mem;
vbe_mode_info_t data;

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "i8254.h"
#include "video.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");
  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");
  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");
  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;
  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();
  return 0;
}

void(timer_int_handler)() {}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  if (video_init(mode) != 0) {
    printf("video init error\n");
    return 1;
  }

  tickdelay(micros_to_ticks(delay * 1000 * 1000));
  vg_exit();
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  if (video_init(mode) != 0) {
    printf("video init error\n");
    vg_exit();
    return 1;
  }

  int i = 0;
  uint64_t color_aux = 0;

  while (i < data.BitsPerPixel) {
    color_aux = (color_aux << 1);
    color_aux = color_aux + 0x1;
    i++;
  }
  color = (color & color_aux);

  vg_draw_rectangle(x, y, width, height, color);

  int ipc_status;
  uint8_t g, bit_no = 0;
  message msg;
  int hook_id = bit_no;
  uint32_t scancode = 0;

  if (sys_irqsetpolicy(IRQ_KB, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0)
    return 1;

  while (scancode != EXIT_CODE) {
    if ((g = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", g);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                            /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(0)) { /* subscribed interrupt */
            sys_inb(IN_BUF, &scancode);
            if (scancode & O_BUFFER_FULL) {
              sys_inb(I_O_BUFFER, &scancode);
            }

            break;
            default:

              break; /* no other notifications expected: do nothing */
          }
      }
    }
  }

  if (sys_irqrmpolicy(&hook_id) != 0)
    return 1;

  vg_exit();
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */

  if (video_init(mode) != 0) {
    printf("video init error\n");
    vg_exit();
    return 1;
  }

  int width = data.XResolution / no_rectangles;
  int height = data.YResolution / no_rectangles;
  int cont = 0;

  uint32_t R, G, B, color;
  for (int row = 0; row < no_rectangles; row++) {
    for (int col = 0; col < no_rectangles; col++) {
      cont++;
      if (data.MemoryModel != INDEXED) {
        R = (((first >> data.RedFieldPosition) % BIT(data.RedMaskSize) + col * step) % (1 << data.RedMaskSize)) << data.RedFieldPosition;
        G = (((first >> data.GreenFieldPosition) % BIT(data.GreenMaskSize) + row * step) % (1 << data.GreenMaskSize)) << data.GreenFieldPosition;
        B = (((first >> data.BlueFieldPosition) % BIT(data.BlueMaskSize) + (col + row) * step) % (1 << data.BlueMaskSize)) << data.BlueFieldPosition;
        color = R + G + B;
      }
      else {
        color = (first + (row * no_rectangles + col) * step) % (1 << data.BitsPerPixel);
      }
      vg_draw_rectangle(col * width, row * height, width, height, color);
    }
  }

  int ipc_status;
  uint8_t g, bit_no = 0;
  message msg;
  int hook_id = bit_no;
  uint32_t scancode = 0;

  if (sys_irqsetpolicy(IRQ_KB, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0)
    return 1;

  while (scancode != EXIT_CODE) {
    if ((g = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", g);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                            /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(0)) { /* subscribed interrupt */
            sys_inb(IN_BUF, &scancode);
            if (scancode & O_BUFFER_FULL) {
              sys_inb(I_O_BUFFER, &scancode);
            }

            break;
            default:
              break; /* no other notifications expected: do nothing */
          }
      }
    }
  }

  if (sys_irqrmpolicy(&hook_id) != 0)
    return 1;

  vg_exit();
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  if (video_init(0x105) != 0) {
    printf("video init error\n");
    vg_exit();
    return 1;
  }

  enum xpm_image_type type = XPM_INDEXED;
  xpm_image_t img;

  uint8_t *sprite = NULL;
  sprite = xpm_load(xpm, type, &img);

  for (int i = 0; i < img.width; i++) {
    for (int j = 0; j < img.height; j++) {
      if (print_pixel(x + i, y + j, sprite[j * img.width + i]) != 0) {
        printf("vg_draw_xpm :: draw_pixel error\n");
        return 1;
      }
    }
  }

  int ipc_status;
  uint8_t g, bit_no = 0;
  message msg;
  int hook_id = bit_no;
  uint32_t scancode = 0;

  if (sys_irqsetpolicy(IRQ_KB, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0)
    return 1;

  while (scancode != EXIT_CODE) {
    if ((g = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", g);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                            /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(0)) { /* subscribed interrupt */
            sys_inb(IN_BUF, &scancode);
            if (scancode & O_BUFFER_FULL) {
              sys_inb(I_O_BUFFER, &scancode);
            }

            break;
            default:
              break; /* no other notifications expected: do nothing */
          }
      }
    }
  }

  if (sys_irqrmpolicy(&hook_id) != 0)
    return 1;

  vg_exit();
  return 0;
}

void(kbc_ih)() {}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

  if (video_init(0x105) != 0) {
    printf("video init error\n");
    vg_exit();
    return 1;
  }

  uint8_t def = video_mem[0];

  enum xpm_image_type type = XPM_INDEXED;
  xpm_image_t img;

  uint8_t *sprite = NULL;
  sprite = xpm_load(xpm, type, &img);
  uint8_t bit_no_timer = 0, bit_no_kbd = 1;
  int hook_id_kbd = bit_no_kbd, hook_id_timer = bit_no_timer;
  int ipc_status;
  message msg;
  uint8_t g;
  uint32_t scancode = 0;
  int32_t speed_aux = speed;
  int x = xi, y = yi, flag = 0;
  int xa = x, ya = y;

  if (speed < 0) {
    speed = 1;
  }

  if (sys_irqsetpolicy(IRQ_KB, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) != 0)
    return 1;

  if (sys_irqsetpolicy(IRQ_TIMER, IRQ_REENABLE, &hook_id_timer) != 0)
    return 1;

  while (1) {
    uint8_t br = 0;

    for (int i = 0; i < img.width; i++) {
      for (int j = 0; j < img.height; j++) {
        if (print_pixel(xa + i, ya + j, def) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          return 1;
        }
      }
    }

    for (int i = 0; i < img.width; i++) {
      for (int j = 0; j < img.height; j++) {
        if (print_pixel(x + i, y + j, sprite[j * img.width + i]) != 0) {
          printf("vg_draw_xpm :: print_pixel error\n");
          return 1;
        }
      }
    }

    if (flag == 3) {
      flag = 4;
      goto WAIT;
    }

    br = 0;

    if (speed_aux < 0) {
      timer_int(-speed_aux * DEFAULT_FREQ / fr_rate, &br);
    }

    else
      timer_int(DEFAULT_FREQ / fr_rate, &br);

    if (br == 1)
      break;
    xa = x;
    ya = y;

    if (xi < xf) {
      flag = 1;
      x = x + speed;
      if (x >= xf) {
        x = xf;
        flag = 3;
      }
    }
    if (xi > xf) {
      flag = 1;
      x = x - speed;
      if (x <= xf) {
        x = xf;
        flag = 3;
      }
    }
    if (yi < yf) {
      flag = 2;
      y = y + speed;
      if (y >= yf) {
        y = yf;
        flag = 3;
      }
    }
    if (yi > yf) {
      flag = 2;
      y = y - speed;
      if (y <= yf) {
        y = yf;
        flag = 3;
      }
    }

  WAIT:
    if (flag == 4) {
      while (scancode != EXIT_CODE) {
        if ((g = driver_receive(ANY, &msg, &ipc_status)) != 0) {
          printf("driver_receive failed with: %d", g);
          continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:                            /* hardware interrupt notification */
              if (msg.m_notify.interrupts & BIT(1)) { /* subscribed interrupt */
                sys_inb(IN_BUF, &scancode);
                if (scancode & O_BUFFER_FULL) {
                  sys_inb(I_O_BUFFER, &scancode);
                }

                break;
                default:
                  break; /* no other notifications expected: do nothing */
              }
          }
        }
      }
    }
    if (flag == 4)
      break;
  }

  if (sys_irqrmpolicy(&hook_id_kbd) != 0)
    return 1;
  if (sys_irqrmpolicy(&hook_id_timer) != 0)
    return 1;
  vg_exit();
  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  	mmap_t map;
	 reg86_t r;
   struct vg_vbe_contr_info dat;
	phys_bytes buf;

	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) //alloc memory needed
	{
		printf("Erro em lm_alloc\n");//reboot minix if there is problem or do retrys but may not solve
		return -1;
	}

	memset(&r, 0, sizeof(reg86_t));
	buf = map.phys;

	r.ax = 0x4F00; // VBE get mode info (0x4F01)
	r.es = PB2BASE(buf); // PB2BASE Is a macro for computing the base of a segment, a 16-bit value, given a 32-bit linear address;
	r.di = PB2OFF(buf); // PB2OFF Is a macro for computing the offset with respect to the base of a segment, a 16-bit value, given a 32-bit linear address;

	r.intno = 0x10	;// (0x10)

	if (sys_int86(&r) != OK) { // call BIOS
			lm_free(&map); //free memory
			return -1;
	}

	if(r.ah == 01 || r.ah == 02 || r.ah == 03){ // (return status)
		lm_free(&map); //free memory
		return -1;
	}

	memcpy(&dat, map.virt, map.size);
	lm_free(&map);//free memory
	

 vg_vbe_contr_info_t info;
  
  strcpy(info.VBESignature, "VBE2");
  info.VBEVersion[1] = dat.VBEVersion[1];
  info.VBEVersion[0] = dat.VBEVersion[0];
  info.TotalMemory = 64*dat.TotalMemory;


  //lm_free(&dat);

  vg_display_vbe_contr_info(&info);
  return 0;
}
