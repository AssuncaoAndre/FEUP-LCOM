#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  *lsb=val&0xFF;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
  *msb=(val&0xFF00)>>8;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
  uint32_t b32;
 sys_inb(port,&b32);
 //printf("%x",b32);
 *value=b32&0xFF;
 //printf("%x\n",*value);
  

  //printf("%s is not yet implemented!\n", __func__);

  return 0;
}

/* a = 133
b = a & 0x0F
1000 0101
 */
