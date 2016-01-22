#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "globals.h"
#include "io.h"
using namespace std;

int R, C;
int init_board[MAX_SIZE][MAX_SIZE];
char out_board[MAX_SIZE*2+5][MAX_SIZE*4+5];
vector<Ans_Node> ans;


bool input(char* fileName){
    FILE* fin;
    fin = fopen(fileName, "r");
    if(fin!=NULL){
        fscanf(fin, "%d%d", &R, &C);
        for(int r=0;r<R;r++){
            for(int c=0;c<C;c++){
                fscanf(fin, "%d", &init_board[r][c]);
            }
        }
        fclose(fin);
        return true;
    }
    return false;
}


void genInitial(){
    memset(out_board, ' ', sizeof(out_board[0][0])*(MAX_SIZE*2+5)*(MAX_SIZE*4+5));

    //initial original board with numbers
    for(int r=0;r<R;r++){
        for(int c=0;c<C;c++){
            out_board[2*(r+1)-1][4*(c+1)-1]='0'+init_board[r][c]%10;
            if(init_board[r][c]>=10) out_board[2*(r+1)-1][4*(c+1)-2]='0'+init_board[r][c]/10;
        }
    }
}

void genOutput(){
    //circle the numbers according to ans
    for(int i=0;i<int(ans.size());i++){
        Ans_Node node = ans[i];
        int r1, c1, r2, c2;
        int tr, tc, dr, dc;

        r1 = 2*(node.r1+1)-1;
        c1 = 4*(node.c1+1)-1;
        r2 = 2*(node.r2+1)-1;
        c2 = 4*(node.c2+1)-1;

        tr = r1-1;
        tc = c1-2;
        dr = r2+1;
        dc = c2+2;

        out_board[tr][tc] = out_board[tr][dc] = out_board[dr][tc] = out_board[dr][dc] = '+';

        //top & down
        for(int c=tc+1;c<dc;c++){
            if(out_board[tr][c]!='+') out_board[tr][c]='-';
            if(out_board[dr][c]!='+') out_board[dr][c]='-';
        }

        //left & right
        for(int r=tr+1;r<dr;r++){
            if(out_board[r][tc]!='+') out_board[r][tc]='|';
            if(out_board[r][dc]!='+') out_board[r][dc]='|';
        }

    }


    /*
    printf(" +---+-------+\n");
    printf(" | 3 | 0   3 |\n");
    printf(" |   +-------+\n");
    printf(" | 1 | 2   5 |\n");
    printf(" +---+---+---+\n");
    printf(" | 4   3 | 0 |\n");
    printf(" +-------+   |\n");
    */

}

void output(char *fileName){
    FILE* fout;
    if(fileName!=NULL) fout = fopen(fileName, "w");
    else fout = stdout;

    genInitial();
    //print original board
    for(int r=0;r<R*2+1;r++){
        for(int c=0;c<C*4+3;c++){
            fprintf(fout, "%c", out_board[r][c]);
        }
        fprintf(fout, "\n");
    }

    genOutput();
    //print answer
    for(int r=0;r<R*2+1;r++){
        for(int c=0;c<C*4+3;c++){
            fprintf(fout, "%c", out_board[r][c]);
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
}


