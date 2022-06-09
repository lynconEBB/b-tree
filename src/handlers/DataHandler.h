#ifndef BTREE_DATAHANDLER_H
#define BTREE_DATAHANDLER_H
#include <stdio.h>

// Estrutura representando as informacoes de um profissional
typedef struct {
    int code;
    char name[51];
    char cpf[13];
    char registryNumber[31];
    char address[101];
    char phone[20];
} Professional;

// Estrutura do cabeçalho do arquivo de dados
typedef struct {
    int top;
    int free;
} DataHeader;

// Estrutura do Manipulador de dados
// Utilizado para gerenciamento das informações no arquivo de dados
typedef struct {
    FILE* file;
    DataHeader* header;
} DataHandler;

// Cria um manipulador do arquivo de dados, iniciando o cabeçalho e criando um arquivo,
// caso não exista
// Pré-condição: Nenhuma
// Pós-condição: Ponteiro válido para um manipulador de dados
DataHandler* createDataHandler();

// Liber memoria utilizada pelo manipulador de dados e fecha arquivo utilizado
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Manipulador de dados invalidado
void freeDataHandler(DataHandler* this);

// Adiciona novo profissional ao arquivo de dados, caso nenhuma posiçao livre exista
// o profissional é adicionado no topo do arquivo, caso contrario a posicao livre é utilizada
// Pré-condição: Ponteiro para manipulador de dados valido e ponteiro para profissional
// Pós-condição: Retorna a posição do profissional no arquivo de dados
int addProfessional(DataHandler* this, Professional* professional);

// Adiciona novo profissional ao arquivo de dados, caso nenhuma posiçao livre exista
// o profissional é adicionado no topo do arquivo, caso contrario a posicao livre é utilizada
// Pré-condição: Ponteiro para manipulador de dados valido e ponteiro para profissional
// Pós-condição: Retorna a posição do profissional no arquivo de dados
void removeProfessional(DataHandler* this, int pos);

// Escreve o cabeçalho atual no arquivo de dados
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Cabeçalho salvo no arquivo
void writeDataHeader(DataHandler* this);

// Lê e retorna o cabeçalho do arquivo de dados mo
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Caso o cabeçalho não seja encontrado no arquivo um cabeçalho novo é adicionado,
// o ponteiro de cabeçalho do manipulador de dados é preenchido com o cabeçalho lido
DataHeader* readDataHeader(DataHandler* this);

// Fornece o cabeçalho de dados atual
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Ponteiro para cabeçalho atual
DataHeader* getDataHeader(DataHandler* this);

// Imprime as posições livres do arquivo
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Nenhuma
void printFreePositionsDataHeader(DataHandler* this);

// Lê o profissional do arquivo de dados na posição fornecida
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Retorna profissional lido na posição
Professional* readProfessional(DataHandler* memory, int pos);

// Escreve o profissional fornecido no arquivo de dados na posição fornecida
// Pré-condição: Ponteiro para manipulador de dados, profissional valida
// Pós-condição: Profissional salvo no arquivo de dados
void writeProfessional(DataHandler* file, Professional* professional, int pos);

#endif