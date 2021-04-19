
#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <stdio.h>
#include <ctype.h>
#include <unordered_map>
#include <list>
#include <limits.h>
#include <math.h>

using namespace std;
fstream infile;
ofstream outfile;
int step;

class Piece
{
public:
    char color;
    bool king;
    bool is_empty = true;
    Piece() : color('.'), king(false) {}
    Piece(char color, bool king) : color(color), king(king) {}
};

class Board
{
public:
    vector<vector<Piece>> board;
    vector<pair<int, int>> path;
    vector<vector<Piece>> input();
    void print_board();
    void move_piece(int, int, int, int);
    int b_left = 0;
    int w_left = 0;
    int b_king = 0;
    int w_king = 0;

    string Pmethod;
    char max_color;
    char min_color;
    double Pclock;

    //evaluate
    vector<pair<int, int>> get_all_piece(char);
    char winner();
    void Jump(vector<Board> &, Board, int, int);
    bool canJump(Board, int, int);
    bool canMove(Board, int, int);
    void Move(vector<Board> &, Board, int, int);
    bool judge_King(char);
    void _remove(int row, int col);
    float evaluation();
    double evaluationII();
    int evaluationIII();
    int evaluationEND();
    void get_king(vector<pair<int, int>> &, vector<pair<int, int>> &);
    int dist(pair<int, int>, pair<int, int>);

    Board() : board({{}}) {}
    Board(vector<vector<Piece>> board) : board(board) {}
};

class Game
{
public:
    pair<Board, int> min_max(Board, int, char player, int, int);
    pair<Board, double> min_max2(Board, int, char player, double, double);
    Board min_max3(Board, int, char player, int, int);
    vector<Board> get_all_moves(Board, char);
    string path_trans(int, int);
    void createOutput(Board &, Board &);
    void createCal();
};

vector<vector<Piece>> Board::input()
{

    infile.open("input.txt");
    infile >> Pmethod;
    string Pcolor;
    infile >> Pcolor;
    if (Pcolor == "BLACK")
    {
        max_color = 'b';
        min_color = 'w';
    }
    else
    {
        max_color = 'w';
        min_color = 'b';
    }
    infile >> Pclock;
    vector<vector<Piece>> board(8, vector<Piece>(8));

    for (int i = 0; i < 8; ++i)
    {
        string temp;
        infile >> temp;
        for (int j = 0; j < 8; ++j)
        {
            board[i][j].color = tolower(temp[j]);
            board[i][j].king = judge_King(temp[j]);
            if (board[i][j].color == 'b')
            {
                b_left++;
                if (board[i][j].king)
                {
                    b_king++;
                }
            }
            else if (board[i][j].color == 'w')
            {
                w_left++;
                if (board[i][j].king)
                {
                    w_king++;
                }
            }
            board[i][j].is_empty = false;
        }
    }
    infile.close();
    return board;
}

bool Board::judge_King(char piece)
{
    if (piece == 'B' || piece == 'W')
    {
        return true;
    }
    return false;
}

char Board::winner()
{
    if (b_left <= 0)
    {
        return 'w';
    }
    if (w_left <= 0)
    {
        return 'b';
    }
    return ' ';
}

float Board::evaluation()
{

    if (max_color == 'b')
        return float(b_left - w_left + (2 * b_king - 2 * w_king));
    else
        return float(w_left - b_left + (2 * w_king - 2 * b_king));
}

double Board::evaluationII()
{
    vector<int> B(7, 0);
    vector<int> W(7, 0);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j].color == 'b')
            {
                if (!board[i][j].king)
                {
                    B[0] += i;
                }
                else
                {
                    B[1] += 7;
                }

                if (i == 7)
                {
                    B[2]++;
                    B[6]++;
                }
                else
                {
                    if (i == 3 || i == 4)
                    {
                        if (j >= 2 && j <= 5)
                        {
                            B[3]++;
                        }
                        else
                        {
                            B[4]++;
                        }
                    }

                    if (i > 0)
                    {
                        if (j > 0 && j < 7)
                        {
                            if (board[i - 1][j - 1].color == 'w' && board[i + 1][j + 1].color == '.')
                            {
                                B[5]++;
                            }
                            if (board[i - 1][j + 1].color == 'w' && board[i + 1][j - 1].color == '.')
                            {
                                B[5]++;
                            }
                        }
                    }

                    if (i < 7)
                    {
                        if (j == 0 || j == 7)
                        {
                            B[6]++;
                        }
                        else
                        {
                            if ((board[i + 1][j - 1].color != '.' && (board[i + 1][j - 1].color == 'b' || !board[i + 1][j - 1].king)) && (board[i + 1][j + 1].color != '.' && (board[i + 1][j + 1].color == 'b' || !board[i + 1][j + 1].king)))
                            {
                                B[6]++;
                            }
                        }
                    }
                }
            }
            if (board[i][j].color == 'w')
            {
                if (!board[i][j].king)
                {
                    W[0] += (7 - i);
                }
                else
                {
                    W[1] += 7;
                }
                if (i == 0)
                {
                    W[2]++;
                    W[6]++;
                }
                else
                {
                    if (i == 3 || i == 4)
                    {
                        if (j >= 2 && j <= 5)
                        {
                            W[3]++;
                        }
                        else
                        {
                            W[4]++;
                        }
                    }

                    if (i < 7)
                    {
                        if (j > 0 && j < 7)
                        {
                            if (board[i + 1][j - 1].color == 'b' && board[i - 1][j + 1].color == '.')
                            {
                                W[5]++;
                            }
                            if (board[i + 1][j + 1].color == 'b' && board[i - 1][j - 1].color == '.')
                            {
                                W[5]++;
                            }
                        }
                    }

                    if (i > 0)
                    {
                        if (j == 0 || j == 7)
                        {
                            W[6]++;
                        }
                        else
                        {
                            if ((board[i - 1][j - 1].color != '.' && (board[i - 1][j - 1].color == 'w' || !board[i - 1][j - 1].king)) && (board[i - 1][j + 1].color != '.' && (board[i - 1][j + 1].color == 'w' || !board[i - 1][j + 1].king)))
                            {
                                W[6]++;
                            }
                        }
                    }
                }
            }
        }
    }

    vector<double> Weight = {5, 12, 4, 2.5, 0.5, -3, 3};
    double sum = 0;
    for (int i = 0; i < 7; i++)
    {
        if (max_color == 'b')
        {
            sum += (B[i] - W[i]) * Weight[i];
        }
        else
        {
            sum += (W[i] - B[i]) * Weight[i];
        }
    }
    return sum;
}

int Board::dist(pair<int, int> p, pair<int, int> o)
{
    int x = p.first - o.first;
    int y = p.second - o.second;
    double dist = sqrt(x * x - y * y);
    return round(dist);
}

int Board::evaluationIII()
{

    int b_val = 0;
    int w_val = 0;
    int total_pieces = b_left + w_left;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j].color == 'b')
            {
                if (!board[i][j].king)
                {
                    b_val += (5 + i + 1);
                }
                else
                {
                    b_val += 15;
                }
            }
            else if (board[i][j].color == 'w')
            {
                if (!board[i][j].king)
                {
                    w_val += (5 + (8 - i));
                }
                else
                {
                    w_val += 15;
                }
            }
        }
    }

    if (b_left == b_king && w_left == w_king)
    {
        vector<pair<int, int>> b_king;
        vector<pair<int, int>> w_king;
        get_king(b_king, w_king);
        int dis = 0;
        if (max_color == 'b')
        {
            for (auto b : b_king)
            {
                for (auto w : w_king)
                {
                    dis += dist(b, w);
                }
            }
            if (b_king >= w_king)
            {
                return dis * (-1);
            }
            return dis;
        }
        else
        {
            for (auto w : w_king)
            {
                for (auto b : b_king)
                {
                    dis += dist(w, b);
                }
            }
            if (w_king >= b_king)
            {
                return dis * (-1);
            }
            return dis;
        }
    }

    if (max_color == 'b')
    {
        return (b_val - w_val);
    }
    else
    {
        return (w_val - b_val);
    }
}

void Board::get_king(vector<pair<int, int>> &b_king, vector<pair<int, int>> &w_king)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j].color == 'b')
            {
                if (board[i][j].king)
                {
                    b_king.push_back(make_pair(i, j));
                }
            }
            else if (board[i][j].color == 'w')
            {
                if (board[i][j].king)
                {
                    w_king.push_back(make_pair(i, j));
                }
            }
        }
    }
}

void Board::move_piece(int cur_row, int cur_col, int row, int col)
{
    swap(board[cur_row][cur_col], board[row][col]);

    if (!board[row][col].king && (row == 7 || row == 0))
    {
        if (board[row][col].color == 'b')
        {
            board[row][col].king = true;
            b_king++;
        }
        else if (board[row][col].color == 'w')
        {
            board[row][col].king = true;
            w_king++;
        }
    }
}

void Board::print_board()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j].king)
            {
                cout << char(toupper(board[i][j].color));
            }
            else
            {
                cout << board[i][j].color;
            }
        }
        cout << endl;
    }

    cout << endl;
}

void Board::_remove(int row, int col)
{
    if (board[row][col].color == 'b')
    {
        b_left--;
        if (board[row][col].king)
        {
            b_king--;
        }
    }
    else if (board[row][col].color == 'w')
    {
        w_left--;
        if (board[row][col].king)
        {
            w_king--;
        }
    }
    board[row][col].color = '.';
    board[row][col].king = false;
}

vector<pair<int, int>> Board::get_all_piece(char color)
{
    vector<pair<int, int>> get_all;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j].color == color)
            {
                get_all.push_back(make_pair(i, j));
            }
        }
    }

    return get_all;
}

bool Board::canJump(Board cur_board, int row, int col)
{
    //cout << "in canJUMP" << endl;
    if (cur_board.board[row][col].color == 'w' && !cur_board.board[row][col].king)
    {
        //cout << "in w can jump" << endl;
        if (row - 1 > -1 && col - 1 > -1 && row - 2 > -1 && col - 2 > -1 && cur_board.board[row - 1][col - 1].color == 'b' && cur_board.board[row - 2][col - 2].color == '.')
        {
            return true;
        }
        if (row - 1 > -1 && col + 1 < 8 && row - 2 > -1 && col + 2 < 8 && cur_board.board[row - 1][col + 1].color == 'b' && cur_board.board[row - 2][col + 2].color == '.')
        {
            return true;
        }
    }
    if (cur_board.board[row][col].color == 'b' && !cur_board.board[row][col].king)
    {
        // cout << "in b can jump" << endl;
        if (row + 1 < 8 && col - 1 > -1 && row + 2 < 8 && col - 2 > -1 && cur_board.board[row + 1][col - 1].color == 'w' && cur_board.board[row + 2][col - 2].color == '.')
        {
            return true;
        }
        if (row + 1 < 8 && col + 1 < 8 && row + 2 < 8 && col + 2 < 8 && cur_board.board[row + 1][col + 1].color == 'w' && cur_board.board[row + 2][col + 2].color == '.')
        {
            return true;
        }
    }

    if (cur_board.board[row][col].color == 'w' && cur_board.board[row][col].king)
    {

        if (row - 1 > -1 && col - 1 > -1 && row - 2 > -1 && col - 2 > -1 && cur_board.board[row - 1][col - 1].color == 'b' && cur_board.board[row - 2][col - 2].color == '.')
        {
            return true;
        }
        if (row - 1 > -1 && col + 1 < 8 && row - 2 > -1 && col + 2 < 8 && cur_board.board[row - 1][col + 1].color == 'b' && cur_board.board[row - 2][col + 2].color == '.')
        {
            return true;
        }
        if (row + 1 < 8 && col - 1 > -1 && row + 2 < 8 && col - 2 > -1 && cur_board.board[row + 1][col - 1].color == 'b' && cur_board.board[row + 2][col - 2].color == '.')
        {
            return true;
        }
        if (row + 1 < 8 && col + 1 < 8 && row + 2 < 8 && col + 2 < 8 && cur_board.board[row + 1][col + 1].color == 'b' && cur_board.board[row + 2][col + 2].color == '.')
        {
            return true;
        }
    }

    if (cur_board.board[row][col].color == 'b' && cur_board.board[row][col].king)
    {

        if (row - 1 > -1 && col - 1 > -1 && row - 2 > -1 && col - 2 > -1 && cur_board.board[row - 1][col - 1].color == 'w' && cur_board.board[row - 2][col - 2].color == '.')
        {
            return true;
        }
        if (row - 1 > -1 && col + 1 < 8 && row - 2 > -1 && col + 2 < 8 && cur_board.board[row - 1][col + 1].color == 'w' && cur_board.board[row - 2][col + 2].color == '.')
        {
            return true;
        }
        if (row + 1 < 8 && col - 1 > -1 && row + 2 < 8 && col - 2 > -1 && cur_board.board[row + 1][col - 1].color == 'w' && cur_board.board[row + 2][col - 2].color == '.')
        {
            return true;
        }
        if (row + 1 < 8 && col + 1 < 8 && row + 2 < 8 && col + 2 < 8 && cur_board.board[row + 1][col + 1].color == 'w' && cur_board.board[row + 2][col + 2].color == '.')
        {
            return true;
        }
    }

    return false;
}

bool Board::canMove(Board cur_board, int row, int col)
{
    //cout << "in canMove" << endl;
    if (cur_board.board[row][col].color == 'w' || cur_board.board[row][col].king)
    {
        if (row - 1 > -1 && col - 1 > -1 && cur_board.board[row - 1][col - 1].color == '.')
        {
            // cout << row << col << endl;
            return true;
        }
        if (row - 1 > -1 && col + 1 < 8 && cur_board.board[row - 1][col + 1].color == '.')
        {
            //cout << row << col << endl;
            return true;
        }
    }
    if (cur_board.board[row][col].color == 'b' || cur_board.board[row][col].king)
    {
        if (row + 1 < 8 && col - 1 > -1 && cur_board.board[row + 1][col - 1].color == '.')
        {
            //cout << row << col << endl;
            return true;
        }
        if (row + 1 < 8 && col + 1 < 8 && cur_board.board[row + 1][col + 1].color == '.')
        {
            //cout << row << col << endl;
            return true;
        }
    }

    return false;
}

void Board::Jump(vector<Board> &boards, Board cur_board, int row, int col)
{

    if (!canJump(cur_board, row, col))
    {
        boards.push_back(cur_board);
        return;
    }

    if (cur_board.board[row][col].color == 'w' && !cur_board.board[row][col].king)
    {
        if (row - 1 > -1 && col - 1 > -1 && row - 2 > -1 && col - 2 > -1 && cur_board.board[row - 1][col - 1].color == 'b' && cur_board.board[row - 2][col - 2].color == '.')
        {
            // cout << "J w move left"<< endl;
            Board temp = cur_board;
            temp._remove(row - 1, col - 1);
            temp.move_piece(row, col, row - 2, col - 2);
            temp.path.push_back(make_pair(row - 2, col - 2));
            if (row - 2 == 0)
            {
                boards.push_back(temp);
                return;
            }
            Jump(boards, temp, row - 2, col - 2);
        }
        if (row - 1 > -1 && col + 1 < 8 && row - 2 > -1 && col + 2 < 8 && cur_board.board[row - 1][col + 1].color == 'b' && cur_board.board[row - 2][col + 2].color == '.')
        {
            //cout << "J w move right"<< endl;
            Board temp = cur_board;
            temp._remove(row - 1, col + 1);
            temp.move_piece(row, col, row - 2, col + 2);
            temp.path.push_back(make_pair(row - 2, col + 2));
            if (row - 2 == 0)
            {
                boards.push_back(temp);
                return;
            }
            Jump(boards, temp, row - 2, col + 2);
        }
    }
    if (cur_board.board[row][col].color == 'b' && !cur_board.board[row][col].king)
    {
        if (row + 1 < 8 && col - 1 > -1 && row + 2 < 8 && col - 2 > -1 && cur_board.board[row + 1][col - 1].color == 'w' && cur_board.board[row + 2][col - 2].color == '.')
        {
            //cout << " J b move left"<< endl;
            Board temp = cur_board;
            temp._remove(row + 1, col - 1);
            temp.move_piece(row, col, row + 2, col - 2);
            temp.path.push_back(make_pair(row + 2, col - 2));
            if (row + 2 == 7)
            {
                boards.push_back(temp);
                return;
            }
            Jump(boards, temp, row + 2, col - 2);
        }
        if (row + 1 < 8 && col + 1 < 8 && row + 2 < 8 && col + 2 < 8 && cur_board.board[row + 1][col + 1].color == 'w' && cur_board.board[row + 2][col + 2].color == '.')
        {
            //cout << " J b move right"<< endl;
            Board temp = cur_board;
            temp._remove(row + 1, col + 1);
            temp.move_piece(row, col, row + 2, col + 2);
            temp.path.push_back(make_pair(row + 2, col + 2));
            if (row + 2 == 7)
            {
                boards.push_back(temp);
                return;
            }
            Jump(boards, temp, row + 2, col + 2);
        }
    }

    if (cur_board.board[row][col].color == 'w' && cur_board.board[row][col].king)
    {

        if (row - 1 > -1 && col - 1 > -1 && row - 2 > -1 && col - 2 > -1 && cur_board.board[row - 1][col - 1].color == 'b' && cur_board.board[row - 2][col - 2].color == '.')
        {
            //cout << "J king w move up left"<< endl;
            Board temp = cur_board;
            temp._remove(row - 1, col - 1);
            temp.move_piece(row, col, row - 2, col - 2);
            temp.path.push_back(make_pair(row - 2, col - 2));
            Jump(boards, temp, row - 2, col - 2);
        }
        if (row - 1 > -1 && col + 1 < 8 && row - 2 > -1 && col + 2 < 8 && cur_board.board[row - 1][col + 1].color == 'b' && cur_board.board[row - 2][col + 2].color == '.')
        {
            //cout << "J king w move up right"<< endl;
            Board temp = cur_board;
            temp._remove(row - 1, col + 1);
            temp.move_piece(row, col, row - 2, col + 2);
            temp.path.push_back(make_pair(row - 2, col + 2));
            Jump(boards, temp, row - 2, col + 2);
        }
        if (row + 1 < 8 && col - 1 > -1 && row + 2 < 8 && col - 2 > -1 && cur_board.board[row + 1][col - 1].color == 'b' && cur_board.board[row + 2][col - 2].color == '.')
        {
            //cout << " J king w move down left"<< endl;
            Board temp = cur_board;
            temp._remove(row + 1, col - 1);
            temp.move_piece(row, col, row + 2, col - 2);
            temp.path.push_back(make_pair(row + 2, col - 2));
            Jump(boards, temp, row + 2, col - 2);
        }
        if (row + 1 < 8 && col + 1 < 8 && row + 2 < 8 && col + 2 < 8 && cur_board.board[row + 1][col + 1].color == 'b' && cur_board.board[row + 2][col + 2].color == '.')
        {
            //cout << " J king w move down right"<< endl;
            Board temp = cur_board;
            temp._remove(row + 1, col + 1);
            temp.move_piece(row, col, row + 2, col + 2);
            temp.path.push_back(make_pair(row + 2, col + 2));
            Jump(boards, temp, row + 2, col + 2);
        }
    }

    if (cur_board.board[row][col].color == 'b' && cur_board.board[row][col].king)
    {

        if (row - 1 > -1 && col - 1 > -1 && row - 2 > -1 && col - 2 > -1 && cur_board.board[row - 1][col - 1].color == 'w' && cur_board.board[row - 2][col - 2].color == '.')
        {
            Board temp = cur_board;
            temp._remove(row - 1, col - 1);
            temp.move_piece(row, col, row - 2, col - 2);
            temp.path.push_back(make_pair(row - 2, col - 2));
            Jump(boards, temp, row - 2, col - 2);
        }
        if (row - 1 > -1 && col + 1 < 8 && row - 2 > -1 && col + 2 < 8 && cur_board.board[row - 1][col + 1].color == 'w' && cur_board.board[row - 2][col + 2].color == '.')
        {
            Board temp = cur_board;
            temp._remove(row - 1, col + 1);
            temp.move_piece(row, col, row - 2, col + 2);
            temp.path.push_back(make_pair(row - 2, col + 2));
            Jump(boards, temp, row - 2, col + 2);
        }
        if (row + 1 < 8 && col - 1 > -1 && row + 2 < 8 && col - 2 > -1 && cur_board.board[row + 1][col - 1].color == 'w' && cur_board.board[row + 2][col - 2].color == '.')
        {
            Board temp = cur_board;
            temp._remove(row + 1, col - 1);
            temp.move_piece(row, col, row + 2, col - 2);
            temp.path.push_back(make_pair(row + 2, col - 2));
            Jump(boards, temp, row + 2, col - 2);
        }
        if (row + 1 < 8 && col + 1 < 8 && row + 2 < 8 && col + 2 < 8 && cur_board.board[row + 1][col + 1].color == 'w' && cur_board.board[row + 2][col + 2].color == '.')
        {
            Board temp = cur_board;
            temp._remove(row + 1, col + 1);
            temp.move_piece(row, col, row + 2, col + 2);
            temp.path.push_back(make_pair(row + 2, col + 2));
            Jump(boards, temp, row + 2, col + 2);
        }
    }
    return;
}

void Board::Move(vector<Board> &boards, Board cur_board, int row, int col)
{

    if (cur_board.board[row][col].color == 'w' || cur_board.board[row][col].king)
    {
        Board wleft = cur_board;
        if (row - 1 > -1 && col - 1 > -1 && wleft.board[row - 1][col - 1].color == '.')
        {
            wleft.move_piece(row, col, row - 1, col - 1);
            wleft.path.push_back(make_pair(row - 1, col - 1));
            boards.push_back(wleft);
        }
        Board wright = cur_board;
        if (row - 1 > -1 && col + 1 < 8 && wright.board[row - 1][col + 1].color == '.')
        {
            wright.move_piece(row, col, row - 1, col + 1);
            wright.path.push_back(make_pair(row - 1, col + 1));
            boards.push_back(wright);
        }
    }
    if (cur_board.board[row][col].color == 'b' || cur_board.board[row][col].king)
    {
        Board bleft = cur_board;
        if (row + 1 < 8 && col - 1 > -1 && bleft.board[row + 1][col - 1].color == '.')
        {
            bleft.move_piece(row, col, row + 1, col - 1);
            bleft.path.push_back(make_pair(row + 1, col - 1));
            boards.push_back(bleft);
        }
        Board bright = cur_board;
        if (row + 1 < 8 && col + 1 < 8 && bright.board[row + 1][col + 1].color == '.')
        {
            bright.move_piece(row, col, row + 1, col + 1);
            bright.path.push_back(make_pair(row + 1, col + 1));
            boards.push_back(bright);
        }
    }

    return;
}

vector<Board> Game::get_all_moves(Board board, char color)
{
    vector<Board> moves;
    vector<pair<int, int>> all_pieces = board.get_all_piece(color);
    // bool forceJump = false;
    for (auto piece : all_pieces)
    {
        Board temp = board;
        if (temp.canJump(temp, piece.first, piece.second))
        {
            // cout <<"can Jump" << endl;
            temp.path.push_back(make_pair(piece.first, piece.second));
            temp.Jump(moves, temp, piece.first, piece.second);
        }
    }
    if (!moves.empty())
    {
        return moves;
    }

    for (auto piece : all_pieces)
    {
        Board temp = board;
        if (temp.canMove(temp, piece.first, piece.second))
        {
            temp.path.push_back(make_pair(piece.first, piece.second));
            temp.Move(moves, temp, piece.first, piece.second);
        }
    }

    return moves;
}

pair<Board, int> Game::min_max(Board board, int depth, char player, int alp, int beta)
{
    if (depth == 0 || board.winner() != ' ')
    {
        int eval = board.evaluationIII();
        // board.print_board();
        return make_pair(board, eval);
    }
    if (player == board.max_color)
    {
        int MAX_eval = INT_MIN;
        pair<Board, int> best_move = make_pair(board, INT_MIN);
        vector<Board> all_moves = get_all_moves(board, board.max_color);
        for (auto move : all_moves)
        {
            pair<Board, int> reBoard = min_max(move, depth - 1, board.min_color, alp, beta);
            MAX_eval = max(MAX_eval, reBoard.second);
            alp = max(alp, MAX_eval);
            if (MAX_eval == reBoard.second)
            {
                best_move = make_pair(move, reBoard.second);
            }
            if (alp >= beta)
                break;
        }
        return best_move;
    }
    else
    {
        int MIN_eval = INT_MAX;
        pair<Board, int> best_move = make_pair(board, INT_MAX);
        vector<Board> all_moves = get_all_moves(board, board.min_color);
        for (auto move : all_moves)
        {
            pair<Board, int> reBoard = min_max(move, depth - 1, board.max_color, alp, beta);
            MIN_eval = min(MIN_eval, reBoard.second);
            beta = min(beta, MIN_eval);
            if (MIN_eval == reBoard.second)
            {
                best_move = make_pair(move, reBoard.second);
            }

            if (alp >= beta)
                break;
        }
        return best_move;
    }
}

pair<Board, double> Game::min_max2(Board board, int depth, char player, double alp, double beta)
{
    if (depth == 0 || board.winner() != ' ')
    {
        double eval = board.evaluationII();
        return make_pair(board, eval);
    }
    if (player == board.max_color)
    {
        double MAX_eval = INT_MIN;
        pair<Board, double> best_move = make_pair(board, INT_MIN);
        vector<Board> all_moves = get_all_moves(board, board.max_color);
        for (auto move : all_moves)
        {
            pair<Board, double> reBoard = min_max2(move, depth - 1, board.min_color, alp, beta);
            MAX_eval = max(MAX_eval, reBoard.second);
            alp = max(alp, MAX_eval);
            if (MAX_eval == reBoard.second)
            {
                best_move = make_pair(move, reBoard.second);
            }
            if (alp >= beta)
                break;
        }
        return best_move;
    }
    else
    {
        double MIN_eval = INT_MAX;
        pair<Board, int> best_move = make_pair(board, INT_MAX);
        vector<Board> all_moves = get_all_moves(board, board.min_color);
        for (auto move : all_moves)
        {
            pair<Board, double> reBoard = min_max2(move, depth - 1, board.max_color, alp, beta);
            MIN_eval = min(MIN_eval, reBoard.second);
            beta = min(beta, MIN_eval);
            if (MIN_eval == reBoard.second)
            {
                best_move = make_pair(move, reBoard.second);
            }

            if (alp >= beta)
                break;
        }
        return best_move;
    }
}

Board Game::min_max3(Board board, int depth, char player, int alp, int beta)
{
    if (depth == 0 || board.winner() != ' ')
    {
        return board;
    }
    if (player == board.max_color)
    {
        int MAX_eval = INT_MIN;
        Board best_move = board;
        vector<Board> all_moves = get_all_moves(board, board.max_color);
        for (auto move : all_moves)
        {
            Board evaluation = min_max3(move, depth - 1, board.min_color, alp, beta);
            MAX_eval = max(MAX_eval, evaluation.evaluationIII());
            alp = max(alp, MAX_eval);
            if (MAX_eval == evaluation.evaluationIII())
            {
                best_move = move;
            }
            if (alp >= beta)
                break;
        }
        return best_move;
    }
    else
    {
        int MIN_eval = INT_MAX;
        Board best_move = board;
        vector<Board> all_moves = get_all_moves(board, board.min_color);
        for (auto move : all_moves)
        {
            Board evaluation = min_max3(move, depth - 1, board.max_color, alp, beta);
            MIN_eval = min(MIN_eval, evaluation.evaluationIII());
            beta = min(beta, MIN_eval);
            if (MIN_eval == evaluation.evaluationIII())
            {
                best_move = move;
            }

            if (alp >= beta)
                break;
        }
        return best_move;
    }
}

string Game::path_trans(int row, int col)
{
    string ans;
    ans += 'a' + col;
    ans += to_string((row - 8) * (-1));
    return ans;
}

void Game::createOutput(Board &board, Board &res_board)
{
    outfile.open("output.txt");
    int start_left = board.b_left + board.w_left;
    int end_left = res_board.b_left + res_board.w_left;
    string out = "";
    if (start_left == end_left)
        out += "E ";
    else
        out += "J ";
    vector<string> output;
    for (auto res : res_board.path)
    {
        output.push_back(path_trans(res.first, res.second));
    }

    for (int i = 0; i < output.size() - 1; i++)
    {

        string ans = "";
        // cout << output[i] << endl;
        ans += output[i] + " " + output[i + 1];
        ans = out + ans;
        cout << ans << endl;
        outfile << ans << endl;
        outfile << endl;
    }
}

void handlePlayerDate()
{
    ifstream file("playdata.txt");
    if (file)
    {
        file >> step;
        cout << "exist " << step << endl;
        ofstream out;
        out.open("playdata.txt");
        step += 1;
        out << step;
        out.close();
    }
    else
    {
        cout << "not exist" << endl;
        step = 1;
        ofstream out;
        out.open("playdata.txt");
        out << step;
        out.close();
    }
}
