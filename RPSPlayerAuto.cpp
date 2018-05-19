#include "RPSPlayerAuto.h"

using namespace std;

void RPSPlayerAuto::printBoardToCout() {
    cout << "BOARD OF PLAYER: " << this->m_num_player << endl;
    for (int r = BOARD_ROWS-1; r >= 0 ; r--) {
        // Draw dashed line
        cout << "--+";
        for (int c = 0; c < BOARD_COLS; c++)
            cout << "---+";
        cout << "\n";

        // Draw board contents
        cout << r << " | ";
        for (int c = 0; c < BOARD_COLS; c++)
            if (this->m_my_board[r][c] == nullptr)
                cout << " " << " | ";
            else if (this->m_my_board[r][c]->getNumPlayer() == PLAYER_1) //Capital letters
                cout << this->m_my_board[r][c]->getPiece() << " | ";
            else
                cout << (char) std::tolower(this->m_my_board[r][c]->getPiece()) << " | ";
        cout << "\n";
    }
    // Draw dashed line
    cout << "--+";
    for (int c = 0; c < BOARD_COLS; c++)
        cout << "---+";
    cout << "\n";

    // Draw column numbers
    cout << "    ";
    for (int c = 0; c < BOARD_COLS; c++)
        cout << c << "   ";
    cout << "\n\n";
    cout << "here" << endl;
}
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
void RPSPlayerAuto::setOppPieceInBoard(char piece, bool is_joker, int row, int col) {

    RPSPoint piece_loc(col, row);
    //this->m_my_board[row][col] = new RPSPiece(piece, is_joker, this->m_num_player, piece_loc);
    //TODO: change to shared_ptr
    int opponentPlayer = this->m_num_player == 1 ? 2 : 1;
    this->m_my_board[row][col] = make_shared<RPSPiece>(piece, is_joker, opponentPlayer, piece_loc);
}

void RPSPlayerAuto::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {

    //set the player number

//    cout << "num player :" << this->m_num_player <<endl;
//    this->m_num_player = player;
    int total_pieces = NUM_OF_BOMBS+NUM_OF_PAPERS+NUM_OF_FLAGS+NUM_OF_JOKERS+NUM_OF_ROCKS+NUM_OF_SCISSORS-2;
    if (player == PLAYER_1) {
        // set the flag on the board - left upper point
        setPieceOnBoard(FLAG, false, BOARD_COLS-1,0);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, this->m_num_player, RPSPoint(0,BOARD_COLS-1))));
        updateNumPiece(FLAG);
        // set the bombs around
        setPieceOnBoard(BOMB, false,  BOARD_COLS-1,1);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, this->m_num_player, RPSPoint(1,BOARD_COLS-1))));
        updateNumPiece(BOMB);

        if (NUM_OF_BOMBS > 1) {
            setPieceOnBoard(BOMB, false, BOARD_COLS-2, 0);
            vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, this->m_num_player, RPSPoint(0,BOARD_COLS-2))));
            updateNumPiece(BOMB);
            total_pieces--;
        }
    } else if (player == PLAYER_2) {
        // set the flag on the board - left upper point
        setPieceOnBoard(FLAG, false,  0, BOARD_ROWS-1);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, this->m_num_player, RPSPoint(BOARD_ROWS-1, 0))));
        updateNumPiece(FLAG);
        // set the bombs around
        setPieceOnBoard(BOMB, false, 0, BOARD_ROWS-2);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, this->m_num_player, RPSPoint(BOARD_ROWS-2, 0))));
        updateNumPiece(BOMB);

        if (NUM_OF_BOMBS > 1) {
            setPieceOnBoard(BOMB, false, 1, BOARD_ROWS-1);
            vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, this->m_num_player, RPSPoint(BOARD_ROWS-1, 1))));
            updateNumPiece(BOMB);
            total_pieces--;
        }
    }

    // set the other pieces
    vector<pair<int, int>> coords;
    vector<pair<int, int>> coordsPositions(total_pieces);

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if ((this->m_num_player == PLAYER_1) &&
                ((i == 0 && j == BOARD_COLS - 1) || (i == 1 && j == BOARD_COLS - 1) || (i == 0 && j == BOARD_COLS - 2))){
                continue;
            }

            else if ((this->m_num_player == PLAYER_2) &&
                     ((i == BOARD_ROWS - 1 && j == 0) || (i == BOARD_ROWS - 2 && j == 0) || (i == BOARD_ROWS - 1 && j == 1))) {
                continue;
            }
            else {
                coords.push_back(make_pair(i, j));
            }
        }
    }


    //shuffle the coordinates
    //std::srand(std::time(0));
    auto rng = std::default_random_engine {};
    //random_shuffle(coords.begin(), coords.end());
    shuffle(begin(coords), end(coords), rng);
    copy(coords.begin(), coords.begin()+total_pieces, coordsPositions.begin());
    for (int i=0; i<coordsPositions.size(); i++) {
        cout << coordsPositions[i].first << "," << coordsPositions[i].second << endl;
    }
    //for each piece- get the next coordinate and set it on board
    int index = 0;
    while (this->myPieces[PAPER] < NUM_OF_PAPERS) {
        pair<int, int> pos = coordsPositions[index];
        int row = pos.second;
        int col = pos.first;
        setPieceOnBoard(PAPER, false , row, col);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(PAPER, false, this->m_num_player, RPSPoint(col,row))));
        updateNumPiece(PAPER);
        this->myMovingPieces.push_back(make_unique<RPSPoint> (col,row));
        index++;

    }
    while (this->myPieces[SCISSOR] < NUM_OF_SCISSORS) {
        pair<int, int> pos = coordsPositions[index];
        int row = pos.second;
        int col = pos.first;
        setPieceOnBoard(SCISSOR, false ,row, col);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(SCISSOR, false, this->m_num_player, RPSPoint(col, row))));
        updateNumPiece(SCISSOR);
        this->myMovingPieces.push_back(make_unique<RPSPoint> (col, row));
        index++;
    }

    while (this->myPieces[ROCK] < NUM_OF_ROCKS) {
        pair<int, int> pos = coordsPositions[index];
        int row = pos.second;
        int col = pos.first;
        setPieceOnBoard(ROCK, false , row, col);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(ROCK, false, this->m_num_player, RPSPoint(col, row))));
        updateNumPiece(ROCK);
        this->myMovingPieces.push_back(make_unique<RPSPoint> (col, row));
        index++;
    }
    while (this->myPieces[BOMB] < NUM_OF_BOMBS) {
        pair<int, int> pos = coordsPositions[index];
        int row = pos.second;
        int col = pos.first;
        setPieceOnBoard(BOMB, false , row, col);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(BOMB, false, this->m_num_player, RPSPoint(col, row))));
        updateNumPiece(BOMB);
        index++;
    }
    while (this->myPieces[FLAG] < NUM_OF_FLAGS) {
        pair<int, int> pos = coordsPositions[index];
        int row = pos.second;
        int col = pos.first;
        setPieceOnBoard(FLAG, false ,row, col);
        vectorToFill.push_back(std::move(make_unique<RPSPiece>(FLAG, false, this->m_num_player, RPSPoint(col, row))));
        updateNumPiece(FLAG);
        index++;
    }
//    for(auto elem : this->myPieces)
//    {
//        std::cout << elem.first << " " << elem.second << "\n";
//    }
}


void RPSPlayerAuto::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {
    // update fights result on our private board
    cout << "notify on init board" << endl;
    int opponentPlayer = this->m_num_player == 1 ? 2 : 1;
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            RPSPoint cur_point(j,i);
            if (b.getPlayer(cur_point) == opponentPlayer) {
                setOppPieceInBoard(UNKNOWN_PIECE, false, i, j);
                this->m_opponent_total++;
                this->unknownOpponentPieces.push_back(make_unique<RPSPoint>(j,i));
            }
        }
    }
    for (size_t i = 0; i < fights.size(); i++) {
        // get iterator
        auto fight = fights[i].get();
        this->notifyFightResult(*fight);
    }
    this->printBoardToCout();
}


void RPSPlayerAuto::notifyOnOpponentMove(const Move &move) {
    cout << "notify on opponent move! player:" << this->m_num_player << "move:" << move.getFrom().getY() << "," << move.getFrom().getX() << "-->" << move.getTo().getY()<<","<< move.getTo().getX() << endl;
    // update our board accordingly
    int from_row = move.getFrom().getY();
    int from_col = move.getFrom().getX();
    int to_row = move.getTo().getY();
    int to_col = move.getTo().getX();
    const RPSPoint move_to(to_col, to_row);
    RPSPoint move_from(from_col, from_row);
    char piece_before = this->getPieceFromBoard(move_from);
    this->m_my_board[from_row][from_col] = nullptr;
    // remove form unknown
    removeFromVector(2, move_from);
    // set the move on out
    char piece_now = piece_before == UNKNOWN_PIECE ? NEUTRAL_CHAR : piece_before;
    setOppPieceInBoard(piece_now, false, to_row , to_col);
}

void RPSPlayerAuto::notifyFightResult(const FightInfo &fightInfo) {
    cout << "NOTIFY on fight result in player "  << this->m_num_player<<endl;
    int opponentPlayer = this->m_num_player == 1 ? 2 : 1;
    char oppPiece = fightInfo.getPiece(opponentPlayer);
    //Point fight_place = fight->getPosition();
    int fight_row = fightInfo.getPosition().getY();
    int fight_col = fightInfo.getPosition().getX();
    RPSPoint fight_place(fight_col, fight_row);
    cout << "PLAYER: " <<this->m_num_player <<"FIGHT PLACE : row(y)" << fight_place.getY() << " , col(x)" << fight_place.getX() <<endl;
    // create RPSFight
    RPSFight fight(fightInfo.getPiece(this->m_num_player), oppPiece, fight_place, this->m_num_player);
    cout << "fight winner :" << fightInfo.getWinner() << endl;
    if (fightInfo.getWinner() == this->m_num_player) { //current player won
        cout << "In current player won PLAYER: " <<this->m_num_player << endl;
        cout << "my piece: " << fightInfo.getPiece(this->m_num_player)<<endl;
        // update position on the shared board
        //this->m_my_board[fight_place.getX()][fight_place.getY()] = make_shared<RPSPiece>(fightInfo.getPiece(this->m_num_player), false, this->m_num_player, RPSPoint(fight_place.getX(), fight_place.getY()));
        //setPieceOnBoard(fightInfo.getPiece(this->m_num_player),false, )
        this->setPieceOnBoard(fightInfo.getPiece(this->m_num_player), false, fight_row, fight_col);
        // update saved information
        this->m_opponent_total--;
        this->opponentPieces[oppPiece]--;
        // remove form unknown
        removeFromVector(2, fight_place);
        cout << "NOW:" << endl;
        this->printBoardToCout();

    } else if (fightInfo.getWinner() == opponentPlayer) { //opponent player won
        this->myPieces[fightInfo.getPiece(this->m_num_player)]--;
        // update position on the shared board
        //this->m_my_board[fight_place.getX()][fight_place.getY()] = make_shared<RPSPiece>(fightInfo.getPiece(opponentPlayer), false, opponentPlayer, RPSPoint(fight_place.getX(), fight_place.getY()));
        setOppPieceInBoard(oppPiece,  false, fight_row, fight_col);
        // remove form unknown
        removeFromVector(2, fight_place);
        // remove from moving vector
        removeFromVector(1, fight_place);
        cout << this->m_num_player << " Delete from moving vector:" << fight_place.getY() << " , " << fight_place.getX() <<endl;
        cout << " MOVING PIECES for PLAYER: " << this->m_num_player << endl;
        for(const unique_ptr<RPSPoint>& pos : this->myMovingPieces) {
            std::cout << pos->getX() << " " << pos->getY() << "\n";
        }

    } else if(fightInfo.getWinner() == TIE) {
        cout << "TIE" << endl;
        this->myPieces[fightInfo.getPiece(this->m_num_player)]--;
        this->opponentPieces[oppPiece]--;
        this->m_opponent_total--;
        // update position on the shared board
        this->m_my_board[fight_row][fight_col] = nullptr;
        // update saved information
        // remove form unknown
        removeFromVector(2, fight_place);
        // remove from known vector
        removeFromVector(1, fight_place);
    }
}
// update the shared board- the loser is the current player (this->m_num_player)
void RPSPlayerAuto::updateSharedBoard(RPSMove move, RPSPoint& fight_place) {

}

unique_ptr<Move> RPSPlayerAuto::getMove() {
    // RPSMove move(RPSPoint(-1,-1), RPSPoint(-1,-1));
    RPSMove move = this->getBestMove();
    int from_row = move.getFrom().getY();
    int from_col = move.getFrom().getX();
    int to_row = move.getTo().getY();
    int to_col = move.getTo().getX();
    unique_ptr<Move> move_ptr = make_unique<RPSMove>(RPSPoint(from_col, from_row), RPSPoint(to_col, to_row));
    this->addToMovingVector(move);
    // update my board
    cout << "BEFORE move board:" << endl;
    this->printBoardToCout();
    RPSPoint src_point(from_col, from_row);
    char piece = getPieceFromBoard(src_point);
    this->m_my_board[from_row][from_col] = nullptr;
    setPieceOnBoard(piece, false, to_row, to_col);
    cout << "AFTER move board:" << endl;
    this->printBoardToCout();
    cout << this->m_my_board[to_row][to_col]->getNumPlayer() << endl;
    return move_ptr;
}

RPSMove RPSPlayerAuto::getBestMove() {
    RPSMove bestMove(RPSPoint(-1,-1), RPSPoint(-1,-1));
    // calculate score of the opponent piece:
    int not_known_opp = this->unknownOpponentPieces.size();
    // --> if we have more unknown than knwon- it's good because there are higher chances it's a flag
    int opp_score = this->myPieces.find(FLAG)->second*5  + not_known_opp*20 - (this->getOpponentMovingPieces() - not_known_opp)*10;

    int inf_min = std::numeric_limits<int>::min();

    int max_score = inf_min;
    this->printBoardToCout();
    cout << "MY MOVING PIECES PLAYER: " << this->m_num_player << endl;
    for(const unique_ptr<RPSPoint>& pos : this->myMovingPieces) {
        std::cout << pos->getY() << " " << pos->getX() << "\n";
    }

    // now iterate over my pieces:
    for (const unique_ptr<RPSPoint>& myPiece : this->myMovingPieces) {
        vector<RPSMove> movesForPiece;
        getLegalMoves(*myPiece, movesForPiece);
        char pieceType = this->getPieceFromBoard(*myPiece);
        cout << "for moving piece in " << pieceType << " row:" << myPiece->getY() << " col: " << myPiece->getX() << endl;
        cout << "num of valid moves:" << movesForPiece.size() << endl;
        for (RPSMove cur_move : movesForPiece) {
            cout << " Current suggested move: " << cur_move.getFrom().getY() << "," <<cur_move.getFrom().getX() << "-->" << cur_move.getTo().getY()<<","<<cur_move.getTo().getX()<<endl;
            int scoreMove = this->getScoreForMove(cur_move, pieceType);
            cout << "The score for this move:" << scoreMove << endl;
            if (scoreMove > max_score) {
                max_score = scoreMove;
                bestMove = cur_move;
                cout << "========> NOW BEST:" << bestMove.getFrom().getY() << "," <<bestMove.getFrom().getX() << "-->" << bestMove.getTo().getY()<<","<<bestMove.getTo().getX() << endl;
            }
        }
    }
    return bestMove;
}

void RPSPlayerAuto::getLegalMoves(RPSPoint piece_point, std::vector<RPSMove> &vectorToFill) {
    int row = piece_point.getY();
    int col = piece_point.getX();
    // UP
    RPSPoint point_up(col + 1, row);
    if (this->checkValidMove(point_up)) {
        RPSMove move_up(RPSPoint(col, row), point_up);
        vectorToFill.push_back(move_up);
    }
    // RIGHT
    RPSPoint point_right(col , row + 1);
    if (this->checkValidMove(point_right)) {
        RPSMove move_right(RPSPoint(col, row), point_right);
        vectorToFill.push_back(move_right);
    }
    // DOWN
    RPSPoint point_down(col - 1, row);
    if (this->checkValidMove(point_down)) {
        RPSMove move_down(RPSPoint(col, row) , point_down);
        vectorToFill.push_back(move_down);
    }
    // LEFT
    RPSPoint point_left(col, row - 1);
    if (this->checkValidMove(point_left)) {
        RPSMove move_left(RPSPoint(col, row) , point_left);
        vectorToFill.push_back(move_left);
    }
}


bool RPSPlayerAuto::checkValidMove(RPSPoint point) {
    int col = point.getX();
    int row = point.getY();
    if (row >= 0 && row < BOARD_ROWS && col >= 0 && col < BOARD_COLS) {
        if (this->m_my_board[row][col] != nullptr) {
            int player_to = this->m_my_board[row][col]->getNumPlayer();
            // first check if there is a piece of ours there
            cout << "for point: " << point.getY() << " , " << point.getX() << endl;
            cout << "my player: " << this->m_num_player << " player_to:" << player_to << endl;
            if (this->m_num_player == player_to) {
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}

int RPSPlayerAuto::getScoreForMove(RPSMove move , char myPiece) {
    int score = 0;
    int from_row = move.getFrom().getY();
    int from_col = move.getFrom().getX();
    int to_row = move.getTo().getY();
    int to_col = move.getTo().getX();

    int opponentPlayer = this->m_num_player == 1 ? 2 : 1;
    int inf_min = std::numeric_limits<int>::min();

    // first check if there is a piece there
    if (this->m_my_board[to_row][to_col] == nullptr) {
        score += 5;
    } else {
//        int player_to = this->m_my_board[move.getTo().getX()][move.getTo().getY()]->getNumPlayer();
//        // first check if there is a piece of ours there
//        if (this->m_num_player == player_to) {
//            score += inf_min;
//            cout << "****************NOT SOPPOSE to be here!" << endl;
//            return score;
//        }
        // create fight
        char oppPiece = this->m_my_board[to_row][to_col]->getPiece();
        if (oppPiece == '#') {
            cout << "OPP PIECE (scoring) :" << oppPiece << endl;
            score += 10;
            return score;
        }
        RPSFight fight(myPiece, oppPiece, RPSPoint(to_col,to_row), this->m_num_player);
        if (fight.getWinner() == this->m_num_player) {
            // 2 cases - if the number of my pieces is less than 1/4 of the pieces - not attack (it might be joker)
            if (this->getNumMovingPieces() < 0.5*NUM_OF_PIECES) {
                score += 5000;
            } else {
                score += 10000;
            }
        } else if (fight.getWinner() == opponentPlayer || fight.getWinner() == TIE){ // wer'e losing or it's a tie
            score += -500;
        } else {
            score += 50;
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
            break;
        case PAPER:
            this->myPieces[PAPER]++;
            break;
        case SCISSOR:
            this->myPieces[SCISSOR]++;
            break;
        case BOMB:;
            this->myPieces[BOMB]++;
            break;
        case JOKER:
            this->myPieces[JOKER]++;
            break;
        case FLAG:
            this->myPieces[FLAG]++;
            break;

    }
}
char RPSPlayerAuto::getPieceFromBoard(Point& pos){
    //this->printBoardToCout();
    return this->m_my_board[pos.getY()][pos.getX()]->getPiece();
}

//bool isPointEqual(RPSPoint a, RPSPoint b) {
//    return (a.getX() == b.getX()) && (a.getY() == b.getY());
//}

void RPSPlayerAuto::removeFromVector(int type_vector, const RPSPoint& pos) {
    // type_vector == 1 -> my moving pieces, 2 -> unknown
    if (type_vector == 1) {
        // remove from vector with lambda function
        auto new_vec = remove_if(this->myMovingPieces.begin(), this->myMovingPieces.end(), [pos] (const unique_ptr<RPSPoint> &cur_pos) {
            return (pos.getX() == cur_pos->getX()) && (pos.getY() == cur_pos->getY());
        });
        this->myMovingPieces.erase(new_vec, this->myMovingPieces.end());
    } else if (type_vector == 2) {
        auto new_vec = remove_if(this->unknownOpponentPieces.begin(), this->unknownOpponentPieces.end(), [pos] (const unique_ptr<RPSPoint> &cur_pos) {
            return (pos.getX() == cur_pos->getX()) && (pos.getY() == cur_pos->getY());
        });
        this->unknownOpponentPieces.erase(new_vec, this->unknownOpponentPieces.end());
    }
}
void RPSPlayerAuto::addToMovingVector(const RPSMove& move) {
    int x = move.getFrom().getX();
    int y = move.getFrom().getY();
    cout << "MOVE FROM POINT: Y(row): " << y << " X(col): " << x << endl;
    cout << "MOVE FROM POINT: Y(row): " << move.getTo().getY() << " X(col): " << move.getTo().getX() << endl;
    RPSPoint src_point(x,y);
    for (unique_ptr<RPSPoint>& point : this->myMovingPieces) {
        if (point->getX() == x && point->getY() == y) {
            this->removeFromVector(1, src_point);
            this->myMovingPieces.push_back(make_unique<RPSPoint> (move.getTo().getX(), move.getTo().getY()));
            cout << "MOVING PIECES NOW for player " << this->m_num_player << endl;
            for(const unique_ptr<RPSPoint>& pos : this->myMovingPieces) {
                std::cout << pos->getY() << " " << pos->getX() << "\n";
            }
            break;
        }
    }
    this->printBoardToCout();


}