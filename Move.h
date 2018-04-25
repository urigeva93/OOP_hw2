#ifndef OOP_HW2_MOVE_H
#define OOP_HW2_MOVE_H

class Move {
public:
    virtual const Point &getFrom() const;

    virtual const Point &getTo() const;
};


#endif //OOP_HW2_MOVE_H
