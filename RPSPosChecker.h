#ifndef OOP_HW2_RPSPARSER_H
#define OOP_HW2_RPSPARSER_H

#include "RPSPiece.h"
#include <memory>
#include <iostream>

#define NUM_OF_ROCKS 2
#define NUM_OF_PAPERS 5
#define NUM_OF_SCISSORS 1
#define NUM_OF_BOMBS 2
#define NUM_OF_JOKERS 2
#define NUM_OF_FLAGS 1

using namespace std;

enum PosMsg {
    EXCEED_NUM_OF_PIECE,
    OVERLOAD_POS,
    INDEX_OUT_OF_RANGE,
    MISS_FLAG_PIECE,
    INVALID_PIECE,
    INVALID_JOKER_REP,
    POS_SUCCESS
};

class RPSPosChecker {

private:

    bool m_pieces_locs[BOARD_ROWS][BOARD_COLS];

    PosMsg m_pos_msg; //represents the status of the position file
    int m_invalid_num_of_piece; //represents the problematic piece

    int m_num_of_rocks;
    int m_num_of_papers;
    int m_num_of_scissors;
    int m_num_of_bombs;
    int m_num_of_jokers;
    int m_num_of_flags;
    int m_num_of_jokers_can_move;

public:

    RPSPosChecker() : m_invalid_num_of_piece(-1), m_num_of_rocks(0), m_num_of_papers(0), m_num_of_scissors(0),
                                   m_num_of_bombs(0), m_num_of_jokers(0), m_num_of_flags(0), m_num_of_jokers_can_move(0) {

        for(int i =0; i < BOARD_ROWS; i++) {
            for(int j =0; j < BOARD_COLS; j++)
                this->m_pieces_locs[i][j] = false;
        }
    }


    static void printVerboseMsg(PosMsg msg); //print error verbosely

    int getNumMovingPieces() const;
    int getNumOfFlags() const;
    int getPosMsg() const;
    int getInvalidNumPiece() const;


    void checkInitPos(std::vector <unique_ptr<PiecePosition>> &initPosVector); // parse the Position File, and set the fields accordingly

    //helper function for checkInitPos() function
    bool isValidPieceType(char piece, bool is_joker_rep); //returns true if the piece symbol given is valid (for regular piece or joker)
    bool checkAndUpdateNumPiece(char piece); //check if the piece type exceeds it's number: if not, update it and return true, false o/w (assumed the piece type is valid!)
    bool isIndicesLegal(int row, int col); //return true iff x, y are inside board! (assumed the indices are 0's base!)
    void setPieceOnBoard(int row, int col); //set a piece on board (assume the piece is valid!)
    bool isPieceAJoker(char piece); //return true iff the piece given is a JOKER
    bool allFlagsSet(); //return true iff the all flags are set on board
    bool isPosEmpty(int row, int col); //return true iff the board[x][y] is empty (assumed the indices are valid and 0's bases!)

};


#endif //OOP_HW2_RPSPARSER_H
