#include "RPSFight.h"

const Point& RPSFight::getPosition() const {
    return this->m_fight_pos;
}

char RPSFight::getPiece(int player) const {
    bool player_1_curr = this->m_curr_player == PLAYER_1;

    if(player == PLAYER_1) {
        return player_1_curr ? this->m_curr_player_piece->getPiece() : this->m_opp_player_piece->getPiece();
    } else //player == PLAYER_2
        return player_1_curr ? this->m_opp_player_piece->getPiece() : this->m_curr_player_piece->getPiece();
}

int RPSFight::getWinner() const {

    FightResult res = manageFight(this->m_curr_player_piece, this->m_opp_player_piece);

    //no winner
    if(res == NO_FIGHT || res == SAME_PIECE_FIGHT || res == BOMB_OPP_PLAYER)
        return TIE;

    //current player wins
    if(res == FLAG_EATEN_OPP_PLAYER || res == WIN_CURR_PLAYER)
        return this->m_curr_player;

    //opp player wins
    return this->m_curr_player == PLAYER_1 ? PLAYER_2 : PLAYER_1;

}

FightResult RPSFight::manageFight(shared_ptr<RPSPiece> piece_curr_player, shared_ptr<RPSPiece> piece_opp_player) const {

    if (piece_curr_player == nullptr ||
        piece_opp_player == nullptr) //in case there aren't really 2 pieces participate in fight
        return NO_FIGHT;


    //case 1: same pieces' type for both players
    if (piece_curr_player->m_symbol == piece_opp_player->m_symbol)
        return SAME_PIECE_FIGHT;

    //case 2: flag
    if (piece_opp_player->m_symbol == FLAG)
        return FLAG_EATEN_OPP_PLAYER;

    //case 3: bombs
    if (piece_opp_player->m_symbol == BOMB)
        return BOMB_OPP_PLAYER;

    //case 4: regular RPS rules
    //wins current player:
    if ((piece_curr_player->m_symbol == ROCK && piece_opp_player->m_symbol == SCISSOR) ||
        (piece_curr_player->m_symbol == PAPER && piece_opp_player->m_symbol == ROCK) ||
        (piece_curr_player->m_symbol == SCISSOR && piece_opp_player->m_symbol == PAPER))
        return WIN_CURR_PLAYER;

    //loses current player:
    if ((piece_curr_player->m_symbol == ROCK && piece_opp_player->m_symbol == PAPER) ||
        (piece_curr_player->m_symbol == PAPER && piece_opp_player->m_symbol == SCISSOR) ||
        (piece_curr_player->m_symbol == SCISSOR && piece_opp_player->m_symbol == ROCK))
        return WIN_OPP_PLAYER;

    //in init phase when current player has flag "fighting"
    if (piece_curr_player->m_symbol == FLAG)
        return WIN_OPP_PLAYER;

    return NO_FIGHT;
}
