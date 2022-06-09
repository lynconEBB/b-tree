#include "view/App.h"

//Funcão principal
int main() {
    App* app = createApp();
    startApp(app);
    destroyApp(app);
    return 0;
}