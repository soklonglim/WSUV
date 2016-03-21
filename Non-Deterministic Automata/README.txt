Name:	Soklong Lim
SID:	11458249
Email:	soklong.lim@wsu.edu
Class:	CS 317 (Automata and Formal Languages)
Due:	October 8, 2015 at 1pm
Project:	"Regular Expression to Finite Automata"

-Description
   The program written in C reads input from command line by specifying the 
name. For example, ./nfa 'filename' without quote. 
   1) it reads input and stores in frm_file[25][50]
   2) it extracts each line and one element at a time
   3) stack is stack[15][150] which is used as a stack and nFa at the same time
   4) there are 3 operators, '&' (Concatenation), '+' (Union), '*' (Kleene star),
	 and one nFa creation
   5) processing is built segment by segment storing in temp[150] and when done 
	it will be push back to stack[15][150]
   6) when finished, program will print each line of inputs followed by the list 
	of states and transition (state and accept will be printed in the end of each list)
   7) any input error, inexisting file will terminate the program and message 
	"File Corrupted" will be printed    

-How to build and compile
  	tar -xvzf nfa.tar.gz 
	gcc -g -std=c99 -Wall nfa.c -o nfa
	./nfa inputFileName

-Included files
   naf.c	:code file
   input.txt 	:samples postfix input file (copy from prject instruction)
   README.txt	:instruction file      
