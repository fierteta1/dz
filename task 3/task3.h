#ifndef TASK3
#define TASK3

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define SIZE 16
#define N 10

int c; //текущий символ 
char ** array_str; // список слов (в виде массива)
char * buf; // буфер для накопления текущего слова
int sizebuf; // размер буфера текущего слова
int sizelist;// размер списка слов
int curbuf; // индекс текущего символа в буфере
int curlist; // индекс текущего слова в списке

// функция очистки памяти
void clearlist(){
	//printf("clearlist\n");
	int i;
	sizelist = 0;
	curlist = 0;
	if (array_str == NULL) 
		return;
	for (i=0; array_str[i] != NULL; i++)
		free (array_str[i]);
	free (array_str);
	array_str = NULL;
}


// функция обнуления списка(создания)
void null_list(){
	//printf("null_list\n");
	sizelist = 0;
	curlist = 0;
	array_str = NULL;
}

//Функция termlist() завершает список, добавляя NULL в позицию curlist и обрезает память, занимаемую списком, до точного размера.
void termlist(){
	//printf("termlist\n");
	if (array_str == NULL) 
		return;
	if ( curlist > sizelist-1 )
		array_str = realloc (array_str, ( sizelist + 1 )*sizeof(*array_str));
	array_str[ curlist ] = NULL;
	array_str = realloc( array_str, ( sizelist = curlist + 1)*sizeof( *array_str ));//выравниваем используемую под список память точно по размеру списка
}

// обнуление buf
void nullbuf(){
	//printf("nullbuf\n");
	buf = NULL;
	sizebuf = 0;
	curbuf = 0;
}


//Функция добавляет очередной символ в буфер. Если буфер был пуст, то он создается.
void addsym(){
	//printf("addsym\n");
	if ( curbuf > sizebuf - 1 )
		buf = realloc( buf, sizebuf += SIZE); // увеличиваем буфер при необходимости 
	buf [curbuf++] = c;
}

//Функция завершает текущее слово в буфере, добавляя ’\0’ и обрезает память до точного
//размера; Если список был пуст, то он создается.
void addword(){
	//printf("addword\n");
	if ( curbuf > sizebuf - 1)
		buf = realloc( buf, sizebuf += 1); // для записи ’\0’ увеличиваем буфер при необходимости 
	buf[ curbuf++ ]='\0';
	buf = realloc( buf, sizebuf = curbuf );//выравниваем используемую память точно по размеру слова
	if ( curlist > sizelist - 1 )
		array_str = realloc( array_str, ( sizelist += SIZE )*sizeof(*array_str)); // увеличиваем массив под список при необходимости 
	array_str[ curlist++ ] = buf;
}

// функция печати списка
void printlist(){
	//printf("printlist\n");
	int i;
	if ( array_str == NULL)
		return;
	printf("%d\n", sizelist - 1);
	for (i=0; i < sizelist - 1; i++)
		printf( "%s\n", array_str[i] );
}

// функция возвращает 1 если обычный символ (не специальный)
int symset ( int c ){
	//printf("symset\n");
	return c != '\n' &&
	c != ' ' &&
	c != '|' &&
	c != '&' &&
	c != ';' &&
	c != '<' &&
	c != '(' &&
	c != ')' &&
	c != '\t' &&
	c != '>' &&
	c != EOF ;
} 


// функция упорядочивает список в лексикографическом порядке
void sortlist (){
	if (array_str == NULL)
		return;
	char *sortbuf = NULL;
	// bubble sort
	for (int i = 0; i < sizelist - 2; i++)
	{
		for (int j = 0; j < sizelist - i - 2; j++){ 
			if (strcmp(array_str[j], array_str[j+1]) > 0){
				sortbuf = array_str[j];
				array_str[j] = array_str[j+1];
				array_str[j+1] = sortbuf;
			}
		}
	}	
}

#endif
