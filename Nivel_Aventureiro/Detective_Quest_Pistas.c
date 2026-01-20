/*
 * Autor: Augusto Bento Carvalho 
 * 
 * Jogo: Detective Quest
 * 
 * Objetivo: Sistema de exploração de mansão com coleta de pistas.
 * Estruturas:
 *   1. Árvore Binária: Representa o mapa da mansão (Salas).
 *   2. BST (Binary Search Tree): Armazena as pistas coletadas em ordem alfabética.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// DEFINIÇÃO DAS ESTRUTURAS
// ============================================================================

// Estrutura para a Árvore de Pistas (BST)
// Armazena o texto da pista e ponteiros para ordenação.
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Estrutura para a Árvore da Mansão (Mapa)
// Representa um cômodo, contendo nome, uma possível pista e caminhos.
typedef struct Sala {
    char nome[50];
    char pista[100];        // Pista associada ao cômodo (pode ser vazia)
    struct Sala* esquerda;  // Caminho à esquerda
    struct Sala* direita;   // Caminho à direita
} Sala;

// ============================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================================================

// Cria dinamicamente um cômodo com nome e pista opcional.
Sala* criarSala(char* nome, char* pista);

// Insere uma nova pista na árvore BST, mantendo a ordem alfabética.
PistaNode* inserirPista(PistaNode* raiz, char* conteudo);

// Controla a navegação entre salas e a coleta automática de pistas.
void explorarSalasComPistas(Sala* mapa, PistaNode** raizPistas);

// Imprime a árvore de pistas em ordem alfabética (Percurso Em-Ordem).
void exibirPistas(PistaNode* raiz);

// Funções auxiliares para liberar memória.
void liberarMapa(Sala* raiz);
void liberarPistas(PistaNode* raiz);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================

int main() {
    // 1. Construção do Mapa da Mansão (Árvore Binária Fixa)
    // Nível 0: Raiz
    Sala* mansao = criarSala("Hall de Entrada", "Pegadas de lama no chão");
    
    // Nível 1
    mansao->esquerda = criarSala("Sala de Estar", "Relógio parado às 10h");
    mansao->direita = criarSala("Cozinha", ""); // Cozinha sem pista
    
    // Nível 2 (Esquerda)
    mansao->esquerda->esquerda = criarSala("Biblioteca", "Livro de venenos aberto");
    mansao->esquerda->direita = criarSala("Jardim de Inverno", "Terra revirada recente");
    
    // Nível 2 (Direita)
    mansao->direita->esquerda = criarSala("Sala de Jantar", "Taça de vinho quebrada");
    mansao->direita->direita = criarSala("Porão", "Chave enferrujada antiga");

    // 2. Inicialização da Árvore de Pistas (Inventário vazio)
    PistaNode* inventarioPistas = NULL;

    printf("=========================================\n");
    printf("      DETECTIVE QUEST: CAÇA ÀS PISTAS    \n");
    printf("=========================================\n");
    printf("Você entrou na mansão. Explore os cômodos e colete evidências.\n");

    // 3. Início da Exploração
    // Passamos o endereço do ponteiro inventarioPistas para poder modificá-lo
    explorarSalasComPistas(mansao, &inventarioPistas);

    // 4. Relatório Final
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

    // 5. Limpeza de Memória
    liberarMapa(mansao);
    liberarPistas(inventarioPistas);
    printf("\nMemória liberada. Caso encerrado.\n");

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================================================

/*
 * Função: criarSala
 * Cria um nó da árvore da mansão.
 */
Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro crítico: Falha na alocação de memória.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    
    // Se houver pista, copia; senão, define como string vazia
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
 * Função: inserirPista
 * Insere uma string na BST. Se a pista já existe, não duplica.
 * Retorna a raiz atualizada da subárvore.
 */
PistaNode* inserirPista(PistaNode* raiz, char* conteudo) {
    // Caso base: posição vazia encontrada, cria o nó aqui
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        if (novo) {
            strcpy(novo->conteudo, conteudo);
            novo->esquerda = NULL;
            novo->direita = NULL;
        }
        return novo;
    }

    // Compara alfabeticamente para decidir o lado (Esquerda < Raiz < Direita)
    int cmp = strcmp(conteudo, raiz->conteudo);

    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, conteudo);
    }
    // Se cmp == 0, a pista é igual e já existe; ignoramos para não duplicar.

    return raiz;
}

/*
 * Função: explorarSalasComPistas
 * Loop principal de navegação.
 * Parâmetro raizPistas é um ponteiro duplo para permitir alterar a raiz da BST.
 */
void explorarSalasComPistas(Sala* salaAtual, PistaNode** raizPistas) {
    char opcao;
    
    while (salaAtual != NULL) {
        printf("\n-----------------------------------------\n");
        printf("LOCAL ATUAL: %s\n", salaAtual->nome);
        
        // --- Lógica de Coleta de Pista ---
        if (strlen(salaAtual->pista) > 0) {
            printf("[!] Pista encontrada: \"%s\"\n", salaAtual->pista);
            printf("    -> Adicionando ao caderno de anotações...\n");
            
            // Insere na BST usando o ponteiro dereferenciado (*raizPistas)
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
        } else {
            printf("(Nenhuma pista visível neste cômodo)\n");
        }
        printf("-----------------------------------------\n");

        // Verifica se é um beco sem saída (folha)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Este cômodo não tem mais saídas. Fim da linha para este caminho.\n");
            break; 
        }

        // Menu de navegação
        printf("Para onde deseja ir?\n");
        if (salaAtual->esquerda) 
            printf(" [e] Esquerda (%s)\n", salaAtual->esquerda->nome);
        
        if (salaAtual->direita) 
            printf(" [d] Direita (%s)\n", salaAtual->direita->nome);
        
        printf(" [s] Sair da Mansão (Encerrar exploração)\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &opcao);

        // Processa a escolha
        if (opcao == 'e' || opcao == 'E') {
            if (salaAtual->esquerda) {
                salaAtual = salaAtual->esquerda;
            } else {
                printf("\n[!] Caminho bloqueado ou inexistente.\n");
            }
        } else if (opcao == 'd' || opcao == 'D') {
            if (salaAtual->direita) {
                salaAtual = salaAtual->direita;
            } else {
                printf("\n[!] Caminho bloqueado ou inexistente.\n");
            }
        } else if (opcao == 's' || opcao == 'S') {
            printf("\nVocê decidiu encerrar a investigação por agora.\n");
            break;
        } else {
            printf("\n[!] Opção inválida.\n");
        }
    }
}

/*
 * Função: exibirPistas
 * Realiza o percurso Em-Ordem (In-Order) na BST para imprimir alfabeticamente.
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->conteudo);
        exibirPistas(raiz->direita);
    }
}

/*
 * Funções de Limpeza (Recursivas Pós-Ordem)
 */
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
