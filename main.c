#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME 30
#define MAX_ITENS 100

void mostrarMenu() {
    printf("==================================\n");
    printf("   Controle de Estoque\n");
    printf("==================================\n");
    printf("1. Adicionar Item\n");
    printf("2. Remover Item\n");
    printf("3. Listar Estoque\n");
    printf("4. Sair\n");
    printf("Opção: ");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int menuOpcao() {
    int op;
    while (scanf("%d", &op) != 1 || op < 1 || op > 4) {
        limparBuffer();
        printf("Opção inválida! Tente novamente.\n");
        printf("Opção: ");
    }
    limparBuffer();
    return op;
}

int carregarEstoque(char nomes[][MAX_NOME], int quantidades[]) {
    FILE *f = fopen("estoque.txt", "r");
    if (f == NULL) return 0;

    int count = 0;
    while (fgets(nomes[count], MAX_NOME, f)) {
        nomes[count][strcspn(nomes[count], "\n")] = '\0';
        fscanf(f, "%d\n", &quantidades[count]);
        count++;
    }

    fclose(f);
    return count;
}

void salvarEstoque(char nomes[][MAX_NOME], int quantidades[], int total) {
    FILE *f = fopen("estoque.txt", "w");
    for (int i = 0; i < total; i++) {
        fprintf(f, "%s\n", nomes[i]);
        fprintf(f, "%d\n", quantidades[i]);
    }
    fclose(f);
}

void adicionarItem() {
    char nome[MAX_NOME];
    int qtd;

    printf("Digite o nome do item: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite a quantidade: ");
    scanf("%d", &qtd);
    limparBuffer();

    FILE *f = fopen("estoque.txt", "a");
    fprintf(f, "%s\n%d\n", nome, qtd);
    fclose(f);

    printf("Item adicionado com sucesso!\n");
}

void removerItem() {
    char nomes[MAX_ITENS][MAX_NOME];
    int quantidades[MAX_ITENS];
    int total = carregarEstoque(nomes, quantidades);

    char nome[MAX_NOME];
    int qtd;
    int encontrado = -1;

    printf("Digite o nome do item: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite a quantidade a ser removida: ");
    scanf("%d", &qtd);
    limparBuffer();

    for (int i = 0; i < total; i++) {
        if (strcmp(nomes[i], nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Item não encontrado.\n");
        return;
    }

    if (quantidades[encontrado] < qtd) {
        printf("Estoque insuficiente. Quantidade disponível: %d\n",
               quantidades[encontrado]);
        return;
    }

    quantidades[encontrado] -= qtd;

    if (quantidades[encontrado] == 0) {
        for (int i = encontrado; i < total - 1; i++) {
            strcpy(nomes[i], nomes[i + 1]);
            quantidades[i] = quantidades[i + 1];
        }
        total--;
        salvarEstoque(nomes, quantidades, total);
        printf("Item removido do estoque!\n");
    } else {
        salvarEstoque(nomes, quantidades, total);
        printf("Quantidade atualizada com sucesso!\n");
    }
}

void listarEstoque() {
    FILE *f = fopen("estoque.txt", "r");
    if (f == NULL) {
        printf("O estoque está vazio.\n");
        return;
    }

    char nome[MAX_NOME];
    int qtd;
    int vazio = 1;

    printf("=============================\n");
    printf("        Estoque Atual\n");
    printf("=============================\n");

    while (fgets(nome, MAX_NOME, f)) {
        nome[strcspn(nome, "\n")] = '\0';
        fscanf(f, "%d\n", &qtd);
        printf("Nome: %s\n", nome);
        printf("Quantidade: %d\n\n", qtd);
        vazio = 0;
    }

    fclose(f);

    if (vazio) {
        printf("O estoque está vazio.\n");
    }
}

int main() {
    int opcao;
    char continuar;

    do {
        mostrarMenu();
        opcao = menuOpcao();

        switch (opcao) {
        case 1: adicionarItem(); break;
        case 2: removerItem(); break;
        case 3: listarEstoque(); break;
        case 4:
            printf("Obrigado por usar o Controle de Estoques! Até a próxima.\n");
            return 0;
        }

        printf("Deseja realizar outra operação? (s/n): ");
        scanf(" %c", &continuar);
        limparBuffer();
        continuar = tolower(continuar);

    } while (continuar == 's');

    printf("Obrigado por usar o Controle de Estoques! Até a próxima.\n");
    return 0;
}
