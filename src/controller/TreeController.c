#include "TreeController.h"
#include <stdlib.h>

TreeController *createTreeController(Memory* memory) {
    TreeController* treeController = malloc(sizeof(TreeController));
    treeController->memory = memory;
    return treeController;
}
