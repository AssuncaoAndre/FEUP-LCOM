#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

lista* lista_nova()
{
	/* cria lista */
	lista *lst = (lista*) malloc(sizeof(lista));
	if(lst == NULL)
		return NULL;

	/* lista esta' vazia */
	lst->inicio = NULL;
	lst->fim = NULL;
	lst->tamanho = 0;

  return lst;
}

l_elemento* novo_elemento(uint32_t seg, uint32_t min, uint32_t hour, uint32_t day, uint32_t month,
	uint32_t year,int valor)
{
	/* aloca memoria para a estrutura lista */
	l_elemento *item = (l_elemento *) malloc(sizeof(l_elemento));
	if(item == NULL)
		return NULL;



	/* copia valor */
	item->dif=valor;
	item->seg=seg;
	item->min=min;
	item->hour=hour;
	item->day=day;
	item->month=month;
	item->year=year;

	/* item ainda nao tem proximo */
  item->proximo = NULL;
  item->anterior = NULL;

	return item;
}

l_elemento* lista_insere(lista *lst, uint32_t seg, uint32_t min, uint32_t hour, uint32_t day, uint32_t month,
	uint32_t year,int valor, l_elemento *pos)
{
	l_elemento *curr = NULL;

	if (lst == NULL )
		return NULL;

	/* cria novo item */
	curr = novo_elemento(seg,min,hour,day,month,year,valor);

	if (curr == NULL)
        return NULL;

	lst->tamanho++;

	/* caso especial: inserir no fim da lista */
	if(pos == NULL)
	{
		/* se e' primeiro l_elemento */
		if (lst->inicio == NULL)
		{
			lst->inicio = lst->fim = curr;
		}
		else
		{
			curr->anterior = lst->fim;
			lst->fim->proximo = curr;
			lst->fim = curr;
		}
		return curr;
	}
	return NULL;
}

void lista_apaga(lista *lst)
{
	l_elemento *aux,*curr;
	curr=lst->inicio;
	while(curr->proximo!=NULL)
	{
		aux=curr;
		curr=curr->proximo;
		free(aux);
	}
	free(lst);
}


