#include "RPSPlayerAuto.h"

using namespace std;

int RPSPlayerAuto::getM_num_of_flags() const {
    return this->myPieces.find(FLAG)->second;
}

int RPSPlayerAuto::getNumMovingPieces() const {
    // int num_all_pieces = this->m_num_of_jokers_can_move + this->m_num_of_scissors + this->m_num_of_papers + this->m_num_of_rocks;
    int num_all_pieces = this->myPieces.find(JOKER)->second + this->myPieces.find(SCISSOR)->second+this->myPieces.find(PAPER)->second+this->myPieces.find(ROCK)->second;
    return num_all_pieces;
}

int RPSPlayerAuto::getOpponentMovingPieces() const {
    int num_all_pieces = this->opponentPieces.find(JOKER)->second + this->opponentPieces.find(SCISSOR)->second+this->opponentPieces.find(PAPER)->second+this->opponentPieces.find(ROCK)->second;
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
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, this->m_num_player, RPSPoint(0,BOARD_COLS-1))));
        updateNumPiece(FLAG);
        // set the bombs around
        setPieceOnBoard(BOMB, false, 1, BOARD_COLS-1);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, this->m_num_player, RPSPoint(1,BOARD_COLS-1))));
        updateNumPiece(BOMB);

        if (NUM_OF_BOMBS > 1) {
            setPieceOnBoard(BOMB, false, 0, BOARD_COLS-2);
            vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, this->m_num_player, RPSPoint(0,BOARD_COLS-2))));
            updateNumPiece(BOMB);
            total_pieces--;
        }
    } else if (player == PLAYER_2) {
        // set the flag on the board - left upper point
        setPieceOnBoard(FLAG, false, BOARD_ROWS-1, 0);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, this->m_num_player, RPSPoint(BOARD_ROWS-1,0))));
        updateNumPiece(FLAG);
        // set the bombs around
        setPieceOnBoard(BOMB, false, BOARD_ROWS-2, 0);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, this->m_num_player, RPSPoint(BOARD_ROWS-2,0))));
        updateNumPiece(BOMB);

        if (NUM_OF_BOMBS > 1) {
            setPieceOnBoard(BOMB, false, BOARD_ROWS-1, 1);
            vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, this->m_num_player, RPSPoint(BOARD_ROWS-1, 1))));
            updateNumPiece(BOMB);
            total_pieces--;
        }
    }


    // set the other pieces
    vector<pair<int, int>> coords;
    vector<pair<int, int>> coordsPositions(total_pieces);
    for (int i=0; i<BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if ((m_num_player == PLAYER_1) &&
                ((i == 0 && j == BOARD_COLS - 1) || (i == 1 && j == BOARD_COLS - 1) || (i == 0 && j == BOARD_COLS - 2)))
                continue;
            else if ((m_num_player == PLAYER_2) &&
                     ((i == BOARD_ROWS - 1 && j == 0) || (i == BOARD_ROWS - 2 && j == 0) ||
                      (i = BOARD_ROWS - 1 && j == 1)))
                continue;
            coords.push_back(make_pair(i, j));
        }
    }
    //shuffle the coordinates
    random_shuffle(coords.begin(), coords.end());
    copy(coords.begin(), coords.begin()+total_pieces, coordsPositions.begin());

    //for each piece- get the next coordinate and set it on board
    int index = 0;
    while (this->myPieces[PAPER] < NUM_OF_PAPERS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(PAPER, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(PAPER, false, this->m_num_player, RPSPoint(pos.first, pos.second))));
        updateNumPiece(PAPER);
        index++;
    }
    while (this->myPieces[ROCK] < NUM_OF_ROCKS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(ROCK, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(ROCK, false, this->m_num_player, RPSPoint(pos.first, pos.second))));
        updateNumPiece(ROCK);
        index++;
    }
    while (this->myPieces[BOMB] < NUM_OF_BOMBS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(BOMB, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, this->m_num_player, RPSPoint(pos.first, pos.second))));
        updateNumPiece(BOMB);
        index++;
    }
    while (this->myPieces[SCISSOR] < NUM_OF_SCISSORS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(SCISSOR, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(SCISSOR, false, this->m_num_player, RPSPoint(pos.first, pos.second))));
        updateNumPiece(SCISSOR);
        index++;
    }
    while (this->myPieces[FLAG] < NUM_OF_FLAGS) {
        pair<int, int> pos = coordsPositions[index];
        setPieceOnBoard(FLAG, false ,pos.first, pos.second);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, this->m_num_player, RPSPoint(pos.first, pos.second))));
        updateNumPiece(FLAG);
        index++;
    }
}


void RPSPlayerAuto::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {
    // update fights result on our private board

    int opponentPlayer = this->m_num_player == 1 ? 0 : 1;

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            RPSPoint cur_point(i,j);
            if (b.getPlayer(cur_point) == opponentPlayer) {
                //setPieceOnBoard(UNKNOWN_PIECE, false, i, j);
                this->m_opponent_total++;
                this->unknownOpponentPieces.push_back(make_unique<RPSPoint>(i,j));
            }
        }
    }
    for (size_t i = 0; i < fights.size(); i++) {
        // get iterator
        auto fight = fights[i].get();
        this->notifyFightResult(*fight);
    }

}


void RPSPlayerAuto::notifyOnOpponentMove(const Move &move) {
    // update our board accordingly
    this->m_my_board[move.getFrom().getX()][move.getFrom().getY()] = nullptr;
    // remove form unknown
    const RPSPoint move_to(move.getTo().getX(), move.getTo().getY());
    removeFromVector(2, move_to);
    // destination - unknown moving character '?'
    // if this position is inside the vector known - use the char
    RPSPoint move_from(move.getFrom().getX(), move.getFrom().getY());
    char piece = this->getPieceFromBoard(move_from);
    // set the move on out board
    setPieceOnBoard(piece, false, move.getTo().getX(), move.getTo().getY());
    RPSPoint src_pos(move.getFrom().getX(), move.getFrom().getY());

}

void RPSPlayerAuto::notifyFightResult(const FightInfo &fightInfo) {
    int opponentPlayer = this->m_num_player == 1 ? 0 : 1;
    char oppPiece = fightInfo.getPiece(opponentPlayer);
    //Point fight_place = fight->getPosition();
    RPSPoint fight_place(fightInfo.getPosition().getX(), fightInfo.getPosition().getY());

    if (fightInfo.getWinner() == this->m_num_player) { //current player won
        // update position on the shared board
        this->m_my_board[fight_place.getX()][fight_place.getY()] = make_shared<RPSPiece>(fightInfo.getPiece(this->m_num_player), false, this->m_num_player, RPSPoint(fight_place.getX(), fight_place.getY()));
        // update saved information
        this->m_opponent_total--;
        this->opponentPieces[oppPiece]--;
        // remove form unknown
        removeFromVector(2, fight_place);

    } else if (fightInfo.getWinner() == opponentPlayer) { //opponent player won
        this->myPieces[fightInfo.getPiece(this->m_num_player)]--;
        // update position on the shared board
        this->m_my_board[fight_place.getX()][fight_place.getY()] = make_shared<RPSPiece>(fightInfo.getPiece(opponentPlayer), false, opponentPlayer, RPSPoint(fight_place.getX(), fight_place.getY()));
        // remove form unknown
        removeFromVector(2, fight_place);
        // remove from known vector
        removeFromVector(1, fight_place);

    } else if(fightInfo.getWinner() == TIE) {
        this->myPieces[fightInfo.getPiece(this->m_num_player)]--;
        this->opponentPieces[oppPiece]--;
        this->m_opponent_total--;
        // update position on the shared board
        this->m_my_board[fight_place.getX()][fight_place.getY()] = nullptr;
        // update saved information
        // remove form unknown
        removeFromVector(2, fight_place);
        // remove from known vector
        removeFromVector(1, fight_place);
    }
}

unique_ptr<Move> RPSPlayerAuto::getMove() {
    // RPSMove move(RPSPoint(-1,-1), RPSPoint(-1,-1));
    RPSMove move = this->getBestMove();
    unique_ptr<Move> move_ptr = make_unique<RPSMove>(RPSPoint(move.getFrom().getX(), move.getFrom().getY()), RPSPoint(move.getTo().getX(), move.getTo().getY()));
    return move_ptr;
}

RPSMove RPSPlayerAuto::getBestMove() {
    RPSMove bestMove(RPSPoint(-1,-1), RPSPoint(-1,-1));
    int my_pieces = this->getNumMovingPieces();
    RPSPoint opp(move.getTo().getX(), move.getTo().getY());
    char opp_piece = getPieceFromBoard(opp);
    RPSPoint cur(move.getFrom().getX(), move.getFrom().getY());
    char cur_piece = getPieceFromBoard(cur);
    // calculate score of the opponent piece:
    int not_known_opp = this->unknownOpponentPieces.size();
    // --> if we have more unknown than knwon- it's good because there are higher chances it's a flag
    int opp_score = this->myPieces.find(FLAG)->second*5  + not_known_opp*20 - (this->getOpponentMovingPieces() - not_known_opp)*10;

    int inf_min = std::numeric_limits<int>::min();

    int max_score = inf_min;

    // now iterate over my pieces:
    for (const unique_ptr<RPSPoint>& myPiece : this->myMovingPieces) {
        vector<RPSMove> movesForPiece;
        getLegalMoves(*myPiece, movesForPiece);
        char pieceType = this->getPieceFromBoard(*myPiece);
        for (RPSMove cur_move : movesForPiece) {
            int scoreMove = this->getScoreForMove(cur_move, pieceType);
            if (scoreMove > max_score) {
                max_score = scoreMove;
                bestMove = cur_move;
            }
        }
    }
    return bestMove;
}

void RPSPlayerAuto::getLegalMoves(RPSPoint piece_point, std::vector<RPSMove> &vectorToFill) {
    // UP
    RPSPoint point_up(piece_point.getX() + 1, piece_point.getY());
    if (this->checkValidMove(point_up)) {
        RPSMove move_up(RPSPoint(piece_point.getX(), piece_point.getY()), point_up);
        vectorToFill.push_back(move_up);
    }
    // RIGHT
    RPSPoint point_right(piece_point.getX(), piece_point.getY() + 1);
    if (this->checkValidMove(point_right)) {
        RPSMove move_right(RPSPoint(piece_point.getX(), piece_point.getY()), point_right);
    }
    // DOWN
    RPSPoint point_down(piece_point.getX() - 1, piece_point.getY());
    if (this->checkValidMove(point_down)) {
        RPSMove move_right(RPSPoint(piece_point.getX(), piece_point.getY()), point_down);
    }
    // LEFT
    RPSPoint point_left(piece_point.getX(), piece_point.getY() - 1);
    if (this->checkValidMove(point_left)) {
        RPSMove move_right(RPSPoint(piece_point.getX(), piece_point.getY()), point_left);
    }
}


bool RPSPlayerAuto::checkValidMove(RPSPoint point) {
    return point.getX() >= 0 && point.getX() < BOARD_ROWS && point.getY() >= 0 && point.getY() < BOARD_COLS;
}

int RPSPlayerAuto::getScoreForMove(RPSMove move , char myPiece) {
    int score = 0;
    int inf_min = std::numeric_limits<int>::min();
    // first check if there is a piece there
    if (this->m_my_board[move.getTo().getX()][move.getTo().getY()] == nullptr) {
        score += 100;
    } else {
        int player_to = this->m_my_board[move.getTo().getX()][move.getTo().getY()]->getNumPlayer();
        // first check if there is a piece of ours there
        if (this->m_num_player == player_to) {
            score += inf_min;
            return score;
        }
        // create fight
        char oppPiece = this->m_my_board[move.getTo().getX()][move.getTo().getY()]->getPiece();
        RPSFight fight(myPiece, oppPiece, RPSPoint(move.getTo().getX(), move.getTo().getY()), this->m_num_player);
        if (fight.getWinner() == this->m_num_player) {
            // 2 cases - if the number of my pieces is less than 1/4 of the pieces - not attack (it might be joker)
            if (this->getNumMovingPieces() < 0.5*NUM_OF_PIECES) {
                score += 10;
            } else {
                score += 10000;
            }
        } else { // wer'e losing or it's a tie
            score += inf_min;
        }
    }
    return score;
}
unique_ptr<JokerChange> RPSPlayerAuto::getJokerChange() {
    return unique_ptr<JokerChange>();
}



void RPSPlayerAuto::updateNumPiece(char piece) {
    switch (piece) {
        case ROCK:
            this->myPieces[ROCK]++;
        case PAPER:
            this->myPieces[PAPER]++;
        case SCISSOR:
            this->myPieces[SCISSOR]++;
        case BOMB:
            this->myPieces[BOMB]++;
        case JOKER:
            this->myPieces[JOKER]++;
        case FLAG:
            this->myPieces[FLAG]++;
    }
}
char RPSPlayerAuto::getPieceFromBoard(Point& pos){
    return this->m_my_board[pos.getX()][pos.getY()]->getPiece();
}

//bool isPointEqual(RPSPoint a, RPSPoint b) {
//    return (a.getX() == b.getX()) && (a.getY() == b.getY());
//}

void RPSPlayerAuto::removeFromVector(int type_vector, const RPSPoint& pos) {
    // type_vector == 1 -> my moving pieces, 2 -> unknown
    if (type_vector == 1) {
        // remove from vector with lambda function
        remove_if(this->myMovingPieces.begin(), this->myMovingPieces.end(), [pos] (const unique_ptr<RPSPoint> &cur_pos) {
            return (pos.getX() == cur_pos->getX()) && (pos.getY() == cur_pos->getY());
        });
    } else if (type_vector == 2) {
        remove_if(this->unknownOpponentPieces.begin(), this->unknownOpponentPieces.end(), [pos] (const unique_ptr<RPSPoint> &cur_pos) {
            return (pos.getX() == cur_pos->getX()) && (pos.getY() == cur_pos->getY());
        });
    }
}