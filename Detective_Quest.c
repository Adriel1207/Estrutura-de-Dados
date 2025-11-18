#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef _MSC_VER
#else
char *strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *p = malloc(len);
    if (p) memcpy(p, s, len);
    return p;
}
#endif

/* ---------------------------- */
/* Estruturas básicas           */
/* ---------------------------- */

typedef struct Sala {
    char *nome;
    char *pista;
    struct Sala *esq;
    struct Sala *dir;
} Sala;

typedef struct BSTNode {
    char *pista;
    struct BSTNode *esq;
    struct BSTNode *dir;
} BSTNode;

typedef struct HashEntry {
    char *pista;
    char *suspeito;
    struct HashEntry *next;
} HashEntry;

#define HASH_SIZE 101
HashEntry *hashTable[HASH_SIZE];

/* Funções auxiliares           */

Sala *criarSala(const char *nome, const char *pista, Sala *esq, Sala *dir) {
    Sala *s = malloc(sizeof(Sala));
    s->nome = strdup(nome);
    s->pista = pista ? strdup(pista) : NULL;
    s->esq = esq;
    s->dir = dir;
    return s;
}

void liberarSalas(Sala *r) {
    if (!r) return;
    liberarSalas(r->esq);
    liberarSalas(r->dir);
    free(r->nome);
    if (r->pista) free(r->pista);
    free(r);
}

/* Inserir pista em BST */
BSTNode *inserirPista(BSTNode *root, const char *pista) {
    if (!pista) return root;
    if (!root) {
        BSTNode *n = malloc(sizeof(BSTNode));
        n->pista = strdup(pista);
        n->esq = n->dir = NULL;
        return n;
    }

    int cmp = strcmp(pista, root->pista);
    if (cmp == 0) return root;
    if (cmp < 0) root->esq = inserirPista(root->esq, pista);
    else root->dir = inserirPista(root->dir, pista);
    return root;
}

void listarPistas(BSTNode *r) {
    if (!r) return;
    listarPistas(r->esq);
    printf(" - %s\n", r->pista);
    listarPistas(r->dir);
}

void liberarBST(BSTNode *r) {
    if (!r) return;
    liberarBST(r->esq);
    liberarBST(r->dir);
    free(r->pista);
    free(r);
}

/* Hash (pista -> suspeito)     */

unsigned long hashString(const char *s) {
    unsigned long h = 5381;
    int c;
    while ((c = *s++)) h = h * 33 + c;
    return h % HASH_SIZE;
}

void inserirNaHash(const char *pista, const char *suspeito) {
    unsigned long h = hashString(pista);
    HashEntry *e = hashTable[h];

    while (e) {
        if (!strcmp(e->pista, pista)) {
            free(e->suspeito);
            e->suspeito = strdup(suspeito);
            return;
        }
        e = e->next;
    }

    e = malloc(sizeof(HashEntry));
    e->pista = strdup(pista);
    e->suspeito = strdup(suspeito);
    e->next = hashTable[h];
    hashTable[h] = e;
}

const char *encontrarSuspeito(const char *p) {
    unsigned long h = hashString(p);
    HashEntry *e = hashTable[h];
    while (e) {
        if (!strcmp(e->pista, p)) return e->suspeito;
        e = e->next;
    }
    return NULL;
}

void liberarHash() {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *e = hashTable[i];
        while (e) {
            HashEntry *n = e->next;
            free(e->pista);
            free(e->suspeito);
            free(e);
            e = n;
        }
    }
}

/* Exploração da mansão*/

void explorarSalas(Sala *root, BSTNode **coletadas) {
    Sala *s = root;
    char cmd[16];

    printf("==============================Bem-vindo ao Detective Quest=============================\n\n");
    printf("Voce é um detetive e foi designado solucuinar um crime que foi cometido em uma manssão.\n");
    printf("Sua missão e buscar evidencias e apontar um suspeito ao fim da sua exploração.");
    printf("➡️  Entrando no Hall!\n");

    while (s) {
        printf("\n📍 Sala atual: %s\n", s->nome);

        if (s->pista) {
            printf("🔍 Pista encontrada: \"%s\"\n", s->pista);
            *coletadas = inserirPista(*coletadas, s->pista);
        }

        if (!s->esq && !s->dir) {
            printf("🚪 Sem caminhos — fim da exploração.\n");
            return;
        }

        printf("Mover: (e) esquerda | (d) direita | (s) sair\n> ");
        fgets(cmd, sizeof(cmd), stdin);
        char c = tolower(cmd[0]);

        if (c == 's') return;
        else if (c == 'e') {
            if (!s->esq) printf("Não há sala à esquerda.\n");
            else s = s->esq;
        }
        else if (c == 'd') {
            if (!s->dir) printf("Não há sala à direita.\n");
            else s = s->dir;
        }
        else printf("Comando inválido!\n");
    }
}

/* Conta quantas pistas da BST apontam p/ um suspeito */
int verificarSuspeito(BSTNode *r, const char *suspeito) {
    if (!r) return 0;
    int soma = 0;
    soma += verificarSuspeito(r->esq, suspeito);
    const char *s = encontrarSuspeito(r->pista);
    if (s && strcmp(s, suspeito) == 0) soma++;
    soma += verificarSuspeito(r->dir, suspeito);
    return soma;
}

/* ---------------------------- */
/* MAIN                         */
/* ---------------------------- */

int main() {
    /* Salas adicionais */

    Sala *garagem      = criarSala("Garagem",      "Lenco com fragnacia de perfume feminino sofisticado.", NULL, NULL);
    Sala *porao        = criarSala("Porão",        "Carta queimada (techos legiveis com letra moderna e escrita aual.", NULL, NULL);
    Sala *varanda      = criarSala("Varanda",      "Corrente de pescoço masculina com pingente de uma banda com fama recente.", NULL, NULL);
    Sala *laboratorio  = criarSala("Laboratório",  "Frasco bombinha de asma quebrado.", NULL, NULL);

    Sala *cozinha      = criarSala("Cozinha",      "Pegadas de lama (sapato social masculino).", garagem, porao);
    Sala *escritorio   = criarSala("Escritório",   "Um bilhete rasgado com escrita estilo classica (antiga).", varanda, laboratorio);
    Sala *jardim       = criarSala("Jardim",       "Fios de tecido cinza puxados e arrebentados (usado em terno social).", NULL, NULL);
    Sala *quarto       = criarSala("Quarto",       "Um anel de noivado perdido.", NULL, NULL);

    Sala *biblioteca   = criarSala("Biblioteca",   "Marcas de batom rosa em copo de café.", cozinha, escritorio);
    Sala *salaJantar   = criarSala("Sala de Jantar","Guardanapo ensanguentado com fios (cabelos) pequenos e grisalhos.", jardim, quarto);

    Sala *hall         = criarSala("Hall",         "Som de passos com salto alto no andar de cima.", biblioteca, salaJantar);

    /* Suspeitos */
    const char *suspeitos[] = {
        "Senhor Oliveira",
        "Dona Helena",
        "Senhorita Beatriz",
        "Senhor Carvalho",
        "Jovem Marcos"
    };
    int numSuspeitos = 5;

    /* Mapeamento pista → suspeito */
    inserirNaHash("Pegadas de lama (sapato social masculino).", "Senhor Oliveira");
    inserirNaHash("Corrente de pescoço masculina com pingente de uma banda com fama recente.", "Jovem Marcos");
    inserirNaHash("Carta queimada (techos legiveis com letra moderna e escrita atual.)", "Jovem Marcos");
    inserirNaHash("Lenco com fragnacia de perfume feminino sofisticado.", "Senhorita Beatriz");
    inserirNaHash("Frasco bombinha de asma quebrado.", "Senhor Carvalho");

    inserirNaHash("Um bilhete rasgado com escrita estilo classica (antiga).", "Dona Helena");
    inserirNaHash("Fios de tecido cinza puxados e arrebentados (usado em terno social).", "Senhor Oliveira");
    inserirNaHash("Um anel de noivado perdido.", "Senhorita Beatriz");
    inserirNaHash("Marcas de batom rosa em copo de café.", "Dona Helena");
    inserirNaHash("Guardanapo ensanguentado com fios (cabelos) pequenos e grisalhos.", "Senhor Carvalho");
    inserirNaHash("Som de passos com salto alto no andar de cima.", "Dona Helena");

    /* Exploração */
    BSTNode *coletadas = NULL;
    explorarSalas(hall, &coletadas);

    printf("\n📄 Pistas coletadas:\n");
    listarPistas(coletadas);

    /* MENU DE ACUSAÇÃO */
    printf("\n=== ACUSAÇÃO FINAL ===\n");
    printf("Escolha o suspeito:\n");
    for (int i = 0; i < numSuspeitos; i++)
        printf("%d) %s\n", i + 1, suspeitos[i]);

    int escolha;
    printf("\nDigite o número do suspeito: ");
    scanf("%d", &escolha);
    getchar();

    if (escolha < 1 || escolha > numSuspeitos) {
        printf("Opção inválida.\n");
    } else {
        const char *s = suspeitos[escolha - 1];
        int qtd = verificarSuspeito(coletadas, s);

        printf("\nVocê acusou: %s\n", s);

        if (qtd >= 2)
            printf("🔥 %d pistas confirmam a culpa. Caso encerrado!\n", qtd);
        else if (qtd == 1)
            printf("⚠️ Apenas 1 pista aponta para o suspeito. Evidência insuficiente.\n");
        else
            printf("❌ Nenhuma pista incrimina o suspeito.\n");
    }

    liberarBST(coletadas);
    liberarSalas(hall);
    liberarHash();

    return 0;
}
