#include "App.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

App *createApp() {
    App *app = malloc(sizeof(App));
    app->dataHandler = createDataHandler();
    app->indexHandler = createIndexHandler();
    app->treeHandler = createTreeHandler(app->indexHandler);
    return app;
}

static char* aux_trim(char *str)
{
    while(*str == ' ')
        str++;

    if(*str == 0)
        return str;

    char* end = str + strlen(str) - 1;
    while(end > str && *end == ' ')
        end--;

    end[1] = '\0';
    return str;
}

static void aux_flushBuffer() {
    int ch;
    do {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}

static char* aux_readLine()
{
    char tmp[100], *str = NULL;
    int n, used = 0;
    while (1) {
        if (!fgets(tmp, sizeof(tmp), stdin))
            return str;

        n = strcspn(tmp,"\n");
        tmp[n] = '\0';

        str = realloc(str, sizeof(char) * (used + n + 1));
        memcpy(str + used,tmp, n + 1 );
        used += n;

        if (n + 1 < sizeof(tmp)) {
            break;
        }
    }
    return str;
}

int requestOption(App *app) {
    if (scanf("%u%*c", &app->option) != 1) {
        aux_flushBuffer();
        return 0;
    }
    return 1;
}

void startApp(App *app) {
    do {
        printMenu();
        if (!requestOption(app))
            continue;
        executeOption(app);
    } while (app->option != OP_EXIT);
}

void executeOption(App *app) {
    switch (app->option) {
        case OP_INSERT: {
            Professional *professional = requestProfessional();
            insert(app, professional);
            free(professional);
        }break;
        case OP_REMOVE: {
            printf("Insira o código do profissional para remover:");
            int code;
            if (scanf("%d%*c", &code) != 1) {
                printf("Entrada invalida!\n");
                return;
            }
            removeOne(app,code);
        } break;
        case OP_UPDATE:
            update(app);
            break;
        case OP_LOAD:
            load(app);
            break;
        case OP_PRINT:
            printProfessionalByCode(app);
            break;
        case OP_PRINT_ASC:
            printAsc(app);
            break;
        case OP_PRINT_LVL:
            printByLevel(app);
            break;
        case OP_FREE_DATA:
            printFreePositionsDataHeader(app->dataHandler);
            break;
        case OP_FREE_INDEX:
            printFreePositionsIndexHeader(app->indexHandler);
            break;
        case OP_EXIT:
            break;
    }
}

Professional *requestProfessional() {
    Professional *p = malloc(sizeof(Professional));
    printf("Insira os dados do profissional\n");
    printf("Codigo: ");
    scanf("%d%*c", &p->code);
    printf("Nome: ");
    fgets(p->name, 51, stdin);
    printf("CPF: ");
    fgets(p->cpf, 12, stdin);
    printf("Numero de Registro: ");
    fgets(p->registryNumber, 31, stdin);
    printf("Endereco: ");
    fgets(p->address, 101, stdin);
    printf("Telefone: ");
    fgets(p->phone, 30, stdin);
    return p;
}

void insert(App *app, Professional* professional) {
    int pos = search(app->treeHandler, professional->code);
    if (pos != -1) {
        printf("Profissional existente no sistema! Operacao de insercao ignorada!\n");
    } else {
        int ref = addProfessional(app->dataHandler, professional);
        insertKey(app->treeHandler, professional->code, ref);
        printf("Profissional com codigo %d inserido com sucesso!!\n", professional->code);
    }
}

void removeOne(App *app, int code) {
    int ref = search(app->treeHandler, code);
    if (ref == -1) {
        printf("Código %d nao encontrado no sistema!\n", code);
        return;
    }
    removeKey(app->treeHandler, code);
    removeProfessional(app->dataHandler, ref);
    printf("Profissional removido com sucesso!\n");
}

void update(App *app) {
    printf("Insira o código do profissional:");
    int code;
    if (scanf("%d%*c", &code) != 1) {
        printf("Entrada invalida!\n");
        return;
    }
    int ref = search(app->treeHandler, code);
    if (ref == -1) {
        printf("Código nao encontrado no sistema!\n");
        return;
    }
    Professional *professional = readProfessional(app->dataHandler, ref);
    char phone[20], address[100];
    printf("Insira o novo telefone (ignore para manter como: %s):", professional->phone);
    fgets(phone, 101, stdin);
    phone[strcspn(phone,"\n")] = '\0';
    printf("Insira o novo endereco (ignore para manter como: %s):", professional->address);
    fgets(address, 101, stdin);
    address[strcspn(address,"\n")] = '\0';
    if (address[0] != '\0')
        strcpy(professional->address, address);
    if (phone[0] != '\0')
        strcpy(professional->phone, phone);
    writeProfessional(app->dataHandler, professional, ref);
    printf("Profissional atualizado com sucesso!\n");
}


FILE* requestFile() {
    printf("Insira o arquivo:");
    FILE* file;
    while (1) {
        char *fileName = aux_readLine();
        file = fopen(fileName, "r");
        free(fileName);

        if (file == NULL)
            printf("Arquivo nao encontrado, insira outro arquivo:");
        else
            break;
    }
    return file;
}


void load(App *app) {
    FILE* file = requestFile();
    char buffer[400];
    char operation;

    while(fgets(buffer, 400, file)) {
        Professional* professional = malloc(sizeof(Professional));
        buffer[strcspn(buffer,"\n")] = '\0';
        char* bufferPtr = aux_trim(buffer);

        if (sscanf(bufferPtr,"%c",&operation) != 1) {
            printf("Nenhuma operação encontrada na linha\n");
            break;
        }
        bufferPtr += 2;
        switch (operation) {
            case 'I': {
                int result = sscanf(bufferPtr,"%d;%[^;];%[^;];%[^;];%[^;];%s",
                                    &professional->code, professional->name,professional->cpf,
                                    professional->registryNumber, professional->address, professional->phone);
                if (result != 6) {
                    printf("Informacoes mal formatadas para processo de insercao!\n");
                    break;
                }
                insert(app, professional);
            } break;
            case 'R': {
                int result = sscanf(bufferPtr,"%d", &professional->code);
                if (result != 1) {
                    printf("Informacoes mal formatadas para processo de remocao!\n");
                    break;
                }
                removeOne(app,professional->code);
            } break;
            case 'A': {
                int result = sscanf(bufferPtr,"%d", &professional->code);
                if (result < 1) {
                    printf("Informacoes mal formatadas para processo de atualizacao!\n");
                    break;
                }
                int ref = search(app->treeHandler, professional->code);
                if (ref == -1) {
                    printf("Código %d nao encontrado no sistema para realizar atualizacao!\n", professional->code);
                    break;
                }
                free(professional);
                professional = readProfessional(app->dataHandler, ref);

                bufferPtr = &bufferPtr[strcspn(bufferPtr,";") + 1];
                sscanf(bufferPtr,"%[^;]", professional->address);
                bufferPtr = &bufferPtr[strcspn(bufferPtr,";") + 1];
                sscanf(bufferPtr,"%s",professional->phone);

                writeProfessional(app->dataHandler, professional, ref);
                printf("Profissional %d atualizado com sucesso!\n", professional->code);
            } break;
        }
        free(professional);
    }
}

void printProfessionalByCode(App *app) {
    printf("Insira o código do profissional:");
    int code;
    if (scanf("%d%*c", &code) != 1) {
        printf("Entrada invalida!\n");
        return;
    }
    int ref = search(app->treeHandler, code);
    if (ref == -1) {
        printf("Código nao encontrado no sistema!\n");
        return;
    }
    Professional *professional = readProfessional(app->dataHandler, ref);
    printProfessional(professional);
    free(professional);
}

void printAsc(App *app) {
    Queue *queue = getReferencesAsc(app->treeHandler);
    if (isEmpty(queue)) {
        printf("Nenhum profissional cadastrado no sistema!\n");
    }
    while (!isEmpty(queue)) {
        Professional *professional = readProfessional(app->dataHandler, queue->first->data);
        printProfessional(professional);
        dequeue(queue);
        free(professional);
    }
    free(queue);
}

void printByLevel(App *app) {
    IndexHeader *header = getIndexHeader(app->indexHandler);
    if (header->root == -1)
        return;

    Queue *queue = newQueue();
    enqueue(queue, header->root);
    enqueue(queue, -1);
    while (1) {
        int pos = queue->first->data;
        dequeue(queue);
        if (pos != -1) {
            Node *node = readNode(app->indexHandler, pos);
            printNode(node);
            for (int i = 0; node->children[i] != -1 && i < DEGREE; i++) {
                enqueue(queue, node->children[i]);
            }
            free(node);
        } else {
            printf("\n");
            if (isEmpty(queue))
                break;
            enqueue(queue, -1);
        }
    }
}

void printProfessional(Professional* professional) {
    printf("=========== Profissional %d ===========\n",professional->code );
    printf("Nome: %s\n", professional->name);
    printf("CPF: %s\n", professional->cpf);
    printf("Numero de Registro: %s\n", professional->registryNumber);
    printf("Endereco: %s\n", professional->address);
    printf("Telefone: %s\n\n", professional->phone);
}

void printNode(Node *node) {
    printf("[ ");
    for (int i = 0; i < node->size; i++) {
        printf("%d ", node->keys[i]);
    }
    printf("] ");
}

void printMenu() {
    printf("\n1 - Cadastrar Profissional\n");
    printf("2 - Remover Profissional\n");
    printf("3 - Atualizar Profissional\n");
    printf("4 - Carregar arquivo de operacoes\n");
    printf("5 - Exibir informacoes de um Profissional\n");
    printf("6 - Listar Profissionais em ordem crescente\n");
    printf("7 - Imprimir arvore por niveis\n");
    printf("8 - Exibir posicoes livres do arquivo de dados\n");
    printf("9 - Exibir posicoes livres do arquivo de indices\n");
    printf("10 - Sair\n\n");
    printf("Insira o numero da opcao desejada: ");
}