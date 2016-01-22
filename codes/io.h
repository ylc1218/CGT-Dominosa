#include <vector>
using namespace std;

#ifndef IO_H
#define IO_H

    struct Ans_Node{
        int r1, r2, c1, c2;
        Ans_Node(int a, int b, int c, int d){
            r1=a;
            c1=b;
            r2=c;
            c2=d;
        }
        void print(){
            printf(" %d,%d -> %d,%d\n", r1, c1, r2, c2);
        }
    };

    bool input(char* fileName);
    void output(char* fileName);
    void output_std();

    extern vector<Ans_Node> ans;
#endif // IO_H
