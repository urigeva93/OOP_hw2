#include "RPSGame.h"

void RPSGame::initGame() {

    //create two profile for player 1 and 2
    RPSPlayerFromFile *player1 = new RPSPlayerFromFile();

    player1->parsePosFile(PATH_POS_FILE_PLAYER1);

    RPSPlayerFromFile *player2 = new RPSPlayerFromFile();
    player2->parsePosFile(PATH_POS_FILE_PLAYER2);

    if (player1->getM_pos_msg() != FILE_SUCCESS &&
        player2->getM_pos_msg() != FILE_SUCCESS) { //both players with invalid position files

        this->m_reason_winner = BAD_POS_FILE_BOTH_PLAYERS;
        this->m_winner = TIE;
        this->m_game_over = true;
        this->m_bad_line1_num = player1->m_invalid_line_num_pos_file;
        this->m_bad_line2_num = player2->m_invalid_line_num_pos_file;

    } else if (player1->getM_pos_msg() == FILE_SUCCESS &&
               player2->getM_pos_msg() != FILE_SUCCESS) { //only player 2 with invalid pos file

        this->m_reason_winner = BAD_POS_FILE_PLAYER_TWO;
        this->m_winner = PLAYER_1;
        this->m_game_over = true;
        this->m_bad_line1_num = player2->m_invalid_line_num_pos_file;

    } else if (player2->getM_pos_msg() == FILE_SUCCESS &&
               player1->getM_pos_msg() != FILE_SUCCESS) { //only player 1 with invalid pos file

        this->m_reason_winner = BAD_POS_FILE_PLAYER_ONE;
        this->m_winner = PLAYER_2;
        this->m_game_over = true;
        this->m_bad_line1_num = player1->m_invalid_line_num_pos_file;
    } else { //both players with valid position files


        //init game attributes according to players
        this->m_num_moving_pieces_player1 = player1->getNumMovingPieces(); //without flags!
        this->m_num_moving_pieces_player2 = player2->getNumMovingPieces();
        this->m_num_flags_player1 = player1->getM_num_of_flags();
        this->m_num_flags_player2 = player2->getM_num_of_flags();

        //iterate the board, and combine the two players board to one shared board
        for (int i = 0; i < BOARD_ROWS; i++) {

            for (int j = 0; j < BOARD_COLS; j++) {

                if (player1->isPosEmpty(i, j) && player2->isPosEmpty(i, j)) //no piece at this location for both players
                    this->m_game_board[i][j] = nullptr;

                else if (!(player1->isPosEmpty(i, j)) &&
                         player2->isPosEmpty(i, j)) { //only player 1 has piece at curr location

                    this->m_game_board[i][j] = new RPSPiece(*(player1->m_my_board[i][j]));

                } else if (player1->isPosEmpty(i, j) &&
                           !(player2->isPosEmpty(i, j))) { //only player 2 has piece at curr location

                    this->m_game_board[i][j] = new RPSPiece(*(player2->m_my_board[i][j]));

                } else { //both players have pieces at curr location, need to fight

                    FightResult res = manageFight(player1->m_my_board[i][j], player2->m_my_board[i][j]);
                    RPSPiece *piece_player1 = new RPSPiece(*(player1->m_my_board[i][j]));
                    RPSPiece *piece_player2 = new RPSPiece(*(player2->m_my_board[i][j]));
                    updateGameAfterFight(res, piece_player1, piece_player2, i, j);
                }
            }
        }

        //check game status after init
        checkGameStatusAfterInit();
        if (this->m_game_over == true) {
            delete (player1);
            delete (player2);
            return;
        }

        //check regular game status
        checkGameStatus();
    }

    delete (player1);
    delete (player2);

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
        this->m_reason_winner = ALL_FLAGS_EATEN_IN_POS_FILE;
        this->m_winner = 0;
        this->m_game_over = true;
        return;
    }

    //check if after init, there is a tie - all moving pieces of both pieces are eaten
    if (this->m_num_moving_pieces_player1 == 0 && this->m_num_moving_pieces_player2 == 0) {
        this->m_reason_winner = ALL_PIECES_EATEN_IN_POS_FILE;
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


FightResult RPSGame::manageFight(RPSPiece *piece_curr_player, RPSPiece *piece_opp_player) {

    if (piece_curr_player == nullptr ||
        piece_opp_player == nullptr) //in case there aren't really 2 pieces participate in fight
        return NO_FIGHT;


    //case 1: same pieces' type for both players
    if (piece_curr_player->m_symbol == piece_opp_player->m_symbol)
        return SAME_PIECE_FIGHT;


    //case 2: flag
    if (piece_opp_player->m_symbol == FLAG)
        return FLAG_EATEN_OPP_PLAYER;


    //case 3: bombs
    if (piece_opp_player->m_symbol == BOMB)
        return BOMB_OPP_PLAYER;

    //case 4: regular RPS rules
    //wins current player:
    if ((piece_curr_player->m_symbol == ROCK && piece_opp_player->m_symbol == SCISSOR) ||
        (piece_curr_player->m_symbol == PAPER && piece_opp_player->m_symbol == ROCK) ||
        (piece_curr_player->m_symbol == SCISSOR && piece_opp_player->m_symbol == PAPER))
        return WIN_CURR_PLAYER;


    //loses current player:
    if ((piece_curr_player->m_symbol == ROCK && piece_opp_player->m_symbol == PAPER) ||
        (piece_curr_player->m_symbol == PAPER && piece_opp_player->m_symbol == SCISSOR) ||
        (piece_curr_player->m_symbol == SCISSOR && piece_opp_player->m_symbol == ROCK))
        return WIN_OPP_PLAYER;


    //in init phase when current player has flag "fighting"
    if (piece_curr_player->m_symbol == FLAG)
        return WIN_OPP_PLAYER;

    return NO_FIGHT;

}

void RPSGame::updateGameAfterFight(FightResult res, RPSPiece *piece_curr_player, RPSPiece *piece_opp_player, int row,
                                   int col) {

    bool player_1_current = (this->m_current_player == PLAYER_1);

    if (res == SAME_PIECE_FIGHT) {
        this->m_game_board[row][col] = nullptr;
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
        this->m_game_board[row][col] = nullptr;
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
        this->m_game_board[row][col] = piece_curr_player;
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

        this->m_game_board[row][col] = piece_opp_player;
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
            this->m_game_board[row][col] = nullptr;
        else
            this->m_game_board[row][col] = piece_curr_player;


        if (player_1_current)
            this->m_num_flags_player2--;
        else
            this->m_num_flags_player1--;

    }
    return;
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
    RPSPiece *piece = this->m_game_board[row][col];
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
    RPSPiece *piece = this->m_game_board[row][col];
    if (piece == nullptr)
        return false;
    return (piece->m_is_joker == true);
}

void RPSGame::playRegularMove(int src_row, int src_col, int dst_row, int dst_col) {

    bool player_1_current = this->m_current_player == PLAYER_1;

    //check if the indices are legal
    if (!isMoveOnBoard(src_row, src_col, dst_row, dst_col)) {
        cout << NOT_IN_RANGE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //check if there is a piece at current location
    if (!isPieceExist(src_row, src_col)) {
        cout << PIECE_NOT_EXIST;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //checks if the piece at the src location belongs to current player
    if (!isSamePieceAsCurrent(src_row, src_col)) {
        cout << SRC_NO_CURR_PIECE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    RPSPiece *piece = this->m_game_board[src_row][src_col];

    //checks if the piece at src location able to move
    if (!isPieceAbleToMove(piece->m_symbol)) {
        cout << NOT_ABLE_TO_MOVE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //checks if the dst location doesn't contain same player piece
    if (isSamePieceAsCurrent(dst_row, dst_col)) {
        cout << DST_PIECE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //checks if the move itself is legal (according to rules of moving pieces)
    if (!isMoveLegal(src_row, src_col, dst_row, dst_col)) {
        cout << MOVE_NOT_LEGAL;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //the move is valid!, thus we can play
    if (this->m_game_board[dst_row][dst_col] == nullptr) { //regular move
        this->m_game_board[dst_row][dst_col] = this->m_game_board[src_row][src_col];

        this->m_game_board[src_row][src_col] = nullptr;
    } else { //two pieces fight!
        FightResult res = manageFight(this->m_game_board[src_row][src_col], this->m_game_board[dst_row][dst_col]);
        updateGameAfterFight(res, this->m_game_board[src_row][src_col], this->m_game_board[dst_row][dst_col], dst_row, dst_col);

        this->m_game_board[src_row][src_col] = nullptr;
    }

}


void RPSGame::playJokerMove(int joker_row, int joker_col, char new_rep) {

    bool player_1_current = this->m_current_player == PLAYER_1;

    //checks if the indices are inside the board
    if (!isInBoard(joker_row, joker_col)) {
        cout << NOT_IN_RANGE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //check if the exits a piece at src location
    if (!isPieceExist(joker_row, joker_col)) {
        cout << PIECE_NOT_EXIST;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //checks if the piece at the src location belongs to current player
    if (!isSamePieceAsCurrent(joker_row, joker_col)) {
        cout << SRC_NO_CURR_PIECE;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //check if the piece at current location is a joker
    if (!isPieceAJoker(joker_row, joker_col)) {
        cout << NO_JOKER;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }

    //checks if the new joker rep is valid
    if (!isJokerNewRepValid(new_rep)) {
        cout << NEW_REP_INVALID;
        if (player_1_current) {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
            this->m_winner = PLAYER_2;
        } else {
            this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
            this->m_winner = PLAYER_1;
        }
        this->m_game_over = true;
        return;
    }
    char orig_rep = this->m_game_board[joker_row][joker_col]->m_symbol;
    //the joker move is valid!, thus we can change it
    this->m_game_board[joker_row][joker_col]->m_symbol = new_rep;

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
    return (this->m_game_board[row][col] != nullptr);
}

void RPSGame::printBoard() {

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if (this->m_game_board[i][j] == nullptr)
                this->m_output_file << " ";
            else if (this->m_game_board[i][j]->m_is_joker == false) { //regular piece
                if (this->m_game_board[i][j]->m_num_player == PLAYER_1) //Capital letters
                    this->m_output_file << this->m_game_board[i][j]->m_symbol;
                else
                    this->m_output_file << (char) std::tolower(this->m_game_board[i][j]->m_symbol);
            } else { //joker piece
                if (this->m_game_board[i][j]->m_num_player == PLAYER_1) //Capital letters
                    this->m_output_file << JOKER;
                else
                    this->m_output_file << (char) std::tolower(JOKER);
            }
        }
        this->m_output_file << endl;
    }
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

    if (this->m_reason_winner == ALL_FLAGS_EATEN_IN_POS_FILE) {
        this->m_output_file << "A tie - all flags are eaten by both players in the position files" << endl;
        return;
    }

    if (this->m_reason_winner == ALL_PIECES_EATEN_IN_POS_FILE) {
        this->m_output_file << "A tie - all moving pieces are eaten by both players in the position files" << endl;
        return;
    }

    if (this->m_reason_winner == BAD_POS_FILE_PLAYER_ONE) {
        this->m_output_file << "Bad Positioning input file for player 1 - line " << this->m_bad_line1_num << endl;
        return;
    }

    if (this->m_reason_winner == BAD_POS_FILE_PLAYER_TWO) {
        this->m_output_file << "Bad Positioning input file for player 2 - line " << this->m_bad_line1_num << endl;
        return;
    }

    if (this->m_reason_winner == BAD_POS_FILE_BOTH_PLAYERS) {
        this->m_output_file << "Bad Positioning input file for both players - player 1: line " << this->m_bad_line1_num
                            << ", player 2: line " << this->m_bad_line2_num << endl;
        return;
    }

    if (this->m_reason_winner == BAD_MOVE_FILE_PLAYER_ONE) {
        this->m_output_file << "Bad Moves input file for player 1 - line " << this->m_bad_line1_num << endl;
        return;
    }

    if (this->m_reason_winner == BAD_MOVE_FILE_PLAYER_TWO) {
        this->m_output_file << "Bad Moves input file for player 2 - line " << this->m_bad_line1_num << endl;
        return;
    }

    if (this->m_reason_winner == AFTER_MOVES_NO_WINNER) {
        this->m_output_file << "A tie - both Moves input files done without a winner" << endl;
        return;
    }

}

void RPSGame::endGame() {

    this->m_output_file << "Winner: " << this->m_winner << endl;
    this->printReasonWinner();
    this->m_output_file << endl; //empty line
    this->printBoard();
    this->m_output_file.close();

}

vector <pair<int, int>> RPSGame::getLegalLMovesForPiece(int row, int col) {
    vector <pair<int, int>> moves;
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
    vector <pair<int, int>> moves = getLegalLMovesForPiece(src_row, src_col);
    int size = moves.size();
    for (int i = 0; i < size; i++) {

        if (moves[i].first == dst_row && moves[i].second == dst_col)
            return true;
    }

    return false;
}

bool RPSGame::isM_game_over() const {
    return this->m_game_over;
}

void RPSGame::playGame() {

    //in case the game is over at position phase
    if (this->m_game_over == true)
        return;


    bool player1_moves_over = false;
    int line_num_player1 = 0;
    bool player2_moves_over = false;
    int line_num_player2 = 0;
    bool player1_current;
    int size = 0, src_row, src_col, dst_row, dst_col, joker_row, joker_col;
    char new_rep;
    string line, clean_line;

    //open to moves files for reading
    ifstream fin_player1(PATH_MOVE_FILE_PLAYER1);
    ifstream fin_player2(PATH_MOVE_FILE_PLAYER2);

    if (!fin_player1.is_open()) {
        this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
        this->m_winner = PLAYER_2;
        this->m_game_over = true;
        cout << FILE_NOT_EXISTS;
        return;
    }

    if (!fin_player2.is_open()) {
        this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
        this->m_winner = PLAYER_1;
        this->m_game_over = true;
        cout << FILE_NOT_EXISTS;
        return;
    }

    //main loop game
    while (this->m_game_over == false && (!player1_moves_over || !player2_moves_over)) {

        player1_current = this->m_current_player == PLAYER_1;

        if (player1_current) {
            if (player1_moves_over == false && getline(fin_player1, line))
                line_num_player1++;
            else {
                player1_moves_over = true;
                this->changeCurrentPlayer();
                continue;
            }
        } else {
            if (player2_moves_over == false && getline(fin_player2, line))
                line_num_player2++;
            else {
                player2_moves_over = true;
                this->changeCurrentPlayer();
                continue;
            }
        }

        //in case there is an empty line - continue read the next line of the same player
        if (line.empty())
            continue;

        clean_line = RPSPlayerFromFile::removeExtraSpaces(line);
        vector <string> tokens = RPSPlayerFromFile::split(clean_line, ' ');
        size = tokens.size();

        if ((size == 4 && RPSPlayerFromFile::isStringRepInt(tokens[0]) && RPSPlayerFromFile::isStringRepInt(tokens[1])
             && RPSPlayerFromFile::isStringRepInt(tokens[2]) && RPSPlayerFromFile::isStringRepInt(tokens[3])) ||
            (size == 8 && RPSPlayerFromFile::isStringRepInt(tokens[0]) && RPSPlayerFromFile::isStringRepInt(tokens[1])
             && RPSPlayerFromFile::isStringRepInt(tokens[2]) && RPSPlayerFromFile::isStringRepInt(tokens[3]) &&
             tokens[4] == "J:" && RPSPlayerFromFile::isStringRepInt(tokens[5]) &&
             RPSPlayerFromFile::isStringRepInt(tokens[6]) &&
             (tokens[7].length() == 1 && isalpha(tokens[7][0])))) { //any move format

            src_col = std::stoi(tokens[0]) - 1;
            src_row = std::stoi(tokens[1]) - 1;
            dst_col = std::stoi(tokens[2]) - 1;
            dst_row = std::stoi(tokens[3]) - 1;

            playRegularMove(src_row, src_col, dst_row, dst_col);
            if (this->m_game_over == true) {
                this->m_bad_line1_num = (player1_current == true) ? line_num_player1 : line_num_player2;
                continue;
            }

            if (size == 8) { //move with joker
                joker_col = std::stoi(tokens[5]) - 1;
                joker_row = std::stoi(tokens[6]) - 1;
                new_rep = tokens[7][0];

                playJokerMove(joker_row, joker_col, new_rep);
                if (this->m_game_over == true) {
                    this->m_bad_line1_num = (player1_current == true) ? line_num_player1 : line_num_player2;
                    continue;
                }
            }

        } else { //bad format
            if (player1_current) {
                this->m_reason_winner = BAD_MOVE_FILE_PLAYER_ONE;
                this->m_winner = PLAYER_2;
                this->m_game_over = true;
                this->m_bad_line1_num = line_num_player1;
                cout << BAD_FORMAT;
                continue;
            } else {
                this->m_reason_winner = BAD_MOVE_FILE_PLAYER_TWO;
                this->m_winner = PLAYER_1;
                this->m_game_over = true;
                this->m_bad_line1_num = line_num_player2;
                cout << BAD_FORMAT;
                continue;
            }
        }

        checkGameStatus();
        changeCurrentPlayer();

    } //end main loop

    //check if both files finished and there is no winner
    if (this->m_game_over == false && player1_moves_over == true && player2_moves_over == true) {
        this->m_reason_winner = AFTER_MOVES_NO_WINNER;
        this->m_winner = TIE;
        this->m_game_over = true;
    }

    fin_player1.close();
    fin_player2.close();
}


int RPSGame::getPlayer(const Point &pos) const {

    //check if the location is legal
    if(!isInBoard(pos.getY(), pos.getX()))
        return TIE;

    RPSPiece* curr_piece = this->m_game_board[pos.getY()][pos.getX()];
    if (curr_piece == nullptr)
        return TIE;

    return curr_piece->m_num_player;
}



