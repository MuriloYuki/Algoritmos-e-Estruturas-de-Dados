#include <stdio.h>
#include "ListaComCursor.h"

int main(){
    ListaCursor *l= criar_lista();
    if(!l) return 1;

    int op, x, valor;

    while(scanf("%d", &op) == 1){
        switch(op){
            case 0:
                destruir_lista(l);
                return 0;
            case 1:
                scanf("%d", &x);
                inserir_antes(l, x);
                break;
            case 2:
                scanf("%d", &x);
                inserir_depois(l, x);
                break;
            case 3:
                remover_atual(l);
                break;
            case 4:
                if(obter_atual(l, &valor)){
                    printf("%d\n", valor);
                }else{
                    printf("ERRO\n");
                }
                break;
            case 5:
                mover_inicio(l);
                break;
            case 6:
                mover_fim(l);
                break;
            case 7:
                printf("%d\n", avancar_cursor(l));
                break;
            case 8:
                printf("%d\n", retroceder_cursor(l));
                break;
            case 9:
                scanf("%d", &x);
                printf("%d\n", mover_para_indice(l, x));
                break;
            case 10:
                printf("%d\n", tamanho_lista(l));
                break;
        }
    }

    destruir_lista(l);
    return 0;
}