#include "Professional.h"

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
