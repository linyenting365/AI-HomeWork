#include "homework11.h"

int main(int argc, char const *argv[])
{

    int depth = 1;
    Board board;
    infile.open("input.txt");
    infile >> board.Pmethod;
    string Pcolor;
    infile >> Pcolor;
    if (Pcolor == "BLACK")
    {
        board.max_color = 'b';
        board.min_color = 'w';
    }
    else
    {
        board.max_color = 'w';
        board.min_color = 'b';
    }
    cout << board.max_color << endl;
    board.board = vector<vector<Piece>>(8, vector<Piece>(8));
    vector<vector<char>> init_board = {{'.', 'b', '.', 'b', '.', 'b', '.', 'W'},
                                       {'.', '.', '.', '.', '.', '.', '.', '.'},
                                       {'.', '.', '.', '.', '.', '.', '.', 'b'},
                                       {'b', '.', '.', '.', '.', '.', '.', '.'},
                                       {'.', 'w', '.', 'b', '.', 'b', '.', '.'},
                                       {'.', '.', 'B', '.', '.', '.', '.', '.'},
                                       {'.', 'w', '.', '.', '.', 'b', '.', '.'},
                                       {'w', '.', 'w', '.', '.', '.', '.', '.'}};

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            board.board[i][j].color = tolower(init_board[i][j]);
            board.board[i][j].king = board.judge_King(init_board[i][j]);
            if (board.board[i][j].color == 'b')
            {
                board.b_left++;
                if (board.board[i][j].king)
                {
                    board.b_king++;
                }
            }
            else if (board.board[i][j].color == 'w')
            {
                board.w_left++;
                if (board.board[i][j].king)
                {
                    board.w_king++;
                }
            }
            board.board[i][j].is_empty = false;
        }
    }
    infile.close();
    outfile.open("cal.txt");
    for (int i = 3; i < 13;)
    {
        Game game;
        Board board_input = board;
        const clock_t start_time = clock();
        Board board1 = game.min_max3(board_input, i, board_input.max_color, INT_MIN, INT_MAX);
        // board1.print_board();
        float time = float(clock() - start_time) / CLOCKS_PER_SEC;
        cout << i << endl;
        cout << time << endl;
        outfile << time;
        outfile << endl;
        i += 2;
    }
    return 0;
}
