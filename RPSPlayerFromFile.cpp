#include "RPSPlayerFromFile.h"
#include "RPSPosParser.h"

int RPSPlayerFromFile::getNumOfFlags() const {
    return this->m_num_of_flags;
}

int RPSPlayerFromFile::getNumMovingPieces() const {
    return this->m_num_of_moving_pieces;
}

void RPSPlayerFromFile::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {

    string path_to_pos_file = (player == PLAYER_1) ? PATH_POS_FILE_PLAYER1 : PATH_POS_FILE_PLAYER2;
    RPSPosParser prs(this->m_num_player);
    prs.parsePosFile(path_to_pos_file);

    //TODO: check if posFile is legal

    //iterate the parser board and take the pieces and enter them to vectorToFill;
    for(int i =0; i < BOARD_ROWS; i++) {
        for(int j =0; j < BOARD_COLS; j++) {
            if(prs.m_my_board[i][j] != nullptr) {
                unique_ptr<PiecePosition> piece = std::move(prs.m_my_board[i][j]);
                vectorToFill.push_back(piece);
            }
        }
    }

    //update player attributes
    this->m_num_of_flags = prs.m_num_of_flags;
    this->m_num_of_moving_pieces = prs.getNumMovingPieces();

}


bool RPSPlayerFromFile::parseMoveFile() {

    this->moves.clear();
    string path_to_move_file = this->m_num_player == PLAYER_1 ? PATH_MOVE_FILE_PLAYER1 : PATH_MOVE_FILE_PLAYER2;

    ifstream fin(path_to_move_file);
    if(!fin.is_open()) {
        cout << FILE_NOT_EXISTS;
        return false;
    }

    string line, clean_line;
    int size = 0, src_col, src_row, dst_col, dst_row, joker_col, joker_row;
    char new_rep;

    while(getline(fin, line)) {

        //in case there is an empty line - continue read the next line of the same player
        if (line.empty())
            continue;


        clean_line = RPSPosParser::removeExtraSpaces(line);
        vector <string> tokens = RPSPosParser::split(clean_line, ' ');
        size = tokens.size();

        //regular move line
        if ((size == 4 && RPSPosParser::isStringRepInt(tokens[0]) && RPSPosParser::isStringRepInt(tokens[1])
             && RPSPosParser::isStringRepInt(tokens[2]) && RPSPosParser::isStringRepInt(tokens[3]))) { //any move format

            src_col = std::stoi(tokens[0]) - 1;
            src_row = std::stoi(tokens[1]) - 1;
            dst_col = std::stoi(tokens[2]) - 1;
            dst_row = std::stoi(tokens[3]) - 1;





        //joker move line
        } else if(size == 8 && RPSPosParser::isStringRepInt(tokens[0]) && RPSPosParser::isStringRepInt(tokens[1])
                  && RPSPosParser::isStringRepInt(tokens[2]) && RPSPosParser::isStringRepInt(tokens[3]) &&
                  tokens[4] == "J:" && RPSPosParser::isStringRepInt(tokens[5]) &&
                  RPSPosParser::isStringRepInt(tokens[6]) &&
                  (tokens[7].length() == 1 && isalpha(tokens[7][0]))) { //bad format

            src_col = std::stoi(tokens[0]) - 1;
            src_row = std::stoi(tokens[1]) - 1;
            dst_col = std::stoi(tokens[2]) - 1;
            dst_row = std::stoi(tokens[3]) - 1;
            joker_col = std::stoi(tokens[5]) - 1;
            joker_row = std::stoi(tokens[6]) - 1;
            new_rep = tokens[7][0];



        //bad format - enter an invalid move
        } else {

        }







    }
    return true;
}


void RPSPlayerFromFile::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {
    //the function in AlgoPlayerFile do nothing
    return;
}

void RPSPlayerFromFile::notifyOnOpponentMove(const Move &move) {
    //the function in AlgoPlayerFile do nothing
    return;
}

void RPSPlayerFromFile::notifyFightResult(const FightInfo &fightInfo) {
    //the function in AlgoPlayerFile do nothing
    return;
}

unique_ptr<Move> RPSPlayerFromFile::getMove() {
    return unique_ptr<Move>();
}

unique_ptr<JokerChange> RPSPlayerFromFile::getJokerChange() {
    return unique_ptr<JokerChange>();
}






