/* #ifndef _VIDEO_H_
#define _VIDEO_H_ */

#include "math.h"


/** @defgroup video video
 * @{
 *
 * Functions for programming the graphics. 
 */

#define INDEXED 4
#pragma once
#pragma pack(1)

/**
 * @brief Struct with selected fields of VBE 2.0's VBEInfoBlock struct.
 */
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

/**
 * @brief Returns information on the input VBE mode, including screen dimensions, color
 * depth and VRAM physical address.
 * 
 * Initializes packed vbe_mode__info_t structure passed as an address with the VBE
 * information on the input mode, by calling VBE function 0x01, Return VBE Mode
 * Information, and copying the ModeInfoBlock struct returned by that function.
 *
 * @param mode mode whose information should be returned  
 * @param vmi_p address of vbe_mode_info_t structure to be initialized 
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Sets Mouse funcionalities.
 *
 * @param x horizontal coordinate of the initial point 
 * @param y vertical coordinate of the initial point 
 * @param click mouse click
 * 
 * @return Return 0 upon sucess and non-zero otherwise.
 */
int(mouse_int)( int16_t *x, int16_t *y, int *click);
/**
 * @brief Initializes the video module in graphics mode.
 *
 * @param mode 16-bit VBE mode to set 
 * 
 * @return Virtual address VRAM was mapped to NULL, upon failure.
 */
int video_init(uint16_t mode);
/**
 * @brief Used to print correspondent pixel
 * 
 * @param x horizontal coordinate of the initial point 
 * @param y vertical coordinate of the initial point 
 * @param color chosen color
 * 
 * @return Virtual address VRAM was mapped to NULL, upon failure.
 */

int print_pixel(uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief Draws a horizontal line by changing the video RAM.
 * Draws a horizontal line with the specified length and color
 * starting at the specified coordinates.
 * Note that the x coordinates start at 0 and increase from
 * left to right
 * Likewise the y coordinates start at 0 and increase from top
 * to bottom
 * 
 * @param x horizontal coordinate of the initial point 
 * @param y vertical coordinate of the initial point
 * @param len	line's length in pixels
 * @param color	color to set pixel
 * 
 * @return 0 on success, non zero otherwise
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief Draws a vertical line by changing the video RAM.
 * Draws a vertical line with the specified length and color
 * starting at the specified coordinates.
 * Note that the x coordinates start at 0 and increase from
 * left to right
 * Likewise the y coordinates start at 0 and increase from top
 * to bottom
 * 
 * @param x horizontal coordinate of the initial point 
 * @param y vertical coordinate of the initial point
 * @param len	line's length in pixels
 * @param color	color to set the pixel
 * 
 * @return 0 on success, non zero otherwise
 */
int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief Draws a filled rectangle by changing the video RAM.
 * Draws a filled rectangle with the specified width, height
 * and color, starting at the specified coordinates.
 *
 * Note that the x coordinates start at 0 and increase from
 * left to right
 * Likewise the y coordinates start at 0 and increase from top
 * to bottom
 * 
 * @param x horizontal coordinate of the rectangle's top-left vertex
 * @param y vertical coordinate of the rectangle's top-left vertex
 * @param width	rectangle's width in pixels
 * @param height	rectangle's height in pixels
 * @param color color to set the pixel
 * 
 * @return 0 on success, non zero otherwise
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
 * @brief Function where the sprites are loaded 
 * 
 * @param x horizontal coordinate of the initial point 
 * @param y vertical coordinate of the initial point
 * @param str 
 * @param tam 
 * 
 * @return 0 on success, non zero otherwise
 */
int v_sprintf(int x, int y, char *str, int tam);
/**
 * @brief Draws the states of the Hangman 
 * 
 * Starts with an empty Hang.  
 * As the user's failed tries increase, the Man's body appears
 * If the failed tries are equal 6, it means the Hangman is complete and the game is over.
 * 
 * @param tentativa User's tries, related to the hangman fases
 * 
 * @return 0 on success, non zero otherwise
 */
int forca_draw(int tentativa);

/* #endif */
/**@}*/
