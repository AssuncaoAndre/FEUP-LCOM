#ifndef MOUSE_H
#define MOUSE_H
#include <stdint.h>
#include <stdio.h>

/** @defgroup mouse mouse
 * @{
 *
 * Functions for programming the Mouse. 
 */

extern struct packet p_packet;
extern uint32_t status;
typedef enum { INIT,
               UP_RIGHT, 
               HOLD,
               DOWN_RIGHT,
               END } state;

/**
 * @brief Subscribes Mouse Interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int subscribe_mouse_int(uint8_t *bit_no);
/**
 * @brief Unsubscribes Mouse Interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int unsubscribe_mouse_int();
/**
 * @brief Enables Mouse
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_enable();
/**
 * @brief Disables Mouse
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_disable();
/**
 * @brief Writes a command to the mouse (using the KBC controller)
 *
 * @param cmd - Command to write to mouse
 *
 * @return 0 on success, and non-zero otherwise
 */
int(kbc_write)(uint32_t cmd);
/**
 * @brief Handles mouse interrutps.
 * 
 * Reads the status register and the output buffer (OB).
 * 
 * If there was some error, the byte read from the OB should be discarded.
 * 
 * Must be defined using parenthesis around the function name.
 * 
 * @param  IHs take no arguments
 * 
 * @return IHs return no values
 */
void(mouse_ih)(void);
/**
 * @brief Reads the packets
 * 
 * @param packet Struct packet 
 * 
 * @return 0 on success, and non-zero otherwise
 */
void packet_processing(struct packet *packet);
/**
 * @brief Disables stream mode data reporting.
 * 
 * Disables stream mode data reporting, by sending the respective command to the mouse.
 * 
 * @return 0 on success, and non-zero otherwise
 */
int mouse_disable_data_reporting();
/**
 * @brief Enables stream mode data reporting.
 * 
 * Enables stream mode data reporting, by sending the respective command to the mouse.
 * 
 * @return 0 on success, and non-zero otherwise
 */
int mouse_enable_data_rep();


#endif /* MOUSE_H */

/**@}*/
