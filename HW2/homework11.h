
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

class Piece{
public:
    int row;
    int col;
    char color;
    bool king = false;
    bool is_empty = true;
    Piece():row(-1),col(-1) {}
	Piece(int row, int col):row(row),col(col) {}

};

class Board{
public:

    vector<vector<Piece>> board;
    vector<pair<int,int>> path;
    vector<vector<Piece>> input();
    void print_board();
    void move_piece(Piece,int ,int );
    int b_left = 0;
    int w_left = 0;
    int b_king = 0;
    int w_king = 0;


    string Pmethod;
    char max_color;
    char min_color;
    double Pclock;

    //evaluate 
    vector<Piece> get_all_piece(char);
    char winner();
    vector<Board> get_valid_move(Piece);
    void count_black(int, int);
    void count_white(int, int);
    Piece get_piece(int row,int col);
    bool judge_King(char);
    void Bleft_moves(vector<Board>&,Board,bool,int, int, int, Piece, int,vector<Piece>);
    void Bright_moves(vector<Board>&,Board,bool,int, int, int, Piece, int,vector<Piece>);
    void Wleft_moves(vector<Board>&,Board,bool,int, int, int, Piece, int,vector<Piece>);
    void Wright_moves(vector<Board>&,Board,bool,int, int, int, Piece, int,vector<Piece>);
    void remove(vector<Piece>&);
    void _remove(Piece);
    float evaluation();


    Board():board({{}}) {}
    Board(vector<vector<Piece>> board): board(board) {}
    
};

class Game{
public:



    Board min_max(Board, int ,char player);
    vector<Board> get_all_moves(Board, char);
    string path_trans(int, int);



};






