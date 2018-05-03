#include "RPSPosParser.h"


#define UPDATE_POS_FILE_AND_CLOSE(err, num_line) this->m_pos_msg = err; \
                                                 this->m_invalid_line_num_pos_file = num_line; \
                                                 fin.close()


int RPSPosParser::getNumMovingPieces() const {
    return this->m_num_of_jokers_can_move + this->m_num_of_papers + this->m_num_of_rocks + this->m_num_of_scissors;
}



bool RPSPosParser::isStringRepInt(string line) {

    if(line.empty())
        return false;

    int i = 0;
    int size = line.length();

    if(line[i] == '-') { //handle negative number
        i++;
        if(i == size)
            return false;
    }

    while(i < size) {
        if(!(std::isdigit(line[i])))
            return false;

        i++;
    }
    return true;
}

string RPSPosParser::removeExtraSpaces(string line) {

    int size = line.size();
    if(size == 0)
        return line;

    int input_ind = 0, output_ind = 0;

    //init a string with same length
    string tmp_line(size, '\0');

    while(line[input_ind] != '\0') {
        tmp_line[output_ind] = line[input_ind];
        if(line[input_ind] == ' ') {
            while(line[input_ind + 1] == ' ')
                input_ind++;

        }

        input_ind++;
        output_ind++;
    }

    int len_clean_line = output_ind;
    string clean_line(len_clean_line, '\0');

    for(int i =0; i < len_clean_line; i++)
        clean_line[i] = tmp_line[i];

    return clean_line;

}

vector<string> RPSPosParser::split(string line, char delimiter) {
    vector<string> elements;
    stringstream str(line);
    string token;

    while (std::getline(str, token, delimiter))
        elements.push_back(token);

    return elements;
}

void RPSPosParser::printVerboseMsg(PosFileMsg msg) {

    if(msg == FILE_NOT_EXIST)
        cout << "ERROR POSITION FILE: file doesn't exist." << endl;

    if(msg == INVALID_FORMAT)
        cout << "ERROR POSITION FILE: One or more lines are in the wrong format." << endl;

    if(msg == EXCEED_NUM_OF_PIECE)
        cout << "ERROR POSITION FILE: One or more pieces appear more than it's number." << endl;

    if(msg == OVERLOAD_POS)
        cout << "ERROR POSITION FILE: One or more pieces are positioned at the same location." << endl;

    if(msg == INDEX_OUT_OF_RANGE)
        cout << "ERROR POSITION FILE: One or more in indices of pieces' location is not in range." << endl;

    if(msg == MISS_FLAG_PIECE)
        cout << "ERROR POSITION FILE: Missing Flags - not all Flags have been positioned." << endl;

    if(msg == INVALID_PIECE)
        cout << "ERROR POSITION FILE: One or more pieces' type is not legal." << endl;

    if(msg == INVALID_JOKER_REP)
        cout << "ERROR POSITION FILE: Joker new representation is not legal." << endl;

}

bool RPSPosParser::isValidPieceType(char piece, bool is_joker_rep) {

    if(is_joker_rep) {
        if (piece == ROCK || piece == PAPER || piece == SCISSOR || piece == BOMB)
            return true;

        return false;
    } else {
        if (piece == ROCK || piece == PAPER || piece == SCISSOR || piece == BOMB || piece == FLAG)
            return true;

        return false;
    }

}

bool RPSPosParser::checkAndUpdateNumPiece(char piece) {

    switch (piece) {
        case ROCK:
            if (this->m_num_of_rocks < NUM_OF_ROCKS) {
                this->m_num_of_rocks++;
                return true;
            } else
                return false;
        case PAPER:
            if (this->m_num_of_papers < NUM_OF_PAPERS) {
                this->m_num_of_papers++;
                return true;
            } else
                return false;
        case SCISSOR:
            if (this->m_num_of_scissors < NUM_OF_SCISSORS) {
                this->m_num_of_scissors++;
                return true;
            } else
                return false;
        case BOMB:
            if (this->m_num_of_bombs < NUM_OF_BOMBS) {
                this->m_num_of_bombs++;
                return true;
            } else
                return false;
        case JOKER:
            if (this->m_num_of_jokers < NUM_OF_JOKERS) {
                this->m_num_of_jokers++;
                return true;
            } else
                return false;
        case FLAG:
            if (this->m_num_of_flags < NUM_OF_FLAGS) {
                this->m_num_of_flags++;
                return true;
            } else
                return false;
    }
    return false;
}

bool RPSPosParser::isIndicesLegal(int row, int col) {
    return (row >= 0 && row < BOARD_ROWS && col >= 0 && col < BOARD_COLS);
}

bool RPSPosParser::isPosEmpty(int row, int col) {
    return (this->m_my_board[row][col] == nullptr);
}

void RPSPosParser::setPieceOnBoard(char piece, bool is_joker, int row, int col) {

    RPSPoint piece_loc(col, row);
    //this->m_my_board[row][col] = new RPSPiece(piece, is_joker, this->m_num_player, piece_loc);
    this->m_my_board[row][col] = make_unique<RPSPiece>(piece, is_joker, this->m_num_player, piece_loc);
}

bool RPSPosParser::isPieceAJoker(char piece) {
    return piece == JOKER;
}

bool RPSPosParser::allFlagsSet() {
    return this->m_num_of_flags == NUM_OF_FLAGS;
}

void RPSPosParser::parsePosFile(string path) {
    ifstream fin(path);

    if (!fin.is_open()) {
        this->m_pos_msg = FILE_NOT_EXIST;
        printVerboseMsg(FILE_NOT_EXIST);
        return;
    }


    string line;
    string clean_line;
    bool is_joker_line;
    int row = 0, col = 0 , size = 0, line_num = 0;
    char piece;
    while (std::getline(fin, line)) {

        //in case the line is empty
        if(line.empty())
            continue;

        line_num++;

        clean_line = RPSPosParser::removeExtraSpaces(line);
        vector<string> tokens = RPSPosParser::split(clean_line, ' ');
        size = tokens.size();

        if (size == 3 && (tokens[0].length() == 1 && isalpha(tokens[0][0])) && isStringRepInt(tokens[1]) &&
            isStringRepInt(tokens[2])) { //regular piece line
            piece = tokens[0][0];
            col = std::stoi(tokens[1]) - 1;
            row = std::stoi(tokens[2]) - 1;
            is_joker_line = false;

        } else if (size == 4 && tokens[0].length() == 1 && isPieceAJoker(tokens[0][0]) && isStringRepInt(tokens[1]) &&
                   isStringRepInt(tokens[1]) && isalpha(tokens[3][0])) { //joker piece line
            piece = tokens[3][0];
            col = std::stoi(tokens[1]) - 1;
            row = std::stoi(tokens[2]) - 1;
            is_joker_line = true;

        } else { //invalid format
            UPDATE_POS_FILE_AND_CLOSE(INVALID_FORMAT, line_num);
            printVerboseMsg(INVALID_FORMAT);
            return;
        }

        if(is_joker_line) {
            if (!isValidPieceType(piece, true)) {
                UPDATE_POS_FILE_AND_CLOSE(INVALID_JOKER_REP, line_num);
                printVerboseMsg(INVALID_JOKER_REP);
                return;
            }
        } else {
            if (!isValidPieceType(piece, false)) {
                UPDATE_POS_FILE_AND_CLOSE(INVALID_PIECE, line_num);
                printVerboseMsg(INVALID_PIECE);
                return;
            }
        }
        if (!isIndicesLegal(row, col)) {
            UPDATE_POS_FILE_AND_CLOSE(INDEX_OUT_OF_RANGE, line_num);
            printVerboseMsg(INDEX_OUT_OF_RANGE);
            return;
        }

        if (!isPosEmpty(row, col)) {
            UPDATE_POS_FILE_AND_CLOSE(OVERLOAD_POS, line_num);
            printVerboseMsg(OVERLOAD_POS);
            return;
        }

        if(is_joker_line) {
            if (!checkAndUpdateNumPiece(JOKER)) {
                UPDATE_POS_FILE_AND_CLOSE(EXCEED_NUM_OF_PIECE, line_num);
                printVerboseMsg(EXCEED_NUM_OF_PIECE);
                return;
            }
            //check if joker new rep cam move (i.e not a bomb)
            if(piece == ROCK || piece == PAPER || piece == SCISSOR)
                this->m_num_of_jokers_can_move++;
        } else {
            if (!checkAndUpdateNumPiece(piece)) {
                UPDATE_POS_FILE_AND_CLOSE(EXCEED_NUM_OF_PIECE, line_num);
                printVerboseMsg(EXCEED_NUM_OF_PIECE);
                return;
            }
        }

        //now, we can update the piece
        if (is_joker_line)
            setPieceOnBoard(piece, true, row, col);
        else
            setPieceOnBoard(piece, false, row, col);

    }

    if (!allFlagsSet()) {
        UPDATE_POS_FILE_AND_CLOSE(MISS_FLAG_PIECE, line_num);
        printVerboseMsg(MISS_FLAG_PIECE);
        return;
    }

    UPDATE_POS_FILE_AND_CLOSE(FILE_SUCCESS, -1);
}


