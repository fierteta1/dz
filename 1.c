#include <stdio.h>
#include <stdlib.h>
#include <math.h>
const int n = 14; // количество уравнений
const long double x[] = {0.15,0.3,0.45,0.6,0.75,0.9,1.05,1.2,1.35,1.5,1.65,1.8,1.95,2};
const long double y[] = {0.002301,0.026932,0.070902,0.030482,0.241127,0.712619,0.994324,0.48283,1.107277,3.16575,4.144453,2.403475,2.369195,4.352169};


void vvod_a(long double matrix[n][n+1]){
	// ввод матрицы для пункта а
	for (int i = 0; i < n; i++){
		matrix[i][0] = 1;
		matrix[i][1] = x[i];
		for (int j = 2; j < n; j++){
			matrix[i][j] = matrix[i][j-1]*x[i];
		}
		matrix[i][n] = y[i];
	}
}

void print(long double matrix[n][n+1]){
	// вывод матрицы
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n + 1; j++){
			printf("%Lf ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("end\n\n");
	
}

void printanswer (long double answer[n], int order[n]){
	//Вывод результата
	printf("answer: \n");
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
		if ( i == order[j] ){ //Расставляем корни по порядку
			printf( "%Lf ", answer[j] );
			break;
        }
    printf("\n");
}

void m_elem ( int k, long double matrix[n][n+1], int n, int order[n]){ // поиск ведущего элемента
	int i, j, imax = k, jmax = k;
	long double temp;
	//Ищем максимальный по модулю элемент
	for ( i = k; i < n; i++ )
		for ( j = k; j < n; j++ )
		if ( fabs( matrix[imax][jmax] ) < fabs( matrix[i][j] ) )
		{
			imax = i;
			jmax = j;
		}
	//Переставляем строки
	for ( j = k; j < n + 1; j++ ){
		temp = matrix[k][j];
		matrix[k][j] = matrix[imax] [j];
		matrix[imax][j] = temp;
	}
	//Переставляем столбцы
	for ( i = 0; i < n; i++ ){
		temp = matrix[i][k];
		matrix[i][k] = matrix[i][jmax];
		matrix[i][jmax] = temp;
	}
	//Учитываем изменение порядка корней
	i = order[k];
	order[k] = order[jmax];
	order[jmax] = i;
}

int main(){
	long double diag, downdiag;
	long double matrix[n][n+1] ; 
	long double answer[n]; // ответы
	int order[n]; // порядок ответов
	vvod_a(matrix);
	print(matrix);
	
	// сначала все корни по порядку
	for ( int i = 0; i < n; i++ ){
		order[i] = i;
	}
	
	// прямой ход
	for (int i = 0; i < n; i++){
		m_elem( i, matrix, n, order);// установка главного элемента
		diag = matrix[i][i];
		for (int j = i; j < n + 1; j++){
			// делим строку на диагональный элемент
			matrix[i][j] = matrix[i][j]/diag;
		}
		for (int k = i + 1; k < n; k++){ // вычитаем из остальных строк
			downdiag = matrix[k][i];
			for (int j = i; j < n + 1; j++){
				matrix[k][j] = matrix[k][j] - downdiag* matrix[i][j];
			}
		}
	}
	
	// обратный ход
	for  (int i = 0; i < n; i++){
		answer[i] = matrix[i][n];
	}
	for  (int i = n-2; i >= 0; i--){
		for  (int j = i+1; j < n; j++){
			answer[i] = answer[i] - answer[j]*matrix[i][j];
		}
	}
	printanswer(answer, order); // вывести ответ, расставить по порядку
	return 0;
}


