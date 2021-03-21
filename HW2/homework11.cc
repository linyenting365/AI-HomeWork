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

Piece Board::get_piece(int row,int col){
    return board[row][col];
}



void Board::_remove(int row, int col){
    board[row][col].color = '.';
    board[row][col].king = false;
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
    if(cur_board.board[row][col].color == 'w'){
        //cout << "in w can jump" << endl;
        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'b' && cur_board.board[row-1][col-1].color == '.'){
            return true;
        }
        if(row -1 > -1 && col+1 < 8 && row -2 > -1 && col + 2 < 8 && cur_board.board[row -1][col+1].color== 'b' &&  cur_board.board[row-2][col+2].color == '.'){
            return true;
        }

    }
    if(cur_board.board[row][col].color == 'b'){
        // cout << "in b can jump" << endl;
        if(row + 1 < 8 && col-1 > -1 && row + 2 < 8 && col -2 > -1 && cur_board.board[row +1][col-1].color == 'w'&& cur_board.board[row+2][col-2].color == '.'){
            return true;
        }
        if(row + 1 < 8 && col+1 < 8 && row +2 < 8 && col +2 < 8 && cur_board.board[row +1][col+1].color== 'w' &&  cur_board.board[row +2][col+2].color == '.'){
            return true;
        }

    }

    if(cur_board.board[row][col].color == 'w' && cur_board.board[row][col].king){

        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'b' && cur_board.board[row-1][col-1].color == '.'){
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

        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'w' && cur_board.board[row-1][col-1].color == '.'){
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

    //cout << "in canMove end" << endl;
    return false;
}





void Board::Jump(vector<Board>& boards,Board cur_board,int row ,int col){
    
    if( !canJump(cur_board,row,col)){
        boards.push_back(cur_board);
        return;
    }


    if(cur_board.board[row][col].color == 'w'){
        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'b' && cur_board.board[row-1][col-1].color == '.'){
            // cout << "J w move left"<< endl;
            cur_board._remove(row -1,col-1);
            cur_board.move_piece(row,col,row-2,col-2);
            cur_board.path.push_back(make_pair(row-2,col-2));
            Jump(boards,cur_board,row -2, col-2);
        }
        if(row -1 > -1 && col+1 < 8 && row -2 > -1 && col + 2 < 8 && cur_board.board[row -1][col+1].color== 'b' &&  cur_board.board[row-2][col+2].color == '.'){
            //cout << "J w move right"<< endl;
            cur_board._remove(row-1,col+1);
            cur_board.move_piece(row,col,row-2,col+2);
            cur_board.path.push_back(make_pair(row-2,col+2));
            Jump(boards,cur_board,row-2, col+2);                
        }
    }
    if(cur_board.board[row][col].color == 'b'){
        if(row + 1 < 8 && col-1 > -1 && row + 2 < 8 && col -2 > -1 && cur_board.board[row +1][col-1].color == 'w'&& cur_board.board[row+2][col-2].color == '.'){
            //cout << " J b move left"<< endl;
            cur_board._remove(row + 1,col-1);
            cur_board.move_piece(row,col,row+2,col-2);
            cur_board.path.push_back(make_pair(row+2,col-2));
            Jump(boards,cur_board,row+2, col-2);
        }
        if(row + 1 < 8 && col+1 < 8 && row +2 < 8 && col +2 < 8 && cur_board.board[row +1][col+1].color== 'w' &&  cur_board.board[row +2][col+2].color == '.'){
            //cout << " J b move right"<< endl;
            cur_board._remove(row+1,col+1);
            cur_board.move_piece(row,col,row+2,col+2);
            cur_board.path.push_back(make_pair(row+2,col+2));
            Jump(boards,cur_board,row+2, col+2);
        }
    }

        if(cur_board.board[row][col].color == 'w' && cur_board.board[row][col].king){

        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'b' && cur_board.board[row-1][col-1].color == '.'){
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

        if(row -1 > -1 && col-1 > -1 && row -2 > -1 && col -2 > -1 && cur_board.board[row -1][col-1].color == 'w' && cur_board.board[row-1][col-1].color == '.'){
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
            //cout << "E w move left"<< endl;

            wleft.move_piece(row,col,row -1,col-1);
            wleft.path.push_back(make_pair(row-1,col-1));
            //wleft.print_board();
            boards.push_back(wleft);
        } 
        Board wright = cur_board;
        if(row -1 > -1 && col+1 < 8 && wright.board[row -1][col+1].color == '.'){
            //cout << "E w move right"<< endl;

            wright.move_piece(row,col,row -1,col+1);
            wright.path.push_back(make_pair(row -1,col+1));
            //wright.print_board();
            boards.push_back(wright);
        }
    }
    if( cur_board.board[row][col].color == 'b' || cur_board.board[row][col].king){
        Board bleft = cur_board;
        if(row + 1 < 8 && col-1 > -1 && bleft.board[row +1][col-1].color == '.'){
            //cout << "E b move left"<< endl;

            bleft.move_piece(row,col,row +1,col-1);
            bleft.path.push_back(make_pair(row+1,col-1));
            //bleft.print_board();
            boards.push_back(bleft);
        }
        Board bright = cur_board;
        if(row + 1 < 8 && col+1 < 8 && bright.board[row +1][col+1].color == '.'){
            //cout << "E b move right"<< endl;
            bright.move_piece(row,col,row +1,col+1);
            bright.path.push_back(make_pair(row+1,col+1));
            //bright.print_board();
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
        //cout << piece.first << piece.second << endl;
        if(board.canJump(board,piece.first,piece.second)){
            board.Jump(moves,board,piece.first,piece.second);
        }

    }
    if(!moves.empty()) {
        return moves;
    }

    for(auto piece:all_pieces){
        if(board.canMove(board,piece.first,piece.second)){
            board.Move(moves,board,piece.first,piece.second);  
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






string Game::path_trans(int row, int col){
    string ans;
    ans+= 'a' +col;
    ans+=to_string((row - 8) *(-1));
    return ans;
}




