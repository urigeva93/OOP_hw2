#ifndef OOP_HW2_JOKERCHANGE_H
#define OOP_HW2_JOKERCHANGE_H

class JokerChange {
public:
    virtual const Point &getJokerChangePosition() const;

    virtual char getJokerNewRep() const; // R, P, S or B (but NOT J and NOT F)

};


#endif //OOP_HW2_JOKERCHANGE_H
