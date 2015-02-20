#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

class arena{

public :
    char board[3][3];       //The board
    int turns;              //number of turns played.
    int compX;              //stores computers predicted move
    int compY;
    arena(){                //Constructor creates an empty board and set no. of terns as zero.
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                this->board[i][j] = '_';
        this->turns = 0;
    }

    char isWon(){
        if(turns>4){        //not possible to win in 4 turns.
            if(board[0][0]==board[1][1] && board[1][1]==board[2][2] && board[0][0]!='_')        // '\'Diagonal win
                return board[0][0];
            if(board[0][2]==board[1][1] && board[1][1]==board[2][0] && board[0][2]!='_')        // '/'Diagonal win
                return board[0][2];
            for(int i=0;i<3;i++){
                if(board[0][i]==board[1][i] && board[1][i]==board[2][i] && board[0][i]!='_')    // column win
                    return board[0][i];
                if(board[i][0]==board[i][1] && board[i][1]==board[i][2] && board[i][0]!='_')    // row win
                    return board[i][0];
            }
        }
        return '.';         //not yet won
    }
    void printBoard(){      //print current state of the board
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++)
                cout<<this->board[i][j];
            cout<<endl;
        }
        return;
    }
    int makeMove(int i,int j,char c){       //execute the move.
        if(this->board[i][j] != '_' || i<0 || i>2 || j<0 || j>2){
            cout<<"invalid move, try again!";   //check for invalid moves.
            return 0;
        }
        this->board[i][j] = c;
        return 1;
    }
    vector<pair<int,int>> left(){           //returns vector of co-ordinates which are empty.
        vector<pair<int,int>> myemp;
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                if(this->board[i][j]=='_')
                    myemp.push_back(make_pair(i,j));
        return myemp;
    }
    int minimax(int depth, char turn){      //recursive min-max algorithm for predicting computer's move.
        int minscore = 2,maxscore = -2;
        int score;
        vector<pair<int,int>> emp;
        emp = this->left();
        if(this->isWon()=='X')
            return 1;
        if(this->isWon()=='O')
            return -1;
        if(this->turns >= 9)      //DRAW
            return 0;

        for(int i=0;i<emp.size();i++){
            if(turn == 'X'){
                if(this->board[emp[i].first][emp[i].second] == '_'){
                    this->makeMove(emp[i].first,emp[i].second,'X');
                    this->turns++;
                    score = this->minimax(depth+1,'O');
                    maxscore = max(score,maxscore);
                }
                else                //this state probably never occurs.
                    cout<<"something wrong here"<<depth<<"\n";

                this->board[emp[i].first][emp[i].second] = '_';
                this->turns--;
                if(score > 0 && depth==0){
                    this->compX = emp[i].first;
                    this->compY = emp[i].second;
                    return 1;
                }
                else if(score > 0)
                    return 1;
                else if(score == 0 && depth==0){
                    this->compX = emp[i].first;
                    this->compY = emp[i].second;
                }
                else if(depth == 0 && i == emp.size()-1 && maxscore < 0){
                    this->compX = emp[i].first;
                    this->compY = emp[i].second;
                    return -1;
                }
            }
            else if(turn == 'O'){
                if(this->board[emp[i].first][emp[i].second] == '_'){
                    this->makeMove(emp[i].first,emp[i].second,'O');
                    this->turns++;
                    score = this->minimax(depth+1,'X');
                    minscore = min(score,minscore);
                }
                else                //this state probably never occurs.
                    cout<<"something wrong here"<<depth<<"\n";

                this->board[emp[i].first][emp[i].second] = '_';
                this->turns--;
                if(score < 0)
                    return -1;
            }
        }
    return turn=='X'?maxscore:minscore;
    }
};


int main(){
    char start;
    cout<<"Welcome to the GAME!!!\n\n";
    playing:
    youfool:
    cout<<"Who starts? (X for Comp and O for Player):\n";
    cin>>start;
    if(start != 'X' && start != 'O')
        goto youfool;
    arena *game = new arena();
    int won=0;
    if(start == 'X'){               //Computer starts with a random position.
        int x,y;
        x = rand();
        y = rand();
        cout<<"\nComputer's turn:\n";
        game->makeMove(x%3,y%3,'X');
        game->turns ++;
        game->printBoard();
    }

    while(game->isWon()=='.' && game->turns<9){     //Play game until 9 moves or an early win.
        int x,y,chance=-1;
        repeat:
        cout<<"\nP1 enter your move (co-ordinates [0-2][0-2]) :\n";    //Player enters his move
        cin>>x>>y;

        if(!game->makeMove(x,y,'O'))        //Player's move is executed
            goto repeat;
        game->turns ++;
        game->printBoard();
        if(game->turns >= 9 || game->isWon()!='.')
            break;
        cout<<"Computer's turn:\n";
        chance = game->minimax(0,'X');      //Computer enters it's move
        game->makeMove(game->compX,game->compY,'X');    //Computer's move is executed
        game->turns ++;
        game->printBoard();
    }

    //Print the result of the match.
    if(game->isWon() == 'X'){
        cout<<"\n1Computer Wins\n";
        won = 1;
    }
    if(game->isWon() == 'O'){
        cout<<"\n2Player Wins\n";
        won = 1;
    }
    if(game->turns>=9 && won==0)
        cout<<"\nDRAW!!";
    char again;
    cout<<"\n\nWanna play again? (Y/N):";
    cin>>again;
    if(again == 'Y' || again == 'y')
        goto playing;
    return 0;
}
