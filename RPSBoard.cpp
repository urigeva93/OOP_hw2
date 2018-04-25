#include "RPSBoard.h"

int RPSBoard::getPlayer(const Point &pos) const {

    //TODO: check if pos is in board...

    RPSPiece *curr_piece = this->m_game_board[pos.getY()][pos.getX()];
    if (curr_piece == nullptr)
        return 0;

    return curr_piece->m_num_player;
}

