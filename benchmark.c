#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ListaComCursor.h"

#define EXECUCOES 10
#define OPERACOES 100

/* Ajuda a manter os resultados das consultas observaveis. */
static volatile long long resultado = 0;

/* Monta a lista em ordem, fora da medicao. */
int preencher_lista(ListaCursor *l, int n) {
    for (int i = 0; i < n; i++) {
        if (inserir_depois(l, i) != 1) return 0;
        if (i > 0 && avancar_cursor(l) != 1) return 0;
    }
    return tamanho_lista(l) == n;
}

/* Evita o vies de rand() % n. Exige RAND_MAX >= n - 1. */
void gerar_indices(int indices[], int n, int repeticao) {
    srand(12345u + (unsigned)n + (unsigned)repeticao);
    long long faixa = (long long)RAND_MAX + 1;
    long long limite = faixa - faixa % n;
    for (int i = 0; i < OPERACOES; i++) {
        int x;
        do {
            x = rand();
        } while (x >= limite);
        indices[i] = x % n;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2 || (strcmp(argv[1], "sequencial") != 0 &&
                      strcmp(argv[1], "encadeada") != 0)) {
        fprintf(stderr, "Uso: %s sequencial|encadeada\n", argv[0]);
        return 1;
    }

    int tamanhos[] = {1000, 50000, 100000, 500000, 1000000};
    const char *nomes[] = {"insercoes_inicio", "acessos_aleatorios",
        "remocoes_inicio", "remocoes_meio", "percurso_completo"};

    /* Protege contra ambientes em que rand() tem uma faixa muito pequena. */
    if (RAND_MAX < 999999) {
        fprintf(stderr, "Faixa de rand insuficiente. Execute em Linux/WSL.\n");
        return 1;
    }

    /* A saida padrao pode ser redirecionada diretamente para um CSV. */
    printf("Implementacao,Experimento,N,Execucoes,TempoMedioNs\n");

    for (int t = 0; t < 5; t++) {
        int n = tamanhos[t];
        for (int caso = 0; caso < 5; caso++) {
            double tempo_total = 0;

            for (int repeticao = 1; repeticao <= EXECUCOES; repeticao++) {
                ListaCursor *l = criar_lista();
                if (!l) {
                    fprintf(stderr, "Falha ao criar lista.\n");
                    return 1;
                }
                if (!preencher_lista(l, n)) {
                    destruir_lista(l);
                    fprintf(stderr, "Falha ao preencher lista.\n");
                    return 1;
                }

                int indices[OPERACOES];
                long long esperado = 0;
                if (caso == 1) {
                    gerar_indices(indices, n, repeticao);
                    for (int i = 0; i < OPERACOES; i++) esperado += indices[i];
                }
                if (caso == 4) esperado = (long long)n * (n - 1) / 2;

                int sucesso = 1;
                if (caso == 2 || caso == 4) mover_inicio(l);
                if (caso == 3) sucesso = mover_para_indice(l, n / 2) == 1;
                if (!sucesso) {
                    destruir_lista(l);
                    fprintf(stderr, "Falha ao posicionar cursor.\n");
                    return 1;
                }

                struct timespec inicio, fim;
                int valor = 0;
                long long soma = 0;
                if (clock_gettime(CLOCK_MONOTONIC, &inicio) != 0) {
                    destruir_lista(l);
                    perror("clock_gettime");
                    return 1;
                }

                /* Somente o bloco de operacoes fica entre os dois instantes. */
                switch (caso) {
                    case 0:
                        for (int i = 0; i < OPERACOES && sucesso; i++) {
                            mover_inicio(l);
                            sucesso = inserir_antes(l, -(i + 1)) == 1;
                        }
                        break;
                    case 1:
                        for (int i = 0; i < OPERACOES && sucesso; i++) {
                            sucesso = mover_para_indice(l, indices[i]) == 1;
                            if (sucesso) sucesso = obter_atual(l, &valor) == 1;
                            if (sucesso) soma += valor;
                        }
                        break;
                    case 2:
                    case 3:
                        for (int i = 0; i < OPERACOES && sucesso; i++)
                            sucesso = remover_atual(l) == 1;
                        break;
                    case 4:
                        /* Visita inclusive o primeiro elemento. */
                        for (int i = 0; i < n && sucesso; i++) {
                            sucesso = obter_atual(l, &valor) == 1;
                            if (sucesso) soma += valor;
                            if (sucesso && i < n - 1)
                                sucesso = avancar_cursor(l) == 1;
                        }
                        break;
                }

                if (clock_gettime(CLOCK_MONOTONIC, &fim) != 0) {
                    destruir_lista(l);
                    perror("clock_gettime");
                    return 1;
                }
                /* Mesma ideia de tempo.c, mas usando nanossegundos. */
                double tempo = (fim.tv_sec - inicio.tv_sec) * 1e9
                             + (fim.tv_nsec - inicio.tv_nsec);

                /* Validacoes e liberacao ficam fora da medicao. */
                if (caso == 0) sucesso = sucesso && tamanho_lista(l) == n + OPERACOES;
                if (caso == 2 || caso == 3)
                    sucesso = sucesso && tamanho_lista(l) == n - OPERACOES;
                if (caso == 1 || caso == 4) sucesso = sucesso && soma == esperado;
                resultado += soma;
                destruir_lista(l);

                if (!sucesso) {
                    fprintf(stderr, "Falha: %s, N=%d, repeticao=%d.\n",
                            nomes[caso], n, repeticao);
                    return 1;
                }
                tempo_total += tempo;
            }

            double media = tempo_total / EXECUCOES;
            printf("%s,%s,%d,%d,%.2f\n", argv[1], nomes[caso], n, EXECUCOES, media);
            fprintf(stderr, "%s | N=%d | media=%.2f ns\n", nomes[caso], n, media);
        }
    }
    return 0;
}
