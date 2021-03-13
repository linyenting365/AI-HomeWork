
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
    unordered_map<string,vector<Piece>> get_valid_move(Piece);
    Piece get_piece(int row,int col);
    bool judge_King(char);
    void Bleft_moves(unordered_map<string,vector<Piece>>&,bool,int, int, int, char, int,Piece);
    void Bright_moves(unordered_map<string,vector<Piece>>&,bool,int, int, int, char, int,Piece);
    void Wleft_moves(unordered_map<string,vector<Piece>>&,bool,int, int, int, char, int,Piece);
    void Wright_moves(unordered_map<string,vector<Piece>>&,bool,int, int, int, char, int,Piece);
    void remove(vector<Piece>&);
    float evaluation();


    Board():board({{}}) {}
    Board(vector<vector<Piece>> board): board(board) {}
    
};


class Game{
public:



    Board min_max(Board, int ,char player);
    vector<Board> get_all_moves(Board, char);
    Board simulation_move(Piece, string,Board, vector<Piece>&);



};






