#include "RPSPlayerAuto.h"

using namespace std;

int RPSPlayerAuto::getM_num_of_flags() const {
    return this->m_num_of_flags;
}

int RPSPlayerAuto::getNumMovingPieces() const {
    int num_all_pieces = this->m_num_of_jokers_can_move + this->m_num_of_scissors + this->m_num_of_papers + this->m_num_of_rocks;

    return num_all_pieces;
}

void RPSPlayerAuto::setPieceOnBoard(char piece, bool is_joker, int row, int col) {

    RPSPoint piece_loc(col, row);
    //this->m_my_board[row][col] = new RPSPiece(piece, is_joker, this->m_num_player, piece_loc);
    //TODO: change to shared_ptr
    this->m_my_board[row][col] = make_shared<RPSPiece>(piece, is_joker, this->m_num_player, piece_loc);

}

void RPSPlayerAuto::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {

    //set the player number
    this->m_num_player = player;
    int total_pieces = NUM_OF_BOMBS+NUM_OF_PAPERS+NUM_OF_FLAGS+NUM_OF_JOKERS+NUM_OF_ROCKS+NUM_OF_SCISSORS-2;

    if (player == PLAYER_1) {
        // set the flag on the board - left upper point
        setPieceOnBoard(FLAG, false, 0, BOARD_COLS-1);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, RPSPoint(0,BOARD_COLS-1)));
        updateNumPiece(FLAG);
        // set the bombs around
        setPieceOnBoard(BOMB, false, 1, BOARD_COLS-1);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, RPSPoint(1,BOARD_COLS-1)));
        updateNumPiece(BOMB);

        if (NUM_OF_BOMBS > 1) {
            setPieceOnBoard(BOMB, false, 0, BOARD_COLS-2);
            vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, RPSPoint(0,BOARD_COLS-2)));
            updateNumPiece(BOMB);
            total_pieces--;
        }
    } else if (player == PLAYER_2) {
        // set the flag on the board - left upper point
        setPieceOnBoard(FLAG, false, BOARD_ROWS-1, 0);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, RPSPoint(BOARD_ROWS-1,0)));
        updateNumPiece(FLAG);
        // set the bombs around
        setPieceOnBoard(BOMB, false, BOARD_ROWS-2, 0);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, RPSPoint(BOARD_ROWS-2,0))));
        updateNumPiece(BOMB);

        if (NUM_OF_BOMBS > 1 && checkAndUpdateNumPiece(BOMB)) {
            setPieceOnBoard(BOMB, false, BOARD_ROWS-1, 1);
            vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, RPSPoint(BOARD_ROWS-1, 1))));
            updateNumPiece(BOMB);
            total_pieces--;
        }
    }


    // set the other pieces
    vector<pair<int, int>> coords;
    vector<pair<int, int>> coordsPositions(total_pieces);
    for (int i=0; i<BOARD_ROWS; i++)
        for (int j=0; j<BOARD_COLS; j++)
            if ((m_num_player == PLAYER_1) && ((i==0 && j==BOARD_COLS-1) || (i==1 && j==BOARD_COLS-1) || (i==0 && j==BOARD_COLS-2)))
                continue;
            else if((m_num_player == PLAYER_2) && ((i==BOARD_ROWS-1 && j==0) || (i==BOARD_ROWS-2 && j == 0) || (i=BOARD_ROWS-1 && j==1)))
                continue;
            coords.push_back(make_pair(i,j));

    //shuffle the coordinates
    random_shuffle(coords.begin(), coords.ends());
    copy(coords.begin(), coords.begin()+total_pieces, coordsPositions.begin());

    //for each piece- get the next coordinate and set it on board
    int index = 0;
    while (this->myPieces[PAPER] < NUM_OF_PAPERS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(PAPER, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(PAPER, false, RPSPoint(pos.first, pos.second))));
        updateNumPiece(PAPER);
        index++;
    }
    while (this->myPieces[ROCK] < NUM_OF_ROCKS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(ROCK, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(ROCK, false, RPSPoint(pos.first, pos.second))));
        updateNumPiece(ROCK);
        index++;
    }
    while (this->myPieces[BOMB] < NUM__OF_BOMBS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(BOMB, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, RPSPoint(pos.first, pos.second))));
        updateNumPiece(BOMB);
        index++;
    }
    while (this->myPieces[SCISSOR] < NUM_OF_SCISSORS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(SCISSOR, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(SCISSOR, false, RPSPoint(pos.first, pos.second))));
        updateNumPiece(SCISSOR);
        index++;
    }
    while (this->myPieces[FLAG] < NUM_OF_FLAGS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(FLAG, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, RPSPoint(pos.first, pos.second))));
        updateNumPiece(FLAG);
        index++;
    }

//    //iterate the parser board and take the pieces and enter them to vectorToFill;
//    for(int i =0; i < BOARD_ROWS; i++) {
//        for(int j =0; j < BOARD_COLS; j++) {
//            if(this->m_my_board[i][j] != nullptr) {
//                unique_ptr<PiecePosition> piece = std::move(this->m_my_board[i][j]);
//                vectorToFill.push_back(std::move(piece);
//            }
//        }
//    }
}


void RPSPlayerAuto::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {
    // update fights result on our private board
    for (size_t i = 0; i < fights.size(); i++) {
        // get iterator
        auto fight = fights[i].get();
        int opponentPlayer = this->m_num_player == 1 ? 0 : 1;
        char oppPiece = fight->getPiece(opponentPlayer);
        Point fight_place = fight->getPosition();

        if (fight->getWinner() == this->m_num_player) { //current player won
            // update position on the shared board
            this->m_my_board[fight_place.getX()][fight_place.getY()] = make_shared<RPSPiece>(fight->getPiece(this->m_num_player), false, RPSPoint(fight_place.getX(), fight_place.getY()));
            // update saved information
            this->m_opponent_total--;
            this->opponentPieces[oppPiece]--;
            this->unknownOpponentPieces.erase(remove(this->unknownOpponentPieces.begin(),this->unknownOpponentPieces.end(), fight_place), this->unknownOpponentPieces.end());
        } else if (fight->getWinner() == opponentPlayer) { //opponent player won
            this->myPieces[fight->getPiece(this->m_num_player)]--;
            // update position on the shared board
            this->m_my_board[fight_place.getX()][fight_place.getY()] = make_shared<RPSPiece>(fight->getPiece(opponentPlayer), false, RPSPoint(fight_place.getX(), fight_place.getY()));
            // update saved information
            this->unknownOpponentPieces.erase(remove(this->unknownOpponentPieces.begin(),this->unknownOpponentPieces.end(), fight_place), this->unknownOpponentPieces.end());
            this->knownOpponentPieces.erase(remove(this->knownOpponentPieces.begin(),this->knownOpponentPieces.end(), fight_place), this->knownOpponentPieces.end());
        } else if(fight->getWinner() == TIE) {
            this->myPieces[fight->getPiece(this->m_num_player)]--;
            this->opponentPieces[oppPiece]--;
            this->m_opponent_total--;
            // update position on the shared board
            this->m_my_board[fight_place.getX()][fight_place.getY()] = nullptr;
            // update saved information
            this->unknownOpponentPieces.erase(remove(this->unknownOpponentPieces.begin(),this->unknownOpponentPieces.end(), fight_place), this->unknownOpponentPieces.end());
            this->knownOpponentPieces.erase(remove(this->knownOpponentPieces.begin(),this->knownOpponentPieces.end(), fight_place), this->knownOpponentPieces.end());
        }
    }
}


void RPSPlayerAuto::notifyOnOpponentMove(const Move &move) {
    // update our board accordingly
    this->m_my_board[move.getFrom().getX()][move.getFrom().getY()] = nullptr;
    // destination - unknown moving character '?'
    // if this position is inside the vector known - use the char
    setPieceOnBoard(UNKNOWN_PIECE, false, move.getTo().getX(), move.getTo().getY());
    RPSPoint src_pos(move.getFrom().getX(), move.getFrom().getY());
    this->unknownOpponentPieces.erase(remove(this->unknownOpponentPieces.begin(),this->unknownOpponentPieces.end(), src_pos), this->unknownOpponentPieces.end());
}

void RPSPlayerAuto::notifyFightResult(const FightInfo &fightInfo) {
    if (this->m_num_player == fightInfo.getWinner()) {
        setPieceOnBoard(fightInfo.getPiece(this->m_num_player), false, fightInfo.getPosition().getX(), fightInfo.getPosition().getY());

    } else if (fightInfo.getWinner() == TIE) {
        int opponentPlayer = this->m_num_player == 1 ? 0 : 1;
        this->m_my_board[fightInfo.getPosition().getX()][fightInfo.getPosition().getY()] = nullptr;
        this->myPieces[fightInfo.getPiece(this->m_num_player)]--;
        this->opponentPieces[fightInfo.getPiece(opponentPlayer)]--;
        this->unknownOpponentPieces.erase(remove(this->unknownOpponentPieces.begin(), this->unknownOpponentPieces.end(), ))
    } else {

    }
}

unique_ptr<Move> RPSPlayerAuto::getMove() {
    return unique_ptr<Move>();
}

unique_ptr<JokerChange> RPSPlayerAuto::getJokerChange() {
    return unique_ptr<JokerChange>();
}

bool RPSPlayerAuto::checkAndUpdateNumPiece(char piece) {
    switch (piece) {
        case ROCK:
            if (this->m_num_of_rocks < NUM_OF_ROCKS) {
                this->m_num_of_rocks++;
                return true;
            } else
                return false;
        case PAPER:
            if (this->m_num_of_papers < MUM_OF_PAPERS) {
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

void RPSPlayerAuto::updateNumPiece(char piece) {
    switch (piece) {
        case ROCK:
            this->m_num_of_rocks++;
        case PAPER:
            this->m_num_of_papers++;
        case SCISSOR:
            this->m_num_of_scissors++;
        case BOMB:
            this->m_num_of_bombs++;
        case JOKER:
            this->m_num_of_jokers++;
        case FLAG:
            this->m_num_of_flags++;
    }
}
char RPSPlayerAuto::getPieceFromBoard(Point& pos){
    return this->m_my_board[pos.getX()][pos.getY()]->getPiece();
}
