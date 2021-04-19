#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <stdio.h>
#include <ctype.h>
#include<unordered_map>
#include<list>
#include <limits>


using namespace std;
// float inf = numeric_limits<float>::infinity();
fstream infile;
ofstream outfile;
class Piece{
public:

    char color;
    bool king ;
    bool is_empty = true;
    Piece():color('.'),king(false) {}
	Piece(char color, bool king):color(color),king(king) {}

};

class Board{
public:

    vector<vector<Piece>> board;
    vector<pair<int,int>> path;
    vector<vector<Piece>> input();
    void print_board();
    void move_piece(int,int,int ,int );
    int b_left = 0;
    int w_left = 0;
    int b_king = 0;
    int w_king = 0;


    string Pmethod;
    char max_color;
    char min_color;
    double Pclock;

    //evaluate 
    vector<pair<int,int>> get_all_piece(char);
    char winner();
    void Jump(vector<Board>&,Board,int,int);
    bool canJump(Board,int,int);
    bool canMove(Board,int,int);
    void Move(vector<Board>&,Board,int,int);
    bool judge_King(char);
    void _remove(int row, int col);
    float evaluation();
    int evaluationII();





    Board():board({{}}) {}
    Board(vector<vector<Piece>> board): board(board) {}
    
};

class Game{
public:

    Board min_max(Board, int ,char player,float,float);
    Board min_max3(Board, int ,char player,int,int);
    vector<Board> get_all_moves(Board, char);
    string path_trans(int, int);
    void createOutput(Board&,Board&);
    



};


int main(int argc, char const * argv[])
{

    Board board;

    board.board = board.input();
    Game game;
    //play game
    board.print_board();
    const clock_t start_time = clock();
    int depth = 1;
    if(board.Pmethod == "SINGLE"){
        depth = 3;
    } else {
        depth = 3;
    }
    Board board1 = game.min_max3(board,depth,board.max_color,INT_MIN,INT_MAX);
    cout << board1.b_left << board1.w_left <<endl;
    cout << "______________________" << endl;
    cout << float(clock() - start_time) /CLOCKS_PER_SEC << endl;
    board1.print_board();
    game.createOutput(board,board1);
    // int k = 0;
    // Board host;
    // host.board = host.input();
    // while(host.winner()== ' ' && k < 150){
    //     Game game;
    //     cout << "............................" << endl;
    //     cout << k << endl;
    //     host.print_board();
    //     if(host.max_color == 'b'){
    //         Board res = game.min_max(host,7,host.max_color,INT_MIN,INT_MAX);
    //     }else{
    //         Board res = game.min_max(host,7,host.max_color,INT_MIN,INT_MAX);
    //     }
        
    //     cout << "----------------------------" << endl;
    //     res.print_board();
    //     cout << "----------------------------" << endl;
    //     host.b_left = res.b_left;
    //     host.w_left = res.w_left;
    //     host.w_king = res.w_king;
    //     host.b_king = res.b_king;
    //     host.board = res.board;
    //     host.min_color = res.max_color;
    //     host.max_color = res.min_color;
    //     host.path.clear();
    //     // cout << host.max_color << host.min_color << endl;
    //     // cout << "............................" << endl;
    //     k++;

    // }

    return 0;
}


vector<vector<Piece>> Board::input(){

    infile.open("input.txt");
    infile >> Pmethod;
    string Pcolor;
    infile >> Pcolor;
    if(Pcolor == "BLACK"){
        max_color = 'b';
        min_color = 'w';
    }else{
        max_color = 'w';
        min_color = 'b';
    }
    infile >> Pclock;
    vector<vector<Piece>> board(8, vector<Piece>(8));


    for( int i = 0; i < 8; ++i){
        string temp;
        infile >> temp;
        for(int j = 0; j < 8; ++j){
            board[i][j].color = tolower(temp[j]);
            board[i][j].king = judge_King(temp[j]);
            if(board[i][j].color == 'b'){
                b_left++;
                if(board[i][j].king){
                    b_king++;
                }
            }else if(board[i][j].color == 'w'){
                w_left++;
                if(board[i][j].king){
                    w_king++;
                }
            }
            board[i][j].is_empty = false;
        }
    }
    infile.close();
    return board;
}





bool Board:: judge_King(char piece){
    if(piece =='B' || piece == 'W'){
        return true;
    }
    return false;
}

char Board:: winner(){
    if(b_left <= 0 ){
        return 'w';
    }
    if(w_left <= 0){
        return 'b';
    }
    return ' ';
}


float Board::evaluation(){

    if(max_color == 'b') return float (b_left-w_left + (0.5 * b_king - 0.5 * w_king));
    else  return float (w_left-b_left + (0.5 * w_king - 0.5 * b_king));  
}

int Board::evaluationII(){
    int b_val = 0;
    int w_val = 0;
    int total_pieces= b_left+w_left;
    for(int i = 0; i< 8 ; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j].color == 'b'){
                if(!board[i][j].king){
                    b_val+=(5+i);
                }else{
                    b_val+=15;
                }
            }else if(board[i][j].color == 'w'){
                if(!board[i][j].king){
                    w_val+=(5+(7-i));
                }else{
                    w_val+=15;
                }

            }
        }
    }

    if(max_color == 'b'){
        return (b_val-w_val);
    }else{
        return (w_val-b_val);
    }


}


void Board::move_piece(int cur_row,int cur_col, int row , int col){
    swap(board[cur_row][cur_col], board[row][col]);

    if(row == 7 || row == 0){
        if(board[row][col].color == 'b'){
            board[row][col].king = true;
            b_king++;
        }else if(board[row][col].color == 'w'){
            board[row][col].king = true;
            w_king++;
        }
    }
}

void Board::print_board(){
    for( int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(board[i][j].king){
                cout << char(toupper(board[i][j].color)) ;
            }else{
                cout << board[i][j].color;
            }
        }
        cout << endl;
    }

    cout << endl;

}



void Board::_remove(int row, int col){
    if(board[row][col].color == 'b'){
        b_left--;
        if(board[row][col].king){
            b_king--;
        }
    }else if(board[row][col].color == 'w'){
        w_left--;
        if(board[row][col].king){
            w_king--;
        }
    }
    board[row][col].color = '.';
    board[row][col].king = false;
    
}

vector<pair<int,int>> Board::get_all_piece(char color){
    vector<pair<int,int>> get_all;

    for( int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(board[i][j].color == color){
                get_all.push_back(make_pair(i,j));
            }
        }
    }

    return get_all;

}


bool Board::canJump(Board cur_board ,int row , int col){
    //cout << "in canJUMP" << endl;
    if(cur_board.board[row][col].color == 'w'&&!cur_board.board[row][col].king){
        //cout << "in w can jump" << endl;
        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'b' && cur_board.board[row-2][col-2].color == '.'){
            return true;
        }
        if(row -1 > -1 && col+1 < 8 && row -2 > -1 && col + 2 < 8 && cur_board.board[row -1][col+1].color== 'b' &&  cur_board.board[row-2][col+2].color == '.'){
            return true;
        }

    }
    if(cur_board.board[row][col].color == 'b'&& !cur_board.board[row][col].king){
        // cout << "in b can jump" << endl;
        if(row + 1 < 8 && col-1 > -1 && row + 2 < 8 && col -2 > -1 && cur_board.board[row +1][col-1].color == 'w'&& cur_board.board[row+2][col-2].color == '.'){
            return true;
        }
        if(row + 1 < 8 && col+1 < 8 && row +2 < 8 && col +2 < 8 && cur_board.board[row +1][col+1].color== 'w' &&  cur_board.board[row +2][col+2].color == '.'){
            return true;
        }

    }

    if(cur_board.board[row][col].color == 'w' && cur_board.board[row][col].king){

        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'b' && cur_board.board[row-2][col-2].color == '.'){
            return true;
        }
        if(row -1 > -1 && col+1 < 8 && row -2 > -1 && col + 2 < 8 && cur_board.board[row -1][col+1].color== 'b' &&  cur_board.board[row-2][col+2].color == '.'){
            return true;
        }
        if(row + 1 < 8 && col-1 > -1 && row + 2 < 8 && col -2 > -1 && cur_board.board[row +1][col-1].color == 'b'&& cur_board.board[row+2][col-2].color == '.'){
            return true;
        }
        if(row + 1 < 8 && col+1 < 8 && row +2 < 8 && col +2 < 8 && cur_board.board[row +1][col+1].color== 'b' &&  cur_board.board[row +2][col+2].color == '.'){
            return true;
        }

    }

    if(cur_board.board[row][col].color == 'b' && cur_board.board[row][col].king){

        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'w' && cur_board.board[row-2][col-2].color == '.'){
            return true;
        }
        if(row -1 > -1 && col+1 < 8 && row -2 > -1 && col + 2 < 8 && cur_board.board[row -1][col+1].color== 'w' &&  cur_board.board[row-2][col+2].color == '.'){
            return true;
        }
        if(row + 1 < 8 && col-1 > -1 && row + 2 < 8 && col -2 > -1 && cur_board.board[row +1][col-1].color == 'w'&& cur_board.board[row+2][col-2].color == '.'){
            return true;
        }
        if(row + 1 < 8 && col+1 < 8 && row +2 < 8 && col +2 < 8 && cur_board.board[row +1][col+1].color== 'w' &&  cur_board.board[row +2][col+2].color == '.'){
            return true;
        }

    }

    return false;

}

bool Board::canMove(Board cur_board, int row,int col){
    //cout << "in canMove" << endl;
    if(cur_board.board[row][col].color == 'w' || cur_board.board[row][col].king){
        if(row -1 > -1 && col-1 > -1 && cur_board.board[row -1][col-1].color == '.'){
            // cout << row << col << endl;
            return true;
        }
        if(row -1 > -1 && col+1 < 8 && cur_board.board[row -1][col+1].color == '.'){
            //cout << row << col << endl;
            return true;
        }

    }
    if(cur_board.board[row][col].color == 'b' || cur_board.board[row][col].king){
        if(row + 1 < 8 && col-1 > -1 && cur_board.board[row +1][col-1].color == '.'){
            //cout << row << col << endl;
            return true;
        }
        if(row + 1 < 8 && col+1 < 8 && cur_board.board[row +1][col+1].color == '.'){
            //cout << row << col << endl;
            return true;
        }

    }

    return false;
}



void Board::Jump(vector<Board>& boards,Board cur_board,int row ,int col){
    
    if( !canJump(cur_board,row,col)){
        boards.push_back(cur_board);
        return;
    }


    if(cur_board.board[row][col].color == 'w' && !cur_board.board[row][col].king){
        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'b' && cur_board.board[row-2][col-2].color == '.'){
            // cout << "J w move left"<< endl;
            cur_board._remove(row -1,col-1);
            cur_board.move_piece(row,col,row-2,col-2);
            cur_board.path.push_back(make_pair(row-2,col-2));
            if(row-2 == 0){
               boards.push_back(cur_board);
               return; 
            }
            Jump(boards,cur_board,row -2, col-2);
        }
        if(row -1 > -1 && col+1 < 8 && row -2 > -1 && col + 2 < 8 && cur_board.board[row -1][col+1].color== 'b' &&  cur_board.board[row-2][col+2].color == '.'){
            //cout << "J w move right"<< endl;
            cur_board._remove(row-1,col+1);
            cur_board.move_piece(row,col,row-2,col+2);
            cur_board.path.push_back(make_pair(row-2,col+2));
            if(row-2 == 0){
               boards.push_back(cur_board);
               return; 
            }
            Jump(boards,cur_board,row-2, col+2);                
        }
    }
    if(cur_board.board[row][col].color == 'b'&&!cur_board.board[row][col].king){
        if(row + 1 < 8 && col-1 > -1 && row + 2 < 8 && col -2 > -1 && cur_board.board[row +1][col-1].color == 'w'&& cur_board.board[row+2][col-2].color == '.'){
            //cout << " J b move left"<< endl;
            cur_board._remove(row + 1,col-1);
            cur_board.move_piece(row,col,row+2,col-2);
            cur_board.path.push_back(make_pair(row+2,col-2));
            if(row+2 == 7){
               boards.push_back(cur_board);
               return; 
            }
            Jump(boards,cur_board,row+2, col-2);
        }
        if(row + 1 < 8 && col+1 < 8 && row +2 < 8 && col +2 < 8 && cur_board.board[row +1][col+1].color== 'w' &&  cur_board.board[row +2][col+2].color == '.'){
            //cout << " J b move right"<< endl;
            cur_board._remove(row+1,col+1);
            cur_board.move_piece(row,col,row+2,col+2);
            cur_board.path.push_back(make_pair(row+2,col+2));
            if(row+2 == 7){
               boards.push_back(cur_board);
               return; 
            }
            Jump(boards,cur_board,row+2, col+2);
        }
    }

    if(cur_board.board[row][col].color == 'w' && cur_board.board[row][col].king){

        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'b' && cur_board.board[row-2][col-2].color == '.'){
            //cout << "J king w move up left"<< endl;
            cur_board._remove(row -1,col-1);
            cur_board.move_piece(row,col,row-2,col-2);
            cur_board.path.push_back(make_pair(row-2,col-2));
            Jump(boards,cur_board,row -2, col-2);
        }
        if(row -1 > -1 && col+1 < 8 && row -2 > -1 && col + 2 < 8 && cur_board.board[row -1][col+1].color== 'b' &&  cur_board.board[row-2][col+2].color == '.'){
            //cout << "J king w move up right"<< endl;
            cur_board._remove(row-1,col+1);
            cur_board.move_piece(row,col,row-2,col+2);
            cur_board.path.push_back(make_pair(row-2,col+2));
            Jump(boards,cur_board,row-2, col+2);   
        }
        if(row + 1 < 8 && col-1 > -1 && row + 2 < 8 && col -2 > -1 && cur_board.board[row +1][col-1].color == 'b'&& cur_board.board[row+2][col-2].color == '.'){
            //cout << " J king w move down left"<< endl;
            cur_board._remove(row + 1,col-1);
            cur_board.move_piece(row,col,row+2,col-2);
            cur_board.path.push_back(make_pair(row+2,col-2));
            Jump(boards,cur_board,row+2, col-2);
        }
        if(row + 1 < 8 && col+1 < 8 && row +2 < 8 && col +2 < 8 && cur_board.board[row +1][col+1].color== 'b' &&  cur_board.board[row +2][col+2].color == '.'){
            //cout << " J king w move down right"<< endl;
            cur_board._remove(row+1,col+1);
            cur_board.move_piece(row,col,row+2,col+2);
            cur_board.path.push_back(make_pair(row+2,col+2));
            Jump(boards,cur_board,row+2, col+2);
        }

    }

    if(cur_board.board[row][col].color == 'b' && cur_board.board[row][col].king){

        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'w' && cur_board.board[row-2][col-2].color == '.'){
            //cout << "J king b move up left"<< endl;
            cur_board._remove(row -1,col-1);
            cur_board.move_piece(row,col,row-2,col-2);
            cur_board.path.push_back(make_pair(row-2,col-2));
            Jump(boards,cur_board,row -2, col-2);
        }
        if(row -1 > -1 && col+1 < 8 && row -2 > -1 && col + 2 < 8 && cur_board.board[row -1][col+1].color== 'w' &&  cur_board.board[row-2][col+2].color == '.'){
            //cout << "J king b move up right"<< endl;
            cur_board._remove(row-1,col+1);
            cur_board.move_piece(row,col,row-2,col+2);
            cur_board.path.push_back(make_pair(row-2,col+2));
            Jump(boards,cur_board,row-2, col+2); 
        }
        if(row + 1 < 8 && col-1 > -1 && row + 2 < 8 && col -2 > -1 && cur_board.board[row +1][col-1].color == 'w'&& cur_board.board[row+2][col-2].color == '.'){
            //cout << " J king b move down left"<< endl;
            cur_board._remove(row + 1,col-1);
            cur_board.move_piece(row,col,row+2,col-2);
            cur_board.path.push_back(make_pair(row+2,col-2));
            Jump(boards,cur_board,row+2, col-2);
        }
        if(row + 1 < 8 && col+1 < 8 && row +2 < 8 && col +2 < 8 && cur_board.board[row +1][col+1].color== 'w' &&  cur_board.board[row +2][col+2].color == '.'){
            //cout << " J king b move down right"<< endl;
            cur_board._remove(row+1,col+1);
            cur_board.move_piece(row,col,row+2,col+2);
            cur_board.path.push_back(make_pair(row+2,col+2));
            Jump(boards,cur_board,row+2, col+2);
        }

    }



    return;

}

void Board::Move(vector<Board>& boards,Board cur_board, int row, int col){

    if(cur_board.board[row][col].color == 'w' || cur_board.board[row][col].king){
        Board wleft = cur_board;
        if(row -1 > -1 && col-1 > -1 && wleft.board[row -1][col-1].color == '.'){
            wleft.move_piece(row,col,row -1,col-1);
            wleft.path.push_back(make_pair(row-1,col-1));
            boards.push_back(wleft);
        } 
        Board wright = cur_board;
        if(row -1 > -1 && col+1 < 8 && wright.board[row -1][col+1].color == '.'){
            wright.move_piece(row,col,row -1,col+1);
            wright.path.push_back(make_pair(row -1,col+1));
            boards.push_back(wright);
        }
    }
    if( cur_board.board[row][col].color == 'b' || cur_board.board[row][col].king){
        Board bleft = cur_board;
        if(row + 1 < 8 && col-1 > -1 && bleft.board[row +1][col-1].color == '.'){
            bleft.move_piece(row,col,row +1,col-1);
            bleft.path.push_back(make_pair(row+1,col-1));
            boards.push_back(bleft);
        }
        Board bright = cur_board;
        if(row + 1 < 8 && col+1 < 8 && bright.board[row +1][col+1].color == '.'){
            bright.move_piece(row,col,row +1,col+1);
            bright.path.push_back(make_pair(row+1,col+1));
            boards.push_back(bright);
        }

    }

    return;
}

vector<Board> Game::get_all_moves(Board board, char color){
    vector<Board> moves;
    vector<pair<int,int>> all_pieces = board.get_all_piece(color);
    // bool forceJump = false;
    for(auto piece:all_pieces){
        Board temp = board;
        if(temp.canJump(temp,piece.first,piece.second)){
            // cout <<"can Jump" << endl;
            temp.path.push_back(make_pair(piece.first,piece.second));
            temp.Jump(moves,temp,piece.first,piece.second);
        }

    }
    if(!moves.empty()) {
        return moves;
    }

    for(auto piece:all_pieces){
        Board temp = board;
        if(temp.canMove(temp,piece.first,piece.second)){
            temp.path.push_back(make_pair(piece.first,piece.second));
            temp.Move(moves,temp,piece.first,piece.second);  
        }
    }
       
    return moves;
}



Board Game::min_max(Board board , int depth, char player,float alp,float beta ){
    if (depth ==0 || board.winner() != ' '){
        return board;
    }
    if(player == board.max_color){
        float MAX_eval = INT_MIN;
        Board best_move= board;
        vector<Board> all_moves = get_all_moves(board,board.max_color);
        for(auto move: all_moves){
            Board evaluation = min_max(move, depth -1,board.min_color,alp,beta);
            MAX_eval = max( MAX_eval, evaluation.evaluation());
            alp = max (alp , MAX_eval);
            if(MAX_eval == evaluation.evaluation()){
                best_move = move;
            }
            if( alp >= beta) break;
        }
        return best_move;
        
    }else{
        float MIN_eval = INT_MAX;
        Board best_move= board;
        vector<Board> all_moves = get_all_moves(board,board.min_color);
        for(auto move: all_moves){
            Board evaluation = min_max(move, depth -1,board.max_color,alp,beta);
            MIN_eval = min( MIN_eval, evaluation.evaluation());
            beta = min ( beta , MIN_eval);
            if(MIN_eval == evaluation.evaluation()){
                best_move = move;
            }
            
            if( alp >= beta) break;
        }
        return best_move;
    }
}


Board Game::min_max3(Board board , int depth, char player,int alp,int beta ){
    if (depth ==0 || board.winner() != ' '){
        return board;
    }
    if(player == board.max_color){
        int MAX_eval = INT_MIN;
        Board best_move= board;
        vector<Board> all_moves = get_all_moves(board,board.max_color);
        for(auto move: all_moves){
            Board evaluation = min_max(move, depth -1,board.min_color,alp,beta);
            MAX_eval = max( MAX_eval, evaluation.evaluationII());
            alp = max (alp , MAX_eval);
            if(MAX_eval == evaluation.evaluationII()){
                best_move = move;
            }
            if( alp >= beta) break;
        }
        return best_move;
        
    }else{
        int MIN_eval = INT_MAX;
        Board best_move= board;
        vector<Board> all_moves = get_all_moves(board,board.min_color);
        for(auto move: all_moves){
            Board evaluation = min_max(move, depth -1,board.max_color,alp,beta);
            MIN_eval = min( MIN_eval, evaluation.evaluationII());
            beta = min ( beta , MIN_eval);
            if(MIN_eval == evaluation.evaluationII()){
                best_move = move;
            }
            
            if( alp >= beta) break;
        }
        return best_move;
    }
}



string Game::path_trans(int row, int col){
    string ans;
    ans+= 'a' + col;
    ans+= to_string((row - 8) *(-1));
    return ans;
}

void Game::createOutput(Board& board,Board&res_board){
    outfile.open("output.txt");
    int start_left = board.b_left + board.w_left;
    // cout << start_left << endl;
    if(!res_board.path.empty()&&res_board.path.size()> 2){
        
        for(int i = 0; i < res_board.path.size();i++){
            
            string out = "J ";
            if(i < res_board.path.size()-1){
                cout << "J" << " ";
                cout<<path_trans(res_board.path[i].first,res_board.path[i].second)<<" ";
                out+=path_trans(res_board.path[i].first,res_board.path[i].second);
                out+=" ";
                cout<<path_trans(res_board.path[i+1].first,res_board.path[i+1].second)<<endl;
                out+= path_trans(res_board.path[i+1].first,res_board.path[i+1].second);
                outfile << out;
                outfile << endl;
            }
        }
    }else if (!res_board.path.empty()&&res_board.path.size()<= 2){
        int end_left = res_board.b_left + res_board.w_left;
        string out = " ";
        if(start_left == end_left){
            cout << "E" << " ";
            out = "E ";
        }else{
            cout << "J" << " ";
            out ="J ";
        }
        cout<<path_trans(res_board.path[0].first,res_board.path[0].second)<<" ";
        out+=path_trans(res_board.path[0].first,res_board.path[0].second);
        out+=" ";
        cout<<path_trans(res_board.path[1].first,res_board.path[1].second)<<endl;;
        out+=path_trans(res_board.path[1].first,res_board.path[1].second);
        outfile << out;
    }
}