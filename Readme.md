--------------------------------------------------------------
Part 1 - create comp.c to compare between content of two files
	system calls: open(), read(), close() (Cant use strcmp(), strncmp(), strlen())
	comp.out a.txt b.txt
	comp.out return 1 if the two files are not identical and 2 if they are the same 
	To see the result you should use the command: echo $?

Part 2 - Check student homework 
	system calls: open(), access(), stat(), S_ISREG(), S_ISDIR(), read(), write(), close(), dup2(), opendir(), closedir(), readdir(), execl(), exit(-1), fork(), wait(),  
				 Do not use strcmp(), strncmp(), strlen() - The code for those functions should be written by yourself. No existing library functions, other than system call functions should be used.
	gcc EX1_2.c
	./a.out /home/oribachar98/Homework/M1/setup/config.txt
	
	for this program we are using configuration file: /home/oribachar98/Homework/M1/setup/config.txt
	This file contain defination:
		/home/oribachar98/Homework/M1/students - The sub folder list location of the student names.
		/home/oribachar98/Homework/M1/setup/test_input.qa - The keyboard input (use dup2 to read input from file instead from keboard).
		/home/oribachar98/Homework/M1/setup/expected_output.qa - The expected output that the student program should print to the screen
	
	Under the student folder there are the student names: David  Joe  Michael  Yael
	Each student folder should contain C code. and its compile version:
		David.c main.out
		Joe.c main.out
		Michael.c main.out
		Yael.c main.out
	The main.out program is an interactive software that prints on the screen requests that the user has to responce with keyboard numbers. 
	The software returns the sum of two numbers.
	main.out screen and keyboard interactions:
					--------------------------
					please enter first number:
					2
					please enter scond number:
					3
					the sum is:
					5
					--------------------------
	
	
	For each student, a.out will use the comp.out to compare between the main.out printout and the expected result that exist in expected_output.qa
	remark: main.out printout will be save into program_output.txt
	comp.out /home/oribachar98/Homework/M1/setup/expected_output.qa /home/oribachar98/Homework/M1/program_output.txt
	
	The score for all students will be save into one file results.csv 
	comp.out return 1 if the two files are not identical and 2 if they are the same
	The comp.out printout will be redirect from srcreen into results.csv file 
	
	
		
		
		