#include <stdio.h>
#include <string.h>

/*
    Definição da estrutura Territorio.
    Esta struct representa cada território do jogo,
    contendo:
        - nome: nome do território
        - cor: cor do exército que controla o território
        - tropas: número de tropas alocadas
*/
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Vetor para armazenar 5 territórios
    Territorio territorios[5];

    // Entrada de dados
    printf("=== Cadastro de Territórios do Jogo War ===\n\n");

    for (int i = 0; i < 5; i++) {
        printf("Cadastro do território %d:\n", i + 1);

        // Lê o nome do território
        printf("Digite o nome do território: ");
        scanf("%29s", territorios[i].nome);

        // Lê a cor do exército
        printf("Digite a cor do exército: ");
        scanf("%9s", territorios[i].cor);

        // Lê a quantidade de tropas
        printf("Digite o número de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\n");
    }

    // Exibição dos dados
    printf("\n=== Territórios Cadastrados ===\n\n");

    for (int i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
        printf("-----------------------------\n");
    }

    return 0;
}
