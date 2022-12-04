#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define SIZE 16
#define N 10
#include "task3.h"


//  функция считывает по очереди все элементы со стандартного потока ввода, используя буффер в N элементов
char getsym(){
	static char symbols[N]= {'a'};
	static int i = N+1;
	if ( i >= N ){
		// Устанавливаем в буфере 0
		for (int k = 0;k != N;k++){
			symbols[k] = '\0';
		}
		if (fscanf( stdin, "%10c", symbols) == EOF){
			printf("EOF\n");
			exit(0);
		}
		if (symbols[0] == '\0'){
			i = N + 1;
			return getsym();
		} else {
			i = 1;
			return symbols[0];
		}
	} else {
		if (symbols[i] == '\0'){
			i = N + 1;
			return getsym();
		} else {
			i++;
			return symbols[i - 1];
		}
	}
}

void start();
void word();
void greater();
void greater2();
void amper();
void amper2();
void stick();
void stick2();
void less();
void semicolon();
void opbr();
void clbr();
void newline();
void stop();

int main(){
	c = getsym();
	//printf("%d\n", c);
	null_list();
	start();
}

void start() {
	//printf("start\n");
	if (c == ' '|| c == '\t'){
		//printf("c = ' '\n");
		c = getsym();
		start();
	} else if (c == EOF){
		//printf("c = EOF\n");
		termlist();
		printf("\n");
		printlist();
		printf("\n");
		clearlist();
		stop();
	} else if (c == '\n'){
		//printf("c = '\n'\n");
		termlist();
		printf("\n");
		printlist();
		printf("\n");
		//sort
		sortlist();
		printlist();
		printf("\n");
		c = getsym();
		newline();
	} else { 
		int newc = c;
		//printf("newc = %d\n", c);
		nullbuf();
		addsym();
		c = getsym();
		//printf("c = %d\n", c);
		switch( newc ){
			case '>':
				greater();
				break;
			case '<' :
				less();
				break;
			case '|' :
				stick();
				break;
			case '&':
				amper();
				break;
			case '(' :
				opbr();
				break;
			case ')' :
				clbr();
				break;
			case ';' :
				semicolon();
				break;
			default:
				word();
				break;
		}
	}
}

void word(){
	//printf("word\n");
	if( symset(c) ) {
		addsym();
		c = getsym();
		//printf("%d\n", c);
		word();
	} else {
		//printf("+word\n");
		addword();
		start();
	}
}

//>
void greater() { 
	if(c == '>') {
		addsym();
		c = getsym();
		greater2();
	} else {
		addword();
		start();
	}
}

//>>
void greater2() {
	addword();
	start();
}

//&
void amper() { 
	if(c == '&') {
		addsym();
		c = getsym();
		amper2();
	} else {
		addword();
		start();
	}
}

//&&
void amper2() {
	addword();
	start();
}

// |
void stick() { 
	if(c == '|') {
		addsym();
		c = getsym();
		stick2();
	} else {
		addword();
		start();
	}
}

// ||
void stick2() {
	addword();
	start();
}

void less() {
	addword();
	start();
}

// ;
void semicolon() {
	addword();
	start();
}

// (
void opbr() {
	addword();
	start();
}

// )
void clbr() {
	addword();
	start();
}

void newline(){
	//printf("newline\n");
	clearlist();
	start();
 }


void stop(){
	//printf("end\n");
	exit(0);
} 

