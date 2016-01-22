#include <cstdio>
#include <cstdlib>

#include "globals.h"
#include "io.h"
#include "solve.h"

//argv[1]: input file, argv[2]: output file(if not specified, use std)
int main(int argc, char *argv[]){
    if(argc<=1){ //if no input file name
        printf("Please provide input file name.\n");
        return 0;
    }

    if(input(argv[1])==false){ //try to open input file
        printf("Unable to open file %s\n", argv[1]);
        return 0;
    }

    solve();

    if(argc<=2) output(NULL); //output file not given, use std
    else output(argv[2]);

    return 0;
}
