#include <stdlib.h>
#include "ListaComCursor.h"

struct lista_cursor{
    int qtd;
    int cap;
    int *dados;
    int cursor;
};

static int lsd_garante_capacidade(ListaCursor *l){
    if(!l) return 0;

    if(l->qtd < l->cap) return 1;

    if(l->cap == 0){
        l->dados= malloc(8*sizeof(int));
        if(!l->dados) return 0;
        l->cap= 8;
    }else{
        int nova_cap= l->cap*2;
        int *novo= realloc(l->dados, nova_cap*sizeof(int));
        if(!novo) return 0;
        l->dados= novo;
        l->cap= nova_cap;
    }

    return 1;
}

ListaCursor *criar_lista(){
    ListaCursor *l= malloc(sizeof(ListaCursor));
    if(!l) return NULL;

    l->dados= NULL;
    l->cap= 0;
    l->qtd= 0;
    l->cursor= -1;
    return l;
}

void destruir_lista(ListaCursor *l){
    if(!l) return;
    free(l->dados);
    l->dados= NULL;

    free(l);

    return;
}

int tamanho_lista(ListaCursor *l){
    if(!l) return 0;

    return l->qtd;
}

void mover_inicio(ListaCursor *l){
    if(!l) return;
    if(tamanho_lista(l) == 0) return;

    l->cursor= 0;
    return;
}

void mover_fim(ListaCursor *l){
    if(!l) return;
    if(tamanho_lista(l) == 0) return;

    l->cursor= l->qtd-1;
    return;
}

int avancar_cursor(ListaCursor *l){
    if(!l) return 0;
    if(tamanho_lista(l) == 0 || l->cursor == l->qtd-1) return 0;

    l->cursor++;
    return 1;
}

int retroceder_cursor(ListaCursor *l){
    if(!l) return 0;
    if(tamanho_lista(l) == 0 || l->cursor == 0) return 0;
    
    l->cursor--;
    return 1;
}

int mover_para_indice(ListaCursor *l, int indice){
    if(!l) return 0;
    if(indice >= l->qtd || indice < 0) return 0;

    l->cursor= indice;
    return 1;
}

int obter_atual(ListaCursor *l, int *valor_saida){
    if(!l) return 0;
    if(tamanho_lista(l) == 0) return 0;

    *valor_saida= l->dados[l->cursor];
    return 1;
}

int inserir_antes(ListaCursor *l, int valor){
    if(!l) return 0;
    if(!lsd_garante_capacidade(l)) return 0;

    if(tamanho_lista(l) == 0){
        l->dados[0]= valor;
        l->cursor= 0;
    }else{
        for(int i=l->qtd; i>l->cursor; i--){
            l->dados[i]= l->dados[i - 1];
        }
        l->dados[l->cursor]= valor;
        l->cursor++;
    }

    l->qtd++;
    return 1;
}

int inserir_depois(ListaCursor *l, int valor){
    if(!l) return 0;

    if(!lsd_garante_capacidade(l)) return 0;

    if(tamanho_lista(l) == 0){
        l->dados[0]= valor;
        l->cursor= 0;
    }else{
        for(int i=l->qtd; i>l->cursor+1; i--){
            l->dados[i]= l->dados[i-1];
        }

        l->dados[l->cursor+1]= valor;
    }

    l->qtd++;
    return 1;
}

int remover_atual(ListaCursor *l){
    if(!l) return 0;
    if(tamanho_lista(l) == 0) return 0;
    
    for(int i=l->cursor; i<l->qtd-1; i++){
        l->dados[i]= l->dados[i+1];
    }
    if(l->cursor == l->qtd-1){
        l->cursor--;
    }
    if(l->qtd == 1){
        l->cursor= -1;
    }
    
    l->qtd--;
    return 1;
}

