#include "homework11.h"


int main(int argc, char const * argv[])
{
    Game game;
    Board board;

    board.board = board.input();

    //play game
    board.print_board();
    //test move
    Piece piece = board.get_piece(1,0);
    Piece piece1 = board.get_piece(3,4);
    unordered_map<string,vector<Piece>> valid_move = board.get_valid_move(piece);
    cout << valid_move.size() << endl;
    for(auto n:valid_move){
        cout << n.first <<" " << endl;
        for(auto k:n.second){
            cout << "row:" << k.row << "col:" << k.col << " ";
            cout << k.is_empty << " ";
            cout << k.color << endl;
        }
    }
    // vector<Piece> all_pieces = board.get_all_piece('b');

    // cout << board.b_left << " " << board.w_left << endl;
    // for(auto n:all_pieces){
    //     cout << n.color << " " << "row:" << n.row <<" " <<"col:" << n.col << endl;
    // }
    // vector<Board> all_moves= game.get_all_moves(board,'b');
    // vector<Board> all_moves= game.get_all_moves(board,'b');

    // Board board1 = game.min_max(board,3,'b');
    // board1.print_board();

    if(board.Pmethod == "SINGLE"){
        //single move

    } else {

        // whole game

    }
    

    return 0;
}



