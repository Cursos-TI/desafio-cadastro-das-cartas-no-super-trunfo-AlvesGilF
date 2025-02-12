#include <stdio.h>

#define MAX_CIDADES 100  // Número máximo de cidades que podem ser cadastradas

// Estrutura para armazenar informações de uma cidade
typedef struct {
    char estado;           // Letra do estado (A-H)
    char codigo[4];        // Código da carta (ex: A01, B02)
    char nome[50];         // Nome da cidade
    int populacao;         // População da cidade
    float area;            // Área da cidade em km²
    float pib;             // PIB da cidade em bilhões
    int pontosTuristicos;  // Número de pontos turísticos
} Cidade;

int main() {
    Cidade cidades[MAX_CIDADES]; // Array para armazenar as cidades
    int count = 0;               // Contador de cidades cadastradas
    char continuar;

    // Loop principal para cadastrar cidades
    do {    
        printf("\nCadastro de Nova Cidade\n");
        
        // Estado (Letra de A a H)
        do {
            printf("Digite a letra do Estado (A a H): ");
            scanf(" %c", &cidades[count].estado);
        } while (cidades[count].estado < 'A' || cidades[count].estado > 'H');

        // Código da Carta (Ex: A01, B02)
        printf("Digite o código da carta (ex: A01, B02): ");
        scanf("%3s", cidades[count].codigo);

        // Nome da Cidade
        printf("Digite o nome da cidade: ");
        scanf(" %[^\n]", cidades[count].nome); // Lê a string com espaços

        // População
        printf("Digite a população da cidade: ");
        scanf("%d", &cidades[count].populacao);

        // Área em km²
        printf("Digite a área da cidade (km²): ");
        scanf("%f", &cidades[count].area);

        // PIB em bilhões
        printf("Digite o PIB da cidade (em bilhões): ");
        scanf("%f", &cidades[count].pib);

        // Número de pontos turísticos
        printf("Digite o número de pontos turísticos: ");
        scanf("%d", &cidades[count].pontosTuristicos);
        
        count++; // Incrementa o número de cidades cadastradas

        // Pergunta se o usuário deseja cadastrar outra cidade
        printf("Deseja cadastrar outra cidade? (s/n): ");
        getchar(); // Limpa buffer antes de ler o caractere
        scanf("%c", &continuar);

    } while ((continuar == 's' || continuar == 'S') && count < MAX_CIDADES);

    // Exibição das cidades cadastradas
    printf("\n=== CIDADES CADASTRADAS ===\n");
    for (int i = 0; i < count; i++) {
        printf("\n---------------------------------------------\n");
        printf("Estado: %c\n", cidades[i].estado);
        printf("Carta: %s\n", cidades[i].codigo);
        printf("Nome da Cidade: %s\n", cidades[i].nome);
        printf("População: %d pessoas\n", cidades[i].populacao);
        printf("Área: %.2f km²\n", cidades[i].area);
        printf("PIB: R$ %.2f bilhões\n", cidades[i].pib);
        printf("Número de Pontos Turísticos: %d\n", cidades[i].pontosTuristicos);
        printf("---------------------------------------------\n");
    }

    return 0;
}