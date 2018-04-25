#ifndef OOP_HW2_RPSFIGHT_H
#define OOP_HW2_RPSFIGHT_H

#include "FightInfo.h"
#include "RPSPoint.h
#include "RPSPiece.h"

class RPSFight : public FightInfo {

private:
    int m_curr_player; //number represents the attacking player
    RPSPoint m_fight_pos; // point represents the position of the fight
    RPSPiece *m_curr_player_piece; //represents the attacking player piece
    RPSPiece *m_opp_player_piece; //represent the opponent player piece

public:

    RPSFight(RPSPiece *curr_player_piece, RPSPiece *opp_player_piece, RPSPoint fight_pos) : m_curr_player_piece(curr_player_piece),
                                                                                            m_opp_player_piece(opp_player_piece),
                                                                                            m_fight_pos(fight_pos) {

        this->m_curr_player = this->m_curr_player_piece->getNumPlayer();

    }

    virtual const Point &getPosition() const override;

    virtual char getOpponentPiece() const override;

    virtual int getWinner() const override;

};


#endif //OOP_HW2_RPSFIGHT_H
