#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stack>
#include <utility>
#include <algorithm>
#include <fstream>
#include <random>

#define grid_x 1000 // change this to adjust the dimension on x axis
#define grid_y 1000 // change this to adjust the dimension on y axis
#define obstacle 'H'
#define opened 'O'
#define start_symbol 'S'
#define goal_symbol 'G'

using namespace std;

char the_grid[grid_x][grid_y];
stack<pair<int,int>> visited;
int start_x = 0;
int start_y = 0;

int goal_x = 0;
int goal_y = 0;

void printgrid(){
    cout << endl;
    for(int x = 0; x < grid_x; x++){
        for(int y = 0; y < grid_y; y++){
            //cout << x << "," << y << "-";
            if(the_grid[x][y] == opened)
                cout << "  ";
            else
                cout << the_grid[x][y] << " ";
        }
        cout << endl;
    }
}

void exportToCSP(){
    ofstream myfile;
    myfile.open ("grid.csp");

    // Start writing

    // #define M 10;
    myfile << "#define M ";
    myfile << grid_x;
    myfile << ";";
    myfile << endl;
    // #define N 10;
    myfile << "#define N ";
    myfile << grid_y;
    myfile << ";";
    myfile << endl;

    // var r:{0..N-1} = xxx;
    myfile << "var r:{0..N-1} = ";
    myfile << start_x;
    myfile << ";";
    myfile << endl;
    // var r:{0..N-1} = yyy;
    myfile << "var c:{0..M-1} = ";
    myfile << start_y;
    myfile << ";";
    myfile << endl;

    // write the board
    myfile << "var board[N][M] = [";
    myfile << endl;
    for(int x = 0; x < grid_x; x++){
        for(int y = 0; y < grid_y; y++) {
            if(x == grid_x-1 && y == grid_y-1)
                myfile << the_grid[x][y];
            else
                myfile << the_grid[x][y] << ",";
        }
        myfile << endl;
    }
    myfile << "];";
    myfile.close();
}

void exportToTXT(){
    ofstream myfile;
    myfile.open ("grid.txt");

    for(int x =0; x < grid_x; x++){
        for(int y =0; y < grid_y; y++) {
            myfile << the_grid[x][y];
        }
        myfile << endl;
    }
    myfile << start_x;
    myfile << endl;
    myfile << start_y;
    myfile.close();
}

void makeEasyPath(){
    // make the solution path if the maze bigger than 100
    vector<pair<int, int>>solution_path;

    // breaking ball position
    int breaking_x = start_x;
    int breaking_y = start_y;

    // move to the correct x coordinate of goal
    while(breaking_x != goal_x){

        if(breaking_x < goal_x){
            breaking_x++;
        }
        else if(breaking_x > goal_x){
            breaking_x--;
        }

        pair <int, int> new_location;
        new_location.first = breaking_x;
        new_location.second = breaking_y;
        solution_path.push_back(new_location);

    }

    // move to the correct y coordinate of goal
    while(breaking_y != goal_y){

        if(breaking_y < goal_y){
            breaking_y++;
        }
        else if(breaking_y > goal_y){
            breaking_y--;
        }

        pair <int, int> new_location;
        new_location.first = breaking_x;
        new_location.second = breaking_y;
        solution_path.push_back(new_location);

    }

    // break the walls to make paths
    for(int i = 0; i < solution_path.size(); i++){
//        cout << solution_path[i].first << " " << solution_path[i].second << endl;
        the_grid[solution_path[i].first][solution_path[i].second] = opened;
    }

    // since it override the goal with path, we need to write the goal on the grid again
    the_grid[goal_x][goal_y] = goal_symbol;
}

int main(int argc, char *argv[]){
    srand(time(0));

    // 1 = down
    // 2 = up
    // 3 = left
    // 4 = right

    //defining the entire grid an an obstacle
    for(int x = 0; x < grid_x; x++){
        for(int y = 0; y < grid_y; y++){
            the_grid[x][y] = obstacle;
        }
    }

    //defining beginning position
    visited.push({0,0});

    int count = 0;

    while(visited.empty() == false){

        //a list of directions that a random.
        int direction[5] = {1,2,3,4};

        int amount = 0;

        while(amount <= 3){
            int random_num = (rand() % 4);
            int a_temp = direction[amount];
            direction[amount] = direction[random_num];
            direction[random_num] = a_temp;
            amount +=1;
        }

        direction[4] = -1;

        pair <int, int> current_node = visited.top();

        //check for a direction and went there. As well as removing the wall
        for(int i =0; i < 5;i++){
            switch(direction[i]){
                case 1:
                    //down
                    //check if it possible
                    if(current_node.second + 2 < grid_y){
                        //check if it open
                        if(the_grid[current_node.first][current_node.second + 2] == obstacle){
                            //yes travese
                            //cout << "down" << endl;
                            the_grid[current_node.first][current_node.second + 1] = opened;
                            the_grid[current_node.first][current_node.second + 2] = opened;
                            visited.push({current_node.first,current_node.second + 2});
                            i = 5;
                        }
                    }
                    break;
                case 2:
                    //up
                    //check if it possible
                    if(current_node.second - 2 >= 0){
                        //check if it open
                        if(the_grid[current_node.first][current_node.second - 2] == obstacle){
                            //yes travese
                            //cout << "up" << endl;
                            the_grid[current_node.first][current_node.second - 1] = opened;
                            the_grid[current_node.first][current_node.second - 2] = opened;
                            visited.push({current_node.first,current_node.second - 2});
                            i = 5;
                        }
                    }
                    break;
                case 3:
                    //left
                    //check if it possible
                    if(current_node.first - 2 >= 0){
                        //check if it open
                        if(the_grid[current_node.first - 2][current_node.second] == obstacle){
                            //yes travese
                            //cout << "left" << endl;
                            the_grid[current_node.first - 1][current_node.second] = opened;
                            the_grid[current_node.first - 2][current_node.second] = opened;
                            visited.push({current_node.first - 2,current_node.second});
                            i = 5;
                        }
                    }
                    break;
                case 4:
                    //right
                    //check if it possible
                    if(current_node.first + 2 < grid_x){
                        //check if it open
                        if(the_grid[current_node.first + 2][current_node.second] == obstacle){
                            //yes travese
                            //cout << "right" << endl;
                            the_grid[current_node.first + 1][current_node.second] = opened;
                            the_grid[current_node.first + 2][current_node.second] = opened;
                            visited.push({current_node.first + 2,current_node.second});
                            i = 5;
                        }
                    }
                    break;
                default:
                    //pops a node if it cannot go anywhere.
                    visited.pop();
                    count += 1;
                    i = 5;
            }
        }
    }

    //define start position
    bool flag = true;
    while(flag){
        start_x = rand() % grid_x + 0;
        start_y = rand() % grid_y + 0;
        if(the_grid[start_x][start_y] == opened){
            the_grid[start_x][start_y] = start_symbol;
            flag = false;
        }
    }

    //define goal position
    flag = true;
    while(flag){
        goal_x = rand() % grid_x + 0;
        goal_y = rand() % grid_y + 0;
        if(the_grid[goal_x][goal_y] == opened){
            the_grid[goal_x][goal_y] = goal_symbol;
            flag = false;
        }
    }

    // define the open
    the_grid[start_x][start_y] = opened;

    // if maze too big then we need to make an easy path for maze runner
    //if(grid_x > 50 || grid_y > 50)
        //makeEasyPath();

    cout << "Maze dimension: " << grid_x << " " << grid_y << endl;
    cout << "Start: " << start_x << " " << start_y << endl;
    cout << "Goal: " << goal_x << " " << goal_y << endl;
    cout << "[Finish making the maze]" << endl;

    if(grid_x <= 50 && grid_y <= 50)
        printgrid();

    // export csp and txt
    exportToCSP();
    exportToTXT();

    cout << endl;
    cout << "[Export to grid.txt and grid.csp]" << endl;
    cout << "[All done]";
}