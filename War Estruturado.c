#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 10

// ---------------------------------------------------------
// Struct de território
// ---------------------------------------------------------
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// ---------------------------------------------------------
// Função auxiliar: limpa buffer do teclado
// ---------------------------------------------------------
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------------------------------------------------------
// Função para ler strings com segurança
// ---------------------------------------------------------
void lerString(char* destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0'; // remove \n
}

// ---------------------------------------------------------
// Cadastro dos territórios
// ---------------------------------------------------------
void cadastrarTerritorios(Territorio* mapa, int n) {
    printf("\n=== CADASTRO DOS TERRITORIOS ===\n");

    for (int i = 0; i < n; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);

        printf("Nome: ");
        lerString(mapa[i].nome, MAX_NOME);

        printf("Cor do exercito: ");
        lerString(mapa[i].cor, MAX_COR);

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();
    }
}

// ---------------------------------------------------------
// Exibir territórios em tabela bonita
// ---------------------------------------------------------
void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n\n==============================================\n");
    printf("   SITUACAO ATUAL DOS TERRITORIOS\n");
    printf("==============================================\n");
    printf("%-3s | %-20s | %-10s | %-6s\n", "#", "Nome", "Cor", "Tropas");
    printf("----------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-3d | %-20s | %-10s | %-6d\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
}

// ---------------------------------------------------------
// Função de ataque entre 2 territórios
// ---------------------------------------------------------
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n\n=========== BATALHA ===========\n");

    if (atacante->tropas <= 1) {
        printf("O atacante precisa de mais tropas para atacar!\n");
        return;
    }

    printf("Atacante: %s (%s) - Tropas: %d\n",
           atacante->nome, atacante->cor, atacante->tropas);

    printf("Defensor: %s (%s) - Tropas: %d\n\n",
           defensor->nome, defensor->cor, defensor->tropas);

    int dadoAtq = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("Dado do atacante = %d\n", dadoAtq);
    printf("Dado do defensor = %d\n\n", dadoDef);

    if (dadoAtq > dadoDef) {
        printf(">>> O atacante venceu!\n");

        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;

        printf("Territorio %s foi conquistado pelo exercito %s!\n",
               defensor->nome, defensor->cor);
    } else {
        printf(">>> O defensor venceu!\n");
        atacante->tropas--;
        printf("%s perdeu 1 tropa.\n", atacante->nome);
    }

    printf("=========== FIM DA BATALHA ===========\n");
}

// ---------------------------------------------------------
// Libera memória alocada
// ---------------------------------------------------------
void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("\nMemoria liberada com sucesso.\n");
}

// ---------------------------------------------------------
// Função principal
// ---------------------------------------------------------
int main() {
    srand(time(NULL));

    int n;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);
    limparBuffer();

    // Alocação dinâmica
    Territorio* mapa = (Territorio*)calloc(n, sizeof(Territorio));

    if (!mapa) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);

    int opcao, atq, def;

    do {
        exibirTerritorios(mapa, n);

        printf("\n=== MENU ===\n");
        printf("1 - Atacar territorio\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            printf("Territorio atacante (#): ");
            scanf("%d", &atq);
            limparBuffer();

            printf("Territorio defensor (#): ");
            scanf("%d", &def);
            limparBuffer();

            atq--; def--;

            if (atq < 0 || atq >= n || def < 0 || def >= n) {
                printf("Escolha invalida!\n");
                continue;
            }

            if (strcmp(mapa[atq].cor, mapa[def].cor) == 0) {
                printf("Nao pode atacar territorios da mesma cor!\n");
                continue;
            }

            atacar(&mapa[atq], &mapa[def]);
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}
