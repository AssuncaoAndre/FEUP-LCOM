/*  #ifndef I8042_H
#define I8042_H  */

#define IRQ_KB 1
#define DELAY_US 20000
#define EXIT_CODE 0x81
#define READ_CMD 0x20
#define IN_BUF 0x64
#define STAT_REG 0x64
#define I_O_BUFFER 0x60
#define O_BUFFER_FULL 0x01
#define PAR_ERR 0x80
#define TO_ERR BIT(6)
// #define PAR_ERR BIT(7)
#define TWO_BYTES 0xe0

#define OBF BIT(0)
#define IBF BIT(1)
#define SYS BIT(2)
#define A2 BIT(3)
#define INH BIT(4)
#define AUX BIT(5)
#define TIMEOUT BIT(6)
#define PARITY BIT(7)

#define IRQ_MOUSE 12
#define MOUSE_R_CMD 0x20
#define MOUSE_W_CMD 0x60
#define MOUSE_DISABLE 0xA7
#define MOUSE_DISABLE_DR 0xF5
#define MOUSE_ENABLE 0xA8
#define MOUSE_ENABLE_DR 0xF4
#define MOUSE_CHECK_INT 0xA9
#define MOUSE_WRITE_BYTE 0xD4
#define MOUSE_STATUS_REQ 0xE9
#define RESET 0xFF
#define RESEND 0xFE
#define SET_REMOTE_MODE 0xF0
#define SETDEFAULT 0XF6
#define DISABLE_DR 0XF5
#define ENABLE_DR 0xF4
#define READ_DATA 0XEB
#define SET_STREAM_MODE 0XEA


#define L_BUTTON BIT(0)
#define R_BUTTON BIT(1)
#define M_BUTTON BIT(2)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVFL BIT(6)
#define Y_OVFL BIT(7)

#define INIT_CMD 0xD4

#define ACK 0xFA   //if everything OK
#define NACK 0xFE  //if invalid byte (may be because of a serial communication error)
#define ERROR 0xFC //second consecutive invalid byte

//#endif /* I8042_H */
