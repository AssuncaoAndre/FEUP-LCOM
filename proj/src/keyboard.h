
#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>


/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for programming the Keyboard. 
 */


/**
 * @brief Subscribes Keyboard
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_subscribe_int(uint8_t*bit_no);
/**
 * @brief Unsubscribes Keyboard Interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_int();
/**
 * @brief Handles keyboard interrupts (C implementation)
 * Reads the status register and the output buffer (OB).
 * If there was some error, the byte read from the OB should   
 *         be discarded.
 *
 * All communication with other code must be done via global  
 *        variables, static if possible.
 * 
 * Must be defined using parenthesis around the function name.
 * @param IH's take no arguments
 * 
 * @return IH's return no values
 */
void(kbc_ih)();

#endif /* KEYBOARD.H */

/**@}*/
