#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 20
#define NOME_LEN 30
#define TIPO_LEN 20

typedef struct {
    char nome[NOME_LEN];
    char tipo[TIPO_LEN];
    int prioridade;
} Componente;

/* -----------------------------------------
   Funções auxiliares
-------------------------------------------*/
void limparBufferString(char *str, int tam) {
    size_t ln = strlen(str);
    if (ln > 0 && str[ln - 1] == '\n')
        str[ln - 1] = '\0';
}

void mostrarComponentes(Componente arr[], int n) {
    printf("\n===== LISTA DE COMPONENTES =====\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] Nome: %-25s | Tipo: %-15s | Prioridade: %d\n",
               i, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
    printf("================================\n\n");
}

/* -----------------------------------------
   Algoritmos de Ordenação
-------------------------------------------*/
long bubbleSortNome(Componente arr[], int n) {
    long comparacoes = 0;
    Componente temp;
    int trocou;

    for (int i = 0; i < n - 1; i++) {
        trocou = 0;
        for (int j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (strcmp(arr[j].nome, arr[j + 1].nome) > 0) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }

    return comparacoes;
}

long insertionSortTipo(Componente arr[], int n) {
    long comparacoes = 0;

    for (int i = 1; i < n; i++) {
        Componente chave = arr[i];
        int j = i - 1;

        while (j >= 0 && strcmp(arr[j].tipo, chave.tipo) > 0) {
            comparacoes++;
            arr[j + 1] = arr[j];
            j--;
        }
        if (j >= 0) comparacoes++;

        arr[j + 1] = chave;
    }

    return comparacoes;
}

long selectionSortPrioridade(Componente arr[], int n) {
    long comparacoes = 0;

    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (arr[j].prioridade < arr[min].prioridade)
                min = j;
        }
        if (min != i) {
            Componente temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
    }

    return comparacoes;
}

/* -----------------------------------------
   Busca binária (apenas após ordenar por nome)
-------------------------------------------*/
int buscaBinariaPorNome(Componente arr[], int n, char nome[]) {
    int ini = 0, fim = n - 1, meio;

    while (ini <= fim) {
        meio = (ini + fim) / 2;
        int cmp = strcmp(nome, arr[meio].nome);

        if (cmp == 0) return meio;
        else if (cmp < 0) fim = meio - 1;
        else ini = meio + 1;
    }

    return -1;
}

/* -----------------------------------------
   Funções de Remoção (DESCARTE)
-------------------------------------------*/
void removerComponentePorIndice(Componente arr[], int *n, int indice) {
    if (indice < 0 || indice >= *n) {
        printf("Índice inválido!\n");
        return;
    }

    for (int i = indice; i < *n - 1; i++) {
        arr[i] = arr[i + 1];
    }

    (*n)--;
    printf("Componente removido com sucesso!\n");
}

void removerComponentePorNome(Componente arr[], int *n, const char nome[]) {
    for (int i = 0; i < *n; i++) {
        if (strcmp(arr[i].nome, nome) == 0) {
            removerComponentePorIndice(arr, n, i);
            return;
        }
    }
    printf("Nenhum componente com o nome \"%s\" foi encontrado.\n", nome);
}

/* -----------------------------------------
   Função principal
-------------------------------------------*/
int main() {
    Componente componentes[MAX];
    int n = 0;
    int opc;
    int ordenadoPorNome = 0;

    char buffer[64];

    do {
        printf("\n=========== MENU PRINCIPAL ===========\n");
        printf("1. Cadastrar componentes\n");
        printf("2. Descartar componentes\n");  // <--- NOVA ORDEM
        printf("3. Mostrar componentes\n");
        printf("4. Bubble Sort (por nome)\n");
        printf("5. Insertion Sort (por tipo)\n");
        printf("6. Selection Sort (por prioridade)\n");
        printf("7. Busca binária (por nome) – após ordenar por nome\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        fgets(buffer, sizeof(buffer), stdin);
        opc = atoi(buffer);

        switch (opc) {

        // --------------------------------
        case 1: {
            if (n >= MAX) {
                printf("Limite máximo atingido!\n");
                break;
            }

            printf("\nCadastro do componente:\n");

            printf("Nome: ");
            fgets(componentes[n].nome, NOME_LEN, stdin);
            limparBufferString(componentes[n].nome, NOME_LEN);

            printf("Tipo: ");
            fgets(componentes[n].tipo, TIPO_LEN, stdin);
            limparBufferString(componentes[n].tipo, TIPO_LEN);

            printf("Prioridade (1 a 10): ");
            fgets(buffer, sizeof(buffer), stdin);
            componentes[n].prioridade = atoi(buffer);

            n++;
            ordenadoPorNome = 0;
            printf("Componente cadastrado!\n");
            break;
        }

        // --------------------------------
        case 2: {
            if (n == 0) {
                printf("Nenhum componente para remover.\n");
                break;
            }

            printf("\n=== DESCARTE DE COMPONENTES ===\n");
            printf("1. Remover por índice\n");
            printf("2. Remover por nome\n");
            printf("Escolha: ");
            fgets(buffer, sizeof(buffer), stdin);
            int modo = atoi(buffer);

            if (modo == 1) {
                mostrarComponentes(componentes, n);
                printf("Informe o índice: ");
                fgets(buffer, sizeof(buffer), stdin);
                int idx = atoi(buffer);
                removerComponentePorIndice(componentes, &n, idx);
                ordenadoPorNome = 0;
            }
            else if (modo == 2) {
                printf("Informe o nome: ");
                char nomeRem[NOME_LEN];
                fgets(nomeRem, NOME_LEN, stdin);
                limparBufferString(nomeRem, NOME_LEN);
                removerComponentePorNome(componentes, &n, nomeRem);
                ordenadoPorNome = 0;
            }
            else {
                printf("Opção inválida.\n");
            }
            break;
        }

        // --------------------------------
        case 3:
            mostrarComponentes(componentes, n);
            break;

        // --------------------------------
        case 4: {
            clock_t ini = clock();
            long comp = bubbleSortNome(componentes, n);
            clock_t fim = clock();
            ordenadoPorNome = 1;

            printf("\nBubble Sort concluído!\nComparações: %ld\nTempo: %.6f s\n",
                   comp, (double)(fim - ini) / CLOCKS_PER_SEC);
            break;
        }

        // --------------------------------
        case 5: {
            clock_t ini = clock();
            long comp = insertionSortTipo(componentes, n);
            clock_t fim = clock();
            ordenadoPorNome = 0;

            printf("\nInsertion Sort concluído!\nComparações: %ld\nTempo: %.6f s\n",
                   comp, (double)(fim - ini) / CLOCKS_PER_SEC);
            break;
        }

        // --------------------------------
        case 6: {
            clock_t ini = clock();
            long comp = selectionSortPrioridade(componentes, n);
            clock_t fim = clock();
            ordenadoPorNome = 0;

            printf("\nSelection Sort concluído!\nComparações: %ld\nTempo: %.6f s\n",
                   comp, (double)(fim - ini) / CLOCKS_PER_SEC);
            break;
        }

        // --------------------------------
        case 7: {
            if (!ordenadoPorNome) {
                printf("É necessário ordenar por nome antes de usar busca binária!\n");
                break;
            }

            printf("Nome do componente para buscar: ");
            char busca[NOME_LEN];
            fgets(busca, NOME_LEN, stdin);
            limparBufferString(busca, NOME_LEN);

            int pos = buscaBinariaPorNome(componentes, n, busca);

            if (pos >= 0)
                printf("Componente encontrado na posição %d!\n", pos);
            else
                printf("Componente não encontrado!\n");

            break;
        }

        // --------------------------------
        case 8:
            printf("Encerrando...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }

    } while (opc != 8);

    return 0;
}
