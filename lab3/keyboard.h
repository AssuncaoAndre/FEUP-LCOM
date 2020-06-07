
#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

int kbd_subscribe_int(uint8_t*bit_no);

int kbd_unsubscribe_int();

void(kbc_ih)();

#endif /* KEYBOARD.H */
