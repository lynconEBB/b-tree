#ifndef BTREE_DATAHANDLER_H
#define BTREE_DATAHANDLER_H
#include <stdio.h>

typedef struct {
    int code;
    char name[51];
    char cpf[12];
    char registryNumber[31];
    char address[101];
    char phone[20];
} Professional;

typedef struct {
    int top;
    int free;
} DataHeader;

typedef struct {
    FILE* file;
    DataHeader* header;
} DataHandler;

DataHandler* createDataHandler();

int addProfessional(DataHandler* this, Professional* professional);

void removeProfessional(DataHandler* this, int pos);

void writeDataHeader(DataHandler* this);

DataHeader* readDataHeader(DataHandler* this);

DataHeader* getDataHeader(DataHandler* this);

void printFreePositionsDataHeader(DataHandler* this);

Professional* readProfessional(DataHandler* memory, int pos);

void writeProfessional(DataHandler* file, Professional* professional, int pos);

#endif