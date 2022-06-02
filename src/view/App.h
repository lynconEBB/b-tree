#ifndef BTREE_APP_H
#define BTREE_APP_H
#include "../controller/ProfessionalController.h"
#include "../controller/TreeController.h"
#include "../model/Professional.h"

typedef enum {
    OP_INSERT=1,
    OP_REMOVE=2,
    OP_CH_ADDR=3,
    OP_CH_PHONE=4,
    OP_LOAD=5,
    OP_PRINT=6,
    OP_PRINT_ASC=7,
    OP_PRINT_LVL=8,
    OP_FREE_DATA=9,
    OP_FREE_INDEX=10,
    OP_EXIT=11
} Option;

typedef struct{
    TreeController* treeController;
    ProfessionalController* professionalController;
} App;

App* createApp();

void startApp();

int requestOption(Option* op);

void executeOption(App* app, Option op);

void insert(App* app);

void printMenu();

Professional* requestProfessional();

#endif
