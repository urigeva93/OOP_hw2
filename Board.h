#ifndef OOP_HW2_BOARD_H
#define OOP_HW2_BOARD_H


class Board {
public:
    virtual int getPlayer(const Point &pos) const; // 1 for player 1’s piece, 2 for 2, 0 if empty
};

#endif //OOP_HW2_BOARD_H
