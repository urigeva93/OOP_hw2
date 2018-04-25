#ifndef OOP_HW2_PIECEPOSITION_H
#define OOP_HW2_PIECEPOSITION_H

#include "Point.h"

class PiecePosition {
public:

    virtual const Point &getPosition() const;
    virtual char getPiece() const; // R, P, S, B, J or F
    virtual char getJokerRep() const; // ONLY for Joker: R, P, S or B -- non-Joker may return ‘#’
};


#endif //OOP_HW2_PIECEPOSITION_H
