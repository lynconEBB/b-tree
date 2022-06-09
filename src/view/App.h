#ifndef BTREE_APP_H
#define BTREE_APP_H
#include "../handlers/TreeHandler.h"
#include "../handlers/DataHandler.h"
#include "../handlers/IndexHandler.h"

// Enum contendo as opcoes disponiveis a serem executadas
typedef enum {
    OP_INSERT = 1,
    OP_REMOVE = 2,
    OP_UPDATE = 3,
    OP_LOAD = 4,
    OP_PRINT = 5,
    OP_PRINT_ASC = 6,
    OP_PRINT_LVL = 7,
    OP_FREE_DATA = 8,
    OP_FREE_INDEX = 9,
    OP_EXIT=10
} Option;

// Estrutura representando a aplicação e suas dependencias
typedef struct{
    Option option;
    DataHandler* dataHandler;
    IndexHandler* indexHandler;
    TreeHandler* treeHandler;
} App;

// Cria uma instancia da aplicação
// Pré-condição: Nenhuma
// Pós-condição: Instancia valida da aplicação
App* createApp();

// Libera memoria utilizada pela aplicação
// Pré-condição: Ponteiro para aplicação valido
// Pós-condição: Aplicação destruida, ponteiro invalidado
void destroyApp(App* app);

// Inicia o loop do programa
// Pré-condição: Ponteiro para instancia valida do app
// Pós-condição: Nenhuma
void startApp(App* app);

// Executa a opção presente no app
// Pré-condição: Ponteiro para instancia valida do app
// Pós-condição: Opcao executada caso encontrada
void executeOption(App* app);

// Insere o profissional fornecido, salvando no arquivo de memória
// Pré-condição: Ponteiro para instancia valida do app e ponteiro válido para profissional
// Pós-condição: Profissional inserido caso código não exista no sistema
void insert(App* app,Professional* professional);

// Remove o profissional com o código fornecido
// Pré-condição: Ponteiro para instancia valida do app
// Pós-condição: Profissional removido caso código seja encontrado
void removeOne(App* app, int code);

// Requisita um código para o usuário e atualiza o profissional com
// com este código usando endereço e telefone fornecidos pleo usuário
// Pré-condição: Ponteiro para instancia valida do app
// Pós-condição: Informaçoes do profissional atualizadas caso código seja encontrado
void update(App* app);

// Requeista e lê arquivo contendo operações executando-as
// linha a linha
// Pré-condição: Ponteiro para instancia valida do app
// Pós-condição: Nós criados, alterados ou removidos
void load(App* app);

// Requisita um código ao usuário e busca esse código, caso seja encontrado
// as informacoes sao apresentadas, caso contrario uma mensagem é apresentada
// Pré-condição: Ponteiro para instancia valida do app
// Pós-condição: Nenhuma
void printProfessionalByCode(App* app);

// Imprime profissionais em ordem crescente, caso nenhum profssional seja encontrado
// uma mensagem é apresentada
// Pré-condição: Ponteiro para instancia valida do app
// Pós-condição: Nenhuma
void printAsc(App* app);

// Imprime os nós por nivel
// Pré-condição: Ponteiro para instancia valida do app
// Pós-condição: Nenhuma
void printByLevel(App* this);

// Imprime as informações do profissional passado
// Pré-condição: Ponteiro para profissional válido
// Pós-condição: Nenhuma
void printProfessional(Professional* professional);

// Imprime o menu com as opçoes disponiveis
// Pré-condição: Nenhuma
// Pós-condição: Nenhuma
void printMenu();

// Imprime as chaves do nó passdo por parametro
// Pré-condição: Ponteiro para nó valido
// Pós-condição: Nenhuma
void printNode(Node *node);

// Requisita opcao ao usuário
// Pré-condição: Ponteiro para instancia valida do app
// Pós-condição: Opcao do app preenchida com opçao lida do usuário
int requestOption(App* app);

// Requisita as informaçẽes necessárias de um profissional
// Pré-condição: Nenhuma
// Pós-condição: Retorna profissional com informacoes fornecidas
Professional* requestProfessional();

// Requisita um caminho para arquivo e o abre
// Pré-condição: Nenhuma
// Pós-condição: Ponteiro para arquivo aberto em modo de leitura
FILE* requestFile();

#endif