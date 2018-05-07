#ifndef OOP_HW2_RPSFIGHT_H
#define OOP_HW2_RPSFIGHT_H

#include <memory>
#include "FightInfo.h"
#include "RPSPoint.h"
#include "RPSPiece.h"

using namespace std;

#define TIE 0
#define PLAYER_1 1
#define PLAYER_2 2

//fight result enum
enum FightResult {
    WIN_CURR_PLAYER,
    WIN_OPP_PLAYER,
    FLAG_EATEN_OPP_PLAYER,
    SAME_PIECE_FIGHT,
    BOMB_OPP_PLAYER,
    NO_FIGHT,
};

class RPSFight : public FightInfo {

private:
    char m_piece_type_curr_player; //represents the attacking player piece type (if joker - it's real rep)
    char m_piece_type_opp_player; //represent the opponent player piece type (if joker - it's real rep)
    RPSPoint m_fight_pos; // point represents the position of the fight
    int m_attacking_player; //represents the current player attacking

public:

    RPSFight(char piece_type_curr_player, char piece_type_opp_player, Point fight_pos, int attacking_player)
            : m_piece_type_curr_player(piece_type_curr_player),
              m_piece_type_opp_player(piece_type_opp_player),
              m_fight_pos(fight_pos),
              m_attacking_player(attacking_player) {}

    FightResult manageFight() const; //manage a fight between to pieces (assumed the fight is legal!)

    //functions from abstract class
    virtual const Point &getPosition() const override;
    virtual char getPiece(int player) const override;
    virtual int getWinner() const override;

};


#endif //OOP_HW2_RPSFIGHT_H
