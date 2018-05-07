#ifndef OOP_HW2_RPSPOINT_H
#define OOP_HW2_RPSPOINT_H

#include "Point.h"

class RPSPoint : public Point {

    int m_x; // represents ***column*** on board
    int m_y; //represents ***row*** on board

public:

    RPSPoint(int x, int y) : m_x(x), m_y(y) {}

    RPSPoint(const Point& p) = default; //default copy constructor

    RPSPoint& operator=(const RPSPoint&) = default; //default operator '='

    //functions form abstract class
    virtual int getX() const override {return m_x;}
    virtual int getY() const override {return m_y;}

    friend class RPSGame;

};


#endif //OOP_HW2_RPSPOINT_H
