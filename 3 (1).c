#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
const int n = 15; // количество точек
const int m = 5; // степень приближающего полинома( <5 )
const long double x[] = {0,0.15,0.3,0.45,0.6,0.75,0.9,1.05,1.2,1.35,1.5,1.65,1.8,1.95,2};
const long double y[] = {0,0.002301,0.026932,0.070902,0.030482,0.241127,0.712619,0.994324,0.48283,1.107277,3.16575,4.144453,2.403475,2.369195,4.352169};


void vvod_b(long double matrix[m][m+1]){
	// ввод для пункта b
	for (int i = 0; i < m; i++){
		for (int j = 0; j < m; j++){
			for (int k = 0; k < n; k++){
				matrix[i][j] = matrix[i][j]+powl(x[k],i+j);
			}
		}
		long double sum = 0;
		for (int k = 0; k < n; k++){
			sum = sum + y[k]*powl(x[k], i);
		}
		matrix[i][m] = sum;
	}
}

void print(long double matrix[m][m+1]){
	for (int i = 0; i < m; i++){
		for (int j = 0; j < m + 1; j++){
			printf("%Lf ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("end\n\n");
	
}

void printanswer (long double answer[m], int order[m]){
	//Вывод результата
	printf("answer: \n");
	for ( int i = 0; i < m; i++ )
		for ( int j = 0; j < m; j++ )
		if ( i == order[j] ){ //Расставляем корни по порядку
			printf( "%Lf ", answer[j] );
			break;
        }
    printf("\n");
}

void m_elem ( int k, long double matrix[m][m+1], int m, int order[m]){
	int i, j, imax = k, jmax = k;
	long double temp;
	//Ищем максимальный по модулю элемент
	for ( i = k; i < m; i++ )
		for ( j = k; j < m; j++ )
		if ( fabs( matrix[imax][jmax] ) < fabs( matrix[i][j] ) )
		{
			imax = i;
			jmax = j;
		}
	//Переставляем строки
	for ( j = k; j < m + 1; j++ ){
		temp = matrix[k][j];
		matrix[k][j] = matrix[imax][j];
		matrix[imax][j] = temp;
	}
	//Переставляем столбцы
	for ( i = 0; i < m; i++ ){
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
	long double matrix[m][m+1];
	for ( int i = 0; i < m; i++ )
		for ( int j = 0; j < m+1; j++ )
			matrix[i][j] = 0;
	long double answer[m]; // ответы
	int order[m]; // порядок ответов
	vvod_b(matrix);
	print(matrix);
	
	// сначала все корни по порядку
	for ( int i = 0; i < m; i++ ){
		order[i] = i;
	}
	
	// прямой ход
	for (int i = 0; i < m; i++){
		m_elem( i, matrix, m, order);// установка главного элемента
		diag = matrix[i][i];
		for (int j = i; j < m + 1; j++){
			// делим строку на диагональный элемент
			matrix[i][j] = matrix[i][j]/diag;;
		}
		for (int k = i + 1; k < m; k++){ // вычитаем из остальных строк
			downdiag = matrix[k][i];
			for (int j = i; j < m + 1; j++){
				matrix[k][j] = matrix[k][j] - downdiag* matrix[i][j];
			}
		}
	}
	
	// обратный ход
	for  (int i = 0; i < m; i++){
		answer[i] = matrix[i][m];
	}
	for  (int i = m-2; i >= 0; i--){
		for  (int j = i+1; j < m; j++){
			answer[i] = answer[i] - answer[j]*matrix[i][j];
		}
	}
	printanswer(answer, order); 
	return 0;
}
