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

    //create the game accordingly. Then, init, play and end the game
    unique_ptr<RPSGame> game = make_unique<RPSGame>(player1, player2);
    game->initGame();
    game->playGame();
    game->endGame();

    return EXIT_SUCCESS;

//    unique_ptr<PlayerAlgorithm> player1;
//    unique_ptr<PlayerAlgorithm> player2;
//
//    player1 = make_unique<RPSPlayerAuto>(PLAYER_1);
//    player2 = make_unique<RPSPlayerAuto>(PLAYER_2);
//    unique_ptr<RPSGame> game = make_unique<RPSGame>(player1, player2);
//    game->initGame();
//    if (game->m_game_over == true)
//        cout << "GAME OVER" << endl;
//
//    int num_move_player1 = 0;
//    int num_move_player2 = 0;
//    int count_moves_till_fight = 0;
//    bool player1_moves_over = false, player2_moves_over = false;
//    bool player1_current, was_fight;
//    int src_row, src_col, dst_row, dst_col, joker_row, joker_col;
//    char real_piece_type_curr, real_piece_type_opp;
//    unique_ptr<Move> curr_move;
//    unique_ptr<JokerChange> curr_joker_change;
//    char new_rep;
//
//    game->printBoardToCout();
//    player1_current = game->m_current_player == PLAYER_1;
//    //check if the current player has a move
//    if (player1_current) {
//        if (player1_moves_over == false && (curr_move = game->m_algo_player1->getMove()) != nullptr) {
//            num_move_player1++;
//        } else {
//            player1_moves_over = true;
//            game->changeCurrentPlayer();
//        }
//    } else {
//        if (player2_moves_over == false && (curr_move = game->m_algo_player2->getMove()) != nullptr) {
//            num_move_player2++;
//        } else {
//            player2_moves_over = true;
//            game->changeCurrentPlayer();
//        }
//    }
//    //if there is am move, do it
//    src_row = curr_move->getFrom().getY();
//    src_col = curr_move->getFrom().getX();
//    real_piece_type_curr = (game->m_board.m_game_board[src_row][src_col] != nullptr)
//                           ? game->m_board.m_game_board[src_row][src_col] -> getPiece(): NEUTRAL_CHAR;
//
//    dst_row = curr_move->getTo().getY();
//    dst_col = curr_move->getTo().getX();
//    real_piece_type_opp = (game->m_board.m_game_board[dst_row][dst_col] != nullptr)
//                          ? game->m_board.m_game_board[dst_row][dst_col]->getPiece() : NEUTRAL_CHAR;
//
//    was_fight = game->playRegularMove(src_row, src_col, dst_row, dst_col);
//    cout << "was fight ? " << was_fight << endl;
//
//    if (player1_current)
//        game->m_algo_player2->notifyOnOpponentMove(*curr_move);
//    else
//        game->m_algo_player1->notifyOnOpponentMove(*curr_move);
}

