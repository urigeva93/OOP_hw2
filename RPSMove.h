#ifndef OOP_HW2_RPSMOVE_H
#define OOP_HW2_RPSMOVE_H

#include "Move.h"
#include "RPSPoint.h"
#include "JokerChange.h"

class RPSMove : public Move, public JokerChange {

private:
    RPSPoint m_from; //from point of move
    RPSPoint m_to; //to point of move

    bool m_is_joker_move; //flag is true iff the move includes joker change
    char m_new_rep; //symbol of the new Joker's rep
    RPSPoint m_joker_pos; //joker position to change

public:

    RPSMove(RPSPoint from, RPSPoint to) : m_from(from), m_to(to) {
        this->m_is_joker_move = false;
        this->m_new_rep = '#';

        RPSPoint p(-1, -1);
        this->m_joker_pos = p;

    }

    RPSMove(RPSPoint from, RPSPoint to, char new_rep, RPSPoint m_joker_rep) : m_from(from), m_to(to),
                                                                              m_new_rep(new_rep),
                                                                              m_joker_pos(m_joker_rep) {
        this->m_is_joker_move = true;
    }


    virtual const Point &getFrom() const override {
        return m_from;
    }

    virtual const Point &getTo() const override {
        return m_to;
    }

};


#endif //OOP_HW2_RPSMOVE_H
