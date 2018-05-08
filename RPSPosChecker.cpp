#include "RPSPosChecker.h"

#define UPDATE_STATUS(err, piece_num) this->m_pos_msg = err; \
                                     this->m_invalid_num_of_piece = piece_num;

void RPSPosChecker::printVerboseMsg(PosMsg msg) {

    if(msg == EXCEED_NUM_OF_PIECE)
        cout << "ERROR POSITION: One or more pieces appear more than it's number." << endl;

    if(msg == OVERLOAD_POS)
        cout << "ERROR POSITION: One or more pieces are positioned at the same location." << endl;

    if(msg == INDEX_OUT_OF_RANGE)
        cout << "ERROR POSITION: One or more in indices of pieces' location is not in range." << endl;

    if(msg == MISS_FLAG_PIECE)
        cout << "ERROR POSITION: Missing Flags - not all Flags have been positioned." << endl;

    if(msg == INVALID_PIECE)
        cout << "ERROR POSITION: One or more pieces' type is not legal." << endl;

    if(msg == INVALID_JOKER_REP)
        cout << "ERROR POSITION: Joker new representation is not legal." << endl;

}

int RPSPosChecker::getNumMovingPieces() const {
    return this->m_num_of_jokers_can_move + this->m_num_of_papers + this->m_num_of_rocks + this->m_num_of_scissors;
}

int RPSPosChecker::getNumOfFlags() const {
    return this->m_num_of_flags;
}

int RPSPosChecker::getPosMsg() const {
    return this->m_pos_msg;
}

int RPSPosChecker::getInvalidNumPiece() const {
    return this->m_invalid_num_of_piece;
}


bool RPSPosChecker::isValidPieceType(char piece, bool is_joker_rep) {

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

bool RPSPosChecker::checkAndUpdateNumPiece(char piece) {

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

bool RPSPosChecker::isIndicesLegal(int row, int col) {
    return (row >= 0 && row < BOARD_ROWS && col >= 0 && col < BOARD_COLS);
}

bool RPSPosChecker::isPosEmpty(int row, int col) {
    return (this->m_pieces_locs[row][col] == false);
}

void RPSPosChecker::setPieceOnBoard(int row, int col) {
    this->m_pieces_locs[row][col] = true;
}

bool RPSPosChecker::isPieceAJoker(char piece) {
    return piece == JOKER;
}

bool RPSPosChecker::allFlagsSet() {
    return this->m_num_of_flags == NUM_OF_FLAGS;
}


void RPSPosChecker::checkInitPos(std::vector <unique_ptr<PiecePosition>>& initPosVector) {

    int index_piece = 0;
    for(vector<unique_ptr<PiecePosition>>::iterator it = initPosVector.begin(); it != initPosVector.end(); ++it) {

        index_piece++;
        char piece_type = (*it)->getPiece();
        char joker_rep = (*it)->getJokerRep(); //'#' if not a joker
        int col = (*it)->getPosition().getX();
        int row = (*it)->getPosition().getY();

        //validity checks
        if(isPieceAJoker(piece_type)) {

            if (isValidPieceType(joker_rep, true) == false) {
                printVerboseMsg(INVALID_JOKER_REP);
                UPDATE_STATUS(INVALID_JOKER_REP, index_piece);
                return;
            }
        } else {
            if(isValidPieceType(piece_type, false) == false) {
                printVerboseMsg(INVALID_PIECE);
                UPDATE_STATUS(INVALID_PIECE, index_piece);
                return;
            }
        }

        if(isIndicesLegal(row, col) == false) {
            printVerboseMsg(INDEX_OUT_OF_RANGE);
            UPDATE_STATUS(INDEX_OUT_OF_RANGE, index_piece);
            return;
        }

        if(isPosEmpty(row, col) == false) {
            printVerboseMsg(OVERLOAD_POS);
            UPDATE_STATUS(OVERLOAD_POS, index_piece);
            return;
        }

        if(checkAndUpdateNumPiece(piece_type) == false) {
            printVerboseMsg(EXCEED_NUM_OF_PIECE);
            UPDATE_STATUS(EXCEED_NUM_OF_PIECE, index_piece);
            return;
        }

        //check if joker rep can move (i.e not a bomb)
        if(joker_rep == ROCK || joker_rep == PAPER || joker_rep == SCISSOR)
            this->m_num_of_jokers_can_move++;


        //now we can set the piece on board
        setPieceOnBoard(row, col);
    }

    if(allFlagsSet() == false) {
        printVerboseMsg(EXCEED_NUM_OF_PIECE);
        UPDATE_STATUS(EXCEED_NUM_OF_PIECE, index_piece);
        return;
    }

    UPDATE_STATUS(POS_SUCCESS, -1);
}


