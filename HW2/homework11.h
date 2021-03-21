
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

    char color;
    bool king ;
    bool is_empty = true;
    Piece():color('.'),king(false) {}
	Piece(char color, bool king):color(color),king(king) {}

};

class Board{
public:

    vector<vector<Piece>> board;
    vector<pair<int,int>> path;
    vector<vector<Piece>> input();
    void print_board();
    void move_piece(int,int,int ,int );
    int b_left = 0;
    int w_left = 0;
    int b_king = 0;
    int w_king = 0;


    string Pmethod;
    char max_color;
    char min_color;
    double Pclock;

    //evaluate 
    vector<pair<int,int>> get_all_piece(char);
    char winner();
    void Jump(vector<Board>&,Board,int,int);
    bool canJump(Board,int,int);
    bool canMove(Board,int,int);
    void Move(vector<Board>&,Board,int,int);
    Piece get_piece(int row,int col);

    bool judge_King(char);
    void Bleft_moves(vector<Board>&,Board,bool,int, int, int, Piece, int,vector<Piece>);
    void Bright_moves(vector<Board>&,Board,bool,int, int, int, Piece, int,vector<Piece>);
    void Wleft_moves(vector<Board>&,Board,bool,int, int, int, Piece, int,vector<Piece>);
    void Wright_moves(vector<Board>&,Board,bool,int, int, int, Piece, int,vector<Piece>);
    void _remove(int row, int col);
    float evaluation();




    Board():board({{}}) {}
    Board(vector<vector<Piece>> board): board(board) {}
    
};

class Game{
public:



    Board min_max(Board, int ,char player,float,float);
    vector<Board> get_all_moves(Board, char);
    string path_trans(int, int);



};






