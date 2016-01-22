#ifndef SOLVE_H
#define SOLVE_H
    struct Dl_Node{
        int id_col; //id of belonging column
        int id_u, id_d, id_l, id_r; //ids of its neighbors
        int r1, c1, r2, c2; //use which two grid
        int v1, v2; //the value circled

        void print(){
            printf("%d: %d\t%d\t%d\t%d\t\n", id_col, id_u, id_d, id_l, id_r);
            printf("\t(%d,%d)\t(%d,%d)\t, (%d %d)\n", r1, c1, r2, c2, v1, v2);
        }
    };


    void solve();
#endif // SOLVE_H
