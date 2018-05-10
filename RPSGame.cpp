#include "RPSGame.h"

void RPSGame::initGame() {

    //ask from players their init pos
    vector<unique_ptr<PiecePosition>> pos_player_1;
    this->m_algo_player1->getInitialPositions(PLAYER_1, pos_player_1);

    vector<unique_ptr<PiecePosition>> pos_player_2;
    this->m_algo_player2->getInitialPositions(PLAYER_2, pos_player_2);

    //check players' pos validity
    RPSPosChecker checker_player1;
    RPSPosChecker checker_player2;
    checker_player1.checkInitPos(pos_player_1);
    checker_player2.checkInitPos(pos_player_2);

    bool is_piece_joker;
    char real_piece_type;
    int row, col;

    if (checker_player1.getPosMsg() != POS_SUCCESS &&
        checker_player2.getPosMsg() != POS_SUCCESS) { //both players with invalid position files
        this->m_reason_winner = BAD_POS_BOTH_PLAYERS;
        this->m_winner = TIE;
        this->m_game_over = true;
        this->m_bad_input_index1 = checker_player1.getInvalidNumPiece();
        this->m_bad_input_index2 = checker_player2.getInvalidNumPiece();
        return;
    }
    if (checker_player1.getPosMsg() != POS_SUCCESS &&
        checker_player2.getPosMsg() == POS_SUCCESS) { //only player 1 with invalid position
        this->m_reason_winner = BAD_POS_BOTH_PLAYERS;
        this->m_winner = TIE;
        this->m_game_over = true;
        this->m_bad_input_index1 = checker_player1.getInvalidNumPiece();
        return;
    }
    if (checker_player1.getPosMsg() == POS_SUCCESS &&
        checker_player2.getPosMsg() != POS_SUCCESS) { //only player 2 with invalid position
        this->m_reason_winner = BAD_POS_PLAYER_TWO;
        this->m_winner = PLAYER_1;
        this->m_game_over = true;
        this->m_bad_input_index1 = checker_player2.getInvalidNumPiece();
        return;
    }

    //both players with valid position. Place player1 pieces on board, then iterate player2' pieces, place his pieces
    //and manage a fight if needed.

    this->m_num_flags_player1 = checker_player1.getNumOfFlags();
    this->m_num_flags_player2 = checker_player2.getNumOfFlags();
    this->m_num_moving_pieces_player1 = checker_player1.getNumMovingPieces();
    this->m_num_moving_pieces_player2 = checker_player2.getNumMovingPieces();

    std::vector<unique_ptr<FightInfo>> fights_pos;
    for (vector<unique_ptr<PiecePosition>>::iterator it = pos_player_1.begin(); it != pos_player_1.end(); ++it) {

        row = (*it)->getPosition().getY();
        col = (*it)->getPosition().getX();

        RPSPoint pos(col, row);
        is_piece_joker = (*it)->getPiece() == JOKER;
        real_piece_type = is_piece_joker ? (*it)->getJokerRep() : (*it)->getPiece();

        shared_ptr<RPSPiece> my_piece_p1 = make_shared<RPSPiece>(real_piece_type, is_piece_joker, PLAYER_1, pos);
        this->m_board.m_game_board[row][col] = my_piece_p1;
    }
    for (vector<unique_ptr<PiecePosition>>::iterator it = pos_player_2.begin(); it != pos_player_2.end(); ++it) {

        row = (*it)->getPosition().getY();
        col = (*it)->getPosition().getX();

        RPSPoint pos(col, row);
        is_piece_joker = (*it)->getPiece() == JOKER;
        real_piece_type = is_piece_joker ? (*it)->getJokerRep() : (*it)->getPiece();

        shared_ptr<RPSPiece> my_piece_p2 = make_shared<RPSPiece>(real_piece_type, is_piece_joker, PLAYER_2, pos);

        auto piece_p1 = this->m_board.m_game_board[row][col];
        if (piece_p1 == nullptr) //the curr entrance is empty and can set player 2 in it
            this->m_board.m_game_board[row][col] = my_piece_p2;
        else { //the curr entrance isn't empty, need to make a fight

            unique_ptr<FightInfo> fight = make_unique<RPSFight>(piece_p1->m_symbol, real_piece_type, pos, PLAYER_1);
            updateGameAfterFight(piece_p1, my_piece_p2, pos);
            fights_pos.push_back(std::move(fight));
        }
    }
    checkGameStatusAfterInit();
    if (this->m_game_over)
        return;

    //notify players pos level has ended
    this->m_algo_player1->notifyOnInitialBoard(this->m_board, fights_pos);
    this->m_algo_player2->notifyOnInitialBoard(this->m_board, fights_pos);
}

void RPSGame::checkGameStatusAfterInit() {

    //check flags
    if (this->m_num_flags_player1 == 0 && this->m_num_flags_player2 > 0) {
        this->m_winner = PLAYER_2;
        this->m_reason_winner = ALL_FLAG_CAPTURED_PLAYER_TWO;
        this->m_game_over = true;
        return;
    }

    if (this->m_num_flags_player1 > 0 && this->m_num_flags_player2 == 0) {
        this->m_winner = PLAYER_1;
        this->m_reason_winner = ALL_FLAG_CAPTURED_PLAYER_ONE;
        this->m_game_over = true;
        return;
    }

    //check if after init, there is a tie - all flags of both players are eaten
    if (this->m_num_flags_player1 == 0 && this->m_num_flags_player2 == 0) {
        this->m_reason_winner = ALL_FLAGS_EATEN_IN_POS;
        this->m_winner = 0;
        this->m_game_over = true;
        return;
    }

    //check if after init, there is a tie - all moving pieces of both pieces are eaten
    if (this->m_num_moving_pieces_player1 == 0 && this->m_num_moving_pieces_player2 == 0) {
        this->m_reason_winner = ALL_PIECES_EATEN_IN_POS;
        this->m_winner = 0;
        this->m_game_over = true;
        return;
    }

}


void RPSGame::checkGameStatus() {


    //check flags
    if (this->m_num_flags_player1 == 0 && this->m_num_flags_player2 > 0) {
        this->m_winner = PLAYER_2;
        this->m_reason_winner = ALL_FLAG_CAPTURED_PLAYER_TWO;
        this->m_game_over = true;
        return;

    }
    if (this->m_num_flags_player1 > 0 && this->m_num_flags_player2 == 0) {
        this->m_winner = PLAYER_1;
        this->m_reason_winner = ALL_FLAG_CAPTURED_PLAYER_ONE;
        this->m_game_over = true;
        return;
    }

    // /check pieces
    if (this->m_num_moving_pieces_player1 == 0 && this->m_num_moving_pieces_player2 > 0) {
        this->m_winner = PLAYER_2;
        this->m_reason_winner = ALL_PIECES_EATEN_PLAYER_ONE;
        this->m_game_over = true;
        return;
    }

    if (this->m_num_moving_pieces_player1 > 0 && this->m_num_moving_pieces_player2 == 0) {
        this->m_winner = PLAYER_1;
        this->m_reason_winner = ALL_PIECES_EATEN_PLAYER_TWO;
        this->m_game_over = true;
        return;
    }

}

void RPSGame::updateGameAfterFight(shared_ptr<RPSPiece> piece_curr_player, shared_ptr<RPSPiece> piece_opp_player,
                                   RPSPoint fight_pos) {

    int row = fight_pos.getY();
    int col = fight_pos.getX();

    RPSFight curr_fight(piece_curr_player->m_symbol, piece_opp_player->m_symbol, fight_pos, this->m_current_player);
    FightResult res = curr_fight.manageFight();

    bool player_1_current = (this->m_current_player == PLAYER_1);

    if (res == SAME_PIECE_FIGHT) {
        this->m_board.m_game_board[row][col] = nullptr;
        if (piece_curr_player->m_symbol == FLAG) {
            this->m_num_flags_player1--;
            this->m_num_flags_player2--;
        } else {
            if (piece_curr_player->m_symbol != BOMB) {
                this->m_num_moving_pieces_player1--;
                this->m_num_moving_pieces_player2--;
            }
        }
        return;
    }

    if (res == BOMB_OPP_PLAYER) {
        this->m_board.m_game_board[row][col] = nullptr;
        if (piece_curr_player->m_symbol == FLAG) {
            if (player_1_current)
                this->m_num_flags_player1--;
            else
                this->m_num_flags_player2--;
        } else {
            if (piece_curr_player->m_symbol != BOMB) { //moving piece (not a flag)
                if (player_1_current)
                    this->m_num_moving_pieces_player1--;
                else
                    this->m_num_moving_pieces_player2--;
            }
        }
        return;
    }

    if (res == WIN_CURR_PLAYER) {
        piece_curr_player->m_curr_pos.m_x = col;
        piece_curr_player->m_curr_pos.m_y = row;
        this->m_board.m_game_board[row][col] = piece_curr_player;
        if (piece_opp_player->m_symbol == FLAG) {
            if (player_1_current)
                this->m_num_flags_player2--;
            else
                this->m_num_flags_player1--;
        } else {

            if (piece_opp_player->m_symbol != BOMB) { //moving piece (not a flag)
                if (player_1_current)
                    this->m_num_moving_pieces_player2--;
                else
                    this->m_num_moving_pieces_player1--;
            }
        }
        return;
    }

    if (res == WIN_OPP_PLAYER) {
        piece_opp_player->m_curr_pos.m_x = col;
        piece_opp_player->m_curr_pos.m_y = row;
        this->m_board.m_game_board[row][col] = piece_opp_player;
        if (piece_curr_player->m_symbol == FLAG) {
            if (player_1_current)
                this->m_num_flags_player1--;
            else
                this->m_num_flags_player2--;
        } else {
            if (piece_curr_player->m_symbol != BOMB) { //moving piece (not a flag)
                if (player_1_current)
                    this->m_num_moving_pieces_player1--;
                else
                    this->m_num_moving_pieces_player2--;
            }
        }
        return;
    }

    if (res == FLAG_EATEN_OPP_PLAYER) {
        if (piece_curr_player->m_symbol == BOMB)
            this->m_board.m_game_board[row][col] = nullptr;
        else {
            piece_curr_player->m_curr_pos.m_x = col;
            piece_curr_player->m_curr_pos.m_y = row;
            this->m_board.m_game_board[row][col] = piece_curr_player;
        }
        if (player_1_current)
            this->m_num_flags_player2--;
        else
            this->m_num_flags_player1--;

    }
}

bool RPSGame::isInBoard(int row, int col) const {
    return row >= 0 && row < BOARD_ROWS && col >= 0 && col < BOARD_COLS;
}

bool RPSGame::isMoveOnBoard(int src_row, int src_col, int dst_row, int dst_col) {
    return isInBoard(src_row, src_col) && isInBoard(dst_row, dst_col);
}

bool RPSGame::isPieceAbleToMove(char piece) {

    if (piece == ROCK || piece == PAPER || piece == SCISSOR)
        return true;

    return false;
}

bool RPSGame::isSamePieceAsCurrent(int row, int col) {

    shared_ptr<RPSPiece> piece = this->m_board.m_game_board[row][col];
    if (piece == nullptr)
        return false;
    return (piece->m_num_player == this->m_current_player);
}

bool RPSGame::isJokerNewRepValid(char new_rep) {

    if (new_rep == ROCK || new_rep == PAPER || new_rep == SCISSOR || new_rep == BOMB)
        return true;

    return false;
}

bool RPSGame::isPieceAJoker(int row, int col) {

    shared_ptr<RPSPiece> piece = this->m_board.m_game_board[row][col];
    if (piece == nullptr)
        return false;
    return (piece->m_is_joker == true);
}

bool RPSGame::playRegularMove(int src_row, int src_col, int dst_row, int dst_col) {

    bool player_1_current = this->m_current_player == PLAYER_1;

    //check if the indices are legal
    if (!isMoveOnBoard(src_row, src_col, dst_row, dst_col)) {
        cout << NOT_IN_RANGE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return false;
    }

    //check if there is a piece at current location
    if (!isPieceExist(src_row, src_col)) {
        cout << PIECE_NOT_EXIST;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return false;
    }

    //checks if the piece at the src location belongs to current player
    if (!isSamePieceAsCurrent(src_row, src_col)) {
        cout << SRC_NO_CURR_PIECE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return false;
    }

    shared_ptr<RPSPiece> piece = this->m_board.m_game_board[src_row][src_col];

    //checks if the piece at src location able to move
    if (!isPieceAbleToMove(piece->m_symbol)) {
        cout << NOT_ABLE_TO_MOVE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return false;
    }

    //checks if the dst location doesn't contain same player piece
    if (isSamePieceAsCurrent(dst_row, dst_col)) {
        cout << DST_PIECE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return false;
    }

    //checks if the move itself is legal (according to rules of moving pieces)
    if (!isMoveLegal(src_row, src_col, dst_row, dst_col)) {
        cout << MOVE_NOT_LEGAL;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return false;
    }


    //the move is valid! thus we can play
    if (this->m_board.m_game_board[dst_row][dst_col] == nullptr) { //regular move
        this->m_board.m_game_board[dst_row][dst_col] = this->m_board.m_game_board[src_row][src_col];

        this->m_board.m_game_board[src_row][src_col] = nullptr;
    } else { //two pieces fight!

        RPSPoint fight_pos(dst_col, dst_row);
        updateGameAfterFight(this->m_board.m_game_board[src_row][src_col],
                             this->m_board.m_game_board[dst_row][dst_col], fight_pos);
        this->m_board.m_game_board[src_row][src_col] = nullptr;
        return true;
    }

    return false;
}


void RPSGame::playJokerMove(int joker_row, int joker_col, char new_rep) {

    bool player_1_current = this->m_current_player == PLAYER_1;

    //checks if the indices are inside the board
    if (!isInBoard(joker_row, joker_col)) {
        cout << NOT_IN_RANGE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //check if the exits a piece at src location
    if (!isPieceExist(joker_row, joker_col)) {
        cout << PIECE_NOT_EXIST;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //checks if the piece at the src location belongs to current player
    if (!isSamePieceAsCurrent(joker_row, joker_col)) {
        cout << SRC_NO_CURR_PIECE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //check if the piece at current location is a joker
    if (!isPieceAJoker(joker_row, joker_col)) {
        cout << NO_JOKER;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //checks if the new joker rep is valid
    if (!isJokerNewRepValid(new_rep)) {
        cout << NEW_REP_INVALID;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    char orig_rep = this->m_board.m_game_board[joker_row][joker_col]->m_symbol;
    //the joker move is valid!, thus we can change it
    this->m_board.m_game_board[joker_row][joker_col]->m_symbol = new_rep;

    //check if the joker rep has changed from piece that can move to a non one and vice versa, and set attribute accordingly
    if (orig_rep != BOMB && new_rep == BOMB) {
        if (player_1_current)
            this->m_num_moving_pieces_player1--;
        else
            this->m_num_moving_pieces_player2--;

    } else if (orig_rep == BOMB && new_rep != BOMB) {
        if (player_1_current)
            this->m_num_moving_pieces_player1++;
        else
            this->m_num_moving_pieces_player2++;
    }
}

void RPSGame::changeCurrentPlayer() {

    if (this->m_current_player == PLAYER_1)
        this->m_current_player = PLAYER_2;
    else
        this->m_current_player = PLAYER_1;
}

bool RPSGame::isPieceExist(int row, int col) {
    return (this->m_board.m_game_board[row][col] != nullptr);
}


void RPSGame::printReasonWinner() {

    this->m_output_file << "Reason: ";

    if (this->m_reason_winner == ALL_FLAG_CAPTURED_PLAYER_ONE ||
        this->m_reason_winner == ALL_FLAG_CAPTURED_PLAYER_TWO) {
        this->m_output_file << "All flags of the opponent are captured" << endl;
        return;
    }

    if (this->m_reason_winner == ALL_PIECES_EATEN_PLAYER_ONE || this->m_reason_winner == ALL_PIECES_EATEN_PLAYER_TWO) {
        this->m_output_file << "All moving PIECEs of the opponent are eaten" << endl;
        return;
    }

    if (this->m_reason_winner == ALL_FLAGS_EATEN_IN_POS) {
        this->m_output_file << "A tie - all flags are eaten by both players in the position files" << endl;
        return;
    }

    if (this->m_reason_winner == ALL_PIECES_EATEN_IN_POS) {
        this->m_output_file << "A tie - all moving pieces are eaten by both players in the position files" << endl;
        return;
    }

    if (this->m_reason_winner == BAD_POS_PLAYER_ONE) {
        this->m_output_file << "Bad Positioning input file for player 1 - line " << this->m_bad_input_index1 << endl;
        return;
    }

    if (this->m_reason_winner == BAD_POS_PLAYER_TWO) {
        this->m_output_file << "Bad Positioning input file for player 2 - line " << this->m_bad_input_index1 << endl;
        return;
    }

    if (this->m_reason_winner == BAD_POS_BOTH_PLAYERS) {
        this->m_output_file << "Bad Positioning input file for both players - player 1: line "
                            << this->m_bad_input_index1
                            << ", player 2: line " << this->m_bad_input_index2 << endl;
        return;
    }

    if (this->m_reason_winner == BAD_MOVE_PLAYER_ONE) {
        this->m_output_file << "Bad Moves input file for player 1 - line " << this->m_bad_input_index1 << endl;
        return;
    }

    if (this->m_reason_winner == BAD_MOVE_PLAYER_TWO) {
        this->m_output_file << "Bad Moves input file for player 2 - line " << this->m_bad_input_index1 << endl;
        return;
    }

    if (this->m_reason_winner == AFTER_MOVES_NO_WINNER) {
        this->m_output_file << "A tie - both Moves input files done without a winner" << endl;
        return;
    }

    if (this->m_reason_winner == MOVES_LIMIT_NO_FIGHT) {
        this->m_output_file << "A tie - no fight occurred for move than 100 turns" << endl;
        return;
    }
}

void RPSGame::endGame() {

    this->m_output_file << "Winner: " << this->m_winner << endl;
    this->printReasonWinner();
    this->m_output_file << endl; //empty line
    this->m_board.printBoard(this->m_output_file);
    this->m_output_file.close();

}

vector<pair<int, int>> RPSGame::getLegalLMovesForPiece(int row, int col) {
    vector<pair<int, int>> moves;
    pair<int, int> loc;

    //down direction
    if (isInBoard(row + 1, col)) {
        loc = std::make_pair(row + 1, col);
        moves.push_back(loc);
    }

    //up direction
    if (isInBoard(row - 1, col)) {
        loc = std::make_pair(row - 1, col);
        moves.push_back(loc);
    }

    //right direction
    if (isInBoard(row, col + 1)) {
        loc = std::make_pair(row, col + 1);
        moves.push_back(loc);
    }

    //left direction
    if (isInBoard(row, col - 1)) {
        loc = std::make_pair(row, col - 1);
        moves.push_back(loc);
    }

    return moves;
}

bool RPSGame::isMoveLegal(int src_row, int src_col, int dst_row, int dst_col) {
    vector<pair<int, int>> moves = getLegalLMovesForPiece(src_row, src_col);
    int size = moves.size();
    for (int i = 0; i < size; i++) {

        if (moves[i].first == dst_row && moves[i].second == dst_col)
            return true;
    }
    return false;
}


void RPSGame::playGame() {

    //in case the game is over at position phase
    if (this->m_game_over == true)
        return;

    int num_move_player1 = 0;
    int num_move_player2 = 0;
    int count_moves_till_fight = 0;
    bool player1_moves_over = false, player2_moves_over = false;
    bool player1_current, was_fight;
    int src_row, src_col, dst_row, dst_col, joker_row, joker_col;
    char real_piece_type_curr, real_piece_type_opp;
    unique_ptr<Move> curr_move;
    unique_ptr<JokerChange> curr_joker_change;
    char new_rep;


    //main loop game
    while (this->m_game_over == false && ((!player1_moves_over || !player2_moves_over))) {

        //check if number of moves without a fight is bigger than limit and if so end the game with tie.
        if (count_moves_till_fight > MOVES_WITHOUT_FIGHT_LIMIT) {
            this->m_reason_winner = MOVES_LIMIT_NO_FIGHT;
            this->m_winner = TIE;
            this->m_game_over = true;
            continue;
        }

        player1_current = this->m_current_player == PLAYER_1;

        //check if the current player has a move
        if (player1_current) {
            if (player1_moves_over == false && (curr_move = this->m_algo_player1->getMove()) != nullptr) {
                num_move_player1++;
            } else {
                player1_moves_over = true;
                this->changeCurrentPlayer();
                continue;
            }
        } else {
            if (player2_moves_over == false && (curr_move = this->m_algo_player2->getMove()) != nullptr) {
                num_move_player2++;
            } else {
                player2_moves_over = true;
                this->changeCurrentPlayer();
                continue;
            }
        }

        count_moves_till_fight++;

        //if there is am move, do it
        src_row = curr_move->getFrom().getY();
        src_col = curr_move->getFrom().getX();
        real_piece_type_curr = (this->m_board.m_game_board[src_row][src_col] != nullptr)
                               ? this->m_board.m_game_board[src_row][src_col]->m_symbol : NEUTRAL_CHAR;

        dst_row = curr_move->getTo().getY();
        dst_col = curr_move->getTo().getX();
        real_piece_type_opp = (this->m_board.m_game_board[dst_row][dst_col] != nullptr)
                              ? this->m_board.m_game_board[dst_row][dst_col]->m_symbol : NEUTRAL_CHAR;

        was_fight = playRegularMove(src_row, src_col, dst_row, dst_col);
        if (this->m_game_over) {
            this->m_bad_input_index1 = num_move_player1;
            continue;
        }

        //handle if was a fight
        if (was_fight) {
            RPSPoint fight_pos(curr_move->getTo().getX(), curr_move->getTo().getY());
            RPSFight curr_fight(real_piece_type_curr,
                                real_piece_type_opp, fight_pos,
                                this->m_current_player);

            if (player1_current)
                this->m_algo_player1->notifyFightResult(curr_fight);
            else
                this->m_algo_player2->notifyFightResult(curr_fight);

            count_moves_till_fight = 0;
        }

        //do a joker change if exists
        curr_joker_change = player1_current ? this->m_algo_player1->getJokerChange()
                                            : this->m_algo_player2->getJokerChange();
        if (curr_joker_change != nullptr) {
            joker_row = curr_joker_change->getJokerChangePosition().getY();
            joker_col = curr_joker_change->getJokerChangePosition().getX();
            new_rep = curr_joker_change->getJokerNewRep();

            playJokerMove(joker_row, joker_col, new_rep);
            if (this->m_game_over) {
                this->m_bad_input_index1 = num_move_player1;
                continue;
            }
        }

        //notify the opponent about the move
        if (player1_current)
            this->m_algo_player2->notifyOnOpponentMove(*curr_move);
        else
            this->m_algo_player1->notifyOnOpponentMove(*curr_move);

        if (was_fight) {
            RPSPoint fight_pos(curr_move->getTo().getX(), curr_move->getTo().getY());
            RPSFight curr_fight(real_piece_type_curr,
                                real_piece_type_opp, fight_pos,
                                this->m_current_player);

            if (player1_current)
                this->m_algo_player1->notifyFightResult(curr_fight);
            else
                this->m_algo_player2->notifyFightResult(curr_fight);
        }

        //check game status after move and change player turn
        checkGameStatus();
        changeCurrentPlayer();

    } //end loop game

    //check if both moves finished and there is still no winner
    if (this->m_game_over == false && player1_moves_over == true && player2_moves_over == true) {
        this->m_reason_winner = AFTER_MOVES_NO_WINNER;
        this->m_winner = TIE;
        this->m_game_over = true;
    }

}