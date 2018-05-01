#ifndef OOP_HW2_RPSBOARD_H
#define OOP_HW2_RPSBOARD_H

#include "Board.h"
#include "RPSProfilePlayer.h"
#include "RPSFight.h"


//reason winner msg
enum ReasonWinner {
    ALL_FLAG_CAPTURED_PLAYER_ONE,
    ALL_FLAG_CAPTURED_PLAYER_TWO,
    ALL_PIECES_EATEN_PLAYER_ONE,
    ALL_PIECES_EATEN_PLAYER_TWO,
    ALL_FLAGS_EATEN_IN_POS_FILE,
    ALL_PIECES_EATEN_IN_POS_FILE,
    BAD_POS_FILE_PLAYER_ONE,
    BAD_POS_FILE_PLAYER_TWO,
    BAD_POS_FILE_BOTH_PLAYERS,
    BAD_MOVE_FILE_PLAYER_ONE,
    BAD_MOVE_FILE_PLAYER_TWO,
    AFTER_MOVES_NO_WINNER
};

//files macro
#define PATH_POS_FILE_PLAYER1 "./player1.rps_board"
#define PATH_POS_FILE_PLAYER2 "./player2.rps_board"
#define PATH_MOVE_FILE_PLAYER1 "./player1.rps_moves"
#define PATH_MOVE_FILE_PLAYER2 "./player2.rps_moves"
#define PATH_OUTPUT_FILE "./rps.output"

//moves files errors:
#define FILE_NOT_EXISTS "ERROR MOVE FILE: file doesn't exist.\n"
#define BAD_FORMAT "ERROR MOVE FILE: One or more lines are in the wrong format.\n"
#define NOT_IN_RANGE "ERROR MOVE FILE: One or more indices are out of range.\n"
#define PIECE_NOT_EXIST "ERROR MOVE FILE: piece is not located in position required.\n"
#define SRC_NO_CURR_PIECE "ERROR MOVE FILE: current location doesn't contains players' piece.\n"
#define NOT_ABLE_TO_MOVE "ERROR MOVE FILE: piece chosen is not able to move.\n"
#define DST_PIECE "ERROR MOVE FILE: required location to move contains same player piece.\n"
#define MOVE_NOT_LEGAL "ERROR MOVE FILE: one or more moves is illegal according to game rules.\n"
#define NO_JOKER "ERROR MOVE FILE: the is no joker at required location.\n"
#define NEW_REP_INVALID "ERROR MOVE FILE: new rep of joker is invalid.\n"


class RPSGame : public Board {

private:

    RPSPiece* m_game_board[BOARD_ROWS][BOARD_COLS]; //board of the game

    int m_current_player; //num represents the current player (1 - P1, 2 - P2, 0 if empty)
    bool m_game_over; //flag indicates if the game is over (different reasons)

    int m_num_moving_pieces_player1; //num of pieces player 1 (includes jokers, without flags!)
    int m_num_moving_pieces_player2; //num of pieces player 2 (includes jokers, without flags!)

    int m_num_flags_player1; //num of flags player 1
    int m_num_flags_player2; //num of flags player 2

    //output file
    ofstream m_output_file; //stream of the output file
    ReasonWinner m_reason_winner; //reason of winning
    int m_bad_line1_num; //bad line #1 index (if exists)
    int m_bad_line2_num; //bad line #2 index (if exists)
    int m_winner; //num of the winner


public:

    RPSGame() : m_current_player(PLAYER_1), m_game_over(false), m_num_moving_pieces_player1(0), m_num_moving_pieces_player2(0),
                m_num_flags_player1(0), m_num_flags_player2(0), m_bad_line1_num(-1), m_bad_line2_num(-1), m_winner(-1) {

        //init the board with NULL's
        for (int i = 0; i < BOARD_ROWS; i++) {
            for (int j = 0; j < BOARD_COLS; j++)
                this->m_game_board[i][j] = nullptr;
        }

        //open output file
        this->m_output_file.open(PATH_OUTPUT_FILE);
    }

    ~RPSGame() {

        //delete m_game_board
        for (int i = 0; i < BOARD_ROWS; i++) {
            for (int j = 0; j < BOARD_COLS; j++) {
                if(this->m_game_board[i][j] != nullptr)
                    delete (m_game_board[i][j]); //delete a RPSPiece
            }
        }
    }

    void initGame(); // init the game
    void playGame(); //play the game
    void endGame(); // end the game (by writing all details to output file)

    bool isM_game_over() const;

private:
    void checkGameStatus(); //checks the status of the game (if there is a winner or a tie)

    void checkGameStatusAfterInit(); //checks the status of the game right after init phase

    //helper function for initGame and playGame:
    FightResult manageFight(RPSPiece *piece_curr_player, RPSPiece *piece_opp_player); //manage a fight between to pieces (assumed the fight is legal!)
    void updateGameAfterFight(FightResult res, RPSPiece *piece_curr_player, RPSPiece *piece_opp_player, int row, int col); //update the game and pieces after fight occur (assume legal indices!).

    //validity regular moves tests
    bool isMoveOnBoard(int src_row, int src_col, int dst_row, int dst_col); //return true iff all the indices are inside board
    bool isInBoard(int row, int col) const; //helper function for isMoveOnBoard
    bool isPieceExist(int row, int col); // return true iff the is a piece at the given location
    bool isPieceAbleToMove(char piece); //return true iff the piece is able to move (not BOMB or FLAG)
    bool isSamePieceAsCurrent(int row, int col); //return true iff the piece at at board[x][y] belongs to current player (assumed legal indices!)
    bool isMoveLegal(int src_row, int src_col, int dst_row, int dst_col); //return true you can from src to dst in a legal move
    vector<pair<int, int>> getLegalLMovesForPiece(int row, int col); //return possible moves from indices given on board.

    //additional validity for joker move
    bool isJokerNewRepValid(char new_rep); //return true iff new_rep is valid new joker rep
    bool isPieceAJoker(int row, int col); //return true iff the piece at board[x][y] is joker (assumed valid indices!)

    void playRegularMove(int src_row, int src_col, int dst_row, int dst_col); //play regular move of the current player
    void playJokerMove(int joker_row, int joker_col, char new_rep); //play joker move of the current player.

    void changeCurrentPlayer(); //change the current player

    //helper function to endGame() function
    void printBoard(); //print the board to output stream (assumed offset of output file exists!)
    void printReasonWinner(); //print reason of the winner (assumed offset of output file exists!)

    //functions form abstract class
    virtual int getPlayer(const Point& pos) const override;
};


#endif //OOP_HW2_RPSBOARD_H
