#ifndef OOP_HW2_RPSPIECE_H
#define OOP_HW2_RPSPIECE_H

#include "PiecePosition.h"
#include "RPSPoint.h"

//pieces types:
#define ROCK 'R'
#define PAPER 'P'
#define SCISSOR 'S'
#define BOMB 'B'
#define JOKER 'J'
#define FLAG 'F'

class RPSPiece : public PiecePosition {

private:

    char m_symbol; //symbol of the piece (Rock, Paper, Scissor, Bomb, Joker, Flag)
    bool m_is_joker; //true iff the piece is a joker
    int m_num_player; //num of the player that the piece belongs to
    RPSPoint m_curr_pos; //represents the position on board

public:

    RPSPiece(char symbol, bool is_joker, int num_player, RPSPoint curr_pos) : m_symbol(symbol), m_is_joker(is_joker),
                                                                              m_num_player(num_player),
                                                                              m_curr_pos(curr_pos) {}
    RPSPiece(const RPSPiece &) = default; //default the copy constructor
    RPSPiece &operator=(const RPSPiece &) = delete; //delete operator '='


    virtual const Point &getPosition() const override {
        return m_curr_pos;
    }

    virtual char getPiece() const override {
        if (this->m_is_joker)
            return JOKER;

        return this->m_symbol;
    }

    virtual char getJokerRep() const override {
        if(this->m_is_joker)
            return this->m_symbol;

        return '#';
    }

    int getNumPlayer() const {
        return this->m_num_player;
    }

    friend class RPSBoard;

};


#endif //OOP_HW2_RPSPIECE_H
