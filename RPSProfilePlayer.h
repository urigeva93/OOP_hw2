#ifndef OOP_HW2_RPSPROFILEPLAYER_H
#define OOP_HW2_RPSPROFILEPLAYER_H

#include "PlayerAlgorithm.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>

#include "RPSPiece.h"
#include "Board.h"
#include "FightInfo.h"
#include "Move.h"
#include "JokerChange.h"

#define BOARD_ROWS 10
#define BOARD_COLS 10

#define NUM_OF_ROCKS 2
#define NUM_OF_PAPERS 5
#define NUM_OF_SCISSORS 1
#define NUM_OF_BOMBS 2
#define NUM_OF_JOKERS 2
#define NUM_OF_FLAGS 1

using namespace std;

enum PosFileMsg {
    FILE_NOT_EXIST,
    INVALID_FORMAT,
    EXCEED_NUM_OF_PIECE,
    OVERLOAD_POS,
    INDEX_OUT_OF_RANGE,
    MISS_FLAG_PIECE,
    INVALID_PIECE,
    INVALID_JOKER_REP,
    FILE_SUCCESS
};

class RPSProfilePlayer : public PlayerAlgorithm {

private:

    RPSPiece *m_my_board[BOARD_ROWS][BOARD_COLS]; //represents the init board of the player
    PosFileMsg m_pos_msg; //represents the status of the position file
    int m_invalid_line_num_pos_file; //represents the wrong line in pos file (if exists)
    int m_num_player;
    int m_num_of_rocks;
    int m_num_of_papers;
    int m_num_of_scissors;
    int m_num_of_bombs;
    int m_num_of_jokers;
    int m_num_of_flags;
    int m_num_of_jokers_can_move;

public:

    RPSProfilePlayer() : m_invalid_line_num_pos_file(-1), m_num_of_rocks(0), m_num_of_papers(0), m_num_of_scissors(0),
                         m_num_of_bombs(0), m_num_of_jokers(0), m_num_of_flags(0), m_num_of_jokers_can_move(0) {

        //init m_my_board with NULL's
        for (int i = 0; i < BOARD_ROWS; i++) {
            for (int j = 0; j < BOARD_COLS; j++)
                this->m_my_board[i][j] = nullptr;
        }

    }

    RPSProfilePlayer(const RPSProfilePlayer &) = delete; //delete copy constructor

    RPSProfilePlayer &operator=(const RPSProfilePlayer &) = delete; //delete operator =

    ~RPSProfilePlayer() {

        //free m_my_board
        for (int i = 0; i < BOARD_ROWS; i++) {
            for (int j = 0; j < BOARD_COLS; j++) {
                if(this->m_my_board[i][j] != nullptr)
                    delete(this->m_my_board[i][j]);
            }
        }
    }

    //getters
    PosFileMsg getM_pos_msg() const;
    int getM_num_of_flags() const;
    int getNumMovingPieces() const;

private:

    //static useful function
    static vector<string> split(string line, char delimiter); //split the line according to delimiter
    static string removeExtraSpaces(string line); //remove extra spaces in line if exists (example: 'ab  c d' --> 'ab c d')
    static bool isStringRepInt(string line); //returns true iff the given string represents a valid integer

    void parsePosFile(string path); // parse the Position File, and set the fields accordingly
    //helper function to parse Position File:
    void setNumPlayerAccordingToFile(string path); //set the player num according to position file path given
    bool isValidPieceType(char piece, bool is_joker_rep); //returns true if the piece symbol given is valid (for regular piece or joker)
    bool checkAndUpdateNumPiece(char piece); //check if the piece type exceeds it's number: if not, update it and return true, false o/w (assumed the piece type is valid!)
    bool isIndicesLegal(int row, int col); //return true iff x, y are inside board! (assumed the indices are 0's base!)
    void setPieceOnBoard(char piece, bool is_joker, int row, int col); //set a piece on board (assume the piece is valid!)
    bool isPieceAJoker(char piece); //return true iff the piece given is a JOKER
    bool allFlagsSet(); //return true iff the all flags are set on board
    static void printVerboseMsg(PosFileMsg msg); //print error verbosely
    bool isPosEmpty(int row, int col); //return true iff the board[x][y] is empty (assumed the indices are valid and 0's bases!)

    //functions form abstract class
    virtual void getInitialPositions(int player, std::vector <unique_ptr<PiecePosition>> &vectorToFill) override;
    virtual void notifyOnInitialBoard(const Board &b, const std::vector <unique_ptr<FightInfo>> &fights) override;
    virtual void notifyOnOpponentMove(const Move &move) override;
    virtual void notifyFightResult(const FightInfo &fightInfo) override;
    virtual unique_ptr <Move> getMove() override;
    virtual unique_ptr <JokerChange> getJokerChange() override;

    friend class RPSGame;

};


#endif //OOP_HW2_RPSPROFILEPLAYER_H
