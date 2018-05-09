#include "RPSGame.h"
#include <iostream>

int main(int argc, char *argv[]) {

    unique_ptr<PlayerAlgorithm> player1;
    unique_ptr<PlayerAlgorithm> player2;

    //create players profile according to CLA
    if(std::strcmp(argv[1], "auto-vs-file") == 0) {
        player1 = make_unique<RPSPlayerAuto>(PLAYER_1);
        player2 = make_unique<RPSPlayerFromFile>(PLAYER_2);
    }
    else if(std::strcmp(argv[1], "file-vs-auto") == 0) {
        player1 = make_unique<RPSPlayerFromFile>(PLAYER_1);
        player2 = make_unique<RPSPlayerAuto>(PLAYER_2);
    }
    else if(std::strcmp(argv[1], "file-vs-file") == 0) {
        player1 = make_unique<RPSPlayerFromFile>(PLAYER_1);
        player2 = make_unique<RPSPlayerFromFile>(PLAYER_2);
    }
    else if(std::strcmp(argv[1], "auto-vs-auto") == 0) {
        player1 = make_unique<RPSPlayerAuto>(PLAYER_1);
        player2 = make_unique<RPSPlayerAuto>(PLAYER_2);
    } else { //error command line
        cout << "USAGE: " << argv[0] << " <auto-vs-file | file-vs-auto | file-vs-file | auto-vs-auto>" << endl;
        return EXIT_FAILURE;
    }

    unique_ptr<RPSGame> game = make_unique<RPSGame>(player1, player2);
    game->initGame();
    game->playGame();
    game->endGame();

    return EXIT_SUCCESS;
}

