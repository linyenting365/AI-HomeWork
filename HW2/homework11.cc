#include "homework11.h"

fstream infile;
ofstream outfile;




vector<vector<Piece>> Board::input(){

    infile.open("input.txt");
    infile >> Pmethod;
    // cout << Pmethod << endl;
    string Pcolor;
    infile >> Pcolor;
    if(Pcolor == "BLACK"){
        max_color = 'b';
        min_color = 'w';
    }else{
        max_color = 'w';
        min_color = 'b';
    }
    // cout << Pcolor << endl;
    infile >> Pclock;
    // cout << Pclock << endl;
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
    Piece temp;
    temp= board[row][col];
    board[row][col] = board[piece.row][piece.col];
    board[piece.row][piece.col] = temp;

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
            // cout << board[i][j].color << " ";
            if(board[i][j].king){
                cout << char(toupper(board[i][j].color)) << " ";
            }else{
                cout << board[i][j].color << " ";
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
            // cout <<"before remove"<< w_left << " ";
            w_left--;
            // cout <<"after remove"<< w_left << endl;
            if(piece.king){
                w_king--;
            }
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

unordered_map<string,vector<Piece>> Board::get_valid_move(Piece piece){
    unordered_map<string,vector<Piece>> moves;
    int left = piece.col - 1;
    int right = piece.col + 1;
    int row = piece.row;
    // cout<<row <<" "<< left << " " << right << endl;
    Piece skip;
    if(piece.color == 'w'|| piece.king){
        Wleft_moves(moves,piece.king ,row-1 , max(row-3, -1), -1,piece.color ,left,skip);
        Wright_moves(moves,piece.king,row-1, max(row-3, -1), -1,piece.color ,right,skip);
    }
    
    if(piece.color == 'b'||piece.king){
        Bleft_moves(moves,piece.king,row + 1, min(row+3, 8), 1,piece.color ,left,skip);
        Bright_moves(moves,piece.king,row + 1, min(row+3, 8), 1,piece.color ,right,skip);
    }
    return moves;
}

void Board::Bleft_moves(unordered_map<string,vector<Piece>> &moves,bool is_king,int start, int stop, int step, char color, int left,Piece skipped){
    Piece last;
    // cout << start << " ";
    // cout << stop << endl;
    cout << "Bleft" << endl;
    for(int i = start ; i <= stop; ++i){
        // cout << "left_move:" << i << endl;
        if(left < 0){
            break;
        }

        Piece cur_piece = board[i][left];

        // cout << cur_piece.row << cur_piece.col << endl;
        // cout << cur_piece.color << color << endl;
        if(cur_piece.color=='.'){
            // cout <<  "left:is_blank" <<endl;
            if(!skipped.is_empty && last.is_empty){
                // cout << "left:skip one" << endl;
                break;
            }else if (!skipped.is_empty){
                // cout << "left:double skip" << " ";
                moves[to_string(i)+to_string(left)].push_back(last);
                moves[to_string(i)+to_string(left)].push_back(skipped);
            }else{
                // cout << "left:no need to skip" << " ";
                // cout << i  << left << endl;
                moves[to_string(i)+to_string(left)].push_back(last);
            }
            int row = stop;

            if(!last.is_empty){
                // cout << "left:last is not empty" << " ";
                row = min(i + 3, 7);
                // if(step == -1){
                //     row = max(i - 3, 0);
                //     // cout << row << endl;
                // }else{
                //     // cout << i <<" ";
                //     row = min(i + 3, 7);
                //     // cout << row << endl;
                // }
            }else{
                break;
            }

            // cout <<"before go " << i + step <<" "<< left <<endl;
            Bleft_moves(moves,is_king,i + step, row, step,color,left - 1,last);
            Bright_moves(moves,is_king,i + step, row, step,color,left + 1,last);
            if(is_king){
                row = max(i - 3, 0);
                Wleft_moves(moves,is_king,i-step, row, -step,color,left - 1,last);
            }
            break;

        }else if(cur_piece.color == color){
            // cout << cur_piece.color << endl;
            // color is same as previous step
            break;
        }else{
            // cout<< last.is_empty <<  endl;
            // cout << last.color << cur_piece.color << endl; 
            if(!last.is_empty && last.color == cur_piece.color) break;
            last = cur_piece;
            // cout << last.color << endl;
        }
        left --;
    }
    return;

}


void Board::Bright_moves(unordered_map<string,vector<Piece>> &moves,bool is_king,int start, int stop, int step, char color, int right,Piece skipped){
    Piece last;
    cout << "Bright" << endl;
    for(int i = start; i <= stop; ++i){
        // cout << "right_move:" << i << endl;
        if(right >=8){
            break;
        }
        // cout << i << " " << right << endl;
        Piece cur_piece = board[i][right];
        // cout << cur_piece.color<< endl;
        if(cur_piece.color=='.'){
            // cout << "right:is_blank" <<endl;
            if(!skipped.is_empty && last.is_empty){
                // cout << "right:skip one" << endl;
                break;
            }else if (!skipped.is_empty){
                // cout << "right:double skip" << " ";
                moves[to_string(i)+to_string(right)].push_back(last);
                moves[to_string(i)+to_string(right)].push_back(skipped);
            }else{
                moves[to_string(i)+to_string(right)].push_back(last);
            }
            int row=stop;
            if(!last.is_empty){
                row = min(i + 3, 7);    
                // if(step == -1){
                //     row = max(i - 3, 0);
                // }else{
                //     row = min(i + 3, 7);
                // }
            }else{
                break;
            }

            Bleft_moves(moves,is_king,i + step, row, step,color,right - 1,last);
            Bright_moves(moves,is_king,i + step, row, step,color,right + 1,last);
            if(is_king){
                row = max(i - 3, 0);
                Wright_moves(moves,is_king,i -step, row, -step,color,right + 1,last); 

            }
        break;

        }else if(cur_piece.color == color){
            // color is same as previous step
            
            break;
        }else{
            // cout << "can jump" << endl;
            if(!last.is_empty && last.color == cur_piece.color) break;
            last = cur_piece;
        }
        right++;
    }

    return;
}

void Board::Wleft_moves(unordered_map<string,vector<Piece>> &moves,bool is_king,int start, int stop, int step, char color, int left,Piece skipped){
    Piece last;
    cout << "Wleft" << endl;
    // cout << start << " ";
    // cout << stop << endl;

    for(int i = start ; i >= stop; --i){
        // cout << "left_move:" << i << endl;
        if(left < 0){
            break;
        }

        Piece cur_piece = board[i][left];
        if(cur_piece.color=='.'){
            // cout <<  "left:is_blank" <<endl;
            if(!skipped.is_empty && last.is_empty){
                // cout << "left:skip one" << endl;
                break;
            }else if (!skipped.is_empty){
                // cout << "left:double skip" << " ";
                moves[to_string(i)+to_string(left)].push_back(last);
                moves[to_string(i)+to_string(left)].push_back(skipped);
            }else{
                // cout << "left:no need to skip" << " ";
                // cout << i  << left << endl;
                moves[to_string(i)+to_string(left)].push_back(last);
            }
            int row = stop;

            if(!last.is_empty){
                // cout << "left:last is not empty" << " ";
                row = max(i - 3, 0);  
                // if(step == -1){
                //     row = max(i - 3, 0);
                //     // cout << row << endl;
                // }else{
                //     // cout << i <<" ";
                //     row = min(i + 3, 7);
                //     // cout << row << endl;
                // }
            }else{
                break;
            }

            // cout <<"before go " << i + step <<" "<< left <<endl;
            Wleft_moves(moves,is_king,i+ step, row, step,color,left - 1,last);
            Wright_moves(moves,is_king,i + step, row, step,color,left + 1,last);
            if(is_king){
                row = min(i + 3, 7);
                Bleft_moves(moves,is_king,i - step, row, -step,color,left - 1,last);
            }
            break;

        }else if(cur_piece.color == color){
            // color is same as previous step
            break;
        }else{
            if(!last.is_empty && last.color == cur_piece.color) break;
            last = cur_piece;
            // cout << last.color << endl;
        }
        left --;
    }
    return;

}


void Board::Wright_moves(unordered_map<string,vector<Piece>> &moves,bool is_king,int start, int stop, int step, char color, int right,Piece skipped){
    Piece last;
    // cout<< "hi" << endl;
    // cout << start << stop << endl;
    cout << "Wright" << endl;
    for(int i = start; i >= stop; --i){
        // cout << "right_move:" << i << endl;
        if(right >=8){
            break;
        }
        // cout << i << " " << right << endl;
        Piece cur_piece = board[i][right];
        // cout << cur_piece.color<< endl;
        if(cur_piece.color=='.'){
            // cout << "right:is_blank" <<endl;
            if(!skipped.is_empty && last.is_empty){
                // cout << "right:skip one" << endl;
                break;
            }else if (!skipped.is_empty){
                // cout << "right:double skip" << " ";
                moves[to_string(i)+to_string(right)].push_back(last);
                moves[to_string(i)+to_string(right)].push_back(skipped);
            }else{
                moves[to_string(i)+to_string(right)].push_back(last);
            }
            int row=stop;
            if(!last.is_empty){
                row = max(i - 3, 0);  
                // if(step == -1){
                //     row = max(i - 3, 0);
                // }else{
                //     row = min(i + 3, 7);
                // }
            }else{
                break;
            }

            // cout <<"before go " << row + step <<" "<< right <<endl;
            Wleft_moves(moves,is_king,i + step, row, step,color,right - 1,last);
            Wright_moves(moves,is_king,i + step, row, step,color,right + 1,last);
            if(is_king){
                row = min(i + 3, 7);
                Bright_moves(moves,is_king,i - step, row, -step,color,right + 1,last);
            }
        break;

        }else if(cur_piece.color == color){
            // color is same as previous step
            break;
        }else{
            // cout << "can jump" << endl;
            if(!last.is_empty && last.color == cur_piece.color) break;
            last = cur_piece;
        }
        right++;
    }

    return;
}


Board Game::min_max(Board board , int depth, char player ){
    if (depth ==0 || board.winner() != ' '){
        // cout<< board.evaluation() << endl;
        // board.print_board();
        return board;
    }
    // cout << board.max_color << endl;
    if(player == board.max_color){
        float MAX_eval = INT_MIN;
        Board best_move;
        vector<Board> all_moves = get_all_moves(board,board.max_color);
        for(auto move: all_moves){
            //  cout<< "depth" <<depth <<endl;
            // move.print_board();
            Board evaluation = min_max(move, depth -1,board.max_color);
           
            // cout <<"cur_max"<<MAX_eval <<"evaluate:"<<evaluation.evaluation() << endl;
            MAX_eval = max( MAX_eval, evaluation.evaluation());
            // cout<<"Max:" <<MAX_eval << endl;
            if(MAX_eval == evaluation.evaluation()){
                best_move = move;
            }
        }
        return best_move;
        
    }else{
        float MIN_eval = INT_MAX;
        Board best_move;
        vector<Board> all_moves = get_all_moves(board,board.min_color);
        // cout << board.max_color << endl;
        for(auto move: all_moves){
            // cout<< "depth" <<depth <<endl;
            // move.print_board();

            Board evaluation = min_max(move, depth -1,board.min_color);
           
            // cout <<"cur_min: "<<MIN_eval <<"  evaluate: "<<evaluation.evaluation() << endl;
            MIN_eval = min( MIN_eval, evaluation.evaluation());
            // cout<<"Min:" <<MIN_eval << endl;
            if(MIN_eval == evaluation.evaluation()){
                best_move = move;
            }
        }
        return best_move;
    }
}

Board Game::simulation_move(Piece piece, string move,Board board, vector<Piece>&skipped){
    board.move_piece( piece, move[0] - '0' , move[1] - '0');
    board.remove(skipped);
    // cout <<"# of left:" <<  board.w_left <<" "<<  board.b_left<< endl;
    return board;
}

vector<Board> Game::get_all_moves(Board board, char color){
    vector<Board> moves;
    vector<Piece> all_pieces = board.get_all_piece(color);
    for(auto piece:all_pieces){
       unordered_map<string,vector<Piece>> valid_move = board.get_valid_move(piece);
       for(auto move: valid_move){
           Board temp_board = board;
           Piece temp_piece= temp_board.get_piece(piece.row, piece.col);
           Board new_board = simulation_move(temp_piece, move.first,temp_board,move.second);
           new_board.print_board();
           cout << "-----------------------"<< endl;
           moves.push_back(new_board); 
        } 
    }

    return moves;
    
}




