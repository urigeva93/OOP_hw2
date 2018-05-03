#ifndef OOP_HW2_RPSPARSER_H
#define OOP_HW2_RPSPARSER_H

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

class RPSPosParser {

private:

    unique_ptr<RPSPiece> m_my_board[BOARD_ROWS][BOARD_COLS]; //represents the init board of the player

    PosFileMsg m_pos_msg; //represents the status of the position file
    int m_invalid_line_num_pos_file; //represents the wrong line in pos file

    int m_num_player; //represents thu num of player
    int m_num_of_rocks;
    int m_num_of_papers;
    int m_num_of_scissors;
    int m_num_of_bombs;
    int m_num_of_jokers;
    int m_num_of_flags;
    int m_num_of_jokers_can_move;

public:

    RPSPosParser(int num_player) : m_num_player(num_player), m_invalid_line_num_pos_file(-1), m_num_of_rocks(0), m_num_of_papers(0), m_num_of_scissors(0),
                                   m_num_of_bombs(0), m_num_of_jokers(0), m_num_of_flags(0), m_num_of_jokers_can_move(0) {}



    int getNumMovingPieces() const;


    //static useful function
    static vector<string> split(string line, char delimiter); //split the line according to delimiter
    static string removeExtraSpaces(string line); //remove extra spaces in line if exists (example: 'ab  c d' --> 'ab c d')
    static bool isStringRepInt(string line); //returns true iff the given string represents a valid integer
    static void printVerboseMsg(PosFileMsg msg); //print error verbosely

    void parsePosFile(string path); // parse the Position File, and set the fields accordingly

    //helper function for parsePosFile
    bool isValidPieceType(char piece, bool is_joker_rep); //returns true if the piece symbol given is valid (for regular piece or joker)
    bool checkAndUpdateNumPiece(char piece); //check if the piece type exceeds it's number: if not, update it and return true, false o/w (assumed the piece type is valid!)
    bool isIndicesLegal(int row, int col); //return true iff x, y are inside board! (assumed the indices are 0's base!)
    void setPieceOnBoard(char piece, bool is_joker, int row, int col); //set a piece on board (assume the piece is valid!)
    bool isPieceAJoker(char piece); //return true iff the piece given is a JOKER
    bool allFlagsSet(); //return true iff the all flags are set on board
    bool isPosEmpty(int row, int col); //return true iff the board[x][y] is empty (assumed the indices are valid and 0's bases!)

    friend class RPSPlayerFromFile;

};


#endif //OOP_HW2_RPSPARSER_H
