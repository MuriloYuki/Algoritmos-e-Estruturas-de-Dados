#ifndef LISTA_COM_CURSOR_H
#define LISTA_COM_CURSOR_H

typedef struct lista_cursor ListaCursor;

ListaCursor *criar_lista(void);
void destruir_lista(ListaCursor *l);

void mover_inicio(ListaCursor *l);
void mover_fim(ListaCursor *l);

int avancar_cursor(ListaCursor *l);
int retroceder_cursor(ListaCursor *l);

int mover_para_indice(ListaCursor *l, int indice);

int obter_atual(ListaCursor *l, int *valor_saida);

int inserir_antes(ListaCursor *l, int valor);
int inserir_depois(ListaCursor *l, int valor);
int remover_atual(ListaCursor *l);

int tamanho_lista(ListaCursor *l);

#endif 
