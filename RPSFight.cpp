#include "RPSFight.h"

const Point& RPSFight::getPosition() const {
    return this->m_fight_pos;
}

char RPSFight::getPiece(int player) const {

    bool player_1_curr = this->m_attacking_player == PLAYER_1;

    if(player == PLAYER_1) {
        return player_1_curr ? this->m_piece_type_curr_player : this->m_piece_type_opp_player;
    } else //player == PLAYER_2
        return player_1_curr ? this->m_piece_type_opp_player : this->m_piece_type_curr_player;
}

int RPSFight::getWinner() const {

    FightResult res = manageFight();

    //no winner
    if(res == NO_FIGHT || res == SAME_PIECE_FIGHT || res == BOMB_OPP_PLAYER)
        return TIE;

    //current player wins
    if(res == FLAG_EATEN_OPP_PLAYER || res == WIN_CURR_PLAYER)
        return this->m_attacking_player;

    //opp player wins - WIN_OPP_PLAYER
    return this->m_attacking_player == PLAYER_1 ? PLAYER_2 : PLAYER_1;

}

FightResult RPSFight::manageFight() const {

    //case 1: same pieces' type for both players
    if (this->m_piece_type_curr_player == this->m_piece_type_opp_player)
        return SAME_PIECE_FIGHT;

    //case 2: flag
    if (this->m_piece_type_opp_player == FLAG)
        return FLAG_EATEN_OPP_PLAYER;

    //case 3: bombs
    if (this->m_piece_type_opp_player == BOMB)
        return BOMB_OPP_PLAYER;

    //case 4: regular RPS rules
    //wins current player:
    if ((this->m_piece_type_curr_player == ROCK && this->m_piece_type_opp_player == SCISSOR) ||
        (this->m_piece_type_curr_player == PAPER && this->m_piece_type_opp_player == ROCK) |
        (this->m_piece_type_curr_player == SCISSOR && this->m_piece_type_opp_player == PAPER))
        return WIN_CURR_PLAYER;

    //loses current player:
    if ((this->m_piece_type_curr_player == ROCK && this->m_piece_type_opp_player == PAPER) ||
        (this->m_piece_type_curr_player == PAPER && this->m_piece_type_opp_player == SCISSOR) ||
        (this->m_piece_type_curr_player == SCISSOR && this->m_piece_type_opp_player == ROCK))
        return WIN_OPP_PLAYER;

    //in init phase when current player has flag "fighting"
    if (this->m_piece_type_curr_player == FLAG)
        return WIN_OPP_PLAYER;

    return NO_FIGHT;
}
