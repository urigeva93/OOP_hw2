#ifndef OOP_HW2_RPSGAMEBOARD_H
#define OOP_HW2_RPSGAMEBOARD_H

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include "Board.h"
#include "RPSPiece.h"
#include "RPSFight.h"

using namespace std;

class RPSGameBoard : public Board {

public:
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


    void printBoard(ofstream& output_file) const {
        for (int i = 0; i < BOARD_ROWS; i++) {
            for (int j = 0; j < BOARD_COLS; j++) {
                if (this->m_game_board[i][j] == nullptr)
                    output_file << " ";
                else if (this->m_game_board[i][j]->m_is_joker == false) { //regular piece
                    if (this->m_game_board[i][j]->m_num_player == PLAYER_1) //Capital letters
                        output_file << this->m_game_board[i][j]->m_symbol;
                    else
                        output_file << (char) std::tolower(this->m_game_board[i][j]->m_symbol);
                } else { //joker piece
                    if (this->m_game_board[i][j]->m_num_player == PLAYER_1) //Capital letters
                        output_file << JOKER;
                    else
                        output_file << (char) std::tolower(JOKER);
                }
            }
            output_file << endl;
        }
    }


    friend class RPSGame;

};


#endif //OOP_HW2_RPSGAMEBOARD_H
