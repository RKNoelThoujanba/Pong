#include "Pong/Game.hpp"
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<const char*> args;
    for(int i = 0; i < argc; ++i)
        args.push_back(argv[i]);
    Pong::Game game(args);
    game.RunGame();
}