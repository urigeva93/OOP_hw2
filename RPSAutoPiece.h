
#ifndef OOP_HW2_RPSAUTOPIECE_H
#define OOP_HW2_RPSAUTOPIECE_H

class RPSAutoPiece : public RPSPiece {
private:
    bool m_moved; // false if the piece didn't move, true otherwise
    bool m_known_piece; // false if we don't know the piece type
    bool m_has_eaten;
public:
    RPSAutoPiece(char m_symbol, RPSPoint& m_curr_pos, int m_num_player, bool m_is_joker )
};
#endif //OOP_HW2_RPSAUTOPIECE_H
