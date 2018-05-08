#ifndef OOP_HW2_RPSGAMEBOARD_H
#define OOP_HW2_RPSGAMEBOARD_H

#include <memory>
#include "Board.h"
#include "RPSPiece.h"
#include "RPSFight.h"

using namespace std;

class RPSGameBoard : public Board {
private:
    shared_ptr<RPSPiece> m_game_board[BOARD_ROWS][BOARD_COLS];

public:

    RPSGameBoard() {
        for(int i= 0; i < BOARD_ROWS; i++) {
            for(int j =0; j < BOARD_ROWS; j++)
                this->m_game_board[i][j] = nullptr;
        }
    }

    virtual int getPlayer(const Point& pos) const override {
        int row = pos.getY();
        int col = pos.getX();

        if(this->m_game_board[row][col] == nullptr)
            return TIE;

        return this->m_game_board[row][col]->getNumPlayer();
    }

    friend class RPSGame;

};


#endif //OOP_HW2_RPSGAMEBOARD_H
