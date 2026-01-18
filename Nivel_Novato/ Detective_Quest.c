/*
 * Jogo: Detective Quest
 * Tema: Exploração de Mansão usando Árvore Binária
 * 
 * Descrição:
 * Este programa simula a exploração de uma mansão onde cada sala é um nó
 * de uma árvore binária. O jogador começa no Hall de Entrada e decide
 * o caminho a seguir (esquerda ou direita) até encontrar um beco sem saída.
 *
 * Conceitos: Structs, Ponteiros, Alocação Dinâmica, Árvores Binárias.
 *
 * Autor: Augusto Bento Carvalho
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura da Sala (Nó da Árvore)
typedef struct Sala {
    char nome[50];          // Nome do cômodo
    struct Sala* esquerda;  // Caminho à esquerda
    struct Sala* direita;   // Caminho à direita
} Sala;

// --- Protótipos das Funções ---

// Cria, de forma dinâmica, uma sala com nome.
Sala* criarSala(char* nome);

// Permite a navegação do jogador pela árvore.
void explorarSalas(Sala* raiz);

// Função auxiliar para liberar a memória ao final.
void liberarMapa(Sala* raiz);

// --- Função Principal ---

int main() {
    // Monta o mapa inicial e dá início à exploração.
    
    // 1. Criação da Raiz (Hall de Entrada)
    Sala* mapa = criarSala("Hall de Entrada");

    // 2. Construção manual da árvore (Mapa da Mansão)
    // Nível 1: Esquerda e Direita do Hall
    mapa->esquerda = criarSala("Sala de Estar");
    mapa->direita = criarSala("Cozinha");

    // Nível 2: Filhos da Sala de Estar
    mapa->esquerda->esquerda = criarSala("Biblioteca");
    mapa->esquerda->direita = criarSala("Jardim de Inverno");

    // Nível 2: Filhos da Cozinha
    mapa->direita->esquerda = criarSala("Sala de Jantar");
    mapa->direita->direita = criarSala("Porão Escuro");

    // Início do Jogo
    printf("=========================================\n");
    printf("      BEM-VINDO AO DETECTIVE QUEST       \n");
    printf("=========================================\n");
    printf("Você é um detetive explorando uma mansão misteriosa.\n");
    
    // Inicia a navegação
    explorarSalas(mapa);

    // Limpeza de memória antes de encerrar
    liberarMapa(mapa);
    printf("\nMemória liberada. Jogo encerrado.\n");

    return 0;
}

// --- Implementação das Funções ---

/*
 * Função: criarSala
 * Objetivo: Aloca memória para uma nova sala e define seu nome.
 * Parâmetros: nome (string) - O nome do cômodo.
 * Retorno: Ponteiro para a nova estrutura Sala criada.
 */
Sala* criarSala(char* nome) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro crítico: Falha na alocação de memória.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/*
 * Função: explorarSalas
 * Objetivo: Gerencia a interação com o usuário para navegar pela árvore.
 *           O loop continua até chegar em uma folha ou o usuário sair.
 * Parâmetros: atual (Sala*) - O ponto de partida (nó atual).
 */
void explorarSalas(Sala* atual) {
    char opcao;

    while (atual != NULL) {
        printf("\n-----------------------------------------\n");
        printf("VOCÊ ESTÁ EM: %s\n", atual->nome);
        printf("-----------------------------------------\n");

        // Verifica se é um nó folha (sem saídas)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Esta sala não tem mais saídas. É um beco sem saída!\n");
            printf("Exploração concluída neste caminho.\n");
            break; // Sai do loop pois não há para onde ir
        }

        printf("Escolha seu caminho:\n");
        printf(" [e] Ir para a Esquerda\n");
        printf(" [d] Ir para a Direita\n");
        printf(" [s] Sair da Mansão\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &opcao);

        // Lógica de navegação baseada na escolha
        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL) atual = atual->esquerda;
            else printf("\n[!] Não há caminho para a esquerda.\n");
        } else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL) atual = atual->direita;
            else printf("\n[!] Não há caminho para a direita.\n");
        } else if (opcao == 's' || opcao == 'S') {
            printf("\nVocê decidiu encerrar a investigação.\n");
            break;
        } else {
            printf("\n[!] Opção inválida. Tente novamente.\n");
        }
    }
}

/*
 * Função: liberarMapa
 * Objetivo: Percorre a árvore recursivamente liberando a memória alocada (Pós-ordem).
 * Parâmetros: raiz (Sala*) - O nó atual a ser liberado.
 */
void liberarMapa(Sala* raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}
