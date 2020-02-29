
#include <iostream>
#include"Game.h"
contactlistener listner_instance;

int main()
{
    Game game(listner_instance);
    game.run();
 }

