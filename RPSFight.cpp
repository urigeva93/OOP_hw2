#include "RPSFight.h"


const Point& RPSFight::getPosition() const {
    return this->m_fight_pos;
}

char RPSFight::getOpponentPiece() const {
    return this->m_opp_player_piece->getPiece();
}
int RPSFight::getWinner() const {
    //TODO: fight!
}