#include <stdio.h>

int main() {

    // Estrutura que representa um pais.
    char estado[4]; 
    int numerodacidade;
    int populacao;
    float area;
    float pib;
    int pontosturisticos;

    char continuar;

    // função abaixo loop principal para cadastrar as cidades
    do 
    {    

    // Função para cadastrar uma cidade.
    printf("Digite o Estado (A a H): \n");
    scanf("%s", &estado);

    printf("Digite o numero da cidade: \n");
    scanf("%d", &numerodacidade);

    printf("Digite a quantidade da população da cidade: \n");
    scanf("%d", &populacao);

    printf("Digite a area: \n");
    scanf("%f", &area);

    printf("Digite o PIB: \n");
    scanf("%f", &pib);

    printf("Digite a quantidade de pontos turisticos: \n");
    scanf("%d", &pontosturisticos);
    printf("\n");

    // Exibição dos Dados das Cartas:
    printf("Carta: %s%02d\n", estado, numerodacidade);
    printf("População: %d pessoas \n", populacao);
    printf("Area: %.2f km²\n", area);
    printf("PIB: R$ %.2f bilhoes \n", pib);
    printf("Numeros de Pontos Turisticos: %d totais \n", pontosturisticos);
    printf("---------------------------------------------\n");

   // Pergunta se o usuário deseja cadastrar outra cidade
        printf("Deseja cadastrar outra cidade? (s/n): ");
        scanf(" %c", &continuar);  // Para recebe a resposta do usuário
        
    } while (continuar == 's' || continuar == 'S');  // Se o usuário digitar 's' ou 'S', continua cadastrando

 
    return 0;
}
