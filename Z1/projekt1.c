/*Name: Zadanie 1.5
  Author: Piotr Jastrzebski 1I1
  Date: 05-03-09 21:08
  Description: Program obliczajacy z zadana dokladnoscia n wartosc ln(x), w zadanym punkcie x.*/
  
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>

double potega(double podstawa, int wykladnik) /*rekurencyjna funkcja potegi*/
      {if(wykladnik==0)
      return 1;
      else
      return podstawa*potega(podstawa, wykladnik-1);
      }

int main(){
int n,i;
double x,blad=0.0,wynik=0.0;
char bufor;

printf("Zadanie 1.5 - Autor: Piotr Jastrzebski - 1I1\n\n");
printf("Program obliczajacy wartosc ln(x), z zadana dokladnoscia n,\ndla zadanej wartosci x.\n\n");

printf("Podaj wartosc x: "); 
while (1)
/*sprawdza, czy  wczytano jeden ciag znakow, czy x dodatnie, zmiennoprzecinkowe
czy ln i x nie przekraczaja zakresu double*/
if(scanf("%lf", &x)!=1 || x>DBL_MAX || x<=0 || log(x)>DBL_MAX || (bufor=getchar())!='\n') 
                {printf("Niepoprawna wartosc x. Wprowadz poprawnie x: ");                
                do bufor=getchar();
                while (bufor!='\n');} 
else break;

printf("Podaj dokladnosc n: "); 
while (1)
/*sprawdza, czy wczytano jeden ciag znakow, czy n nieujemne, calkowite
czy 2*n+1 nie przekracza zakresu int*/
if(scanf("%d", &n)!=1 || n<0 || (2*n+1)>INT_MAX || (bufor=getchar())!='\n')  
               {printf("Niepoprawna wartosc n. Wprowadz poprawnie n: ");    
               do bufor=getchar();
               while (bufor!='\n');}
else break;
   
for(i=0;i<=n;i++) /*obliczenia*/
                 wynik+=(1.0*potega((1.0*(x-1)/(x+1)),2*i+1))/(2*i+1);
wynik*=2.0;
/*liczby zmiennoprzecinkowe nieprzyrownywalne do 0, stad epsilon,
czyli odl miedzy x, a najblizsza kolejna rozroznialna od x liczba (~0)
sprawdza, czy nie dzielimy przez 0, wtedy blad=0.0*/ 
if(log(x)>DBL_EPSILON || log(x)<(-DBL_EPSILON))
                      blad=100.0*(wynik-log(x))/log(x);
printf("\nDla x=%f\n", x);                    
printf("ln(x) wynosi: %f\nwynik funkcji biblioteczniej: %f\n\n", wynik, log(x));
printf("Blad bezwzgledny wynosi: %f%%\n", blad);
return 0;
}
