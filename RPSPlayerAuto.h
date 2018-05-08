#ifndef OOP_HW2_RPSPLAYERAUTO_H
#define OOP_HW2_RPSPLAYERAUTO_H

#include "PlayerAlgorithm.h"
#include "RPSPiece.h"
#include "RPSPosChecker.h"
#include "RPSFight.h"

#include <random>
#include <algorithm>
#include <vector>
#include <map>

#define UNKNOWN_PIECE '?'

using namespace std;
class RPSPlayerAuto : public PlayerAlgorithm {


private:

    shared_ptr<RPSPiece> m_my_board[BOARD_ROWS][BOARD_COLS]; //represents the init board of the player

    //information about the player:
    int m_num_player;
    int m_num_of_jokers_can_move;


    //information about the opponent player
    vector <unique_ptr<RPSPoint>> unknownOpponentPieces;
    vector <unique_ptr<RPSPoint>> knownOpponentPieces;

    std::map <char, int> myPieces = {
            {PAPER, 0},
            {ROCK, 0},
            {FLAG, 0},
            {BOMB, 0},
            {SCISSOR, 0},
            {JOKER, 0}};
    //information about the opponent pieces, other player etc.
    std::map <char, int> opponentPieces = {
            {PAPER, NUM_OF_PAPERS},
            {ROCK, NUM_OF_ROCKS},
            {FLAG, NUM_OF_FLAGS},
            {BOMB, NUM_OF_BOMBS},
            {SCISSOR, NUM_OF_SCISSORS},
            {JOKER, NUM_OF_JOKERS}};
    int m_opponent_total;
public:


    RPSPlayerAuto(int num_player) :m_num_player(num_player), m_num_of_jokers_can_move(0), m_opponent_total(0), unknownOpponentPieces(), knownOpponentPieces() {
        //init m_my_board with nullptr
        for (int i = 0; i < BOARD_ROWS; i++) {
            for (int j = 0; j < BOARD_COLS; j++)
                this->m_my_board[i][j] = nullptr;
        }
    }

    //getters
    int getM_num_of_flags() const;
    int getNumMovingPieces() const;


    //functions form abstract class
    virtual void getInitialPositions(int player, std::vector <unique_ptr<PiecePosition>> &vectorToFill) override;
    virtual void notifyOnInitialBoard(const Board &b, const std::vector <unique_ptr<FightInfo>> &fights) override;
    virtual void notifyOnOpponentMove(const Move &move) override;
    virtual void notifyFightResult(const FightInfo &fightInfo) override;
    virtual unique_ptr <Move> getMove() override;
    virtual unique_ptr <JokerChange> getJokerChange() override;

    //get initial_pos helper
    void setPieceOnBoard(char piece, bool is_joker, int row, int col);
    //bool checkAndUpdateNumPiece(char piece);
    void updateNumPiece(char piece);

    // helper functions
    char getPieceFromBoard(Point& position);
    void removeFromVector(int type_vector, const RPSPoint& pos);
    friend class RPSGame;




};


#endif //OOP_HW2_RPSPLAYERAUTO_H
