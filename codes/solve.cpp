#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "solve.h"
#include "io.h"
#include "globals.h"

#define TYPE_P 1
#define TYPE_D 2

using namespace std;


int DL_H_ID; //dl head id
vector<Dl_Node> dl;
int cnt[MAX_SIZE*MAX_SIZE*2]; //#nodes left in that column
int d_start[MAX_SIZE];

bool hasAns;
int cnt_back=0, cnt_state=0; //for experiment

/**
Col id starts from 0, positions first, followed by dominoes
i.e
-head- --------Positions--------------------------          ---------Dominoes--------
   0     1    2    3    4    5    6    7    8    9  .......  R*C+1  R*C+2  R*C+3  R*C+4......
       (0,0)(0,1)(0,2)(1,0)(1,1)(1,2)(2,0)(2,1)(2,2)         (0,0) (0,1)  (0,2)  (1,1)
*/
int getColId(int a, int b, int type){ //given position(r,c) or domino(v1,v2), return column dl node id
    if(type==TYPE_P) //(a,b) = (r,c)
        return a*C+b+1;
    else{ //(a,b) = (v1,v2)
        if(a>b) swap(a,b);
        return (R*C)+d_start[a]+b-a+1;
    }
}


/**
    just remove the in-edges pointed to cid, do not remove the out-edges pointed out from cid
    so this column can be resume easily

    note that after the removal, the nodes removed are still a connected component
*/
void removeDlCol(int cid){
    //remove this col from head row
    dl[dl[cid].id_l].id_r = dl[cid].id_r;
    dl[dl[cid].id_r].id_l = dl[cid].id_l;

    //for each row connected to cid
    for(int nid = dl[cid].id_d; nid!=cid; nid = dl[nid].id_d){
        //for each node not in column cid
        for(int mid = dl[nid].id_r; mid!=nid; mid=dl[mid].id_r){
            //remove the node from it's column
            dl[dl[mid].id_u].id_d = dl[mid].id_d;
            dl[dl[mid].id_d].id_u = dl[mid].id_u;
            cnt[dl[mid].id_col]--;
        }

        cnt[cid]--;
    }
}

void resumeDlCol(int cid){
    //for each row connected to cid
    for(int nid = dl[cid].id_u; nid!=cid; nid = dl[nid].id_u){
        //for each node not in column cid
        for(int mid = dl[nid].id_l; mid!=nid; mid=dl[mid].id_l){
            //remove the node from it's column
            dl[dl[mid].id_u].id_d = mid;
            dl[dl[mid].id_d].id_u = mid;
            cnt[dl[mid].id_col]++;
        }

        cnt[cid]++;
    }

    //put this column back to head row
    dl[dl[cid].id_l].id_r = cid;
    dl[dl[cid].id_r].id_l = cid;
}


void dfs_dl(int layer){
    cnt_state++;
    if(layer==R*C/2){
        /*printf("#back=%d\n", cnt_back);
        /printf("#state=%d\n", cnt_state-1);*/
        hasAns=true;
        return;
    }

    //option1: find min cnt[col]
    int min_cnt, min_cid;

    for(int id = dl[DL_H_ID].id_r; id!=DL_H_ID ; id =dl[id].id_r){
        if(cnt[dl[id].id_col]==0) return; //if a column is still connected to head, but doesn't have any row can choose, wrong path
        if(id==dl[DL_H_ID].id_r || cnt[dl[id].id_col]<min_cnt){
            min_cnt = cnt[dl[id].id_col];
            min_cid = dl[id].id_col;
        }
    }

    //option2: choose an arbitrary column to start with
    /*min_cid = dl[DL_H_ID].id_r;
    min_cnt = cnt[dl[dl[DL_H_ID].id_r].id_col];*/

    //remove min column
    removeDlCol(min_cid);

    //pick a row in min column to use as ans and dfs
    for(int nid = dl[min_cid].id_d; nid!=min_cid; nid = dl[nid].id_d){
        //push ans
        Ans_Node node = Ans_Node(dl[nid].r1, dl[nid].c1, dl[nid].r2, dl[nid].c2);
        ans.push_back(node);

        //remove every column connected to the picked row (nid)
        for(int mid=dl[nid].id_r; mid!=nid; mid=dl[mid].id_r)
            removeDlCol(dl[mid].id_col);

        //dfs(will check if this move is available in the next layer's "find min cnt[col]")
        dfs_dl(layer+1);
        if(hasAns) return;
        cnt_back++;

        //resume every column connected to the picked row (nid)
        for(int mid=dl[nid].id_l; mid!=nid; mid=dl[mid].id_l)
            resumeDlCol(dl[mid].id_col);

        //pop ans
        ans.pop_back();
    }

    //resume min column
    resumeDlCol(min_cid);
}


/**
simply put the node to the right position, without modifying its other values(ex: r, c, value, cnt)
*/
int addDlNode(int id_u, int id_d, int id_l, int id_r){
    Dl_Node node;
    node.id_u = id_u;
    node.id_d = id_d;
    node.id_l = id_l;
    node.id_r = id_r;

    dl.push_back(node);
    dl[id_u].id_d = dl[id_d].id_u = dl[id_l].id_r = dl[id_r].id_l = dl.size()-1;

    return dl.size()-1;
}

void addDlRow(int r1, int c1, int r2, int c2, int v1, int v2){
    int cid;

    //first position
    cid = getColId(r1,c1,TYPE_P);
    int id_p1 = addDlNode(cid, dl[cid].id_d, dl.size(), dl.size());
    dl[id_p1].id_col = cid;
    cnt[cid]++;

    //second position
    cid = getColId(r2,c2,TYPE_P);
    int id_p2 = addDlNode(cid, dl[cid].id_d, id_p1, dl[id_p1].id_r);
    dl[id_p2].id_col = cid;
    cnt[cid]++;

    //domino node
    cid = getColId(v1,v2,TYPE_D);
    int id_dom = addDlNode(cid, dl[cid].id_d, id_p2, dl[id_p2].id_r);
    dl[id_dom].id_col = cid;
    cnt[cid]++;

    //extra value
    dl[id_p1].r1=dl[id_p2].r1=dl[id_dom].r1=r1;
    dl[id_p1].r2=dl[id_p2].r2=dl[id_dom].r2=r2;
    dl[id_p1].c1=dl[id_p2].c1=dl[id_dom].c1=c1;
    dl[id_p1].c2=dl[id_p2].c2=dl[id_dom].c2=c2;
    dl[id_p1].v1=dl[id_p2].v1=dl[id_dom].v1=v1;
    dl[id_p1].v2=dl[id_p2].v2=dl[id_dom].v2=v2;
}


void createDlRows(){
    for(int r=0;r<R;r++){
        for(int c=0;c<C;c++){
            if(r+1<R) addDlRow(r, c, r+1, c, init_board[r][c], init_board[r+1][c]); //vertical down
            if(c+1<C) addDlRow(r, c, r, c+1, init_board[r][c], init_board[r][c+1]); //horizontal right
        }
    }
}

void initDl(int c){ //#columns(not include head)
    //init head and columns
    dl.clear();
    DL_H_ID = addDlNode(0,0,0,0); //head node
    for(int i=1;i<=c;i++){
        int id = addDlNode(dl.size(), dl.size(), dl[DL_H_ID].id_l, DL_H_ID);
        dl[id].id_col=i;
        cnt[id]=0;
    }

    /**
        init d_start(domino start) array for further indexing

          0     1      2      3     4     5
        (0,0) (0,1)  (0,2)  (1,1) (1,2) (2,2)

        d_start[0]=0, d_start[1]=3, d_start[2]=5

        in this case, N=2(max num), R=N+1=3, C=N+2=4
    */
    int now=0;
    for(int i=0;i<R;i++){
        d_start[i]=now;
        now+=R-i;
    }

    hasAns=false;
}

void solve(){
    ans.clear();
    initDl(R*C+(R*C)/2);
    createDlRows();
    dfs_dl(0);
}
