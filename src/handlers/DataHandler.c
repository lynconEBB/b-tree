#include "DataHandler.h"
#include <stdlib.h>

// Cria um manipulador do arquivo de dados, iniciando o cabeçalho e criando um arquivo,
// caso não exista
// Pré-condição: Nenhuma
// Pós-condição: Ponteiro válido para um manipulador de dados
DataHandler *createDataHandler() {
    DataHandler* this = malloc(sizeof(DataHandler));
    char fileName[] = "dataFile.bin";
    this->file = fopen(fileName,"r+b");
    if (this->file == NULL)  {
        this->file = fopen(fileName, "w");
        fclose(this->file);
        this->file = fopen(fileName,"r+b");
    }
    this->header = readDataHeader(this);
    return this;
}


// Liber memoria utilizada pelo manipulador de dados e fecha arquivo utilizado
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Manipulador de dados invalidado
void freeDataHandler(DataHandler *this) {
    fclose(this->file);
    free(this->header);
    free(this);
}

// Adiciona novo profissional ao arquivo de dados, caso nenhuma posiçao livre exista
// o profissional é adicionado no topo do arquivo, caso contrario a posicao livre é utilizada
// Pré-condição: Ponteiro para manipulador de dados valido e ponteiro para profissional
// Pós-condição: Retorna a posição do profissional no arquivo de dados
int addProfessional(DataHandler* this, Professional* professional) {
    int pos;
    if (this->header->free == -1) {
        pos = this->header->top;
        this->header->top++;
    } else {
        pos = this->header->free;
        Professional* aux = readProfessional(this,this->header->free);
        this->header->free = aux->code;
        free(aux);
    }
    writeProfessional(this, professional,pos);
    writeDataHeader(this);
    return pos;
}

// Adiciona novo profissional ao arquivo de dados, caso nenhuma posiçao livre exista
// o profissional é adicionado no topo do arquivo, caso contrario a posicao livre é utilizada
// Pré-condição: Ponteiro para manipulador de dados valido e ponteiro para profissional
// Pós-condição: Retorna a posição do profissional no arquivo de dados
void removeProfessional(DataHandler* this, int pos) {
    Professional* p = readProfessional(this,pos);
    p->code = this->header->free;
    this->header->free = pos;
    writeDataHeader(this);
    writeProfessional(this, p, pos);
    free(p);
}

// Escreve o cabeçalho atual no arquivo de dados
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Cabeçalho salvo no arquivo
void writeDataHeader(DataHandler* this) {
    fseek(this->file, 0, SEEK_SET);
    fwrite(this->header, sizeof(DataHeader), 1, this->file);
}

// Fornece o cabeçalho de dados atual
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Ponteiro para cabeçalho atual
DataHeader* getDataHeader(DataHandler* this) {
    return this->header;
}

// Lê e retorna o cabeçalho do arquivo de dados mo
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Caso o cabeçalho não seja encontrado no arquivo um cabeçalho novo é adicionado,
// o ponteiro de cabeçalho do manipulador de dados é preenchido com o cabeçalho lido
DataHeader* readDataHeader(DataHandler* this) {
    free(this->header);
    this->header = malloc(sizeof(DataHeader));
    fseek(this->file,0, SEEK_SET);
    int success = fread(this->header, sizeof(DataHeader), 1, this->file);
    if (!success) {
        this->header->top = 0;
        this->header->free = -1;
        writeDataHeader(this);
    }
    return this->header;
}

// Lê o profissional do arquivo de dados na posição fornecida
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Retorna profissional lido na posição
Professional* readProfessional(DataHandler* this, int pos) {
    Professional* p = malloc(sizeof(Professional));
    fseek(this->file, sizeof(DataHeader) + pos * sizeof(Professional), SEEK_SET);
    fread(p, sizeof(Professional), 1, this->file);
    return p;
}

// Escreve o profissional fornecido no arquivo de dados na posição fornecida
// Pré-condição: Ponteiro para manipulador de dados, profissional valida
// Pós-condição: Profissional salvo no arquivo de dados
void writeProfessional(DataHandler* this, Professional* professional, int pos) {
    fseek(this->file, sizeof(DataHeader) + pos * sizeof(Professional), SEEK_SET);
    fwrite(professional, sizeof(Professional), 1, this->file);
}

// Imprime as posições livres do arquivo
// Pré-condição: Ponteiro para manipulador de dados valido
// Pós-condição: Nenhuma
void printFreePositionsDataHeader(DataHandler* this) {
    if (this->header->free == -1) {
        printf("Nenhuma posicao livre no arquivo de dados");
        return;
    }

    int freePos = this->header->free;
    printf("Posições livres: %d ", freePos);
    do {
        Professional* pro = readProfessional(this,freePos);
        freePos = pro->code;
        if (freePos != -1)
            printf("%d ", freePos);
        free(pro);
    } while (freePos != -1);
}