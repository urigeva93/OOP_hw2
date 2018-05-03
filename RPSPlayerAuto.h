
#ifndef OOP_HW2_RPSPLAYERAUTO_H
#define OOP_HW2_RPSPLAYERAUTO_H

#include "PlayerAlgorithm.h"

class RPSPlayerAuto : public PlayerAlgorithm {

private:

    unique_ptr<RPSPiece> m_my_board[BOARD_ROWS][BOARD_COLS]; //represents the init board of the player

    //information about the player:
    int m_num_player;
    int m_num_of_rocks;
    int m_num_of_papers;
    int m_num_of_scissors;
    int m_num_of_bombs;
    int m_num_of_jokers;
    int m_num_of_flags;
    int m_num_of_jokers_can_move;


public:

    RPSPlayerAuto() : m_num_of_rocks(0), m_num_of_papers(0), m_num_of_scissors(0),
                      m_num_of_bombs(0), m_num_of_jokers(0), m_num_of_flags(0), m_num_of_jokers_can_move(0) {

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

    friend class RPSGame;


};


#endif //OOP_HW2_RPSPLAYERAUTO_H
