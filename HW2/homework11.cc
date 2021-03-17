#include "homework11.h"

fstream infile;
ofstream outfile;




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
            board[i][j].row = i;
            board[i][j].col = j;
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



void Board::move_piece( Piece piece , int row , int col){
    char temp_color;
    bool temp_king;
    temp_color = board[row][col].color;
    temp_king = board[row][col].king;
    board[row][col].color = board[piece.row][piece.col].color;
    board[row][col].king = board[piece.row][piece.col].king;
    board[piece.row][piece.col].king = temp_king;
    board[piece.row][piece.col].color =temp_color;

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

Piece Board::get_piece(int row,int col){
    return board[row][col];
}

void Board::remove(vector<Piece>& pieces){
    for(auto piece:pieces){
        if(piece.is_empty){
            break;
        }else{
            board[piece.row][piece.col].color = '.';
            board[piece.row][piece.col].king = false;
        }
        if(piece.color == 'b'){
            b_left--;
            if(piece.king){
                b_king--;
            }
        }else if(piece.color == 'w'){
            w_left--;
            if(piece.king){
                w_king--;
            }
        }


    }
}

void Board::_remove(Piece piece){
    board[piece.row][piece.col].color = '.';
    board[piece.row][piece.col].king = false;
    if(piece.color == 'b'){
        b_left--;
        if(piece.king){
            b_king--;
        }
    }else if(piece.color == 'w'){
        w_left--;
        if(piece.king){
            w_king--;
        }
    }
}

vector<Piece> Board::get_all_piece(char color){
    vector<Piece> get_all;

    for( int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(board[i][j].color == color){
                get_all.push_back(board[i][j]);
            }
        }
    }

    return get_all;

}

vector<Board> Board::get_valid_move(Piece piece){
    vector<Board> moves;
    int left = piece.col - 1;
    int right = piece.col + 1;
    int row = piece.row;
    int count =0;
    vector<Piece> skip;
    // Piece skip;
    vector<Board> all_valid_board;
    Board new_board;
    // new_board.print_board();
    new_board.board = board;
    new_board.max_color = max_color;
    new_board.min_color = min_color;
    new_board.b_left = b_left;
    new_board.w_left = w_left;
    new_board.b_king = b_king;
    new_board.w_king = w_king;
    new_board.path.push_back(make_pair(piece.row,piece.col));

    // cout <<"valid_move:"<< new_board.b_left << new_board.w_left << endl;
    if(piece.color == 'w'|| piece.king){
        Wleft_moves(moves,new_board,piece.king,row - 1, max(row-3, -1), -1,piece ,left,skip);
        Wright_moves(moves,new_board,piece.king,row - 1, max(row-3, -1), -1,piece ,right,skip);
    }
    
    if(piece.color == 'b'||piece.king){
        Bleft_moves(moves,new_board,piece.king,row + 1, min(row+3, 8), 1,piece ,left,skip);
        Bright_moves(moves,new_board,piece.king,row + 1, min(row+3, 8), 1,piece ,right,skip);
    }
    // cout <<"valid_end" <<endl;
    return moves;
}

void Board::Bleft_moves(vector<Board>& moves,Board res_board,bool is_king,int start, int stop, int step, Piece start_piece, int left,vector<Piece> skipped){
    Piece last;
    // cout<< "Bleft" << endl;
    for(int i = start ; i <= stop; ++i){
        if(left < 0 || i >= 8 || i < 0){
            break;
        }

        Piece cur_piece = res_board.board[i][left];
        if(cur_piece.color=='.'){
            if(!skipped.empty() && last.is_empty){
                // cout << "skip one" << endl;
                //moves.push_back(res_board);
                break;
            }else if (!skipped.empty()){
                // cout << "skip two" << endl;
                // res_board._remove(last);
                // cout << last.row << last.col << endl;
                for(auto sk:skipped){
                    res_board._remove(sk);
                }
                res_board.move_piece(start_piece,i,left);
                res_board.path.push_back(make_pair(i,left));
                moves.push_back(res_board);
                start_piece.row = i;
                start_piece.col = left;
            }else{
                res_board.move_piece(start_piece, i,left);
                res_board.path.push_back(make_pair(i,left));
                moves.push_back(res_board);
                start_piece.row = i;
                start_piece.col = left;
            }
            int row = stop;
            // cout << "before got" << endl;
            if(!last.is_empty){

                row = min(i + 3, 7);
                Bleft_moves(moves,res_board,is_king,i + step, row, step,start_piece,left - 1,skipped);
                Bright_moves(moves,res_board,is_king,i + step, row, step,start_piece,left + 1,skipped);
                if(is_king){
                    row = max(i - 3, 0);
                    Wleft_moves(moves,res_board,is_king,i - step, row, -step,start_piece,left - 1,skipped);
                    Wright_moves(moves,res_board,is_king,i - step, row, -step,start_piece,left + 1,skipped);
                }                
            }
            // moves.push_back(res_board);
            break;

        }else if(cur_piece.color == start_piece.color){
            break;
        }else{
            if(!last.is_empty && last.color == cur_piece.color) break;
            last = cur_piece;
            skipped.push_back(cur_piece);
        }
        left --;
    }
    return;

}


void Board::Bright_moves(vector<Board>& moves,Board res_board,bool is_king,int start, int stop, int step, Piece start_piece, int right,vector<Piece> skipped){
    Piece last;
    // cout << "Bright" << endl;
    for(int i = start; i <= stop; ++i){
        if(right >=8 || i >=8 || i < 0){
            // if(skipped.size() > 1){
            //     moves.push_back(res_board);
            // }
            break;
        }
        Piece cur_piece = res_board.board[i][right];
        if(cur_piece.color=='.'){
            if(!skipped.empty() && last.is_empty){
                // moves.push_back(res_board);
                break;
            }else if (!skipped.empty()){
                // res_board._remove(last);
                for(auto sk:skipped){
                    res_board._remove(sk);
                }
                res_board.move_piece(start_piece,i,right);
                res_board.path.push_back(make_pair(i,right));
                moves.push_back(res_board);
                //cout <<"b_left:"<< b_left<< " "<< "w_left:"<< w_left <<" "<<res_board.evaluation() <<endl;
                // res_board.print_board(); 
                start_piece.row = i;
                start_piece.col = right;
            }else{
                res_board.move_piece(start_piece, i,right);
                res_board.path.push_back(make_pair(i,right));
                //cout <<"b_left:"<< b_left<< " "<< "w_left:"<< w_left <<" "<<res_board.evaluation() <<endl;
                // res_board.print_board();
                moves.push_back(res_board);
                start_piece.row = i;
                start_piece.col = right;
            }
            int row=stop;
            if(!last.is_empty){
                row = min(i + 3, 7);
                Bleft_moves(moves,res_board,is_king,i + step, row, step,start_piece,right - 1,skipped);
                Bright_moves(moves,res_board,is_king,i + step, row, step,start_piece,right + 1,skipped);
                if(is_king){
                    row = max(i - 3, 0);
                    Wleft_moves(moves,res_board,is_king,i - step, row, -step,start_piece,right - 1,skipped);
                    Wright_moves(moves,res_board,is_king,i - step, row, -step,start_piece,right + 1,skipped);
                }                   
            }

            break;

        }else if(cur_piece.color == start_piece.color){
            break;
        }else{
            if(!last.is_empty && last.color == cur_piece.color) break;
            last = cur_piece;
            skipped.push_back(cur_piece);
        }
        right++;
    }

    return;
}

void Board::Wleft_moves(vector<Board>& moves,Board res_board,bool is_king,int start, int stop, int step, Piece start_piece, int left,vector<Piece> skipped){
    Piece last;
    // cout << "Wleft" << endl;
    for(int i = start ; i >= stop; --i){
        if(left < 0 || i < 0 || i >=8){
            // if(skipped.size() > 1){
            //     moves.push_back(res_board);
            // }            
            break;
        }

        Piece cur_piece = res_board.board[i][left];
        if(cur_piece.color=='.'){
            if(!skipped.empty() && last.is_empty){
                // moves.push_back(res_board);
                break;
            }else if (!skipped.empty()){
                //res_board._remove(last);
                for(auto sk:skipped){
                    res_board._remove(sk);
                }
                res_board.move_piece(start_piece,i,left);
                res_board.path.push_back(make_pair(i,left));
                moves.push_back(res_board);
                //cout <<"b_left:"<< b_left<< " "<< "w_left:"<< w_left <<" "<<res_board.evaluation() <<endl;
                // res_board.print_board();
                start_piece.row = i;
                start_piece.col = left;
            }else{
                res_board.move_piece(start_piece, i,left);
                res_board.path.push_back(make_pair(i,left));
                //cout <<"b_left:"<< b_left<< " "<< "w_left:"<< w_left <<" "<<res_board.evaluation() <<endl;
                //res_board.print_board();
                moves.push_back(res_board);
                start_piece.row = i;
                start_piece.col = left;
            }
            int row = stop;
            if(!last.is_empty){
                row = max(i - 3, 0);
                Wleft_moves(moves,res_board,is_king,i + step, row, step,start_piece,left - 1,skipped);
                Wright_moves(moves,res_board,is_king,i + step, row, step,start_piece,left + 1,skipped);
                if(is_king){
                    row = min(i + 3, 7);
                    Bleft_moves(moves,res_board,is_king,i - step, row, -step,start_piece,left - 1,skipped);
                    Bright_moves(moves,res_board,is_king,i - step, row, -step,start_piece,left + 1,skipped);
                }  
            }

            break;

        }else if(cur_piece.color == start_piece.color){
            break;
        }else{
            if(!last.is_empty && last.color == cur_piece.color) break;
            last = cur_piece;
            skipped.push_back(cur_piece);
        }
        left --;
    }
    return;

}


void Board::Wright_moves(vector<Board>& moves,Board res_board,bool is_king,int start, int stop, int step, Piece start_piece, int right,vector<Piece> skipped){
    Piece last;
    // cout << "Wright" << endl;
    for(int i = start; i >= stop; --i){
        // cout<< "Wright_for_loop" << endl;
        // res_board.print_board();
        if(right >=8|| i <0 || i >=8){
            // if(skipped.size() > 1){
            //     moves.push_back(res_board);
            // }
            break;
        }
        Piece cur_piece = res_board.board[i][right];
        if(cur_piece.color=='.'){
            if(!skipped.empty() && last.is_empty){
                //moves.push_back(res_board);
                break;
            }else if (!skipped.empty()){
                //res_board._remove(last);
                for(auto sk:skipped){
                    res_board._remove(sk);
                }
                res_board.move_piece(start_piece,i,right);
                res_board.path.push_back(make_pair(i,right));
                // res_board.print_board();
                moves.push_back(res_board);
                // cout <<"b_left:"<< b_left<< " "<< "w_left:"<< w_left <<" "<<res_board.evaluation() <<endl;
                //res_board.print_board();
                start_piece.row = i;
                start_piece.col = right;
            }else{
                res_board.move_piece(start_piece, i,right);
                res_board.path.push_back(make_pair(i,right));
                // cout <<"b_left:"<< b_left<< " "<< "w_left:"<< w_left <<" "<<res_board.evaluation() <<endl;
                // res_board.print_board();
                moves.push_back(res_board);
                start_piece.row = i;
                start_piece.col = right;
            }
            int row=stop;
            if(!last.is_empty){
                row = max(i - 3, 0);
                Wleft_moves(moves,res_board,is_king,i + step, row, step,start_piece,right - 1,skipped);
                Wright_moves(moves,res_board,is_king,i + step, row, step,start_piece,right + 1,skipped);
                if(is_king){
                    row = min(i + 3, 7);
                    Bleft_moves(moves,res_board,is_king,i - step, row, -step,start_piece,right - 1,skipped);
                    Bright_moves(moves,res_board,is_king,i - step, row, -step,start_piece,right + 1,skipped);
                } 
            }

            break;

        }else if(cur_piece.color == start_piece.color){
            break;
        }else{
            if(!last.is_empty && last.color == cur_piece.color) break;
            last = cur_piece;
            // cout <<"jump color:"<< last.color << endl;
            skipped.push_back(cur_piece);
        }
        right++;
    }

    return;
}


Board Game::min_max(Board board , int depth, char player ){
    // cout << player << endl;
    if (depth ==0 || board.winner() != ' '){
        return board;
    }
    // cout << player << endl;
    if(player == board.max_color){
        float MAX_eval = INT_MIN;
        // cout <<  "max" << endl;
        Board best_move= board;
        vector<Board> all_moves = get_all_moves(board,board.max_color);
        for(auto move: all_moves){
            //cout<< "depth_max" <<depth <<endl;
            //move.print_board();
            // cout << board.min_color << endl;
            Board evaluation = min_max(move, depth -1,board.min_color);
            //cout << "b left: " << evaluation.b_left << " "<<"w left: "<< evaluation.w_left << endl;
            //evaluation.print_board();
            //cout <<"cur_max"<<MAX_eval <<"evaluate:"<<evaluation.evaluation() << endl;
            MAX_eval = max( MAX_eval, evaluation.evaluation());
            //cout<<"Max:" <<MAX_eval << endl;
            if(MAX_eval == evaluation.evaluation()){
                best_move = move;
            }
        }
        return best_move;
        
    }else{
        float MIN_eval = INT_MAX;
        Board best_move= board;
        // cout <<  "MIN" << endl;
        vector<Board> all_moves = get_all_moves(board,board.min_color);
        // cout << board.min_color << endl;
        for(auto move: all_moves){
            // cout<< "depth_min" <<depth <<endl;
            // move.print_board();

            Board evaluation = min_max(move, depth -1,board.max_color);
            //cout << "b left: " << evaluation.b_left << " "<<"w left: "<< evaluation.w_left << endl;
            //evaluation.print_board();
            //cout <<"cur_min: "<<MIN_eval <<"  evaluate: "<<evaluation.evaluation() << endl;
            MIN_eval = min( MIN_eval, evaluation.evaluation());
            //cout<<"Min:" <<MIN_eval << endl;
            if(MIN_eval == evaluation.evaluation()){
                best_move = move;
            }
        }
        return best_move;
    }
}


vector<Board> Game::get_all_moves(Board board, char color){
    vector<Board> moves;
    vector<Piece> all_pieces = board.get_all_piece(color);
    for(auto piece:all_pieces){
        Board temp_board = board;
        vector<Board> temp = temp_board.get_valid_move(piece);
        moves.insert(moves.end(), temp.begin(),temp.end());
    }
    return moves;
}

string Game::path_trans(int row, int col){
    string ans;
    ans+= 'a' +col;
    ans+=to_string((row - 8) *(-1));
    return ans;
}




