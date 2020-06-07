#ifndef MOUSE_H
#define MOUSE_H
#include <stdint.h>
#include <stdio.h>

extern struct packet p_packet;
extern uint32_t status;
typedef enum { INIT,
               UP_RIGHT,
               HOLD,
               DOWN_RIGHT,
               END } state;

int subscribe_mouse_int(uint8_t *bit_no);
int unsubscribe_mouse_int();
int mouse_enable();
int mouse_disable();
int(kbc_write)(uint32_t cmd);
void(mouse_ih)(void);
void packet_processing(struct packet *packet);
int mouse_disable_data_reporting();
int mouse_enable_data_rep();
void check_i_v(struct mouse_ev gesture, uint8_t x_len, uint8_t tolerance);

#endif /* MOUSE_H */
