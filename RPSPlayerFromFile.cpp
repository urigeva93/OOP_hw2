#include "RPSPlayerFromFile.h"

bool RPSPlayerFromFile::isStringRepInt(string line) {

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

string RPSPlayerFromFile::removeExtraSpaces(string line) {

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

vector<string> RPSPlayerFromFile::split(string line, char delimiter) {
    vector<string> elements;
    stringstream str(line);
    string token;

    while (std::getline(str, token, delimiter))
        elements.push_back(token);

    return elements;
}


void RPSPlayerFromFile::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {

    string path_to_pos_file = (player == PLAYER_1) ? PATH_POS_FILE_PLAYER1 : PATH_POS_FILE_PLAYER2;
    ifstream fin(path_to_pos_file);

    //in case the file doesn't - return
    if (!fin.is_open())
        return;

    string line, clean_line;
    int row = 0, col = 0, size = 0;
    char piece;

    unique_ptr<PiecePosition> p_piece;

    while (std::getline(fin, line)) {

        //in case the line is empty - continue to the next line
        if (line.empty())
            continue;

        clean_line = removeExtraSpaces(line);
        vector<string> tokens = split(clean_line, ' ');
        size = tokens.size();


        if (size == 3 && (tokens[0].length() == 1 && isalpha(tokens[0][0])) &&
            isStringRepInt(tokens[1]) &&
            isStringRepInt(tokens[2])) { //regular piece line

            piece = tokens[0][0];
            col = std::stoi(tokens[1]) - 1;
            row = std::stoi(tokens[2]) - 1;

            p_piece = make_unique<RPSPiece>(piece, false, player, this->m_num_player, RPSPoint(col, row));

        } else if (size == 4 && tokens[0].length() == 1 && tokens[0][0] == JOKER &&
                   isStringRepInt(tokens[1]) &&
                   isStringRepInt(tokens[1]) && isalpha(tokens[3][0])) {


            piece = tokens[3][0];
            col = std::stoi(tokens[1]) - 1;
            row = std::stoi(tokens[2]) - 1;

            p_piece = make_unique<RPSPiece>(piece, true, player, this->m_num_player, RPSPoint(col, row));

        } else  //bad format line - make a bad position
            p_piece = make_unique<RPSPiece>(NEUTRAL_CHAR, false, this->m_num_player, RPSPoint(-1, -1));


        //push the current piece created by line to vectorToFIll
        vectorToFill.push_back(p_piece);
    }

}

void RPSPlayerFromFile::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {
    //the function in Player Algorithm File does nothing
    return;
}

void RPSPlayerFromFile::notifyOnOpponentMove(const Move &move) {
    //the function in Player Algorithm File does nothing
    return;
}

void RPSPlayerFromFile::notifyFightResult(const FightInfo &fightInfo) {
    //the function in Player Algorithm File does nothing
    return;
}

unique_ptr<Move> RPSPlayerFromFile::getMove() {

    string line, clean_line;
    int size = 0, src_col, src_row, dst_col, dst_row, joker_col, joker_row;
    char new_rep;

    unique_ptr<Move> p_move;

    if(std::getline(this->m_moves_file, line)) {

        //in case there is an empty line - continue read the next line
        if (line.empty())
            RPSPlayerFromFile::getMove();

        clean_line = removeExtraSpaces(line);
        vector <string> tokens = split(clean_line, ' ');
        size = tokens.size();

        //regular move line
        if ((size == 4 && isStringRepInt(tokens[0]) && isStringRepInt(tokens[1])
             && isStringRepInt(tokens[2]) && isStringRepInt(tokens[3]))) {

            src_col = std::stoi(tokens[0]) - 1;
            src_row = std::stoi(tokens[1]) - 1;
            dst_col = std::stoi(tokens[2]) - 1;
            dst_row = std::stoi(tokens[3]) - 1;

            RPSMove move(RPSPoint(src_col, src_row), RPSPoint(dst_col, dst_row));
            this->m_curr_move = move;

            p_move = make_unique<RPSMove>(RPSPoint(src_col, src_row), RPSPoint(dst_col, dst_row));

        //joker move line
        } else if(size == 8 && isStringRepInt(tokens[0]) && isStringRepInt(tokens[1])
                  && isStringRepInt(tokens[2]) && isStringRepInt(tokens[3]) &&
                  tokens[4] == "J:" && isStringRepInt(tokens[5]) &&
                  isStringRepInt(tokens[6]) &&
                  (tokens[7].length() == 1 && isalpha(tokens[7][0]))) {

            src_col = std::stoi(tokens[0]) - 1;
            src_row = std::stoi(tokens[1]) - 1;
            dst_col = std::stoi(tokens[2]) - 1;
            dst_row = std::stoi(tokens[3]) - 1;
            joker_col = std::stoi(tokens[5]) - 1;
            joker_row = std::stoi(tokens[6]) - 1;
            new_rep = tokens[7][0];

            RPSMove move(RPSPoint(src_col, src_row), RPSPoint(dst_col, dst_row), new_rep, RPSPoint(joker_col, joker_row));
            this->m_curr_move = move;
            p_move = make_unique<RPSMove>(RPSPoint(src_col, src_row), RPSPoint(dst_col, dst_row));

        //bad format - enter an invalid move
        } else {
            RPSMove move(RPSPoint(-1, -1), RPSPoint(-1, -1));
            this->m_curr_move = move;
            p_move = make_unique<RPSMove>(RPSPoint(src_col, src_row), RPSPoint(dst_col, dst_row));

        }

        return p_move;
    } else
        return nullptr;

}

unique_ptr<JokerChange> RPSPlayerFromFile::getJokerChange() {

    if(this->m_curr_move.m_is_joker_move) {

        unique_ptr<JokerChange> p_joker_change = make_unique<RPSMove>(m_curr_move.m_from, m_curr_move.m_to, m_curr_move.m_new_rep, m_curr_move.m_joker_pos);
        return p_joker_change;

    } else
        return nullptr;


}
