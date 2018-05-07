#ifndef OOP_HW2_RPSPROFILEPLAYER_H
#define OOP_HW2_RPSPROFILEPLAYER_H

#include "PlayerAlgorithm.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>

#include "RPSMove.h"
#include "RPSPoint.h"
#include "RPSFight.h"

#define PATH_POS_FILE_PLAYER1 "./player1.rps_board"
#define PATH_POS_FILE_PLAYER2 "./player2.rps_board"
#define PATH_MOVE_FILE_PLAYER1 "./player1.rps_moves"
#define PATH_MOVE_FILE_PLAYER2 "./player2.rps_moves"

using namespace std;


class RPSPlayerFromFile : public PlayerAlgorithm {

private:

    //information about the player:
    int m_num_player;
    ifstream m_moves_file;
    RPSMove m_curr_move;

public:

    RPSPlayerFromFile(int num_player) : m_num_player(num_player), m_curr_move(RPSPoint(-1, -1), RPSPoint(-1, -1)) {

        string path_to_move_file = this->m_num_player == PLAYER_1 ? PATH_MOVE_FILE_PLAYER1 : PATH_MOVE_FILE_PLAYER2;
        this->m_moves_file.open(path_to_move_file);
    }

    ~RPSPlayerFromFile() {
        this->m_moves_file.close();
    }

    RPSPlayerFromFile(const RPSPlayerFromFile &) = delete; //delete copy constructor

    RPSPlayerFromFile &operator=(const RPSPlayerFromFile &) = delete; //delete operator

private:

    //static useful functions for parsing
    static vector<string> split(string line, char delimiter); //split the line according to delimiter
    static string removeExtraSpaces(string line); //remove extra spaces in line if exists (example: 'ab  c d' --> 'ab c d')
    static bool isStringRepInt(string line); //returns true iff the given string represents a valid integer

    //functions form abstract class
    virtual void getInitialPositions(int player, std::vector <unique_ptr<PiecePosition>> &vectorToFill) override;
    virtual void notifyOnInitialBoard(const Board &b, const std::vector <unique_ptr<FightInfo>> &fights) override;
    virtual void notifyOnOpponentMove(const Move &move) override;
    virtual void notifyFightResult(const FightInfo &fightInfo) override;
    virtual unique_ptr <Move> getMove() override;
    virtual unique_ptr <JokerChange> getJokerChange() override;

};


#endif //OOP_HW2_RPSPROFILEPLAYER_H
