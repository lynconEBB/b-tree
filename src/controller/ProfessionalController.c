#include "ProfessionalController.h"
#include <stdlib.h>

ProfessionalController *createProfessionalController(Memory* memory) {
    ProfessionalController* professionalController = malloc(sizeof(ProfessionalController));
    professionalController->memory = memory;
    return professionalController;
}

int insertProfessional(ProfessionalController* this, Professional* professional) {
    DataHeader* header = readDataHeader(this->memory);
    int pos;
    if (header->free == -1) {
        pos = header->top;
        header->top++;
    } else {
        pos = header->free;
        Professional* aux = readProfessional(this->memory,header->free);
        header->free = aux->code;
        free(aux);
    }
    writeProfessional(this->memory,professional,pos);
    writeDataHeader(this->memory, header);
    free(header);
    return pos;
}

void removeProfessional(ProfessionalController* this, int pos) {
    DataHeader* header = readDataHeader(this->memory);
    Professional* p = readProfessional(this->memory,pos);
    p->code = header->free;
    header->free = pos;
    writeDataHeader(this->memory, header);
    writeProfessional(this->memory, p, pos);
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
