#ifndef BTREE_PROFESSIONALCONTROLLER_H
#define BTREE_PROFESSIONALCONTROLLER_H
#include "../memory/Memory.h"

typedef struct {
    Memory* memory;
} ProfessionalController;

ProfessionalController* createProfessionalController(Memory* memory);

int insertProfessional(Professional* professional, FILE* file) {
    DataHeader* header = readDataHeader(file);
    int pos;
    if (header->free == -1) {
        pos = header->top;
        header->top++;
    } else {
        pos = header->free;
        Professional* aux = getProfessional(file,header->free);
        header->free = aux->code;
        free(aux);
    }
    writeProfessional(file,professional,pos);
    writeDataHeader(file, header);
    free(header);
    return pos;
}

void removeProfessional(int pos, FILE* file) {
    DataHeader* header = readDataHeader(file);
    Professional* p = getProfessional(file,pos);
    p->code = header->free;
    header->free = pos;
    writeDataHeader(file, header);
    writeProfessional(file, p, pos);
    free(header);
    free(p);
}

void printProfessional(Professional* professional) {
    printf("=========== Profissional %d ===========\n",professional->code );
    printf("Nome: %s\n", professional->name);
    printf("CPF: %s\n", professional->cpf);
    printf("Numero de Registro: %s\n", professional->registryNumber);
    printf("Endereco: %s\n", professional->address);
    printf("Telefone: %s\n", professional->phone);
    printf("=======================================\n");
}
#endif