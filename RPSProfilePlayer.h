#ifndef OOP_HW2_RPSPROFILEPLAYER_H
#define OOP_HW2_RPSPROFILEPLAYER_H

#include "PlayerAlgorithm.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>

#include "RPSPiece.h"
#include "Board.h"
#include "FightInfo.h"
#include "Move.h"
#include "JokerChange.h"

using namespace std;

#define BOARD_ROWS 10
#define BOARD_COLS 10

#define TIE 0
#define PLAYER_1 1
#define PLAYER_2 2

#define NUM_OF_ROCKS 2
#define NUM_OF_PAPERS 5
#define NUM_OF_SCISSORS 1
#define NUM_OF_BOMBS 2
#define NUM_OF_JOKERS 2
#define NUM_OF_FLAGS 1


enum PosFileMsg {
    FILE_NOT_EXIST,
    WRONG_FILE,
    FILE_SUCCESS
};


class RPSProfilePlayer : public PlayerAlgorithm {

private:

    RPSPiece *m_my_board[BOARD_ROWS][BOARD_COLS]; //represents the init board of the player
    PosFileMsg m_pos_msg; //represents the status of the position file
    int m_invalid_line_num_pos_file; //represents the wrong line in pos file (if exists)
    int m_num_player;

    int m_num_of_rocks;
    int m_num_of_papers;
    int m_num_of_scissors;
    int m_num_of_bombs;
    int m_num_of_jokers;
    int m_num_of_flags;

    int m_num_of_jokers_can_move;
public:

    virtual void getInitialPositions(int player, std::vector <unique_ptr<PiecePosition>> &vectorToFill) override;

    virtual void notifyOnInitialBoard(const Board &b, const std::vector <unique_ptr<FightInfo>> &fights) override;

    virtual void notifyOnOpponentMove(const Move &move) override;

    virtual void notifyFightResult(const FightInfo &fightInfo) override;

    virtual unique_ptr <Move> getMove() override;

    virtual unique_ptr <JokerChange> getJokerChange() override;

};


#endif //OOP_HW2_RPSPROFILEPLAYER_H
