#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <lcom/lab5.h>

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>


/** @defgroup proj proj
 * @{
 *
 * Main functions developed in this file. 
 */


/**
 * @brief Sets RTC funcionality
 *
 * The RTC is used to read the current date and hour
 * The 10 best scores are stored with this information 
 * 
 * @param hour Used to read the hours
 * @param min Used to read the minutes
 * @param seg Used to read the seconds
 * @param day Used to read the day
 * @param month Used to read the month
 * @param year Used to read the year
 * 
 * @return Return 0 upon sucess and non-zero otherwise.
 */
void wait_valid_rtc(uint32_t* hour,uint32_t*min,uint32_t*seg,uint32_t*day,uint32_t*month,uint32_t*year);

/**
 * @brief Used for the determination of user's score
 *
 * @param pal Word chosen
 * 
 * @return Return 0 upon sucess and non-zero otherwise.
 */
int pontuacao (char *pal);

/**
 * @brief Main function of the project. 
 * 
 * The control of many IO devices is developed here.
 *
 * @param argc 
 * @param argv[]
 * 
 * @return Return 0 upon sucess and non-zero otherwise.
 */
int(proj_main_loop)(int argc, char *argv[]);

/**@}*/
