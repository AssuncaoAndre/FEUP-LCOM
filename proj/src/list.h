#include <stdint.h>

/** @defgroup list list
 * @{
 *
 * Functions regarding Lists. 
 */

typedef struct _l_elemento
{
	/* string armazenada */
	int dif;
	uint32_t seg;
	uint32_t min;
	uint32_t hour;
	uint32_t day;
	uint32_t month;
	uint32_t year;
	/* apontadores para elementos vizinhos */
    struct _l_elemento *proximo;
    struct _l_elemento *anterior;
} l_elemento;

typedef struct
{
	l_elemento *inicio;
	l_elemento *fim;
	int tamanho;
} lista;

/**
 * @brief Creates a new list
 *
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
lista* lista_nova();
/**
 * @brief Inserts elements on the list
 *
 * @param lst List
 * @param seg Seconds
 * @param min Minutes
 * @param hour Hour
 * @param day Day
 * @param month Month		
 * @param year Year
 * @param valor Value
 * @param pos Position of the element
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
l_elemento* lista_insere(lista *lst, uint32_t seg, uint32_t min, uint32_t hour, uint32_t day, uint32_t month, uint32_t year,int valor, l_elemento *pos);
/**
 * @brief Deletes List
 *
 * @param lst List 
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
void lista_apaga(lista *lst);

/**@}*/

