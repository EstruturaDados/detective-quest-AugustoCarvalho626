/*
 * Autor: Augusto Bento Carvalho 
 * 
 * Jogo: Detective Quest: O Último Caso
 * 
 * Objetivo: Sistema de exploração de mansão com coleta de pistas e julgamento.
 * Estruturas:
 *   1. Árvore Binária: Representa o mapa da mansão (Salas).
 *   2. BST (Binary Search Tree): Armazena as pistas coletadas em ordem alfabética.
 *   3. Tabela Hash: Associa pistas a suspeitos para o veredito final.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================================================
// DEFINIÇÃO DAS ESTRUTURAS
// ============================================================================

// Estrutura para a Árvore de Pistas (BST)
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Estrutura para a Árvore da Mansão (Mapa)
typedef struct Sala {
    char nome[50];
    char pista[100];        // Pista associada ao cômodo
    struct Sala* esquerda;  // Caminho à esquerda
    struct Sala* direita;   // Caminho à direita
} Sala;

// Estrutura para a Tabela Hash (Associação Pista -> Suspeito)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* proximo; // Tratamento de colisão por encadeamento
} HashNode;

#define TAM_HASH 31 // Tamanho primo para a tabela hash

// ============================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================================================

// criarSala() – cria dinamicamente um cômodo.
Sala* criarSala(char* nome, char* pista);

// inserirPista() – insere a pista coletada na árvore de pistas.
PistaNode* inserirPista(PistaNode* raiz, char* conteudo);

// explorarSalas() – navega pela árvore e ativa o sistema de pistas.
void explorarSalas(Sala* mapa, PistaNode** raizPistas);

// inserirNaHash() – insere associação pista/suspeito na tabela hash.
void inserirNaHash(HashNode* tabela[], char* pista, char* suspeito);

// encontrarSuspeito() – consulta o suspeito correspondente a uma pista.
char* encontrarSuspeito(HashNode* tabela[], char* pista);

// verificarSuspeitoFinal() – conduz à fase de julgamento final.
void verificarSuspeitoFinal(PistaNode* raizPistas, HashNode* tabela[], char* suspeitoAcusado);

// Funções auxiliares
void exibirPistas(PistaNode* raiz);
void liberarMapa(Sala* raiz);
void liberarPistas(PistaNode* raiz);
void liberarHash(HashNode* tabela[]);
int contarPistasSuspeito(PistaNode* raiz, HashNode* tabela[], char* suspeitoAlvo);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================

int main() {
    // 1. Construção do Mapa da Mansão (Árvore Binária Fixa)
    Sala* mansao = criarSala("Hall de Entrada", "Pegadas de lama no chão");
    
    mansao->esquerda = criarSala("Sala de Estar", "Relógio parado às 10h");
    mansao->direita = criarSala("Cozinha", ""); // Cozinha sem pista
    
    mansao->esquerda->esquerda = criarSala("Biblioteca", "Livro de venenos aberto");
    mansao->esquerda->direita = criarSala("Jardim de Inverno", "Terra revirada recente");
    
    mansao->direita->esquerda = criarSala("Sala de Jantar", "Taça de vinho quebrada");
    mansao->direita->direita = criarSala("Porão", "Chave enferrujada antiga");

    // 2. Inicialização da Árvore de Pistas (Inventário vazio)
    PistaNode* inventarioPistas = NULL;

    // 3. Inicialização e Configuração da Tabela Hash
    HashNode* tabelaSuspeitos[TAM_HASH] = {NULL};
    
    // Configuração das pistas e suspeitos (Gabarito do Jogo)
    // Jardineiro
    inserirNaHash(tabelaSuspeitos, "Pegadas de lama no chão", "Jardineiro");
    inserirNaHash(tabelaSuspeitos, "Terra revirada recente", "Jardineiro");
    // Mordomo
    inserirNaHash(tabelaSuspeitos, "Relógio parado às 10h", "Mordomo");
    inserirNaHash(tabelaSuspeitos, "Taça de vinho quebrada", "Mordomo");
    // Governanta
    inserirNaHash(tabelaSuspeitos, "Livro de venenos aberto", "Governanta");
    inserirNaHash(tabelaSuspeitos, "Chave enferrujada antiga", "Governanta");

    printf("=========================================\n");
    printf("      DETECTIVE QUEST: O ÚLTIMO CASO     \n");
    printf("=========================================\n");
    printf("Você entrou na mansão. Explore os cômodos e colete evidências.\n");

    // 4. Início da Exploração
    explorarSalas(mansao, &inventarioPistas);

    // 5. Relatório Final
    printf("\n=========================================\n");
    printf("      RELATÓRIO FINAL DO DETETIVE        \n");
    printf("=========================================\n");
    printf("Pistas coletadas (Ordem Alfabética):\n\n");
    
    if (inventarioPistas == NULL) {
        printf("- Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(inventarioPistas);
    }
    printf("=========================================\n");

    // 6. Fase de Acusação
    char acusado[50];
    printf("\nQuem é o culpado? (Jardineiro / Mordomo / Governanta): ");
    scanf(" %[^\n]s", acusado); // Lê string com espaços

    verificarSuspeitoFinal(inventarioPistas, tabelaSuspeitos, acusado);

    // 7. Limpeza de Memória
    liberarMapa(mansao);
    liberarPistas(inventarioPistas);
    liberarHash(tabelaSuspeitos);
    printf("\nMemória liberada. Caso encerrado.\n");

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================================================

/*
 * criarSala() – cria dinamicamente um cômodo.
 * Aloca memória para uma nova sala, define seu nome e a pista associada.
 */
Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro crítico: Falha na alocação de memória.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    
    if (pista != NULL) {
        strcpy(nova->pista, pista);
    } else {
        nova->pista[0] = '\0';
    }
    
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/*
 * inserirPista() – insere a pista coletada na árvore de pistas.
 * Utiliza a lógica de BST (Binary Search Tree) para manter as pistas
 * ordenadas alfabeticamente.
 */
PistaNode* inserirPista(PistaNode* raiz, char* conteudo) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        if (novo) {
            strcpy(novo->conteudo, conteudo);
            novo->esquerda = NULL;
            novo->direita = NULL;
        }
        return novo;
    }

    int cmp = strcmp(conteudo, raiz->conteudo);

    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, conteudo);
    }
    return raiz;
}

/*
 * explorarSalas() – navega pela árvore e ativa o sistema de pistas.
 * Permite ao usuário escolher caminhos (esquerda/direita) e coleta pistas automaticamente.
 */
void explorarSalas(Sala* salaAtual, PistaNode** raizPistas) {
    char opcao;
    
    while (salaAtual != NULL) {
        printf("\n-----------------------------------------\n");
        printf("LOCAL ATUAL: %s\n", salaAtual->nome);
        
        // Coleta de Pista
        if (strlen(salaAtual->pista) > 0) {
            printf("[!] Pista encontrada: \"%s\"\n", salaAtual->pista);
            printf("    -> Adicionando ao caderno de anotações...\n");
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
        } else {
            printf("(Nenhuma pista visível neste cômodo)\n");
        }
        printf("-----------------------------------------\n");

        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Este cômodo não tem mais saídas. Fim da linha para este caminho.\n");
            break; 
        }

        printf("Para onde deseja ir?\n");
        if (salaAtual->esquerda) 
            printf(" [e] Esquerda (%s)\n", salaAtual->esquerda->nome);
        
        if (salaAtual->direita) 
            printf(" [d] Direita (%s)\n", salaAtual->direita->nome);
        
        printf(" [s] Sair da Mansão (Encerrar exploração)\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (salaAtual->esquerda) salaAtual = salaAtual->esquerda;
            else printf("\n[!] Caminho bloqueado.\n");
        } else if (opcao == 'd' || opcao == 'D') {
            if (salaAtual->direita) salaAtual = salaAtual->direita;
            else printf("\n[!] Caminho bloqueado.\n");
        } else if (opcao == 's' || opcao == 'S') {
            printf("\nVocê decidiu encerrar a investigação por agora.\n");
            break;
        } else {
            printf("\n[!] Opção inválida.\n");
        }
    }
}

// --- Funções da Tabela Hash ---

int funcaoHash(char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAM_HASH;
}

/*
 * inserirNaHash() – insere associação pista/suspeito na tabela hash.
 * Cria um novo nó e o insere na lista encadeada correspondente ao índice hash.
 */
void inserirNaHash(HashNode* tabela[], char* pista, char* suspeito) {
    int indice = funcaoHash(pista);
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    if (novo) {
        strcpy(novo->pista, pista);
        strcpy(novo->suspeito, suspeito);
        novo->proximo = tabela[indice];
        tabela[indice] = novo;
    }
}

/*
 * encontrarSuspeito() – consulta o suspeito correspondente a uma pista.
 * Busca na tabela hash pela pista fornecida e retorna o nome do suspeito.
 */
char* encontrarSuspeito(HashNode* tabela[], char* pista) {
    int indice = funcaoHash(pista);
    HashNode* atual = tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Função auxiliar recursiva para contar pistas de um suspeito na BST
int contarPistasSuspeito(PistaNode* raiz, HashNode* tabela[], char* suspeitoAlvo) {
    if (raiz == NULL) return 0;
    
    int contador = 0;
    char* suspeitoDaPista = encontrarSuspeito(tabela, raiz->conteudo);
    
    // Verifica se a pista atual aponta para o suspeito alvo
    if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, suspeitoAlvo) == 0) {
        contador = 1;
    }
    
    return contador + 
           contarPistasSuspeito(raiz->esquerda, tabela, suspeitoAlvo) + 
           contarPistasSuspeito(raiz->direita, tabela, suspeitoAlvo);
}

/*
 * verificarSuspeitoFinal() – conduz à fase de julgamento final.
 * Verifica se há provas suficientes (>= 2 pistas) contra o acusado.
 */
void verificarSuspeitoFinal(PistaNode* raizPistas, HashNode* tabela[], char* suspeitoAcusado) {
    printf("\n--- JULGAMENTO FINAL ---\n");
    printf("Acusado: %s\n", suspeitoAcusado);
    printf("Analisando evidências coletadas...\n");
    
    int qtdProvas = contarPistasSuspeito(raizPistas, tabela, suspeitoAcusado);
    
    printf("Provas encontradas contra %s: %d\n", suspeitoAcusado, qtdProvas);
    
    if (qtdProvas >= 2) {
        printf("\n[VEREDITO] CULPADO!\n");
        printf("Parabéns, detetive! Você reuniu provas suficientes (%d) para prender o %s.\n", qtdProvas, suspeitoAcusado);
        printf("O mistério da mansão foi resolvido.\n");
    } else {
        printf("\n[VEREDITO] INOCENTE (por falta de provas)!\n");
        printf("Você apresentou apenas %d prova(s). O tribunal exige no mínimo 2 evidências concretas.\n", qtdProvas);
        printf("O %s foi liberado e o verdadeiro culpado fugiu.\n", suspeitoAcusado);
        printf("GAME OVER.\n");
    }
}

// --- Funções Auxiliares e de Limpeza ---

void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->conteudo);
        exibirPistas(raiz->direita);
    }
}

void liberarMapa(Sala* raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

void liberarPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

void liberarHash(HashNode* tabela[]) {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabela[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}