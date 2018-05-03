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
#include "RPSFight.h"
#include "Move.h"
#include "JokerChange.h"

#define BOARD_ROWS 10
#define BOARD_COLS 10

#define FILE_NOT_EXISTS "ERROR MOVE FILE: file doesn't exist.\n"


#define NUM_OF_ROCKS 2
#define NUM_OF_PAPERS 5
#define NUM_OF_SCISSORS 1
#define NUM_OF_BOMBS 2
#define NUM_OF_JOKERS 2
#define NUM_OF_FLAGS 1

using namespace std;

enum PosFileMsg {
    FILE_NOT_EXIST,
    INVALID_FORMAT,
    EXCEED_NUM_OF_PIECE,
    OVERLOAD_POS,
    INDEX_OUT_OF_RANGE,
    MISS_FLAG_PIECE,
    INVALID_PIECE,
    INVALID_JOKER_REP,
    FILE_SUCCESS
};

class RPSPlayerFromFile : public PlayerAlgorithm {

private:


    //unique_ptr<RPSPiece> m_my_board[BOARD_ROWS][BOARD_COLS]; //represents the init board of the player

    //information about the player:
    int m_num_player;
    int m_num_of_moving_pieces;
    int m_num_of_flags;

    vector<Move> moves;
    vector<Move>::iterator currMove;


public:

    RPSPlayerFromFile(int num_player) : m_num_player(num_player) {}

    RPSPlayerFromFile(const RPSPlayerFromFile &) = delete; //delete copy constructor

    RPSPlayerFromFile &operator=(const RPSPlayerFromFile &) = delete; //delete operator =

    //getters
    int getNumOfFlags() const;
    int getNumMovingPieces() const;

private:


    bool parseMoveFile();

    //functions form abstract class
    virtual void getInitialPositions(int player, std::vector <unique_ptr<PiecePosition>> &vectorToFill) override;
    virtual void notifyOnInitialBoard(const Board &b, const std::vector <unique_ptr<FightInfo>> &fights) override;
    virtual void notifyOnOpponentMove(const Move &move) override;
    virtual void notifyFightResult(const FightInfo &fightInfo) override;
    virtual unique_ptr <Move> getMove() override;
    virtual unique_ptr <JokerChange> getJokerChange() override;

    friend class RPSGame;

};


#endif //OOP_HW2_RPSPROFILEPLAYER_H
