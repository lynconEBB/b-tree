#ifndef BTREE_APP_H
#define BTREE_APP_H
#include "../handlers/TreeHandler.h"
#include "../handlers/DataHandler.h"
#include "../handlers/IndexHandler.h"

typedef enum {
    OP_INSERT = 1,
    OP_REMOVE = 2,
    OP_UPDATE = 3,
    OP_LOAD = 4,
    OP_PRINT = 5,
    OP_PRINT_ASC = 6,
    OP_PRINT_LVL = 7,
    OP_FREE_DATA = 8,
    OP_FREE_INDEX = 9,
    OP_EXIT=10
} Option;

typedef struct{
    Option option;
    DataHandler* dataHandler;
    IndexHandler* indexHandler;
    TreeHandler* treeHandler;
} App;

App* createApp();

void startApp(App* app);

void executeOption(App* app);

void insert(App* app,Professional* professional);

void removeOne(App* app, int code);

void update(App* app);

void load(App* app);

void printProfessionalByCode(App* app);

void printAsc(App* app);

void printByLevel(App* this);

void printProfessional(Professional* professional);

void printMenu();

void printNode(Node *node);

int requestOption(App* app);

Professional* requestProfessional();

#endif