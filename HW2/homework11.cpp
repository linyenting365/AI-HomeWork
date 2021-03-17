#include "homework11.h"


int main(int argc, char const * argv[])
{
    Game game;
    Board board;

    board.board = board.input();

    //play game
    board.print_board();
    //test move
    Piece piece = board.get_piece(6,3);
    Piece piece1 = board.get_piece(3,4);

    // vector<Board> valid_move = board.get_valid_move(piece);
    //    for(auto n:valid_move){
    //     cout << n.evaluation() << endl;
    //     n.print_board();
    // }

    // vector<Board> all_moves= game.get_all_moves(board,'b');
    // for(auto n:all_moves){
    //     cout << n.evaluation() << endl;
    //     n.print_board();
    //     // vector<Board> valid= game.get_all_moves(n,'w');
    //     // for(auto x:valid){
    //     //     cout << x.evaluation() << endl;
    //     //     x.print_board();
    //     //     vector<Board> move= game.get_all_moves(x,'b');
    //     //     for(auto y:move){
    //     //         cout << y.evaluation() << endl;
    //     //         y.print_board();
    //     //     }
    //     //     cout << "----------------"<< endl;            
    //     // }
    //     cout << "----------------"<< endl;

    // }

    
    vector<Board> valid= game.get_all_moves(board,'b');
    for(auto n:valid){
        cout << n.evaluation() << endl;
        n.print_board();
    }
    // for(auto move: valid_move){

    //     Board temp_board = board;
    //     Piece temp_piece= temp_board.get_piece(piece.row, piece.col);
    //     Board new_board = game.simulation_move(temp_piece, move.first,temp_board,move.second);
    //     new_board.print_board();
    //     cout << "-----------------------"<< endl;
    // } 
    // unordered_map<string,vector<Piece>> valid_move = board.get_valid_move(piece);
    // cout << valid_move.size() << endl;
    // for(auto n:valid_move){
    //     cout << n.first <<" " << endl;
    //     for(auto k:n.second){
    //         cout << "row:" << k.row << "col:" << k.col << " ";
    //         cout << k.is_empty << " ";
    //         cout << k.color << endl;
    //     }
    // }
    // vector<Piece> all_pieces = board.get_all_piece('b');

    // cout << board.b_left << " " << board.w_left << endl;
    // for(auto n:all_pieces){
    //     cout << n.color << " " << "row:" << n.row <<" " <<"col:" << n.col << endl;
    // }
    // vector<Board> all_moves= game.get_all_moves(board,'b');
    // vector<Board> all_moves= game.get_all_moves(board,'w');
    // for(auto n:all_moves){
    //     n.print_board();
    // }

    // board.move_piece(piece,7,0);
    // board.print_board();
    // Piece piece2 = board.get_piece(7,0);
    // cout << piece2.row << piece2.col <<endl;



    if(board.Pmethod == "SINGLE"){
        // Board board1 = game.min_max(board,7,board.max_color);
        // cout << "______________________" << endl;
        // board1.print_board();
        // for(int i = 0; i < board1.path.size();i++){
        //     if(i < board1.path.size()-1){
        //         cout<<game.path_trans(board1.path[i].first,board1.path[i].second)<<" ";
        //         cout<<game.path_trans(board1.path[i+1].first,board1.path[i+1].second)<<endl;;
        //     }
        // }

    } else {

        // whole game

    }
    

    return 0;
}



