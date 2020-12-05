#include "Application/GameApplication.h"

int main(int argc, char const *argv[])
{
    GameApplication gameApp;
    gameApp.init();
    gameApp.run();

    return 0;
}
