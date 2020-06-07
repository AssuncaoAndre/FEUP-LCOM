#include <stdint.h>
#include <stdbool.h>

/** @defgroup menu menu
 * @{
 *
 * Functions for the game's menus.
 */

#define jogar 1 /**< @brief Main Menu 'JOGAR' option */
#define opcoes 2 /**< @brief Main Menu 'OPCOES' option */
#define highscores 3 /**< @brief Main Menu 'HIGHSCORES' option */
#define sair 4 /**< @brief Main Menu & Options Menu 'SAIR' option */

#define dificuldade 1 /**< @brief Options Menu 'DIFICULDADE' option */
#define jogadores 2 /**< @brief Options Menu 'JOGADORES' option */
#define cores 3 /**< @brief Options Menu 'OPCOES' option */

/**
 * @brief Displays the main menu with its funcionalities.
 *
 * @param opcao Main menu's option: 'JOGAR', 'HIGHSCORES', 'OPCOES', SAIR'.
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int menu_init(int *opcao);
/**
 * @brief Displays the options menu with its funcionalities.
 *
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int opcoes_menu();
/**
 * @brief Displays the colours menu and its funcionalities.
 *
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int cores_menu();
/**
 * @brief Displays the dificulty menu with its funcionalities.
 *
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int dificuldade_menu();

/**@}*/
