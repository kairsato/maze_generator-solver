#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stack>
#include <utility>
#include <algorithm>
#include <fstream>
#include <random>

#define grid_x 20 // change this to adjust the dimension on x axis
#define grid_y 20 // change this to adjust the dimension on y axis
#define obstacle 'H'
#define opened 'O'
#define startsymbol 'S'
#define goalsymbol 'G'

using namespace std;

char thegrid[grid_x][grid_y];
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
            if(thegrid[x][y] == opened)
                cout << "  ";
            else
                cout << thegrid[x][y] << " ";
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
                myfile << thegrid[x][y];
            else
                myfile << thegrid[x][y] << ",";
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
            myfile << thegrid[x][y];
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
    vector<pair<int, int>>solutionPath;

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

        pair <int, int> newLocation;
        newLocation.first = breaking_x;
        newLocation.second = breaking_y;
        solutionPath.push_back(newLocation);

    }

    // move to the correct y coordinate of goal
    while(breaking_y != goal_y){

        if(breaking_y < goal_y){
            breaking_y++;
        }
        else if(breaking_y > goal_y){
            breaking_y--;
        }

        pair <int, int> newLocation;
        newLocation.first = breaking_x;
        newLocation.second = breaking_y;
        solutionPath.push_back(newLocation);

    }

    // break the walls to make paths
    for(int i = 0; i < solutionPath.size(); i++){
//        cout << solutionPath[i].first << " " << solutionPath[i].second << endl;
        thegrid[solutionPath[i].first][solutionPath[i].second] = opened;
    }

    // since it override the goal with path, we need to write the goal on the grid again
    thegrid[goal_x][goal_y] = goalsymbol;
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
            thegrid[x][y] = obstacle;
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
            int randomnum = (rand() % 4);
            int atemp = direction[amount];
            direction[amount] = direction[randomnum];
            direction[randomnum] = atemp;
            amount +=1;
        }

        direction[4] = -1;

        pair <int, int> currentnode = visited.top();

        //check for a direction and went there. As well as removing the wall
        for(int i =0; i < 5;i++){
            switch(direction[i]){
                case 1:
                    //down
                    //check if it possible
                    if(currentnode.second + 2 < grid_y){
                        //check if it open
                        if(thegrid[currentnode.first][currentnode.second + 2] == obstacle){
                            //yes travese
                            //cout << "down" << endl;
                            thegrid[currentnode.first][currentnode.second + 1] = opened;
                            thegrid[currentnode.first][currentnode.second + 2] = opened;
                            visited.push({currentnode.first,currentnode.second + 2});
                            i = 5;
                        }
                    }
                    break;
                case 2:
                    //up
                    //check if it possible
                    if(currentnode.second - 2 >= 0){
                        //check if it open
                        if(thegrid[currentnode.first][currentnode.second - 2] == obstacle){
                            //yes travese
                            //cout << "up" << endl;
                            thegrid[currentnode.first][currentnode.second - 1] = opened;
                            thegrid[currentnode.first][currentnode.second - 2] = opened;
                            visited.push({currentnode.first,currentnode.second - 2});
                            i = 5;
                        }
                    }
                    break;
                case 3:
                    //left
                    //check if it possible
                    if(currentnode.first - 2 >= 0){
                        //check if it open
                        if(thegrid[currentnode.first - 2][currentnode.second] == obstacle){
                            //yes travese
                            //cout << "left" << endl;
                            thegrid[currentnode.first - 1][currentnode.second] = opened;
                            thegrid[currentnode.first - 2][currentnode.second] = opened;
                            visited.push({currentnode.first - 2,currentnode.second});
                            i = 5;
                        }
                    }
                    break;
                case 4:
                    //right
                    //check if it possible
                    if(currentnode.first + 2 < grid_x){
                        //check if it open
                        if(thegrid[currentnode.first + 2][currentnode.second] == obstacle){
                            //yes travese
                            //cout << "right" << endl;
                            thegrid[currentnode.first + 1][currentnode.second] = opened;
                            thegrid[currentnode.first + 2][currentnode.second] = opened;
                            visited.push({currentnode.first + 2,currentnode.second});
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
        if(thegrid[start_x][start_y] == opened){
            thegrid[start_x][start_y] = startsymbol;
            flag = false;
        }
    }

    //define goal position
    flag = true;
    while(flag){
        goal_x = rand() % grid_x + 0;
        goal_y = rand() % grid_y + 0;
        if(thegrid[goal_x][goal_y] == opened){
            thegrid[goal_x][goal_y] = goalsymbol;
            flag = false;
        }
    }

    // define the open
    thegrid[start_x][start_y] = opened;

    // if maze too big then we need to make an easy path for maze runner
    if(grid_x > 50 || grid_y > 50)
        makeEasyPath();

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