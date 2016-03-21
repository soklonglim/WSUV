Name:		Soklong Lim
SID:		11458249
Project:	LU Decomposition Engine
Class:		CS 330 (Numerical Computing)
Due date:	November 11, 2015

Description: this is a C99 module for solving linear equation of the
form Ax = b using LU decomposition. it will take input and create a 
matrix and decompose into LU form, (LU)x = b. then split the equation
into 2 steps solving process, Ly = b using forward and Ux = y using 
backward substitution to get x.   

List of files:	LUdecomp.c LUdecomp.h makefile LUtest.c README.txt 

Usage:
	1) Unzip:	tar -xvzf LU.tar.gz 
	2) Compile:	make
	3) Run: 	./LUtest	
