#include "RPSGame.h"
#include <iostream>

int main(int argc, char* argv[]) {

    unique_ptr<PlayerAlgorithm> player1;
    unique_ptr<PlayerAlgorithm> player2;

    //check command line to know with players to create
    if(std::strcmp(argv[1], "auto-vs-file")) {
        player1 = make_unique<RPSPlayerAuto>(PLAYER_1);
        player2 = make_unique<RPSPlayerFromFile>(PLAYER_2);
    }
    else if(std::strcmp(argv[1], "file-vs-auto")) {
        player1 = make_unique<RPSPlayerFromFile>(PLAYER_1);
        player2 = make_unique<RPSPlayerAuto>(PLAYER_2);
    }
    else if(std::strcmp(argv[1], "file-vs-file")) {
        player1 = make_unique<RPSPlayerFromFile>(PLAYER_1);
        player2 = make_unique<RPSPlayerFromFile>(PLAYER_2);
    }
    else if(std::strcmp(argv[1], "auto-vs-auto")) {
        player1 = make_unique<RPSPlayerAuto>(PLAYER_1);
        player2 = make_unique<RPSPlayerAuto>(PLAYER_2);
    } else {
        cout << "please enter a legal command line" << endl;
        return EXIT_FAILURE;
    }

    unique_ptr<RPSGame> game = make_unique<RPSGame>(player1, player2);
    game->initGame();

    if(game->getGameOver()) {
        game->endGame();
        return EXIT_SUCCESS;
    }

    game->playGame();
    game->endGame();
    return EXIT_SUCCESS;
}

