#include "RPSPlayerAuto.h"

int RPSPlayerAuto::getM_num_of_flags() const {
    return this->m_num_of_flags;
}

int RPSPlayerAuto::getNumMovingPieces() const {
    int num_all_pieces = this->m_num_of_jokers_can_move + this->m_num_of_scissors + this->m_num_of_papers + this->m_num_of_rocks;

    return num_all_pieces;
}

void RPSPlayerAuto::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {
    return;
}

void RPSPlayerAuto::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {
    return;
}

void RPSPlayerAuto::notifyOnOpponentMove(const Move &move) {
    return;
}

void RPSPlayerAuto::notifyFightResult(const FightInfo &fightInfo) {
    return;
}

unique_ptr<Move> RPSPlayerAuto::getMove() {
    return unique_ptr<Move>();
}

unique_ptr<JokerChange> RPSPlayerAuto::getJokerChange() {
    return unique_ptr<JokerChange>();
}