#include "Memory.h"
#include <stdlib.h>

Memory* createMemory() {
    Memory* memory = malloc(sizeof(Memory));
    memory->dataFile = fopen("dataMem.txt","r+b");
    memory->indexFile = fopen("indexMem.txt","r+b");
    return memory;
}

void writeDataHeader(Memory* memory, DataHeader* header) {
    fseek(memory->dataFile, 0, SEEK_SET);
    fwrite(header, sizeof(DataHeader), 1, memory->dataFile);
}

DataHeader* readDataHeader(Memory* memory) {
    DataHeader* header = malloc(sizeof(DataHeader));
    fseek(memory->dataFile,0, SEEK_SET);
    int success = fread(header, sizeof(DataHeader), 1, memory->dataFile);
    if (!success) {
        header->top = 0;
        header->free = -1;
        writeDataHeader(memory, header);
    }
    return header;
}

Professional* readProfessional(Memory* memory, int pos) {
    Professional* p = malloc(sizeof(Professional));
    fseek(memory->dataFile, sizeof(DataHeader) + pos * sizeof(Professional), SEEK_SET);
    fread(p, sizeof(Professional), 1, memory->dataFile);
    return p;
}

void writeProfessional(Memory* memory, Professional* professional, int pos) {
    fseek(memory->dataFile, sizeof(DataHeader) + pos * sizeof(Professional), SEEK_SET);
    fwrite(professional, sizeof(Professional), 1, memory->dataFile);
}

int getFreePositionsDataHeader(Memory* memory) {
    DataHeader* header = readDataHeader(memory);
    if (header->free == -1) {
        printf("Nenhuma posicao livre no arquivo de dados");
        return 0;
    }

    int freeCount = 0;
    int freePos = header->free;
    while (freePos != -1) {
        Professional* pro = readProfessional(memory,freePos);
        freePos = pro->code;
        free(pro);
        freeCount++;
    }
    return freeCount;
    printf("O arquivo de dados possui %d posicoes livres\n", freeCount);
}