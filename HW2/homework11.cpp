#include "homework11.h"

int main(int argc, char const *argv[])
{

    Board board;

    board.board = board.input();
    Game game;
    board.print_board();
    const clock_t start_time = clock();
    int depth = 1;
    handlePlayerDate();
    if (board.Pmethod == "SINGLE")
    {
        depth = 1;
    }
    else
    {
        cout << step << endl;
        if (step < 6)
        {
            depth = 5;
        }
        else
        {
            if (board.Pclock < 0.1)
            {
                depth = 1;
            }
            else if (board.Pclock < 2.0)
            {
                depth = 3;
            }
            else if (board.Pclock < 10.0)
            {
                depth = 5;
            }
            else
            {
                depth = 7;
            }
        }
    }
    cout << depth << endl;
    pair<Board, int> board1 = game.min_max2(board, depth, board.max_color, INT_MIN, INT_MAX);
    board1.first.print_board();
    game.createOutput(board, board1.first);

    return 0;
}
