#include <stdint.h>
#include <stdbool.h>


/** @defgroup makecodes makecodes
 * @{
 *
 * Functions for recognizing makecodes. 
 */


#define facil 50 /**< @brief Dificulty Option 'FACIL' */
#define medio 100 /**< @brief Dificulty Option 'MEDIO' */
#define dificil 150 /**< @brief Dificulty Option 'DIFICIL' */
#define extreme 300/**< @brief Dificulty Option 'EXTREME' */

#define am 0x1e /**< @brief makecode 'a' */
#define bm 0x30 /**< @brief makecode 'b' */
#define cm 0x2e /**< @brief makecode 'c' */
#define dm 0x20 /**< @brief makecode 'd' */
#define em 0x12 /**< @brief makecode 'e' */
#define fm 0x21 /**< @brief makecode 'f' */
#define gm 0x22 /**< @brief makecode 'g' */
#define hm 0x23 /**< @brief makecode 'h' */
#define im 0x17 /**< @brief makecode 'i' */
#define jm 0x24 /**< @brief makecode 'j' */
#define km 0x25 /**< @brief makecode 'k' */
#define lm 0x26 /**< @brief makecode 'l' */
#define mm 0x32 /**< @brief makecode 'm' */
#define nm 0x31 /**< @brief makecode 'n' */
#define om 0x18 /**< @brief makecode 'o' */
#define pm 0x19 /**< @brief makecode 'p' */
#define qm 0x10 /**< @brief makecode 'q' */
#define rm 0x13 /**< @brief makecode 'r' */
#define sm 0x1f /**< @brief makecode 's' */
#define tm 0x14 /**< @brief makecode 't' */
#define um 0x16 /**< @brief makecode 'u' */
#define vm 0x2f /**< @brief makecode 'v' */
#define wm 0x11 /**< @brief makecode 'w' */
#define xm 0x2d /**< @brief makecode 'x' */
#define ym 0x15 /**< @brief makecode 'y' */
#define zm 0x2c /**< @brief makecode 'z' */


//pontuações de cada letra props to scrabble
#define apont 1 /**< @brief points for letter 'a' based on Scrabble */
#define bpont 3 /**< @brief points for letter 'b' based on Scrabble */
#define cpont 3 /**< @brief points for letter 'c' based on Scrabble */
#define dpont 2 /**< @brief points for letter 'd' based on Scrabble */
#define epont 1 /**< @brief points for letter 'e' based on Scrabble */
#define fpont 4 /**< @brief points for letter 'f' based on Scrabble */
#define gpont 2 /**< @brief points for letter 'g' based on Scrabble */
#define hpont 4 /**< @brief points for letter 'h' based on Scrabble */
#define ipont 1 /**< @brief points for letter 'i' based on Scrabble */
#define jpont 8 /**< @brief points for letter 'j' based on Scrabble */
#define kpont 5 /**< @brief points for letter 'k' based on Scrabble */
#define lpont 1 /**< @brief points for letter 'l' based on Scrabble */
#define mpont 3 /**< @brief points for letter 'm' based on Scrabble */
#define npont 1 /**< @brief points for letter 'n' based on Scrabble */
#define opont 1 /**< @brief points for letter 'o' based on Scrabble */
#define ppont 3 /**< @brief points for letter 'p' based on Scrabble */
#define qpont 10 /**< @brief points for letter 'q' based on Scrabble */
#define rpont 1 /**< @brief points for letter 'r' based on Scrabble */
#define spont 1 /**< @brief points for letter 's' based on Scrabble */
#define tpont 1 /**< @brief points for letter 't' based on Scrabble */
#define upont 1 /**< @brief points for letter 'u' based on Scrabble */
#define vpont 4 /**< @brief points for letter 'v' based on Scrabble */
#define wpont 4 /**< @brief points for letter 'w' based on Scrabble */
#define xpont 8 /**< @brief points for letter 'x' based on Scrabble */
#define ypont 4 /**< @brief points for letter 'y' based on Scrabble */
#define zpont 10 /**< @brief points for letter 'z' based on Scrabble */

/**
 * @brief Used to associate the makecode received from the keyboard to a letter
 *
 * @param makecode Makecode associated to a letter 
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
char make_to_char(uint32_t makecode);

/**
 * @brief Used to determine the score of a User based on each letter points on Scrabble
 *
 * @param letra Used letter 
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int char_to_pont(char letra);

/**@}*/
