#include <stdio.h>
#include <string.h>

/*
    Definição da struct Territorio:
    Armazena informações de cada território (nome, cor do exército, tropas).
*/
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {

    // Vetor para armazenar os 5 territórios
    Territorio territorios[5];

    printf("=== Cadastro de Territorios ===\n");

    // Cadastro dos territórios
    for (int i = 0; i < 5; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);

        printf("Digite o nome do territorio: ");
        scanf("%s", territorios[i].nome);

        printf("Digite a cor do exercito: ");
        scanf("%s", territorios[i].cor);

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // Exibição de todos os resultados ao final
    printf("\n\n=====================================\n");
    printf("     RESULTADOS DO CADASTRO\n");
    printf("=====================================\n");

    for (int i = 0; i < 5; i++) {
        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
    }

    printf("\n=====================================\n");
    printf("   Fim da exibicao de resultados.\n");
    printf("=====================================\n");

    return 0;
}
