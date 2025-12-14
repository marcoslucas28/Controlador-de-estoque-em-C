#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <stdbool.h>

#define ITEM_MAX 30

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int lerNumero() {
    int num;
    while (scanf("%d", &num) != 1) {
        printf("Entrada inválida. Digite um número válido: ");
        limparBuffer();
    }
    limparBuffer();
    return num;
}

int contarItens() {
    FILE *ponteiro = fopen("estoque.txt", "r");
    if (ponteiro == NULL) return 0;

    int count = 0;
    char linha[40];

    while (fscanf(ponteiro, "%s", linha) == 1) {
        count++;
    }

    fclose(ponteiro);
    return count / 2;
}

void load_file(char **nomes, int *quantidades, int quant) {
    FILE *ponteiro = fopen("estoque.txt", "r");
    if (ponteiro == NULL) return;

    for (int i = 0; i < quant; i++) {
        fscanf(ponteiro, "%s", nomes[i]);
        fscanf(ponteiro, "%d", &quantidades[i]);
    }

    fclose(ponteiro);
}

void salvarFile(char **nomes, int *quantidades, int quant) {
    FILE *ponteiro = fopen("estoque.txt", "w");

    for (int i = 0; i < quant; i++) {
        fprintf(ponteiro, "%s\n", nomes[i]);
        fprintf(ponteiro, "%d\n", quantidades[i]);
    }

    fclose(ponteiro);
}

void listarItem(char **nomes, int quant) {
    printf("--------------------------------------------------\n");
    for (int i = 0; i < quant; i++) {
        printf("%d - %s\n", i + 1, nomes[i]);
    }
    printf("--------------------------------------------------\n");
}

void mostrarTabela(char **nomes, int *quantidades, int quant) {
    printf("--------------------------------------------------\n");
    printf("%-30s%-20s\n", "Nome", "Quantidade");
    for (int i = 0; i < quant; i++) {
        printf("%-30s%-20d\n", nomes[i], quantidades[i]);
    }
    printf("--------------------------------------------------\n");
}

void adicionarItem(char **nomes, int *quantidades, int quant) {
    int escolha, qtd;

    listarItem(nomes, quant);
    printf("Escolha o item: ");
    escolha = lerNumero();

    if (escolha < 1 || escolha > quant) return;

    printf("Quantidade a adicionar: ");
    qtd = lerNumero();

    quantidades[escolha - 1] += qtd;
    salvarFile(nomes, quantidades, quant);
}

void removerItem(char **nomes, int *quantidades, int quant) {
    int escolha, qtd;

    listarItem(nomes, quant);
    printf("Escolha o item: ");
    escolha = lerNumero();

    if (escolha < 1 || escolha > quant) return;

    printf("Quantidade a remover: ");
    qtd = lerNumero();

    quantidades[escolha - 1] -= qtd;
    salvarFile(nomes, quantidades, quant);
}

int main() {
    setlocale(LC_ALL, "");

    int quant = contarItens();
    if (quant == 0) {
        printf("Arquivo vazio ou inexistente.\n");
        return 1;
    }

    char **nomes = malloc(quant * sizeof(char *));
    for (int i = 0; i < quant; i++) {
        nomes[i] = malloc(ITEM_MAX * sizeof(char));
    }

    int *quantidades = malloc(quant * sizeof(int));

    load_file(nomes, quantidades, quant);

    int opcao;
    bool continuar = true;

    while (continuar) {
        printf("\n1. Listar\n2. Adicionar\n3. Remover\n4. Sair\nOpção: ");
        opcao = lerNumero();

        switch (opcao) {
        case 1: mostrarTabela(nomes, quantidades, quant); break;
        case 2: adicionarItem(nomes, quantidades, quant); break;
        case 3: removerItem(nomes, quantidades, quant); break;
        case 4: continuar = false; break;
        }
    }

    printf("Obrigado por usar o sistema!\n");
    return 0;
}
