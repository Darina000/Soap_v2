#include <iostream>
#include "Game.h"



int main(int argc, char** argv)
{
    
    try
    {
        Game game;
        game.Run();;
    }
    
    catch (const std::exception & exception)
    {
        std::cerr << exception.what() << '\n';

        system("pause");

        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown exception\n";

        system("pause");

        return EXIT_FAILURE;
    }

    system("pause");
    

    return 0;
}
