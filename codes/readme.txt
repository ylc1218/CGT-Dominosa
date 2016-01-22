Language:
	C++

Code Structure:
	main.cpp
	solve.cpp
	io.cpp
	(globals.h)
	(solve.h)
	(io.h)
	
Compile:
	$ g++ main.cpp solve.cpp io.cpp -o hw1

Run Command
	$ hw1 [input file] [output file]
	
	Need to provide an input file.
	If didn't provide an output file name, will display at stdout.
	
	For example:
		$ hw1 input/n10.txt out.txt	//create out.txt
		$ hw1 input/n10.txt	//use stdout

Input
	I generated some input files from websites. They are put in the folder "/input".
	The format of an input file:
		Two integers R and C on first line (5<=R<=35, C=R+1), followed by R lines, representing the initial board.

Output
	The program will output the initial board and the solution.