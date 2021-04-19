#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <stack>
#include <list>
#include<math.h>

using namespace std;


ifstream infile;
ofstream outfile;
string algor;

struct Node{
    int row;
    int column;
    int cost;
    double pathcost;
};


int row, column;
int max_elv,target;
int reval;
Node s;

vector<vector<int>>T_map;
vector<Node> target_pt;
unordered_map < string , string > path;
typedef  pair<int, int> pi;


//function
vector<vector<int>> input();
void output();
int BFS();
int UCS();
int Astar();
void printPath(string g);
int judgeRock(int parent, int cur);
double eudist(int srow , int scolum, int drow, int dcolum);
int Rock(int parent, int cur);
int mud( int cur);
bool UCS_compare(const Node& first, const Node& second);
bool Astar_compare(const Node& first, const Node& second);

int main(int argc, char const * argv[])
{


    T_map = input();
    if (algor == "BFS"){

        reval = BFS();
        output();
    }else if (algor == "UCS"){

        reval = UCS();
        output();
    }else if (algor == "A*"){
        
        outfile.open("output.txt");
        reval = Astar();

    }

    return 0;
}

vector<vector<int>> input(){
    
    infile.open("input.txt");

    infile >> algor;
    infile >> column;
    infile >> row;
    infile >> s.column;
    infile >> s.row;
    infile >> max_elv;
    infile >> target;
    cout << target << endl;
    for (int i = 0; i < target;i ++){
        int x, y;
        Node tar;
        infile >> tar.column;
        infile >> tar.row;
        target_pt.push_back(tar);
    }

    vector<vector<int>> m(row, vector<int>(column, 0));

    for( int i = 0; i < row; i++){
        for( int j = 0; j < column; j++){
            infile >> m[i][j];
        }
    }
    infile.close();

    return m;
}



int BFS(){

    vector<vector<int>> visited(row, vector<int>(column, 0));

    visited[s.row][s.column] = 1;
    queue<Node> bfs_q;
    bfs_q.push(s);
    path[to_string(s.column) + "," + to_string(s.row)] = "s";
    int ans_cnt = 0;


    while(!bfs_q.empty() && ans_cnt != target){


        Node cur = bfs_q.front();
        bfs_q.pop();

        for(int i = 0; i < target_pt.size(); i++){
            if( cur.row == target_pt[i].row && cur.column == target_pt[i].column) ans_cnt++;
        }

        for(int i = cur.row-1; i <= cur.row +1; i++ ){
            for(int j = cur.column-1; j<= cur.column+1; j++){
                
                if( i == cur.row && j == cur.column) continue;
                if(i >=0 && i < row && j >= 0 && j < column ){

                   if(judgeRock(T_map[cur.row][cur.column] , T_map[i][j]  ) == 1){
                        
                        if( visited[i][j] == 0 ){
                            // cout << j << i << " ";
                            visited[i][j] = 1;
                            string st =  to_string(j) + "," + to_string(i);
                            //cout << st << "++" << " ";
                            path[st] = to_string(cur.column)+ "," +to_string(cur.row);
                            //cout << path[st] << endl;
                            Node next;
                            next.column = j;
                            next.row = i;
                            bfs_q.push(next);
                        }
                    }
                }


            }
        }

    }

    if(ans_cnt == 0){
        return 0;
    }else{
        return 1;
    }

}

bool UCS_compare(const Node& first, const Node& second){

    if(first.cost == second.cost){

        return first.row < second.row;

    }else{
        
        return first.cost < second.cost;

    }

}

bool Astar_compare(const Node& first, const Node& second){

    if(first.pathcost == second.pathcost){

        return first.row < second.row;

    }else{
        
        return first.pathcost < second.pathcost;

    }

}

int UCS(){

    list<Node> open;
    list<Node> closed;
    list<Node>::iterator it;
    vector<vector<int>> visited(row, vector<int>(column, 0));
    vector<vector<bool>> openTrack(row, vector<bool>(column, false));
    vector<vector<bool>> closedTrack(row, vector<bool>(column, false));

    Node child;
    s.cost = 0;
    open.push_front(s);
    path[to_string(s.column) + "," + to_string(s.row)] = "s";

    int ans_cnt = 0;


    while(!open.empty() && ans_cnt != target){
        
        Node cur = open.front();
        open.pop_front();
        openTrack[cur.row][cur.column] = false;

        
        for(int i = 0; i < target_pt.size(); i++){
            if( cur.row == target_pt[i].row && cur.column == target_pt[i].column) ans_cnt++;
        }

        for(int i = cur.row-1; i <= cur.row +1; i++ ){
            for(int j = cur.column-1; j<= cur.column+1; j++){

                if( (i == cur.row - 1 && j == cur.column) || (i == cur.row && j == cur.column - 1) || (i == cur.row && j == cur.column + 1) || (i == cur.row + 1 && j == cur.column)){

                    if(i >=0 && i < row && j >= 0 && j < column){
                        if(judgeRock(T_map[cur.row][cur.column] , T_map[i][j]  ) == 1){
                            child.row = i;
                            child.column = j;
                            child.cost = cur.cost + 10;
                            if( !openTrack[i][j] && !closedTrack[i][j]){

                                openTrack[i][j] = true;
                                open.push_back(child);
                                path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);

                            } else if(openTrack[i][j]) {

                                for(it = open.begin(); it != open.end(); it++){
                                    if(it->row == i && it->column == j){
                                        break;
                                    }
                                }

                                if( child.cost < it->cost){
                                    openTrack[it->row][it->column] = false;
                                    open.erase(it);
                                    openTrack[i][j] = true;
                                    open.push_back(child);
                                    path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);
                                }



                            } else if(closedTrack[i][j]) {


                                for(it = closed.begin(); it != closed.end(); it++){
                                    if(it->row == i && it->column == j){
                                        break;
                                    }
                                }

                                if( child.cost < it->cost){
                                    closedTrack[it->row][it->column] = false;
                                    closed.erase(it);
                                    openTrack[i][j] = true;
                                    open.push_back(child);
                                    path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);
                                }
                            }
                        }    
                    }

                } else if ((i == cur.row - 1 && j == cur.column-1) || (i == cur.row -1 && j == cur.column + 1) || (i == cur.row + 1 && j == cur.column - 1) || (i == cur.row + 1 && j == cur.column + 1)){
                    
                    if(i >=0 && i < row && j >= 0 && j < column){
                        if(judgeRock(T_map[cur.row][cur.column] , T_map[i][j]  ) == 1){
                            child.row = i;
                            child.column = j;
                            child.cost = cur.cost + 14;
                            if( !openTrack[i][j] && !closedTrack[i][j]){

                                openTrack[i][j] = true;
                                open.push_back(child);
                                path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);

                            } else if(openTrack[i][j]) {

                                for(it = open.begin(); it != open.end(); it++){
                                    if(it->row == i && it->column == j){
                                        break;
                                    }
                                }

                                if( child.cost < it->cost){
                                    openTrack[it->row][it->column] = false;
                                    open.erase(it);
                                    openTrack[i][j] = true;
                                    open.push_back(child);
                                    path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);
                                }



                            } else if(closedTrack[i][j]) {


                                for(it = closed.begin(); it != closed.end(); it++){
                                    if(it->row == i && it->column == j){
                                        break;
                                    }
                                }

                                if( child.cost < it->cost){
                                    closedTrack[it->row][it->column] = false;
                                    closed.erase(it);
                                    openTrack[i][j] = true;
                                    open.push_back(child);
                                    path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);
                                }
                            }
                        }    
                    }

                }

            }
        }

        closedTrack[cur.row][cur.column] = true;
        closed.push_back(cur);

        open.sort(UCS_compare);

    }

    if(ans_cnt == 0){
        return 0;
    }else{
        return 1;
    }

}

int Astar(){


    


    for( int k = 0 ; k < target_pt.size(); k ++){

        list<Node> open;
        list<Node> closed;
        list<Node>::iterator it;
        vector<vector<int>> visited(row, vector<int>(column, 0));
        vector<vector<bool>> openTrack(row, vector<bool>(column, false));
        vector<vector<bool>> closedTrack(row, vector<bool>(column, false));
        int ans_cnt = 0;
        Node child;

        s.cost = 0;
        s.pathcost = s.cost + eudist(s.row, s.column,target_pt[k].row,target_pt[k].column);
        open.push_front(s);
        path[to_string(s.column) + "," + to_string(s.row)] = "s";


        while(ans_cnt != 1){

            if(open.empty()){
                break;
            }

            Node cur = open.front();
            open.pop_front();
            openTrack[cur.row][cur.column] = false;

            if( cur.row == target_pt[k].row && cur.column == target_pt[k].column) ans_cnt++;
            for(int i = cur.row-1; i <= cur.row +1; i++ ){
                for(int j = cur.column-1; j<= cur.column+1; j++){

                    if( (i == cur.row - 1 && j == cur.column) || (i == cur.row && j == cur.column - 1) || (i == cur.row && j == cur.column + 1) || (i == cur.row + 1 && j == cur.column)){
                        if(i >=0 && i < row && j >= 0 && j < column){
                            if(judgeRock(T_map[cur.row][cur.column] , T_map[i][j]  ) == 1){
                                child.row = i;
                                child.column = j;
                                int rockLV = Rock(T_map[cur.row][cur.column],T_map[i][j]);
                                int mudness = mud(T_map[i][j]);
                                child.cost = cur.cost + 10 + rockLV + mudness;
                                child.pathcost = child.cost + eudist(child.row, child.column,target_pt[k].row,target_pt[k].column);

                                if( !openTrack[i][j] && !closedTrack[i][j]){
                                    
                                    openTrack[i][j] = true;
                                    open.push_back(child);
                                    path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);

                                } else if(openTrack[i][j]){

                                    for(it = open.begin(); it != open.end(); it++){
                                        if(it->row == i && it->column == j){
                                            break;
                                        }
                                    }

                                    if( child.pathcost < it->pathcost){
                                        openTrack[it->row][it->column] = false;
                                        open.erase(it);
                                        openTrack[i][j] = true;
                                        open.push_back(child);
                                        path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);
                                    }
                                } else if(closedTrack[i][j]){

                                    for(it = closed.begin(); it != closed.end(); it++){
                                        if(it->row == i && it->column == j){
                                            break;
                                        }
                                    }
                                    if( child.pathcost < it->pathcost){
                                        closedTrack[it->row][it->column] = false;
                                        closed.erase(it);
                                        openTrack[i][j] = true;
                                        open.push_back(child);
                                        path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);
                                    }




                                }

                            }
                        }

                    } else if ((i == cur.row - 1 && j == cur.column-1) || (i == cur.row -1 && j == cur.column + 1) || (i == cur.row + 1 && j == cur.column - 1) || (i == cur.row + 1 && j == cur.column + 1)){

                        if(i >=0 && i < row && j >= 0 && j < column){
                            if(judgeRock(T_map[cur.row][cur.column] , T_map[i][j]  ) == 1){
                                child.row = i;
                                child.column = j;
                                int rockLV = Rock(T_map[cur.row][cur.column],T_map[i][j]);
                                int mudness = mud(T_map[i][j]);
                                child.cost = cur.cost + 14 + rockLV + mudness;
                                child.pathcost = child.cost + eudist(child.row, child.column,target_pt[k].row,target_pt[k].column);

                                if( !openTrack[i][j] && !closedTrack[i][j]){
                                    openTrack[i][j] = true;
                                    open.push_back(child);
                                    path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);
                                } else if(openTrack[i][j]){

                                    for(it = open.begin(); it != open.end(); it++){
                                        if(it->row == i && it->column == j){
                                            break;
                                        }
                                    }

                                    if( child.pathcost < it->pathcost){
                                        openTrack[it->row][it->column] = false;
                                        open.erase(it);
                                        openTrack[i][j] = true;
                                        open.push_back(child);
                                        path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);
                                    }
                                } else if(closedTrack[i][j]){

                                    for(it = closed.begin(); it != closed.end(); it++){
                                        if(it->row == i && it->column == j){
                                            // cout <<  it->column << "," << it->column << endl;
                                            break;
                                        }
                                    }
                                    if( child.pathcost < it->pathcost){
                                        closedTrack[it->row][it->column] = false;
                                        closed.erase(it);
                                        openTrack[i][j] = true;
                                        open.push_back(child);
                                        path[to_string(j) + "," + to_string(i)] = to_string(cur.column)+ "," +to_string(cur.row);

                                    }

                                }

                            }
                        }
                    }
                }
            }

            closedTrack[cur.row][cur.column] = true;
            closed.push_back(cur);

            open.sort(Astar_compare);


        }


        printPath(to_string(target_pt[k].column) + "," + to_string(target_pt[k].row));
        outfile<< endl;
    }

    return 0;
}




void output(){

    outfile.open("output.txt");
    if( reval == 0){
        cout << "FAIL" << endl;
    } else {

        for(int i = 0; i < target_pt.size(); i++){
            string goal = to_string(target_pt[i].column) + "," + to_string(target_pt[i].row);
            printPath(goal);
            outfile<< endl;
        }

    }
}

void printPath(string g){

    stack<string> st;
    if( path[g] != "s"){
        if(path[g] == ""){
            outfile<< "FAIL";
            return;
        }
        printPath(path[g]);
    }
    cout << g << endl;
    outfile<< g << " ";

}

int judgeRock(int parent, int cur){

    if(parent >= 0) parent = 0;
    else parent = abs(parent);

    if(cur >= 0) cur = 0;
    else cur = abs(cur);

    int diff = abs( parent - cur);
    if( diff <= max_elv) return 1;
    
    return 0;

}

double eudist(int srow , int scolum, int drow, int dcolum){

   return ((double)10 * sqrt( (srow - drow)* (srow - drow) + (scolum - dcolum)*(scolum - dcolum)));

}

int Rock(int parent, int cur){

    if(parent >= 0) parent = 0;
    else parent = abs(parent);

    if(cur >= 0) cur = 0;
    else cur = abs(cur);

    int diff = abs( parent - cur);
    
    return diff;

}

int mud(int cur){

    if(cur < 0) cur = 0;

    return cur;
}
