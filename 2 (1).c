#include <stdio.h>
#include <stdlib.h>
#include <math.h>
const int n = 14; // количество уравнений
const long double masx[] = {0,0.15,0.3,0.45,0.6,0.75,0.9,1.05,1.2,1.35,1.5,1.65,1.8,1.95,2};
const long double masy[] = {0,0.002301,0.026932,0.070902,0.030482,0.241127,0.712619,0.994324,0.48283,1.107277,3.16575,4.144453,2.403475,2.369195,4.352169};
// вычисления значений функций по формулам


double omega ( int k, int i){
	double w = 1;
	for (int j = 0; j <= i; j++){
		if (k != j){
			w = w * (masx[k] - masx[j]);
		}
	}
	return w;
}


double Ai (int i){
	double a = 0;
	for (int k = 0; k <= i; k++){
		a = a + masy[k]/omega(k,i);
	}
	return a;
}


double makeP (double x, int n){
	if (n == 0){
		return masy[0];
	} else {
		double p = 1;
		for (int k = 0; k < n; k++){
			p = p * (x-masx[k]);
		} 
		return (makeP(x,n-1)+Ai(n)*p);
	}
}




// построение интерполяционного многочлена в форме Ньютона по точкам Xi
int main(){
	double a = 0;
	while (a <= 2){
		printf("%lf %lf\n", a, makeP(a,n));
		a = a + 0.01;
	}
	printf("%lf %lf\n", a, makeP(a,n));
	return 0;
}
