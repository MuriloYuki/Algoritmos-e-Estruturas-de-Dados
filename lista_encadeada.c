#include <stdlib.h>
#include "ListaComCursor.h"

typedef struct node{
    int data;
    struct node *next;
    struct node *ant;
} Node;

struct lista_cursor{
    Node *head;
    Node *cursor;
    Node *fim;
    int qtd;
};

ListaCursor *criar_lista(){
    ListaCursor *l= malloc(sizeof(ListaCursor));
    if(!l) return NULL;

    l->head= NULL;
    l->fim= NULL;
    l->qtd= 0;
    l->cursor= NULL;
    return l;
}

void destruir_lista(ListaCursor *l){
    if(!l) return;

    Node *tmp= l->head;
    while(tmp != NULL){
        Node *rem= tmp;
        tmp= tmp->next;
        free(rem);
    }
    l->cursor= NULL;

    free(l);
    return;
}

int tamanho_lista(ListaCursor *l){
    if(!l) return -1;

    return l->qtd;
}

void mover_inicio(ListaCursor *l){
    if(!l) return;
    if(tamanho_lista(l) == 0) return;

    l->cursor= l->head;
    return;
}

void mover_fim(ListaCursor *l){
    if(!l) return;
    if(tamanho_lista(l) == 0) return;
    l->cursor= l->fim;
    return;
}

int avancar_cursor(ListaCursor *l){
    if(!l) return -1;
    if(tamanho_lista(l) == 0 || l->cursor == l->fim) return 0;

    l->cursor= l->cursor->next;
    return 1;
}

int retroceder_cursor(ListaCursor *l){
    if(!l) return -1;
    if(tamanho_lista(l) == 0 || l->cursor == l->head) return 0;

    l->cursor= l->cursor->ant;

    return 1;
}

int mover_para_indice(ListaCursor *l, int indice){
    if(!l) return -1;
    if(indice >= l->qtd || indice < 0) return 0;

    if(indice > l->qtd/2){
        Node *tmp= l->fim;
        for(int i=l->qtd-1; i>indice; i--){
            tmp= tmp->ant;
        }
        l->cursor= tmp;
    }else{
        Node *tmp= l->head;
        for(int i=0; i<indice; i++){
        tmp= tmp->next;
        }
        l->cursor= tmp;
    }

    return 1;
}

int obter_atual(ListaCursor *l, int *valor_saida){
    if(!l) return -1;
    if(tamanho_lista(l) == 0) return 0;

    *valor_saida= l->cursor->data;
    return 1;
}

int inserir_antes(ListaCursor *l, int valor){
    if(!l) return -1;

    Node *novo= malloc(sizeof(Node));
    if(!novo) return 0;

    novo->data= valor;
    if(tamanho_lista(l) == 0){
        l->head= novo;
        l->fim= novo;
        l->cursor= novo;

        novo->next= NULL;
        novo->ant= NULL;
        
        l->qtd++;

    }else if(l->cursor == l->head){
        novo->next= l->head;
        novo->ant= NULL;

        l->head->ant= novo;
        l->head= novo;

        l->qtd++;

    }else{
        novo->next= l->cursor;
        novo->ant= l->cursor->ant;
        
        l->cursor->ant= novo;
        novo->ant->next= novo;

        l->qtd++;
    }
    
    return 1;
}

int inserir_depois(ListaCursor *l, int valor){
    if(!l) return -1;

    Node *novo= malloc(sizeof(Node));
    if(!novo) return 0;

    novo->data= valor;
    if(tamanho_lista(l) == 0){
        l->head= novo;
        l->fim= novo;
        l->cursor= novo;

        novo->next= NULL;
        novo->ant= NULL;

        l->qtd++;

    }else if(l->cursor == l->fim){
        l->cursor->next= novo;
        l->fim= novo;

        novo->ant= l->cursor;
        novo->next= NULL;
        
        l->qtd++;

    }else{
        novo->ant= l->cursor;
        novo->next= novo->ant->next;

        novo->next->ant= novo;
        l->cursor->next= novo;

        l->qtd++;
    }
    return 1;
}

int remover_atual(ListaCursor *l){
    if(!l) return -1;
    if(tamanho_lista(l) == 0) return 0;

    Node *rem= l->cursor;

    if(l->head == l->fim){
        l->head= NULL;
        l->fim= NULL;
        l->cursor= NULL;
    }else if(l->cursor == l->head){
        l->head= rem->next;
        l->head->ant= NULL;
        l->cursor= l->head;
    }else if(l->cursor == l->fim){
        l->fim= rem->ant;
        l->fim->next= NULL;
        l->cursor= l->fim;
    }else{
        rem->ant->next= rem->next;
        rem->next->ant= rem->ant;
        l->cursor= rem->next;
    }

    free(rem);
    l->qtd--;

    return 1;
}