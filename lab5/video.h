#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "math.h"
#define INDEXED 4
#pragma once
#pragma pack(1)
typedef struct {
  uint8_t VbeSignature[4];
  uint16_t VbeVersion;
  phys_bytes OemStringPtr;
  uint8_t Capabilities[4];
  phys_bytes VideoModePtr;
  uint16_t TotalMemory;
  uint16_t OemSoftwareRev;
  phys_bytes OemVendorNamePtr;
  phys_bytes OemProductNamePtr;
  phys_bytes OemProductRevPtr;
  uint8_t Reserved[222];
  uint8_t OemData[256];
} vbe_ctrl_info;
#pragma options align = reset

int get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p);
int(timer_int)(uint8_t time, uint8_t *flag);
int video_init(uint16_t mode);
int print_pixel(uint16_t x, uint16_t y, uint32_t color);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

#endif
