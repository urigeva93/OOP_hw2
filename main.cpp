#include "RPSGame.h"

int main() {
    RPSGame* game = new RPSGame();
    game->initGame();
    if(game->isM_game_over()) {
        game->endGame();
        delete(game);
        return EXIT_SUCCESS;
    }

    game->playGame();
    game->endGame();
    delete(game);
    return EXIT_SUCCESS;
}

