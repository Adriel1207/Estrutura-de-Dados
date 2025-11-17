#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Configurações das capacidades */
#define QUEUE_CAP 5
#define STACK_CAP 3

/* Tipo de peça */
typedef struct {
    char nome;  /* 'I', 'O', 'T', 'L' */
    int id;     /* id único incremental */
} Peca;

/* Fila circular */
typedef struct {
    Peca dados[QUEUE_CAP];
    int front;  /* índice do elemento da frente */
    int count;  /* número de elementos na fila */
} Fila;

/* Pilha */
typedef struct {
    Peca dados[STACK_CAP];
    int top;    /* índice de próximo livre; top==0 => pilha vazia */
} Pilha;

/* Contador global para id das peças */
int proxId = 0;

/* Prototipos */
void iniciarFila(Fila *q);
void iniciarPilha(Pilha *p);
Peca gerarPeca();
int filaVazia(Fila *q);
int filaCheia(Fila *q);
void enqueue(Fila *q, Peca item);
Peca dequeue(Fila *q);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void push(Pilha *p, Peca item);
Peca pop(Pilha *p);
void mostrarEstado(Fila *q, Pilha *p);
void trocarFrenteTopo(Fila *q, Pilha *p);
void trocaMultipla3(Fila *q, Pilha *p);
void limparBufferEntrada();

/* Implementações */

void iniciarFila(Fila *q) {
    q->front = 0;
    q->count = 0;
}

void iniciarPilha(Pilha *p) {
    p->top = 0;
}

Peca gerarPeca() {
    Peca x;
    int r = rand() % 4;
    switch (r) {
        case 0: x.nome = 'I'; break;
        case 1: x.nome = 'O'; break;
        case 2: x.nome = 'T'; break;
        default: x.nome = 'L'; break;
    }
    x.id = proxId++;
    return x;
}

int filaVazia(Fila *q) {
    return q->count == 0;
}

int filaCheia(Fila *q) {
    return q->count == QUEUE_CAP;
}

void enqueue(Fila *q, Peca item) {
    if (filaCheia(q)) {
        /* normalmente não deve acontecer se geramos após remoções,
           mas protegemos o código */
        fprintf(stderr, "Erro: tentativa de enfileirar em fila cheia.\n");
        return;
    }
    int rear = (q->front + q->count) % QUEUE_CAP;
    q->dados[rear] = item;
    q->count++;
}

Peca dequeue(Fila *q) {
    Peca vazio = {'?', -1};
    if (filaVazia(q)) {
        fprintf(stderr, "Erro: tentativa de desenfileirar de fila vazia.\n");
        return vazio;
    }
    Peca item = q->dados[q->front];
    q->front = (q->front + 1) % QUEUE_CAP;
    q->count--;
    return item;
}

int pilhaVazia(Pilha *p) {
    return p->top == 0;
}
int pilhaCheia(Pilha *p) {
    return p->top == STACK_CAP;
}

void push(Pilha *p, Peca item) {
    if (pilhaCheia(p)) {
        fprintf(stderr, "Erro: tentativa de empilhar em pilha cheia.\n");
        return;
    }
    p->dados[p->top++] = item;
}

Peca pop(Pilha *p) {
    Peca vazio = {'?', -1};
    if (pilhaVazia(p)) {
        fprintf(stderr, "Erro: tentativa de desempilhar de pilha vazia.\n");
        return vazio;
    }
    return p->dados[--p->top];
}

/* Exibe fila e pilha. Pilha é mostrada Topo -> base */
void mostrarEstado(Fila *q, Pilha *p) {
    printf("\nEstado atual:\n");

    /* Mostrar fila */
    printf("Fila de peças: ");
    if (filaVazia(q)) {
        printf("[vazia]");
    } else {
        for (int i = 0; i < q->count; ++i) {
            int idx = (q->front + i) % QUEUE_CAP;
            printf("[%c %d] ", q->dados[idx].nome, q->dados[idx].id);
        }
    }
    printf("\n");

    /* Mostrar pilha */
    printf("Pilha de reserva (Topo -> base): ");
    if (pilhaVazia(p)) {
        printf("[vazia]");
    } else {
        /* top-1 é o topo */
        for (int i = p->top - 1; i >= 0; --i) {
            printf("[%c %d] ", p->dados[i].nome, p->dados[i].id);
        }
    }
    printf("\n\n");
}

/* Troca a peça da frente da fila com o topo da pilha */
void trocarFrenteTopo(Fila *q, Pilha *p) {
    if (filaVazia(q)) {
        printf("Operacao invalida: fila vazia.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("Operacao invalida: pilha vazia.\n");
        return;
    }

    int idxFrente = q->front;
    /* topo está em p->top-1 */
    Peca temp = q->dados[idxFrente];
    q->dados[idxFrente] = p->dados[p->top - 1];
    p->dados[p->top - 1] = temp;

    printf("Troca realizada entre a frente da fila e o topo da pilha.\n");
}

/* Troca múltipla: alterna as três primeiras peças da fila com as três peças da pilha
   Condição: fila deve ter >=3 e pilha >=3 */
void trocaMultipla3(Fila *q, Pilha *p) {
    if (q->count < 3) {
        printf("Operacao invalida: fila possui menos de 3 pecas.\n");
        return;
    }
    if (p->top < 3) {
        printf("Operacao invalida: pilha possui menos de 3 pecas.\n");
        return;
    }

    /* Trocar elemento a elemento: fila i (i=0..2) <-> pilha top-1-i (mas queremos keep top->base order match example)
       Observando o exemplo do enunciado: stack Top->base [O8][L7][T6]
       fila primeiros [I0][L1][T2]
       Após troca: fila [O8][L7][T6]  pilha [T2][L1][I0]
       Isso significa mapear fila[0] <-> pilha[top-1] (topo),
                      fila[1] <-> pilha[top-2],
                      fila[2] <-> pilha[top-3]
       E deixar pilha na mesma ordem topo->base (após troca os ids de fila aparecem na pilha em top->base ordem).
    */
    for (int i = 0; i < 3; ++i) {
        int idxFila = (q->front + i) % QUEUE_CAP;
        int idxPilha = p->top - 1 - i;
        Peca tmp = q->dados[idxFila];
        q->dados[idxFila] = p->dados[idxPilha];
        p->dados[idxPilha] = tmp;
    }

    printf("Troca realizada entre os 3 primeiros da fila e as 3 pecas da pilha.\n");
}

/* Limpar buffer de entrada para evitar loops em scanf */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Função principal com menu */
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    srand((unsigned int) time(NULL));

    iniciarFila(&fila);
    iniciarPilha(&pilha);

    /* Inicializar fila com 5 peças */
    for (int i = 0; i < QUEUE_CAP; ++i) {
        enqueue(&fila, gerarPeca());
    }

    printf("=== Tetris Stack ===\n");
    printf("\n");

    while (1) {
        mostrarEstado(&fila, &pilha);

        printf("Menu:\n");
        printf("\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva (Reservar)\n");
        printf("3 - Usar peca da pilha de reserva (Remover topo)\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("Digite sua opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }

        switch (opcao) {
            case 1: {
                /* Jogar peça: remove da frente da fila (dequeue).
                   Gera nova peça e enfileira para manter fila cheia.
                */
                if (filaVazia(&fila)) {
                    printf("Fila vazia: nao ha peca para jogar.\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    printf("Acao: Jogou a peca [%c %d].\n", jogada.nome, jogada.id);
                    /* gerar nova peca e enfileirar */
                    Peca nova = gerarPeca();
                    enqueue(&fila, nova);
                    printf("Gerada nova peca [%c %d] para preencher a fila.\n", nova.nome, nova.id);
                }
                break;
            }

            case 2: {
                /* Reservar: move peça da frente da fila para o topo da pilha,
                   se houver espaço. Após remoção da fila, gerar nova peça e enfileirar.
                */
                if (pilhaCheia(&pilha)) {
                    printf("Nao foi possivel reservar: pilha de reserva cheia.\n");
                } else if (filaVazia(&fila)) {
                    printf("Fila vazia: nao ha peca para reservar.\n");
                } else {
                    Peca mover = dequeue(&fila);
                    push(&pilha, mover);
                    printf("Acao: Peça [%c %d] movida.\n", mover.nome, mover.id);
                    /* gerar nova peca e enfileirar */
                    Peca nova = gerarPeca();
                    enqueue(&fila, nova);
                    printf("Gerada nova peca [%c %d].\n", nova.nome, nova.id);
                }
                break;
            }

            case 3: {
                /* Usar peça reservada: remove topo da pilha (pop).
                   Não gera nova peça na fila (pois nenhuma remoção da fila ocorreu).
                */
                if (pilhaVazia(&pilha)) {
                    printf("Pilha vazia: nao ha peca reservada para usar.\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("Acao: Usou a peca reservada [%c %d].\n", usada.nome, usada.id);
                    /* Conforme enunciado, pecas removidas nao voltam ao jogo */
                }
                break;
            }

            case 4: {
                trocarFrenteTopo(&fila, &pilha);
                break;
            }

            case 5: {
                trocaMultipla3(&fila, &pilha);
                break;
            }

            case 0: {
                printf("Encerrando o programa.\n");
                exit(0);
                break;
            }

            default:
                printf("Opcao invalida. Digite um numero entre 0 e 5.\n");
                break;
        }

        /* Pequena separacao visual */
        printf("\n---------------------------------------------\n");
    }

    return 0;
}
