#include "ProfessionalController.h"
#include <stdlib.h>

ProfessionalController *createProfessionalController(Memory* memory) {
    ProfessionalController* professionalController = malloc(sizeof(ProfessionalController));
    professionalController->memory = memory;
    return professionalController;
}
