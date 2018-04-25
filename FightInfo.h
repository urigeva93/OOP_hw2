#ifndef OOP_HW2_FIGHTINFO_H
#define OOP_HW2_FIGHTINFO_H


class FightInfo {
public:
    virtual const Point &getPosition() const;

    virtual char getOpponentPiece() const; // R, P, S, B or F (but NOT J)
    virtual int getWinner() const; // 0 - both lost, 1 - player 1 won, 2 - player 2 won
};


#endif //OOP_HW2_FIGHTINFO_H
