#include "App.h"
#include <stdlib.h>
#include <stdio.h>

App* createApp() {
    App* app = malloc(sizeof(App));
    Memory* memory = createMemory();
    app->treeController = createTreeController(memory);
    app->professionalController = createProfessionalController(memory);
    return app;
}

void flushBuffer() {
    int ch;
    do {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}

void startApp(App* app)
{
    Option option;

    do {
        printMenu();

        if (!requestOption(&option))
            continue;

        executeOption(app,option);

    } while (option != OP_EXIT);
}

int requestOption(Option* op)
{
    if (scanf("%u%*c", op) != 1) {
        flushBuffer();
        return 0;
    }
    return 1;
}

Professional* requestProfessional()
{
    Professional* p = malloc(sizeof(Professional));
    printf("Insira os dados do profissional\n");
    printf("Codigo: ");
    scanf("%d%*c", &p->code);
    printf("Nome: ");
    fgets(p->name, 51, stdin);
    printf("CPF: ");
    fgets(p->cpf, 12, stdin);
    printf("Numero de Registro: ");
    fgets(p->registryNumber,31 , stdin);
    printf("Endereco: ");
    fgets(p->address, 101, stdin);
    printf("Telefone: ");
    fgets(p->phone, 30, stdin);
    return p;
}


void executeOption(App* app, Option op)
{
    switch (op) {
        case OP_INSERT:
            insert(app);
            break;
        case OP_REMOVE:
            break;
        case OP_CH_ADDR:
            break;
        case OP_CH_PHONE:
            break;
        case OP_LOAD:
            break;
        case OP_PRINT:
            break;
        case OP_PRINT_ASC:
            printAsc(app->treeController);
            break;
        case OP_PRINT_LVL:
            printByLevel(app->treeController);
            break;
        case OP_FREE_DATA:
            printFreePositionsDataHeader(app->treeController->memory);
            break;
        case OP_FREE_INDEX:
            printFreePositionsIndexHeader(app->treeController->memory);
            break;
        case OP_EXIT:
            break;
    }
}

void printDatafile(FILE* file) {
    DataHeader* h = readDataHeader(file);
    printf("Topo => %d | Livre => %d\n",h->top,h->free);
    Professional* p = malloc(sizeof(Professional));
    while (fread(p,sizeof(Professional), 1, file)) {
        printf("Codigo => %d\n", p->code);
    }
    free(h);
    free(p);
}


void printIndexFile(App* app) {
    IndexHeader * h = readIndexHeader(app->treeController->memory);
    printf("Topo => %d | Livre => %d | Raiz => %d\n", h->top, h->free, h->root);
    printf("POS => %d ", h->root);
    free(h);
}


void insert(App* app)
{
//    Professional* professional = requestProfessional();
//    int index = insertProfessional(app->professionalController, professional);
    insertKey(app->treeController, 32, 3);
    insertKey(app->treeController, 33, 5);
    insertKey(app->treeController, 12, 7);
    insertKey(app->treeController, 11, 9);
    insertKey(app->treeController, 30, 7);
    insertKey(app->treeController, 80, 7);
    insertKey(app->treeController, 82, 7);
    insertKey(app->treeController, 91, 7);
    insertKey(app->treeController, 4, 7);
    insertKey(app->treeController, 23, 7);
    insertKey(app->treeController, 1, 7);
    insertKey(app->treeController, 23, 7);
    insertKey(app->treeController, 14, 7);
    insertKey(app->treeController, 75, 7);
    insertKey(app->treeController, 63, 7);
    insertKey(app->treeController, 23, 7);
    insertKey(app->treeController, 10, 7);
    insertKey(app->treeController, 45, 7);
    insertKey(app->treeController, 6, 7);
    insertKey(app->treeController, 65, 7);
    insertKey(app->treeController, 64, 7);
    insertKey(app->treeController, 18, 7);
    insertKey(app->treeController, 7, 7);

    printf("=====================\n");
    printByLevel(app->treeController);
    printf("=====================\n");
//    free(professional);
}
void printMenu() {
    printf("\n1 - Cadastrar Profissional\n");
    printf("2 - Remover Profissional\n");
    printf("3 - Alterar Endereco de um Profissional\n");
    printf("4 - Alterar Telefone de um Profissional\n");
    printf("5 - Carregar arquivo de operacoes\n");
    printf("6 - Exibir informacoes de um Profissional\n");
    printf("7 - Listar Profissionais em ordem crescente\n");
    printf("8 - Imprimir arvore por niveis\n");
    printf("9 - Exibir posicoes livres do arquivo de dados\n");
    printf("10 - Exibir posicoes livres do arquivo de indices\n");
    printf("11 - Sair\n\n");
    printf("Insira o numero da opcao desejada: ");
}