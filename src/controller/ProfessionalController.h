#ifndef BTREE_PROFESSIONALCONTROLLER_H
#define BTREE_PROFESSIONALCONTROLLER_H
#include "../memory/Memory.h"

typedef struct {
    Memory* memory;
} ProfessionalController;

ProfessionalController* createProfessionalController(Memory* memory);

int insertProfessional(ProfessionalController* this, Professional* professional);

void removeProfessional(ProfessionalController* this,int pos);

void printProfessional(Professional* professional);

#endif