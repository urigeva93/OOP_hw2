#ifndef OOP_HW2_RPSBOARD_H
#define OOP_HW2_RPSBOARD_H

#include "RPSGameBoard.h"
#include "RPSPlayerFromFile.h"
#include "RPSFight.h"
#include "RPSPosChecker.h"

//reason winner msg
enum ReasonWinner {
    ALL_FLAG_CAPTURED_PLAYER_ONE,
    ALL_FLAG_CAPTURED_PLAYER_TWO,
    ALL_PIECES_EATEN_PLAYER_ONE,
    ALL_PIECES_EATEN_PLAYER_TWO,
    ALL_FLAGS_EATEN_IN_POS,
    ALL_PIECES_EATEN_IN_POS,
    BAD_POS_PLAYER_ONE,
    BAD_POS_PLAYER_TWO,
    BAD_POS_BOTH_PLAYERS,
    BAD_MOVE_PLAYER_ONE,
    BAD_MOVE_PLAYER_TWO,
    AFTER_MOVES_NO_WINNER,
    MOVES_LIMIT_NO_FIGHT
};

//files macro
#define PATH_OUTPUT_FILE "./rps.output"

//moves files errors:
#define NOT_IN_RANGE "ERROR MOVE FILE: One or more indices are out of range.\n"
#define PIECE_NOT_EXIST "ERROR MOVE FILE: piece is not located in position required.\n"
#define SRC_NO_CURR_PIECE "ERROR MOVE FILE: current location doesn't contains players' piece.\n"
#define NOT_ABLE_TO_MOVE "ERROR MOVE FILE: piece chosen is not able to move.\n"
#define DST_PIECE "ERROR MOVE FILE: required location to move contains same player piece.\n"
#define MOVE_NOT_LEGAL "ERROR MOVE FILE: one or more moves is illegal according to game rules.\n"
#define NO_JOKER "ERROR MOVE FILE: the is no joker at required location.\n"
#define NEW_REP_INVALID "ERROR MOVE FILE: new rep of joker is invalid.\n"

#define MOVES_WITHOUT_FIGHT_LIMIT 100


class RPSGame {

private:

    RPSGameBoard m_board; // represents the board of the game

    int m_current_player; //num represents the current player (1 - P1, 2 - P2, 0 if empty)
    bool m_game_over; //flag indicates if the game is over (different reasons)

    unique_ptr<PlayerAlgorithm> m_algo_player1; //Algo Player1
    unique_ptr<PlayerAlgorithm> m_algo_player2; //Algo Player2
    int m_num_moving_pieces_player1; //num of pieces player 1 (includes jokers, without flags!)
    int m_num_moving_pieces_player2; //num of pieces player 2 (includes jokers, without flags!)
    int m_num_flags_player1; //num of flags player 1
    int m_num_flags_player2; //num of flags player 2

    //output file
    ofstream m_output_file; //stream of the output file
    ReasonWinner m_reason_winner; //reason of winning
    int m_bad_input_index1; //bad action #1 index (if exists)
    int m_bad_input_index2; //bad action #2 index (if exists)
    int m_winner; //num of the winner

public:

    RPSGame(unique_ptr<PlayerAlgorithm> algo_player1, unique_ptr<PlayerAlgorithm> algo_player2) : m_current_player(PLAYER_1), m_game_over(false), m_num_moving_pieces_player1(0), m_num_moving_pieces_player2(0),
                m_num_flags_player1(0), m_num_flags_player2(0), m_bad_input_index1(-1), m_bad_input_index2(-1), m_winner(-1)  {


        //open output file
        this->m_output_file.open(PATH_OUTPUT_FILE);

        this->m_algo_player1 = std::move(algo_player1);
        this->m_algo_player2 = std::move(algo_player2);

    }

    ~RPSGame() {
        this->m_output_file.close();
    }

    void initGame(); // init the game
    void playGame(); //play the game
    void endGame(); // end the game (by writing all details to output file)
    bool getGameOver() const;

private:

    void checkGameStatus(); //checks the status of the game (if there is a winner or a tie)
    void checkGameStatusAfterInit(); //checks the status of the game right after init phase

    //helper function for initGame and playGame:
    //FightResult manageFight(RPSPiece *piece_curr_player, RPSPiece *piece_opp_player); //manage a fight between to pieces (assumed the fight is legal!)
    void updateGameAfterFight(shared_ptr<RPSPiece> piece_curr_player, shared_ptr<RPSPiece> piece_opp_player, RPSPoint fight_pos); //update the game and pieces after fight occur

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

    //play move functions
    bool playRegularMove(int src_row, int src_col, int dst_row, int dst_col); //play regular move of the current player (return true if there was a fight, false o/w)
    void playJokerMove(int joker_row, int joker_col, char new_rep); //play joker move of the current player.

    void changeCurrentPlayer(); //change the current player

    //helper function to endGame() function
    void printBoard(); //print the board to output stream (assumed offset of output file exists!)
    void printReasonWinner(); //print reason of the winner (assumed offset of output file exists!)


};


#endif //OOP_HW2_RPSBOARD_H
