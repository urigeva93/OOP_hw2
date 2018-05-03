#ifndef OOP_HW2_RPSFIGHT_H
#define OOP_HW2_RPSFIGHT_H

#include <memory>
#include "FightInfo.h"
#include "RPSPoint.h"
#include "RPSPiece.h"

using namespace std;

//fight result enum
enum FightResult {
    WIN_CURR_PLAYER,
    WIN_OPP_PLAYER,
    FLAG_EATEN_OPP_PLAYER,
    SAME_PIECE_FIGHT,
    BOMB_OPP_PLAYER,
    NO_FIGHT,
};

#define TIE 0
#define PLAYER_1 1
#define PLAYER_2 2

class RPSFight : public FightInfo {

private:
    int m_curr_player; //number represents the attacking player
    RPSPoint m_fight_pos; // point represents the position of the fight

    //RPSPiece *m_curr_player_piece; //represents the attacking player piece
    //RPSPiece *m_opp_player_piece; //represent the opponent player piece

    shared_ptr<RPSPiece> m_curr_player_piece; //represents the attacking player piece
    shared_ptr<RPSPiece> m_opp_player_piece; //represent the opponent player piece

public:

    RPSFight(RPSPiece *curr_player_piece, RPSPiece *opp_player_piece, RPSPoint fight_pos) : m_curr_player_piece(curr_player_piece),
                                                                                            m_opp_player_piece(opp_player_piece),
                                                                                            m_fight_pos(fight_pos) {

        this->m_curr_player = this->m_curr_player_piece->getNumPlayer();

    }

    FightResult manageFight(shared_ptr<RPSPiece> piece_curr_player, shared_ptr<RPSPiece> piece_opp_player) const; //manage a fight between to pieces (assumed the fight is legal!)

    //functions from abstract class
    virtual const Point &getPosition() const override;
    virtual char getPiece(int player) const override;
    virtual int getWinner() const override;

};


#endif //OOP_HW2_RPSFIGHT_H
