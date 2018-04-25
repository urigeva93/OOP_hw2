#ifndef OOP_HW2_RPSBOARD_H
#define OOP_HW2_RPSBOARD_H

#include "Board.h"

class RPSBoard : public Board {

private:

    RPSPiece* m_game_board[BOARD_ROWS][BOARD_COLS]; //board of the game

    int m_current_player; //num represents the current player (1 - P1, 2 - P2, 0 if empty)
    bool m_game_over; //flag indicates if the game is over (different reasons)

    int m_num_moving_pieces_player1; //num of pieces player 1 (includes jokers, without flags!)
    int m_num_moving_pieces_player2; //num of pieces player 2 (includes jokers, without flags!)

    int m_num_flags_player1; //num of flags player 1
    int m_num_flags_player2; //num of flags player 2

    //output file attributes
    ofstream m_output_file; //stream of the output file
    ReasonWinner m_reason_winner; //reason of winning
    int m_bad_line1_num; //bad line #1 index (if exists)
    int m_bad_line2_num; //bad line #2 index (if exists)
    int m_winner; //num of the winner


public:
    virtual int getPlayer(const Point& pos ) const override;

};


#endif //OOP_HW2_RPSBOARD_H
