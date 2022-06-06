#include "DataHandler.h"
#include <stdlib.h>

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

void removeProfessional(DataHandler* this, int pos) {
    Professional* p = readProfessional(this,pos);
    p->code = this->header->free;
    this->header->free = pos;
    writeDataHeader(this);
    writeProfessional(this, p, pos);
    free(p);
}

void writeDataHeader(DataHandler* this) {
    fseek(this->file, 0, SEEK_SET);
    fwrite(this->header, sizeof(DataHeader), 1, this->file);
}

DataHeader* getDataHeader(DataHandler* this) {
    return this->header;
}

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

Professional* readProfessional(DataHandler* this, int pos) {
    Professional* p = malloc(sizeof(Professional));
    fseek(this->file, sizeof(DataHeader) + pos * sizeof(Professional), SEEK_SET);
    fread(p, sizeof(Professional), 1, this->file);
    return p;
}

void writeProfessional(DataHandler* this, Professional* professional, int pos) {
    fseek(this->file, sizeof(DataHeader) + pos * sizeof(Professional), SEEK_SET);
    fwrite(professional, sizeof(Professional), 1, this->file);
}

void printFreePositionsDataHeader(DataHandler* this) {
    if (this->header->free == -1) {
        printf("Nenhuma posicao livre no arquivo de dados");
        return;
    }

    int freePos = this->header->free;
    printf("Posições livres: %d ", freePos);
    while (freePos != -1) {
        Professional* pro = readProfessional(this,freePos);
        freePos = pro->code;
        printf("%d ", freePos);
        free(pro);
    }
}