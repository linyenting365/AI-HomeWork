#include "homework11.h"


int main(int argc, char const * argv[])
{

    Board board;

    board.board = board.input();

    //play game
    board.print_board();

    if(board.Pmethod == "SINGLE"){
        // const clock_t start_time = clock();
        // Board board1 = game.min_max(board,10,board.max_color,INT_MIN,INT_MAX);
        // cout << "______________________" << endl;
        // cout << float(clock() - start_time) /CLOCKS_PER_SEC << endl;
        // board1.print_board();
        // if(!board1.path.empty()&&board1.path.size()> 2){
        //     cout << "J" << " ";
        //     for(int i = 0; i < board1.path.size();i++){
        //         if(i < board1.path.size()-1){
        //             cout<<game.path_trans(board1.path[i].first,board1.path[i].second)<<" ";
        //             cout<<game.path_trans(board1.path[i+1].first,board1.path[i+1].second)<<endl;;
        //         }
        //     }
        // }else if (!board1.path.empty()&&board1.path.size()< 2){
        //     cout << "E" << " ";
        //     cout<<game.path_trans(board1.path[0].first,board1.path[0].second)<<" ";
        //     cout<<game.path_trans(board1.path[1].first,board1.path[1].second)<<endl;;
        // }
    } else {

        // whole game

    }
    int k = 0;
    Board host;
    host.board = host.input();
    while(host.winner()== ' ' && k < 150){
        Game game;
        cout << "............................" << endl;
        cout << k << endl;
        host.print_board();
        Board res = game.min_max(host,7,host.max_color,INT_MIN,INT_MAX);
        cout << "----------------------------" << endl;
        res.print_board();
        cout << "----------------------------" << endl;
        host.b_left = res.b_left;
        host.w_left = res.w_left;
        host.w_king = res.w_king;
        host.b_king = res.b_king;
        host.board = res.board;
        host.min_color = res.max_color;
        host.max_color = res.min_color;
        host.path.clear();
        // cout << host.max_color << host.min_color << endl;
        // cout << "............................" << endl;
        k++;

    }

    return 0;
}



