#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 16
#define MAX_MISSAO_LEN 128

/* -------------------------
   Tipos
   ------------------------- */
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    char *missao; // alocada dinamicamente
} Jogador;

/* -------------------------
   Missões disponíveis
   ------------------------- */
const char *MISSOES[] = {
    "Conquistar 3 territorios seguidos",           // id 0
    "Eliminar todas as tropas da cor Vermelha",    // id 1
    "Possuir 4 ou mais territorios no mapa",       // id 2
    "Reduzir a zero as tropas de pelo menos 1 inimigo", // id 3
    "Possuir todos os territorios com nome iniciando com A", // id 4
    "Controlar exatamente 2 territorios"           // id 5
};
const int TOTAL_MISSOES = sizeof(MISSOES) / sizeof(MISSOES[0]);

/* -------------------------
   Funções utilitárias I/O
   ------------------------- */
// Lê uma linha do stdin e remove o newline
void lerLinha(char *dest, int maxLen) {
    if (fgets(dest, maxLen, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }
    dest[strcspn(dest, "\n")] = '\0';
}

// Lê um inteiro com prompt, usando fgets + sscanf para evitar problemas do scanf
int lerInteiro(const char *prompt) {
    char buffer[64];
    int valor;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
        if (sscanf(buffer, "%d", &valor) == 1) return valor;
        printf("Entrada invalida. Por favor digite um numero inteiro.\n");
    }
}

// Comparação case-insensitive simples (ASCII)
int equalsIgnoreCase(const char *a, const char *b) {
    if (!a || !b) return 0;
    while (*a && *b) {
        char ca = *a;
        char cb = *b;
        if (ca >= 'A' && ca <= 'Z') ca = ca - 'A' + 'a';
        if (cb >= 'A' && cb <= 'Z') cb = cb - 'A' + 'a';
        if (ca != cb) return 0;
        a++; b++;
    }
    return *a == *b;
}

/* -------------------------
   Cadastro e exibição
   ------------------------- */
Territorio* criarMapa(int n) {
    Territorio *mapa = (Territorio*) calloc(n, sizeof(Territorio));
    return mapa;
}

void cadastrarTerritorios(Territorio *mapa, int n) {
    printf("\n=== Cadastro de Territorios ===\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritorio %d - Nome: ", i + 1);
        lerLinha(mapa[i].nome, MAX_NOME);

        printf("Cor do exercito (dono): ");
        lerLinha(mapa[i].cor, MAX_COR);

        mapa[i].tropas = lerInteiro("Quantidade de tropas: ");
    }
}

void exibirMapa(Territorio *mapa, int n) {
    printf("\n================ MAPA ATUAL ================\n");
    printf("%-3s | %-20s | %-12s | %-6s\n", "#", "Nome", "Cor", "Tropas");
    printf("--------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-20s | %-12s | %-6d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("============================================\n");
}

/* -------------------------
   Missões
   ------------------------- */
void atribuirMissao(char *destino, int maxLen) {
    int idx = rand() % TOTAL_MISSOES;
    strncpy(destino, MISSOES[idx], maxLen - 1);
    destino[maxLen - 1] = '\0';
}

/* -------------------------
   Combate / Ataque
   ------------------------- */
// Regra: cada lado rola 1..6. Se atacante vence, defensor troca de cor e
// recebe metade das tropas do atacante (transferidas). Se perde, atacante perde 1 tropa.
void atacar(Territorio *atacante, Territorio *defensor) {
    printf("\n>>> BATALHA: %s (%s, %d) ATACA %s (%s, %d)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    if (atacante->tropas <= 1) {
        printf("Atacante nao possui tropas suficientes (>1) para atacar.\n");
        return;
    }

    if (defensor->tropas <= 0) {
        // conquista automática
        int mov = atacante->tropas / 2;
        if (mov <= 0) mov = 1;
        atacante->tropas -= mov;
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = mov;
        printf("Defensor sem tropas. Conquista imediata. Tropas transferidas: %d\n", mov);
        return;
    }

    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        int mov = atacante->tropas / 2;
        if (mov <= 0) mov = 1;
        atacante->tropas -= mov;
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = mov;
        printf("Atacante venceu! Territorio %s conquistado. Tropas transferidas: %d\n",
               defensor->nome, mov);
    } else {
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Defensor venceu. Atacante perde 1 tropa.\n");
    }
}

/* -------------------------
   Verificação de Missão
   - adicionamos 'corJogador' para checar propriedades do jogador.
   - retornamos 1 se cumprida, 0 se não.
   ------------------------- */
int verificarMissao(const char *missao, Territorio *mapa, int n, const char *corJogador) {
    // identificar id da missão
    int id = -1;
    for (int i = 0; i < TOTAL_MISSOES; i++) {
        if (strcmp(missao, MISSOES[i]) == 0) { id = i; break; }
    }
    if (id == -1) return 0; // desconhecida

    switch (id) {
        case 0: { // "Conquistar 3 territorios seguidos"
            int consec = 0;
            for (int i = 0; i < n; i++) {
                if (equalsIgnoreCase(mapa[i].cor, corJogador)) {
                    consec++;
                    if (consec >= 3) return 1;
                } else consec = 0;
            }
            return 0;
        }
        case 1: { // "Eliminar todas as tropas da cor Vermelha"
            // missão específica: verifica se NÃO existe mais tropa em territórios com cor "Vermelha"
            for (int i = 0; i < n; i++) {
                if (equalsIgnoreCase(mapa[i].cor, "Vermelha") && mapa[i].tropas > 0) return 0;
            }
            return 1;
        }
        case 2: { // "Possuir 4 ou mais territorios no mapa"
            int cont = 0;
            for (int i = 0; i < n; i++)
                if (equalsIgnoreCase(mapa[i].cor, corJogador)) cont++;
            return cont >= 4;
        }
        case 3: { // "Reduzir a zero as tropas de pelo menos 1 inimigo"
            for (int i = 0; i < n; i++) {
                if (!equalsIgnoreCase(mapa[i].cor, corJogador) && mapa[i].tropas == 0) return 1;
            }
            return 0;
        }
        case 4: { // "Possuir todos os territorios com nome iniciando com A"
            for (int i = 0; i < n; i++) {
                if (mapa[i].nome[0] == 'A' || mapa[i].nome[0] == 'a') {
                    if (!equalsIgnoreCase(mapa[i].cor, corJogador)) return 0;
                }
            }
            return 1;
        }
        case 5: { // "Controlar exatamente 2 territorios"
            int cont = 0;
            for (int i = 0; i < n; i++) if (equalsIgnoreCase(mapa[i].cor, corJogador)) cont++;
            return cont == 2;
        }
        default:
            return 0;
    }
}

/* -------------------------
   Liberação de memória
   ------------------------- */
void liberarTudo(Territorio *mapa, Jogador *jogadores, int numJog) {
    if (mapa) free(mapa);
    if (jogadores) {
        for (int i = 0; i < numJog; i++) {
            if (jogadores[i].missao) {
                free(jogadores[i].missao);
                jogadores[i].missao = NULL;
            }
        }
        free(jogadores);
    }
}

/* -------------------------
   Programa principal
   ------------------------- */
int main(void) {
    srand((unsigned int) time(NULL));

    printf("=== WAR Estruturado (versao reescrita) ===\n");

    int n = lerInteiro("Quantos territorios deseja cadastrar? ");
    if (n <= 0) {
        printf("Numero de territorios invalido.\n");
        return 1;
    }

    Territorio *mapa = criarMapa(n);
    if (!mapa) {
        printf("Erro de alocacao de memoria para mapa.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);

    int numJogadores = 2; // mantemos 2 para simplicidade (pode ser parametrizado)
    Jogador *jogadores = (Jogador*) calloc(numJogadores, sizeof(Jogador));
    if (!jogadores) {
        printf("Erro de alocacao de jogadores.\n");
        free(mapa);
        return 1;
    }

    // cadastro dos jogadores
    for (int i = 0; i < numJogadores; i++) {
        printf("\n--- Cadastro Jogador %d ---\n", i + 1);
        printf("Nome: ");
        lerLinha(jogadores[i].nome, MAX_NOME);

        printf("Cor do jogador (identificador): ");
        lerLinha(jogadores[i].cor, MAX_COR);

        // aloca e atribui missão
        jogadores[i].missao = (char*) malloc(MAX_MISSAO_LEN);
        if (!jogadores[i].missao) {
            printf("Erro ao alocar missao.\n");
            liberarTudo(mapa, jogadores, numJogadores);
            return 1;
        }
        atribuirMissao(jogadores[i].missao, MAX_MISSAO_LEN);
        printf("Missao atribuida a %s: %s\n", jogadores[i].nome, jogadores[i].missao);
    }

    int vez = 0;
    int vencedor = -1;
    while (1) {
        printf("\n----------------------------------------\n");
        printf("Vez do jogador: %s (cor: %s)\n", jogadores[vez].nome, jogadores[vez].cor);
        exibirMapa(mapa, n);

        printf("1 - Atacar\n2 - Exibir mapa\n0 - Sair\n");
        int opc = lerInteiro("Escolha: ");

        if (opc == 0) {
            printf("Encerrando por escolha do usuario.\n");
            break;
        } else if (opc == 1) {
            int a = lerInteiro("Escolha territorio atacante (#): ");
            int d = lerInteiro("Escolha territorio defensor (#): ");
            a--; d--;
            if (a < 0 || a >= n || d < 0 || d >= n) {
                printf("Indices invalidos.\n");
            } else if (!equalsIgnoreCase(mapa[a].cor, jogadores[vez].cor)) {
                printf("Erro: voce so pode atacar com territorios que voce controla.\n");
            } else if (equalsIgnoreCase(mapa[d].cor, jogadores[vez].cor)) {
                printf("Erro: nao pode atacar territorios da mesma cor.\n");
            } else {
                atacar(&mapa[a], &mapa[d]);
            }
        } else if (opc == 2) {
            exibirMapa(mapa, n);
        } else {
            printf("Opcao invalida.\n");
        }

        // verificar missões dos jogadores silenciosamente ao final do turno
        for (int j = 0; j < numJogadores; j++) {
            if (verificarMissao(jogadores[j].missao, mapa, n, jogadores[j].cor)) {
                vencedor = j;
                break;
            }
        }
        if (vencedor != -1) break;

        vez = (vez + 1) % numJogadores;
    }

    if (vencedor != -1) {
        printf("\n*** PARABENS! Jogador %s (cor %s) cumpriu sua missao: %s ***\n",
               jogadores[vencedor].nome, jogadores[vencedor].cor, jogadores[vencedor].missao);
    }

    liberarTudo(mapa, jogadores, numJogadores);
    printf("Jogo finalizado. Memoria liberada.\n");
    return 0;
}
