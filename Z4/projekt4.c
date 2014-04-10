/*Name: 		Zadanie 4.3 - projekt rozbudowanej bazy przedszkola
  Author: 		1I1 - Piotr Jastrzebski
  Date: 		30-05-09 11:11
  Description:
  
ZADANIE 4.3

Program Spis dzieci przedszkola wykonany w ramach zadania 3 należy
rozszerzyć o możliwość zapisywania/odczytywania danych do/z plików. Program
powinien obsługiwać dwa rodzaje plików:
c)      plik tekstowy, umożliwiający odczytywanie i zapisywanie oraz
modyfikację danych (także struktury drzewa) przy użyciu dowolnego edytora
tekstowego (rozszerzenie *.txt)
d)      plik binarny zabezpieczony hasłem, sposób zapisu danych
uniemożliwiający "rozpoznanie" ich pod edytorem tekstowym (rozszerzenie
*.pdz).
Zapis i odczyt danych do/z pliku realizowany poprzez wydawanie odpowiednich
komend z parametrami. Rodzaj i sposób odczytu/zapisu program rozpoznaje po
rozszerzeniu w nazwie pliku. Wszelkie inne wymagania dotyczące programu
takie same jak w zadaniu 3.

Przykłady
e)      zapis do pliku tekstowego:
save plik.txt
f)      odczyt z pliku tekstowego
load plik.txt
g)      zapis do pliku binarnego
save plik.pdz hasło
h)      odczyt z pliku binarnego
load plik.pdz hasło
				
				Program przyjmuje 20 znakowe ciagi znakow zlozone z wielkich liter alfabetu lacinskiego wraz z
				znakiem podkreslenia. Minimalny i maksymalny rok urodzenia dziecka moze byc zmieniany przez
				predefiniowane stale YMIN, YMAX, ktore wynosza odpowiednio 2000 i 2009*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define N 20 /*dlugosc stringow*/
#define YMIN 2000 /*minimalny rok urodzenia przedszkolaka*/
#define YMAX 2009 /*maksymalny rok urodzenia przedszkolaka*/

char bufor, nazwa_tmp[N], imie_tmp[N], nazwisko_tmp[N], grupa_tmp[N], nazwa_tmp2[N], imie_tmp2[N], nazwisko_tmp2[N], grupa_tmp2[N], czyn[N/4], roz[N/4], obiekt[N], obiekt_pass[N], pass[N], pass_file[N]; /*bufor, tablice pomocnicze*/
int rok_tmp, rok_tmp2, pozycja, war, i; /*zmienne pomocnicze roku urodzenia, zmienna wyboru pozycji, zmienna warunku unikalnosci rekordu*/

typedef struct przedszkolak przedszkolak;
typedef struct zajecia zajecia;

struct przedszkolak{ /*pojedynczy rekord w bazie*/
		char imie[N], nazwisko[N], grupa[N];
		int rok;
		przedszkolak *next, *prev;
		}*head_p=NULL, *el_p=NULL, *temp_p=NULL;

struct zajecia{ /*pojedynczy element poziomu - zajecie*/
		char nazwa[N];
		zajecia *next, *prev, *down, *up; /*wskaznik na element wczesniejszy/nastepny/nizej/wyzej w drzewie*/
		przedszkolak *head_p, *el_p, *temp_p;
		}*head_z=NULL, *el_z=NULL, *temp_z=NULL;

FILE *plik, *plik_pass; /*uchwyt do pliku*/
				
/*FUNKCJE PROGRAMU*/
/*OK*/ void menu(void); /*wyswietla menu*/
/*OK*/ void linia(void); /*wyswietla stala linie*/
/*OK*/ void wroc(void); /*wraca do menu*/
/*OK*/ void monit(void); /*wyswietla aktualna grupe*/
/*OK*/ void linia_komend(void); /*oczekuje na komende dot plikow i wykonuje ja*/

/*FUNKCJE GRUP*/
/*OK*/ void add_z(void); /*dodaje grupe w aktualnym wezle*/
/*OK*/ void add_sub_z(void); /*dodaje podgrupe*/
/*OK*/ void del_z(void); /*usuwa aktualna grupe*/
/*OK*/ void fun_del_z(void); /*funkcja usuniecia grupy z podgrupami*/
/*OK*/ void rename_z(void); /*zmienia nazwe aktualnej grupy*/
/*OK*/ void down_z(void); /*hop w dol*/
/*OK*/ void up_z(void); /*hop w gore*/
/*OK*/ void left_z(void); /*hop w lewo*/
/*OK*/ void right_z(void); /*hop w prawo*/
/*OK*/ void move_z(void); /*przenosi grupe*/
/*OK*/ void fun_move_z(int wybor); /*funkcja przeniesienia grupy z podgrupami*/
/*OK*/ void tnij_wskazniki(void); /*tnie wskazniki wychodzace z elementu przenoszonego, spaja pozostale elementy*/
/*OK*/ void tree_z(void); /*wyswietla aktualna grupe z podgrupami*/
/*OK*/ void fun_tree_z(int licznik); /*funkcja rekurencyjna rysujaca drzewo*/

/*FUNKCJE PRZEDSZKOLAKA*/
/*OK*/ void add_p(void); /*dodaje przedszkolaka do bazy*/
/*OK*/ void edit_p(void); /*edytuje przedszkolaka */
/*OK*/ void fun_edit_p(char nazwisko_tmp[], char imie_tmp[], char grupa_tmp[], int rok_tmp, char nazwisko_tmp2[], char imie_tmp2[], char grupa_tmp2[], int rok_tmp2); /*wyszukuje w drzewie i podmienia *_tmp na *_tmp2*/
/*OK*/ void search_p(void); /*wyszukuje i wypisuje pozycje spelniajace kryterium*/
/*OK*/ void google(char nazwisko_tmp[], char imie_tmp[], char grupa_tmp[], int rok_tmp); /*googluje zadane pozycje w drzewie*/

/*FUNKJCE PLIKOW*/

/*BINARNE*/
/*OK*/ void save_bin(void); /*zapisuje drzewo do pliku binarnego*/
/*OK*/ void load_bin(void); /*laduje drzewo z pliku binarnego*/
/*OK*/ void tree_bin(int licznik); /*dokonuje przejscia przez drzewo i zapisu do pliku bin*/
/*OK*/ void czytaj_bin(void); /*dokonuje przejscia przez plik binarny i zczytuje do drzewa*/
/*OK*/ void save_pass(void); /*zapisuje ukryte haslo do pliku*/
/*OK*/ int check_pass(void); /*sprawdza haslo z pliku*/

/*TEKSTOWE*/
/*OK*/ void save_txt(void); /*zapisuje drzewo do pliku tekstowego*/
/*OK*/ void load_txt(void); /*laduje drzewo z pliku tekstowego*/
/*OK*/ void tree_txt(int licznik); /*dokonuje przejscia przez drzewo i zapisu do pliku txt*/
/*OK*/ void czytaj_txt(void); /*dokonuje przejscia przez plik tekstowy i zczytuje do drzewa*/


int main(){
	menu();
	return 0;
}

void menu(void){
	system("tput clear");
	printf("\n ::Radosny Przedszkolak 4.0::\n");
	monit();
	linia();
	printf(" :: Zajecia\t\t\t\t\t\t\t:: Przedszkolak\n\n");
	printf(" 0. dodaj grupe\t\t\t\t\t\t\t10. dodaj\n");
	printf(" 1. dodaj podgrupe\t\t\t\t\t\t11. edytuj\n");
	printf(" 2. grupa nizej\t\t\t\t\t\t\t12. wyszukaj\n");
	printf(" 3. zmien nazwe\n");
	printf(" 4. grupa na lewo\n");
	printf(" 5. usun\t\t\t\t\t\t\t:: Pliki\n");
	printf(" 6. grupa po prawo\n");
	printf(" 7. przenies\t\t\t\t\t\t\t13. obsluga\n");
	printf(" 8. grupa wyzej\n");
	printf(" 9. wyswietl grupe z podgrupami\n\n\n");
	printf("\t\t\t\t   [8] GORA\n\n");
	printf("\t\t\t   [4] LEWO\t   [6] PRAWO\n\n");
	printf("\t\t\t\t   [2] DOL\n");
	linia();
	printf(" Pozycja: ");

while(1){/*sprawdzenie poprawnosci wprowadzanych danych z zkresu + nie zezwala operowac, gdy nie ma grup*/
	if(scanf("%d", &pozycja)!=1 || (head_z==NULL && pozycja!=0 && pozycja!=13) || pozycja>13 || pozycja<0 || (bufor=getchar())!='\n'){
			if(head_z==NULL && pozycja!=0)
				printf(" Do korzystania z programu wymagana jest chociaz jedna grupa. Dodaj ja teraz.\n By wprowadzic ponownie");
			else
				printf(" Podano niepoprawna pozycje z menu. By wprowadzic ponownie");
			do bufor=getchar();
			while (bufor!='\n');
		wroc();
	}
else break;
	
	if(scanf("%d", &pozycja)!=1 || pozycja>12 || pozycja<0 || (bufor=getchar())!='\n'){
		printf(" Podano niepoprawna pozycje z menu. By wprowadzic ponownie");
			do bufor=getchar();
			while (bufor!='\n');
		wroc();
	}
else break;}

switch(pozycja){
	case 0 : add_z(); break;
	case 1 : add_sub_z(); break;
	case 2 : down_z(); break;
	case 3 : rename_z(); break;
	case 4 : left_z(); break;
	case 5 : del_z(); break;
	case 6 : right_z(); break;
	case 7 : move_z(); break;
	case 8 : up_z(); break;
	case 9 : tree_z(); break;
	case 10: add_p(); break;
	case 11: edit_p(); break;
	case 12: search_p(); break;
	case 13: linia_komend(); break;
	}
}

void linia(void){
	printf(" ______________________________________________________________________________\n\n");
}

void wroc(void){
printf(" nacisnij ENTER. ");
	while(1){
		if((bufor=getchar())=='\n')
		menu();
		}
}

void monit(void){
	if(head_z!=NULL)
		printf("                                           aktualna grupa: %s\n", el_z->nazwa); 
	else
		printf("                                           aktualna grupa: PUSTA\n"); 
}

void linia_komend(void){
i=0;
pozycja=0;
system("tput clear");
printf("\n Linia komend obslugi plikow\n");
monit();
linia();
printf(" $ "); scanf("%s", czyn);
if(strcmp(czyn, "load")==0){
	scanf("%s", obiekt);
	while(obiekt[pozycja]!='\0')
		pozycja++;
	for (i=0; i<4;i++)
		roz[i]=obiekt[pozycja-4+i];
	if(strcmp(roz, ".txt")==0){
		load_txt();
	}
	else	if(strcmp(roz, ".pdz")==0){
				scanf("%s", pass);
				if(check_pass()==1){
					load_bin();
					}
				else
					printf("\n Podane haslo pliku nie jest poprawne.\n");
			}
			else
				printf("\n Podane rozszerzenie pliku nie jest obslugiwane.\n Obslugiwany format to nazwa.txt albo nazwa.pdz.\n");
	
}
else	if(strcmp(czyn, "save")==0){
			scanf("%s", obiekt);
			while(obiekt[pozycja]!='\0')
				pozycja++;
			for (i=0; i<4;i++)
				roz[i]=obiekt[pozycja-4+i];
			if(strcmp(roz, ".txt")==0){
				save_txt();
			}
			else	if(strcmp(roz, ".pdz")==0){
						scanf("%s", pass);
						save_pass();
						save_bin();
					}
			else
				printf("\n Podane rozszerzenie pliku nie jest obslugiwane.\n Obslugiwany format to nazwa.txt albo nazwa.pdz.\n");
		}
		else
			printf("\n Polecenie %s nie istnieje.\n", czyn);

do bufor=getchar();
while (bufor!='\n');
linia();
printf(" Aby powrocic do menu");
wroc();
}

void add_z(void){
	pozycja=1;
	system("tput clear");
	printf("\n Dodaj grupe\n");
	monit();
	linia();
	printf(" Podaj nazwe grupy, ktora chcesz utworzyc: ");
	while(1)
		if(scanf("%[A-Z_]s",nazwa_tmp)!=1 || (bufor=getchar())!='\n') /*spr, czy tylko litery*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie nazwe grupy: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
		else break;

	if(head_z==NULL){
		head_z=(zajecia*)malloc(sizeof(zajecia));
		strcpy(head_z->nazwa, nazwa_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
		head_z->next=NULL;
		head_z->prev=NULL;
		head_z->up=NULL;
		head_z->down=NULL;
		head_z->head_p=NULL;
		el_z=head_z;
		printf("\n Dodanie grupy do bazy zakonczone sukcesem!\n");
		}
	else{while(el_z->prev!=NULL) /* spr, czy w wezle element ma unikalna nazwe*/
			el_z=el_z->prev;
		if(strcmp(el_z->nazwa, nazwa_tmp)==0)
					pozycja=0;
		while(el_z->next!=NULL && pozycja){
			if(strcmp(el_z->nazwa, nazwa_tmp)==0)
					pozycja=0;
			el_z=el_z->next;
			}
		if(strcmp(el_z->nazwa, nazwa_tmp)==0)
					pozycja=0;
		if(pozycja==1){
			temp_z=el_z;
			el_z=el_z->next;
			el_z=(zajecia*)malloc(sizeof(zajecia));
			strcpy(el_z->nazwa, nazwa_tmp);
			temp_z->next=el_z;
			el_z->prev=temp_z;
			el_z->up=NULL;
			el_z->down=NULL;
			el_z->next=NULL;
			el_z->head_p=NULL;
			printf("\n Dodanie grupy do bazy zakonczone sukcesem!\n");
		}
		else
			printf("\n Element o podanej nazwie juz istnieje w aktualnym wezle, zaproponuj inna nazwe!\n");
		}
	linia();
	printf(" Aby powrocic do menu");
	wroc();
}

void add_sub_z(void){
	system("tput clear");
	printf("\n Dodaj podgrupe\n");
	monit();
	linia();
	if(el_z->down!=NULL)
			printf(" Juz istnieja podgrupy aktualnie wybranej grupy.\n Aby kontynuowac zejdz poziom nizej i dodaj grupe rownorzedna.\n");
	else{
	printf(" Podaj nazwe grupy, ktora chcesz utworzyc: ");
	while(1)
		if(scanf("%[A-Z_]s",nazwa_tmp)!=1 || (bufor=getchar())!='\n') /*spr, czy tylko litery*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie nazwe grupy: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
		else break;
		
		temp_z=el_z;
		el_z=el_z->down;
		el_z=(zajecia*)malloc(sizeof(zajecia));
		strcpy(el_z->nazwa, nazwa_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
		el_z->next=NULL;
		el_z->prev=NULL;
		el_z->up=temp_z;
		el_z->down=NULL;
		el_z->head_p=NULL;
		temp_z->down=el_z;
		    
	printf("\n Dodanie podgrupy do bazy zakonczone sukcesem!\n");
	}
	linia();
	printf(" Aby powrocic do menu");
	wroc();
}

void del_z(void){
	tnij_wskazniki();
	fun_del_z();
	el_z=head_z;
	printf(" Pomyslnie usunieto grupe z bazy. Aby powrocic do menu");
    wroc();
}

void fun_del_z(void){
	if(el_z->down){ /*rekurencyjnie usuwanie wezla+nizsze galezie*/
		temp_z=el_z;
		el_z=el_z->down;
		fun_del_z();
		}
	else if (el_z->next){
		temp_z=el_z;
		el_z=el_z->next;
		fun_del_z();
		}
		else{
		free(el_z);
		el_z=NULL;
		}
}

void rename_z(void){
	pozycja=1;
	system("tput clear");
	printf("\n Zmien nazwe grupy\n");
	monit();
	linia();
	printf(" Podaj nowa nazwe dla grupy %s: ", el_z->nazwa);
	while(1)
		if(scanf("%[A-Z_]s",nazwa_tmp)!=1 || (bufor=getchar())!='\n') /*spr, czy tylko litery*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie nazwe grupy: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
		else break;
	temp_z=el_z;
	while(el_z->prev!=NULL) /* spr, czy w wezle element ma unikalna nazwe*/
			el_z=el_z->prev;
		if(strcmp(el_z->nazwa, nazwa_tmp)==0)
					pozycja=0;
		while(el_z->next!=NULL && pozycja){
			if(strcmp(el_z->nazwa, nazwa_tmp)==0)
					pozycja=0;
			el_z=el_z->next;
			}
		if(strcmp(el_z->nazwa, nazwa_tmp)==0)
					pozycja=0;
	if(pozycja==1){
		strcpy(temp_z->nazwa, nazwa_tmp);
		printf("\n Zmiana nazwy zakonczona sukcesem!\n");
	}
	else
		printf("\n Element o podanej nazwie juz istnieje w aktualnym wezle, zaproponuj inna nazwe!\n");
	linia();
	printf(" Aby powrocic do menu");
	wroc();
}

void up_z(void){
if(el_z->up!=NULL){
	el_z=el_z->up;
	menu();
	}
else {
	printf(" Operujesz na grupie glownej.\n W strukturze drzewa nie ma nic wyzej.\n Aby wybrac inna pozycje z menu");
	wroc();
	}
}

void down_z(void){
if(el_z->down!=NULL){
	el_z=el_z->down;
	menu();
	}
else {
	printf(" Operujesz na najnizszej grupie.\n W aktualnej galezi drzewa nie ma nic nizej.\n Aby wybrac inna pozycje z menu");
	wroc();
	}
}

void left_z(void){
if(el_z->prev!=NULL){
	el_z=el_z->prev;
	menu();
	}
else{
    printf(" Operujesz na najbardziej skrajnej grupie.\n W aktualnym wezle drzewa nie ma nic bardziej po lewo.\n Aby wybrac inna pozycje z menu");
	wroc();
	}
}
    
void right_z(void){
if(el_z->next!=NULL){
	el_z=el_z->next;
	menu();
	}
else{
    printf(" Operujesz na najbardziej skrajnej grupie.\n W aktualnym wezle drzewa nie ma nic bardziej po prawo.\n Aby wybrac inna pozycje z menu");
	wroc();
	}
}
  
void move_z(void){
system("tput clear");
printf("\n Przenies grupe\n"); 
monit();
linia();
if(el_z==head_z && el_z->prev==NULL && el_z->next==NULL && el_z->up==NULL){ /*nie pozwala przenosic, gdy istnieje 1 grupa*/
	printf(" Nie ma dokad przeniesc pojedynczej grupy. By wrocic do menu");
	wroc();
}
else{
printf(" Wybierz sposob przeniesienia aktualnej grupy\n\n");
printf(" 1. dopisz do innej grupy jako rownorzedna pozycja\n");
printf(" 2. dopisz do innej grupy jako podgrupa\n\n");
printf(" 0. wroc do menu\n");	
linia();
printf(" Pozycja: ");         

while(1){
if(scanf("%d", &pozycja)!=1 || pozycja>2 || pozycja<0 || (bufor=getchar())!='\n'){
	printf(" Podano niepoprawna pozycje z menu. By powrocic do menu");
	do bufor=getchar();
	while (bufor!='\n');
	wroc();
}
else break;}
switch(pozycja){
case 0: menu(); break;
case 1: tnij_wskazniki(); temp_z=el_z; el_z=head_z; fun_move_z(1); break; /*dopisz jako grupa*/
case 2: tnij_wskazniki(); temp_z=el_z; el_z=head_z; fun_move_z(2); break; /*dopisz jako podgrupa*/
}
}
}

void fun_move_z(int wybor){
war=1;
system("tput clear");
printf("\n Wybierz grupe, do ktorej chcesz dodac grupe jako"); 
if(wybor==1)
	printf(" rownorzedna pozycje\n");
else
	printf(" podgrupe\n");
monit();
linia();
printf("\t\t\t\t   [8] GORA\n\n");
printf("\t\t\t   [4] LEWO\t   [6] PRAWO\n\n");
printf("\t\t\t\t   [2] DOL\n\n\n");
printf(" [0] DODAJ\n");
linia();
printf(" Pozycja: ");

while(1){
if(scanf("%d", &pozycja)!=1 || (pozycja!=0 && pozycja!=2 && pozycja!=4 && pozycja!=6 && pozycja!=8) || (bufor=getchar())!='\n'){
	printf(" Podano niepoprawna pozycje z menu. By powrocic do menu nacisnij ENTER");
	do bufor=getchar();
	while (bufor!='\n');
	fun_move_z(wybor);
}
else break;}

switch(pozycja){
	case 0 : 
		if (wybor==1){
			while(el_z->prev!=NULL) /* spr, czy w wezle element ma unikalna nazwe*/
				el_z=el_z->prev;
			if(strcmp(el_z->nazwa, temp_z->nazwa)==0)
						war=0;
			while(el_z->next!=NULL && war){
				if(strcmp(el_z->nazwa, temp_z->nazwa)==0)
						war=0;
				el_z=el_z->next;
				}
			if(strcmp(el_z->nazwa, temp_z->nazwa)==0)
						war=0;
			if(war==0){/*nie pozwala dodac na poziomie takich samych grup*/
				printf("\n Element o podanej nazwie juz istnieje w aktualnym wezle, zaproponuj inna lokalizacje!\n");
				do bufor=getchar();
				while (bufor!='\n');
				fun_move_z(wybor);
			}
			else{		
				while(el_z->next!=NULL)
					el_z=el_z->next;
				el_z->next=temp_z;
				temp_z->prev=el_z;
			}
		}
		else{
			if(el_z->down!=NULL){/*nie pozwala dopisac, jako podgrupa, gdy juz istnieja inne podgrupy*/
				printf(" Juz istnieja podgrupy aktualnie wybranej grupy.\n Aby kontynuowac zejdz poziom nizej i dodaj jako grupe rownorzedna.\n");
				do bufor=getchar();
				while (bufor!='\n');
				fun_move_z(wybor);
			}
			else{
				el_z->down=temp_z;
				temp_z->up=el_z;
			}
		}
	printf(" Pomyslnie przeniesiono grupe. Aby wrocic do menu");
	wroc();
	break;
	case 2 : if(el_z->down!=NULL) el_z=el_z->down; fun_move_z(wybor); break;
	case 4 : if(el_z->prev!=NULL) el_z=el_z->prev; fun_move_z(wybor); break;
	case 6 : if(el_z->next!=NULL) el_z=el_z->next; fun_move_z(wybor); break;
	case 8 : if(el_z->up!=NULL) el_z=el_z->up; fun_move_z(wybor); break;
	}
}

void tnij_wskazniki(void){
if(el_z->prev==NULL && el_z->up==NULL && el_z->next==NULL){/*gdy pojedynczy element*/
free(head_z);
head_z=NULL;
}
else if(el_z->prev==NULL && el_z->up==NULL && el_z->next!=NULL){ /*gdy element glowa*/
	head_z=el_z->next;
	head_z->prev=NULL;
	el_z->next=NULL;
}
else if(el_z->prev!=NULL && el_z->up==NULL && el_z->next==NULL){ /*gdy element ostatni w liscie*/
	el_z->prev->next=NULL;
	el_z->prev=NULL;
}
else if(el_z->prev==NULL && el_z->up!=NULL && el_z->next==NULL){ /*gdy element jedyna podgrupa*/
	el_z->up->down=NULL;
	el_z->up=NULL;
}
else if(el_z->prev!=NULL && el_z->up==NULL && el_z->next!=NULL){/*gdy element w srodku listy*/
	el_z->prev->next=el_z->next;
	el_z->next->prev=el_z->prev;
	el_z->next=NULL;
	el_z->prev=NULL;
}
else if(el_z->prev==NULL && el_z->up!=NULL && el_z->next!=NULL){ /*gdy element pierwszym elementem w podgrupie*/
	el_z->up->down=el_z->next;
	el_z->next->prev=NULL;
	el_z->next=NULL;
	el_z->prev=NULL;
}
}

void tree_z(void){
	system("tput clear");
	printf("\n Wyswietl grupe z podgrupami\n");
	monit();
	linia();
	printf(" Wypisz:\n\n");
	printf(" 1. aktualna grupe z podgrupami\n");
	printf(" 2. grupe glowna z podgrupami (cale drzewo)\n\n");
	printf(" 0. wroc do menu\n");	
	linia();
	printf(" Pozycja: ");         

while(1){
if(scanf("%d", &pozycja)!=1 || pozycja>2 || pozycja<0 || (bufor=getchar())!='\n'){
	printf(" Podano niepoprawna pozycje z menu. By powrocic do menu");
	do bufor=getchar();
	while (bufor!='\n');
	wroc();
}
else break;}

printf("\n");

switch(pozycja){
case 0: menu(); break;
case 1:
	printf(" %s\n", el_z->nazwa);
	if(el_z->head_p!=NULL){
		el_z->el_p=el_z->head_p;
		while(el_z->el_p!=NULL){
			printf(" :: nazwisko: %s\n    imie: %s\n    grupa: %s\n    rok urodzenia: %d\n", el_z->el_p->nazwisko, el_z->el_p->imie, el_z->el_p->grupa, el_z->el_p->rok);
			el_z->el_p=el_z->el_p->next;
		}
	}
if(el_z->down!=NULL){el_z=el_z->down; temp_z=el_z; fun_tree_z(1);} break;
case 2: el_z=head_z; temp_z=el_z; fun_tree_z(0); break;
}
	linia();
	printf(" Aby powrocic do menu");
	wroc();
}

void fun_tree_z(int licznik){
i=0;
while(1){
	printf(" ");
	for(i=0;i<licznik;i++)
		printf(".");
	printf("%s\n", el_z->nazwa);
	if(el_z->head_p!=NULL){
		el_z->el_p=el_z->head_p;
		while(el_z->el_p!=NULL){
			printf(" :: nazwisko: %s\n    imie: %s\n    grupa: %s\n    rok urodzenia: %d\n", el_z->el_p->nazwisko, el_z->el_p->imie, el_z->el_p->grupa, el_z->el_p->rok);
			el_z->el_p=el_z->el_p->next;
		}
	}
	
	if(el_z->down!=NULL){
		el_z=el_z->down;
		licznik++;
		fun_tree_z(licznik);
		licznik--;}
	if(el_z->next==NULL)
		break;
	else
		el_z=el_z->next;
}

if(temp_z==head_z){
	while(1){
	if(el_z->prev==NULL)
		if(el_z->up==NULL)
			break;
		else{
			el_z=el_z->up;
			break;
			}
	else
		el_z=el_z->prev;
	}
}
else{
	while(el_z==temp_z){
		if(el_z->prev==NULL)
			if(el_z->up==NULL)
				break;
			else{
				el_z=el_z->up;
				break;
				}
		else
			el_z=el_z->prev;
	}
}
}

void add_p(void){
	system("tput clear");
	printf("\n Dodaj przedszkolaka\n"); 
	monit();
    linia();
	printf(" Witaj w module dodawania przedszkolaka do bazy.\n\n");
	printf(" Podaj:\n");

printf(" - nazwisko dziecka: ");
while(1)
if(scanf("%[A-Z_]s",nazwisko_tmp)!=1 || (bufor=getchar())!='\n') /*spr, czy tylko litery*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - nazwisko dziecka: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - imie dziecka: ");
while(1)
if(scanf("%[A-Z_]s",imie_tmp)!=1 || (bufor=getchar())!='\n')
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - imie dziecka: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - grupe: ");
while(1)
if(scanf("%[A-Z_]s",grupa_tmp)!=1 || (bufor=getchar())!='\n')
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - grupe: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - rok urodzenia: ");
while(1)
if(scanf("%d",&rok_tmp)!=1 || rok_tmp<YMIN || rok_tmp>YMAX || (bufor=getchar())!='\n') /*spr, czy liczba*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - rok: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

     if(el_z->head_p==NULL) 
       {el_z->head_p=(przedszkolak*)malloc(sizeof(przedszkolak));
       strcpy(el_z->head_p->imie, imie_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
       strcpy(el_z->head_p->nazwisko, nazwisko_tmp);
       strcpy(el_z->head_p->grupa, grupa_tmp);
       el_z->head_p->rok=rok_tmp;
       el_z->head_p->next=NULL;
       el_z->head_p->prev=NULL;
       }
     else 
        {el_z->el_p=el_z->head_p;
        if(strcmp(nazwisko_tmp, el_z->el_p->nazwisko)<0 || (strcmp(nazwisko_tmp, el_z->el_p->nazwisko)==0 && strcmp(imie_tmp,el_z->el_p->imie)<0))
             {el_z->el_p=(przedszkolak*)malloc(sizeof(przedszkolak)); /*wpisywanie przed obecnym el, zastepowanie glowy*/
              strcpy(el_z->el_p->imie, imie_tmp);
              strcpy(el_z->el_p->nazwisko, nazwisko_tmp);
              strcpy(el_z->el_p->grupa, grupa_tmp);
              el_z->el_p->rok=rok_tmp;
              el_z->el_p->next=el_z->head_p;
              el_z->el_p->prev=NULL;
              el_z->head_p->prev=el_z->el_p;
              el_z->head_p=el_z->el_p;
             }
        else /*dodawanie przed nastepnym elementem:*/
           {while(1){el_z->temp_p=el_z->el_p;
              if(el_z->el_p->next==NULL) 
                {el_z->el_p=(przedszkolak*)malloc(sizeof(przedszkolak));
                strcpy(el_z->el_p->imie, imie_tmp); /*jesli wstawaimy na koniec listy*/
                strcpy(el_z->el_p->nazwisko, nazwisko_tmp);
                strcpy(el_z->el_p->grupa, grupa_tmp);
                el_z->el_p->rok=rok_tmp;
				el_z->el_p->next=NULL;
                el_z->temp_p->next=el_z->el_p;
                el_z->el_p->prev=el_z->temp_p;
                break;
                }
              else 
              {el_z->el_p=el_z->el_p->next;
              if(strcmp(nazwisko_tmp, el_z->el_p->nazwisko)<0 || (strcmp(nazwisko_tmp, el_z->el_p->nazwisko)==0 && strcmp(imie_tmp,el_z->el_p->imie)<0))
                {el_z->el_p=(przedszkolak*)malloc(sizeof(przedszkolak));
                strcpy(el_z->el_p->imie, imie_tmp); /*jesli wstawiamy w srodek listy*/
                strcpy(el_z->el_p->nazwisko, nazwisko_tmp);
                strcpy(el_z->el_p->grupa, grupa_tmp);
				el_z->el_p->rok=rok_tmp;
                el_z->el_p->prev=el_z->temp_p;
                el_z->el_p->next=el_z->temp_p->next;
                el_z->temp_p->next->prev=el_z->el_p;
                el_z->temp_p->next=el_z->el_p;
                break;
                }
              }
              }
           }
        }
	printf("\n Dodanie przedszkolaka do bazy zakonczone sukcesem!\n");
	linia();
	printf(" Aby powrocic do menu");
	wroc();
}

void edit_p(void){
system("tput clear");
printf("\n Edycja danych przedszkolaka\n"); 
monit();
linia();
printf(" Podaj dane przedszkolaka, ktorego chcesz edytowac. \n Konieczne jest podanie wszystkich jego danych.\n\n");

printf(" - nazwisko dziecka: ");
while(1)
if(scanf("%[A-Z_]s",nazwisko_tmp)!=1 || (bufor=getchar())!='\n') /*spr, czy tylko litery*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - nazwisko dziecka: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - imie dziecka: ");
while(1)
if(scanf("%[A-Z_]s",imie_tmp)!=1 || (bufor=getchar())!='\n')
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - nazwisko dziecka: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - grupe: ");
while(1)
if(scanf("%[A-Z_]s",grupa_tmp)!=1 || (bufor=getchar())!='\n')
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - grupe: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - rok urodzenia: ");
while(1)
if(scanf("%d",&rok_tmp)!=1 || rok_tmp<YMIN || rok_tmp>YMAX || (bufor=getchar())!='\n') /*spr, czy liczba*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - rok: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf("\n");

printf(" - nowe nazwisko dziecka: ");
while(1)
if(scanf("%[A-Z_]s",nazwisko_tmp2)!=1 || (bufor=getchar())!='\n') /*spr, czy tylko litery*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - nazwisko dziecka: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - nowe imie dziecka: ");
while(1)
if(scanf("%[A-Z_]s",imie_tmp2)!=1 || (bufor=getchar())!='\n')
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - nazwisko dziecka: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - nowa grupe: ");
while(1)
if(scanf("%[A-Z_]s",grupa_tmp2)!=1 || (bufor=getchar())!='\n')
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - grupe: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - nowy rok urodzenia: ");
while(1)
if(scanf("%d",&rok_tmp2)!=1 || rok_tmp<YMIN || rok_tmp>YMAX || (bufor=getchar())!='\n') /*spr, czy liczba*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - rok: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;
printf("\n\n");
pozycja=1;
fun_edit_p(nazwisko_tmp, imie_tmp, grupa_tmp, rok_tmp, nazwisko_tmp2, imie_tmp2, grupa_tmp2, rok_tmp2);
if(pozycja==1)
	printf(" Zmiana nie powiodla sie. Podanego przedszkolaka nie ma w bazie!\n");
else
	printf(" Zmiana dokonana prawidlowo!\n");
linia();
printf(" Aby powrocic do menu");
wroc();
}

void fun_edit_p(char nazwisko_tmp[], char imie_tmp[], char grupa_tmp[], int rok_tmp, char nazwisko_tmp2[], char imie_tmp2[], char grupa_tmp2[], int rok_tmp2){
while(1){
	if(el_z->head_p!=NULL){
		el_z->el_p=el_z->head_p;
		while(el_z->el_p!=NULL && pozycja){
			if(strcmp(imie_tmp, el_z->el_p->imie)==0 && strcmp(nazwisko_tmp, el_z->el_p->nazwisko)==0 && strcmp(el_z->el_p->grupa, grupa_tmp)==0 && el_z->el_p->rok==rok_tmp){
			strcpy(el_z->el_p->imie, imie_tmp2);
			strcpy(el_z->el_p->nazwisko, nazwisko_tmp2);
			strcpy(el_z->el_p->grupa, grupa_tmp2);
			el_z->el_p->rok=rok_tmp2;
			pozycja=0;
			}
		el_z->el_p=el_z->el_p->next;
		}
	}
	
	if(el_z->down!=NULL){
		el_z=el_z->down;
		fun_edit_p(nazwisko_tmp, imie_tmp, grupa_tmp, rok_tmp, nazwisko_tmp2, imie_tmp2, grupa_tmp2, rok_tmp2);
		}
	if(el_z->next==NULL)
		break;
	else
		el_z=el_z->next;
}

while(1){
	if(el_z->prev==NULL)
		if(el_z->up==NULL)
			break;
		else{
			el_z=el_z->up;
			break;
			}
	else
		el_z=el_z->prev;
}
}

void search_p(void){
war=1;
system("tput clear");
printf("\n Wyszukaj przedszkolaka\n"); 
monit();
linia();
printf(" Podaj dane do wyszukania przedszkolaka, lub wpisz '0' /zero/,\n aby pominac w wyszukiwaniu dana pozycje.\n\n");
printf(" - nazwisko dziecka: ");
while(1)
if(scanf("%[0A-Z_]s",nazwisko_tmp)!=1 || (bufor=getchar())!='\n') /*spr, czy tylko litery, lub podkreslenie*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - nazwisko dziecka: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - imie dziecka: ");
while(1)
if(scanf("%[0A-Z_]s",imie_tmp)!=1 || (bufor=getchar())!='\n')
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - nazwisko dziecka: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - grupe: ");
while(1)
if(scanf("%[0A-Z_]s",grupa_tmp)!=1 || (bufor=getchar())!='\n')
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - grupe: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;

printf(" - rok urodzenia: ");
while(1)
if(scanf("%d",&rok_tmp)!=1 || (rok_tmp!=0 && rok_tmp<YMIN) || rok_tmp>YMAX || (bufor=getchar())!='\n') /*spr, czy liczba*/
				{printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - rok: ");
				do bufor=getchar();
				while (bufor!='\n');
				}
else break;
printf("\n\n");
google(nazwisko_tmp, imie_tmp, grupa_tmp, rok_tmp);
if(war==1)
	printf(" Nie znaleziono pozycji w bazie spelniajacej powyzsze kryteria.\n Wyszukaj jeszcze raz.\n");
linia();
printf(" Aby powrocic do menu");
wroc();
}

void google(char nazwisko_tmp[], char imie_tmp[], char grupa_tmp[], int rok_tmp){
while(1){
	if(el_z->head_p!=NULL){
		el_z->el_p=el_z->head_p;
		while(el_z->el_p!=NULL){
			if(strcmp(imie_tmp, el_z->el_p->imie)==0 || strcmp(nazwisko_tmp, el_z->el_p->nazwisko)==0 || strcmp(el_z->el_p->grupa, grupa_tmp)==0 || el_z->el_p->rok==rok_tmp){
				war=0;
				printf(" :: imie: %s\n    nazwisko: %s\n    zajecia: %s\n    grupa: %s\n    rok urodzenia: %d\n", el_z->el_p->imie, el_z->el_p->nazwisko, el_z->nazwa, el_z->el_p->grupa, el_z->el_p->rok);
			}
			el_z->el_p=el_z->el_p->next;
		}
	}
	
	if(el_z->down!=NULL){
		el_z=el_z->down;
		google(nazwisko_tmp, imie_tmp, grupa_tmp, rok_tmp);}
	if(el_z->next==NULL)
		break;
	else
		el_z=el_z->next;
}

while(1){
	if(el_z->prev==NULL)
		if(el_z->up==NULL)
			break;
		else{
			el_z=el_z->up;
			break;
			}
	else
		el_z=el_z->prev;
}
}

void save_bin(void){
if(head_z!=NULL){
	plik=fopen(obiekt, "wb");
	el_z=head_z;
	temp_z=el_z;
	tree_bin(1);
	printf("\n Plik %s zostal zapisany!\n", obiekt);
	fclose(plik);
}
else{
	printf("\n Baza pusta. Nie ma czego zapisac. Aby powrocic do menu");
	do bufor=getchar();
	while (bufor!='\n');
	wroc();
}
}

void load_bin(void){
if((plik=fopen(obiekt, "rb"))!=NULL){
	if(head_z!=NULL){
		el_z=head_z;
		tnij_wskazniki();
		fun_del_z();
		head_z=NULL;
		el_z=head_z;
	}
	czytaj_bin();
	printf("\n Plik %s zostal zaladowany!\n", obiekt);
	fclose(plik);
}
else{
	printf("\n Plik %s nie istnieje! Aby powrocic do menu", obiekt);
	do bufor=getchar();
	while (bufor!='\n');
	wroc();
}
}

void tree_bin(int licznik){
int minus=-1, zero=0, chwilowa_zmienna=0;
char ola[N];
strcpy(ola, "x ");
while(1){

{licznik++; /*szyfruje grupe on*/
zero++;

i=0;
while(el_z->nazwa[i]!='\0'){
	chwilowa_zmienna=el_z->nazwa[i];
chwilowa_zmienna++;
el_z->nazwa[i]=chwilowa_zmienna;
i++;
}

i=0;
while(ola[i]!='\0'){
	chwilowa_zmienna=ola[i];
chwilowa_zmienna++;
ola[i]=chwilowa_zmienna;
i++;
}
}

	fwrite(&licznik,sizeof(int),1,plik);
	fwrite(&el_z->nazwa,sizeof(char)*N,1,plik);
	fwrite(&ola,sizeof(char)*N,1,plik);
	fwrite(&ola,sizeof(char)*N,1,plik);
	fwrite(&zero,sizeof(zero),1,plik);

{i=0; /*szyfruje grupe off*/
while(el_z->nazwa[i]!='\0'){
	chwilowa_zmienna=el_z->nazwa[i];
chwilowa_zmienna--;
el_z->nazwa[i]=chwilowa_zmienna;
i++;
}

i=0;
while(ola[i]!='\0'){
	chwilowa_zmienna=ola[i];
chwilowa_zmienna--;
ola[i]=chwilowa_zmienna;
i++;
}
	
zero--;
licznik--;
}
	if(el_z->head_p!=NULL){
		el_z->el_p=el_z->head_p;
		while(el_z->el_p!=NULL){
{minus++; /*szyfruje osobe on*/
el_z->el_p->rok++;

i=0;
while(el_z->el_p->nazwisko[i]!='\0'){
	chwilowa_zmienna=el_z->el_p->nazwisko[i];
chwilowa_zmienna++;
el_z->el_p->nazwisko[i]=chwilowa_zmienna;
i++;
}

i=0;
while(el_z->el_p->imie[i]!='\0'){
	chwilowa_zmienna=el_z->el_p->imie[i];
chwilowa_zmienna++;
el_z->el_p->imie[i]=chwilowa_zmienna;
i++;
}

i=0;
while(el_z->el_p->grupa[i]!='\0'){
	chwilowa_zmienna=el_z->el_p->grupa[i];
chwilowa_zmienna++;
el_z->el_p->grupa[i]=chwilowa_zmienna;
i++;
}
}
			fwrite(&minus,sizeof(int),1,plik);
			fwrite(&el_z->el_p->nazwisko,sizeof(char)*N,1,plik);
			fwrite(&el_z->el_p->imie,sizeof(char)*N,1,plik);
			fwrite(&el_z->el_p->grupa,sizeof(char)*N,1,plik);
			fwrite(&el_z->el_p->rok,sizeof(int),1,plik);
{minus--; /*szyfruje osobe on*/
el_z->el_p->rok--;

i=0;
while(el_z->el_p->nazwisko[i]!='\0'){
	chwilowa_zmienna=el_z->el_p->nazwisko[i];
chwilowa_zmienna--;
el_z->el_p->nazwisko[i]=chwilowa_zmienna;
i++;
}

i=0;
while(el_z->el_p->imie[i]!='\0'){
	chwilowa_zmienna=el_z->el_p->imie[i];
chwilowa_zmienna--;
el_z->el_p->imie[i]=chwilowa_zmienna;
i++;
}

i=0;
while(el_z->el_p->grupa[i]!='\0'){
	chwilowa_zmienna=el_z->el_p->grupa[i];
chwilowa_zmienna--;
el_z->el_p->grupa[i]=chwilowa_zmienna;
i++;
}
}		 /*szyfruje osobe off*/
			el_z->el_p=el_z->el_p->next;
		}
	}
	if(el_z->down!=NULL){
		el_z=el_z->down;
		licznik++;
		tree_bin(licznik);
		licznik--;
	}
	if(el_z->next==NULL)
		break;
	else
		el_z=el_z->next;
}

if(temp_z==head_z){
	while(1){
	if(el_z->prev==NULL)
		if(el_z->up==NULL)
			break;
		else{
			el_z=el_z->up;
			break;
			}
	else
		el_z=el_z->prev;
	}
}
else{
	while(el_z==temp_z){
		if(el_z->prev==NULL)
			if(el_z->up==NULL)
				break;
			else{
				el_z=el_z->up;
				break;
				}
		else
			el_z=el_z->prev;
	}
}
}

void czytaj_bin(void){
int poziom_p=1,poziom_f, chwilowa_zmienna=0;
while(fread(&poziom_f,sizeof(int),1,plik) && fread(&nazwisko_tmp,sizeof(char)*N,1,plik) && fread(&imie_tmp,sizeof(char)*N,1,plik) && fread(&grupa_tmp,sizeof(char)*N,1,plik) && fread(&rok_tmp,sizeof(int),1,plik)){
{poziom_f--; /*defszyfruje on*/
rok_tmp--;

i=0;
while(nazwisko_tmp[i]!='\0'){
	chwilowa_zmienna=nazwisko_tmp[i];
chwilowa_zmienna--;
nazwisko_tmp[i]=chwilowa_zmienna;
i++;
}

i=0;
while(imie_tmp[i]!='\0'){
	chwilowa_zmienna=imie_tmp[i];
chwilowa_zmienna--;
imie_tmp[i]=chwilowa_zmienna;
i++;
}

i=0;
while(grupa_tmp[i]!='\0'){
	chwilowa_zmienna=grupa_tmp[i];
chwilowa_zmienna--;
grupa_tmp[i]=chwilowa_zmienna;
i++;
}
} /*defszyfruje off*/
	while(1){
	if(poziom_f==-1){
		if(el_z->head_p==NULL) 
       {el_z->head_p=(przedszkolak*)malloc(sizeof(przedszkolak));
       strcpy(el_z->head_p->imie, imie_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
       strcpy(el_z->head_p->nazwisko, nazwisko_tmp);
       strcpy(el_z->head_p->grupa, grupa_tmp);
       el_z->head_p->rok=rok_tmp;
       el_z->head_p->next=NULL;
       el_z->head_p->prev=NULL;
       }
     else 
        {el_z->el_p=el_z->head_p;
        if(strcmp(nazwisko_tmp, el_z->el_p->nazwisko)<0 || (strcmp(nazwisko_tmp, el_z->el_p->nazwisko)==0 && strcmp(imie_tmp,el_z->el_p->imie)<0))
             {el_z->el_p=(przedszkolak*)malloc(sizeof(przedszkolak)); /*wpisywanie przed obecnym el, zastepowanie glowy*/
              strcpy(el_z->el_p->imie, imie_tmp);
              strcpy(el_z->el_p->nazwisko, nazwisko_tmp);
              strcpy(el_z->el_p->grupa, grupa_tmp);
              el_z->el_p->rok=rok_tmp;
              el_z->el_p->next=el_z->head_p;
              el_z->el_p->prev=NULL;
              el_z->head_p->prev=el_z->el_p;
              el_z->head_p=el_z->el_p;
             }
        else /*dodawanie przed nastepnym elementem:*/
           {while(1){el_z->temp_p=el_z->el_p;
              if(el_z->el_p->next==NULL) 
                {el_z->el_p=(przedszkolak*)malloc(sizeof(przedszkolak));
                strcpy(el_z->el_p->imie, imie_tmp); /*jesli wstawaimy na koniec listy*/
                strcpy(el_z->el_p->nazwisko, nazwisko_tmp);
                strcpy(el_z->el_p->grupa, grupa_tmp);
                el_z->el_p->rok=rok_tmp;
				el_z->el_p->next=NULL;
                el_z->temp_p->next=el_z->el_p;
                el_z->el_p->prev=el_z->temp_p;
                break;
                }
              else 
              {el_z->el_p=el_z->el_p->next;
              if(strcmp(nazwisko_tmp, el_z->el_p->nazwisko)<0 || (strcmp(nazwisko_tmp, el_z->el_p->nazwisko)==0 && strcmp(imie_tmp,el_z->el_p->imie)<0))
                {el_z->el_p=(przedszkolak*)malloc(sizeof(przedszkolak));
                strcpy(el_z->el_p->imie, imie_tmp); /*jesli wstawiamy w srodek listy*/
                strcpy(el_z->el_p->nazwisko, nazwisko_tmp);
                strcpy(el_z->el_p->grupa, grupa_tmp);
				el_z->el_p->rok=rok_tmp;
                el_z->el_p->prev=el_z->temp_p;
                el_z->el_p->next=el_z->temp_p->next;
                el_z->temp_p->next->prev=el_z->el_p;
                el_z->temp_p->next=el_z->el_p;
                break;
                }
              }
              }
           }
        }
		break;
	}
	else{	
		if(poziom_f==poziom_p){
	if(head_z==NULL){
		head_z=(zajecia*)malloc(sizeof(zajecia));
		strcpy(head_z->nazwa, nazwisko_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
		head_z->next=NULL;
		head_z->prev=NULL;
		head_z->up=NULL;
		head_z->down=NULL;
		head_z->head_p=NULL;
		el_z=head_z;
		break;
		}
	else{while(el_z->prev!=NULL) /* spr, czy w wezle element ma unikalna nazwe*/
			el_z=el_z->prev;
		pozycja=1;
		if(strcmp(el_z->nazwa, nazwisko_tmp)==0)
					pozycja=0;
		while(el_z->next!=NULL && pozycja){
			if(strcmp(el_z->nazwa, nazwisko_tmp)==0)
					pozycja=0;
			el_z=el_z->next;
			}
		if(strcmp(el_z->nazwa, nazwisko_tmp)==0)
					pozycja=0;
		if(pozycja==1){
			temp_z=el_z;
			el_z=el_z->next;
			el_z=(zajecia*)malloc(sizeof(zajecia));
			strcpy(el_z->nazwa, nazwisko_tmp);
			temp_z->next=el_z;
			el_z->prev=temp_z;
			el_z->up=NULL;
			el_z->down=NULL;
			el_z->next=NULL;
			el_z->head_p=NULL;
			break;
		}
		}
	}
	else 	if(poziom_p<poziom_f){
				poziom_p++;
				temp_z=el_z;
				el_z=el_z->down;
				el_z=(zajecia*)malloc(sizeof(zajecia));
				strcpy(el_z->nazwa, nazwisko_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
				el_z->next=NULL;
				el_z->prev=NULL;
				el_z->up=temp_z;
				el_z->down=NULL;
				el_z->head_p=NULL;
				temp_z->down=el_z;
				break;
			}
			else{
				while(el_z->prev!=NULL)
					el_z=el_z->prev;
				el_z=el_z->up;
				poziom_p--;
				}
			}
		}
}
}

void save_pass(void){
int chwilowa_zmienna;
	strcpy(obiekt_pass, obiekt); /*kopiowanie do obiekt_pass nazwy pliku dla hasla*/
	obiekt_pass[pozycja-3]='p';
	obiekt_pass[pozycja-2]='s';
	obiekt_pass[pozycja-1]='s';
plik_pass=fopen(obiekt_pass, "wb");
i=0;
while(pass[i]!='\0'){
	chwilowa_zmienna=pass[i];
chwilowa_zmienna++;
pass[i]=chwilowa_zmienna;
i++;
}
fwrite(&pass,sizeof(char)*N,1,plik_pass);
fclose(plik_pass);
}

int check_pass(void){ /*zwraca 1 dla poprawnego hasla i 0 dla blednego*/
int chwilowa_zmienna;	
	strcpy(obiekt_pass, obiekt);
	obiekt_pass[pozycja-3]='p';
	obiekt_pass[pozycja-2]='s';
	obiekt_pass[pozycja-1]='s';
	
if((plik_pass=fopen(obiekt_pass, "rb"))!=NULL){
	fread(&pass_file, sizeof(char)*N,1,plik_pass);
	fclose(plik_pass);
	
	i=0;
while(pass_file[i]!='\0'){
	chwilowa_zmienna=pass_file[i];
chwilowa_zmienna--;
pass_file[i]=chwilowa_zmienna;
i++;
}
	
if(strcmp(pass_file,pass)==0){
	return 1;
}
else return 0;
}
else return 0;
}

void save_txt(void){
if(head_z!=NULL){
	plik=fopen(obiekt, "w");
	el_z=head_z;
	temp_z=el_z;
	tree_txt(1);
	printf("\n Plik %s zostal zapisany!\n", obiekt);
	fclose(plik);

}
else{
	printf("\n Baza pusta. Nie ma czego zapisac. Aby powrocic do menu");
	do bufor=getchar();
	while (bufor!='\n');
	wroc();
}
}

void load_txt(void){
int czy=1,poziom_f=1, op=0; /*op - ostatni poziom*/
if((plik=fopen(obiekt, "r"))!=NULL){
	while((fscanf(plik, "%d %s %s %s %d", &poziom_f, nazwisko_tmp, imie_tmp, grupa_tmp, &rok_tmp))!=EOF && czy==1){
	
	if(op==0 && poziom_f!=1) /*jesli nie istnieje grupa head_z*/
		czy=0;
	
	if(poziom_f<-1 || poziom_f==0) /*jesli numer poziomu jest mniejszy od -1 lub rowny 0 /stany zabronione/ */
		czy=0;
		
	if(poziom_f>=(op+2)) /*spr, czy nie ma sytuacji, ze zaglebienie rosnie o +2 zamiast o +1 /brak grupy pomiedzy/ */
		czy=0;
	else if(poziom_f!=-1)
			op=poziom_f;
		
	i=0;
	while(nazwisko_tmp[i]!='\0' && czy==1){ /*spr, czy tylko wielkie litery*/
		if(isalpha((unsigned char)nazwisko_tmp[i])==0 || isupper((unsigned char)nazwisko_tmp[i])==0)
			czy=0;
		else i++;
		}
		
	i=0;
	while(imie_tmp[i]!='\0' && czy==1){ /*spr, czy tylko wielkie litery*/
		if(isalpha((unsigned char)imie_tmp[i])==0 || isupper((unsigned char)imie_tmp[i])==0)
			czy=0;
		else i++;
		}
		
	i=0;
	while(grupa_tmp[i]!='\0' && czy==1){ /*spr, czy tylko wielkie litery*/
		if(isalpha((unsigned char)grupa_tmp[i])==0 || isupper((unsigned char)grupa_tmp[i])==0)
			czy=0;
		else i++;
		}
		
	if(rok_tmp!=0 && (rok_tmp<YMIN || rok_tmp>YMAX)) /*spr czy rok zawiera sie w przedzialach dla osoby lub czy rowne 0 dla grupy*/
		czy=0;
}
fclose(plik);

if(czy==1){
	if(head_z!=NULL){
		el_z=head_z;
		tnij_wskazniki();
		fun_del_z();
		head_z=NULL;
		el_z=head_z;
	}
	plik=fopen(obiekt, "r");
	czytaj_txt();
	fclose(plik);
}
else{
	printf("\n Plik %s zawiera niepoprawne dane! Nie wczytano.\n Aby powrocic do menu", obiekt);
	do bufor=getchar();
	while (bufor!='\n');
	wroc();
}
}
else{
	printf("\n Plik %s nie istnieje! Aby powrocic do menu", obiekt);
	do bufor=getchar();
	while (bufor!='\n');
	wroc();
}
}

void tree_txt(int licznik){
while(1){
	fprintf(plik, "%d %s X X 0\n", licznik, el_z->nazwa);
	if(el_z->head_p!=NULL){
		el_z->el_p=el_z->head_p;
		while(el_z->el_p!=NULL){
			fprintf(plik, "-1 %s %s %s %d\n", el_z->el_p->nazwisko, el_z->el_p->imie, el_z->el_p->grupa, el_z->el_p->rok);
			el_z->el_p=el_z->el_p->next;
		}
	}
	
	if(el_z->down!=NULL){
		el_z=el_z->down;
		licznik++;
		tree_txt(licznik);
		licznik--;}
	if(el_z->next==NULL)
		break;
	else
		el_z=el_z->next;
}

if(temp_z==head_z){
	while(1){
	if(el_z->prev==NULL)
		if(el_z->up==NULL)
			break;
		else{
			el_z=el_z->up;
			break;
			}
	else
		el_z=el_z->prev;
	}
}
else{
	while(el_z==temp_z){
		if(el_z->prev==NULL)
			if(el_z->up==NULL)
				break;
			else{
				el_z=el_z->up;
				break;
				}
		else
			el_z=el_z->prev;
	}
}
}

void czytaj_txt(void){
int poziom_p=1,poziom_f, f=1;
while(f!=0 && (fscanf(plik, "%d %s %s %s %d", &poziom_f, nazwisko_tmp, imie_tmp, grupa_tmp, &rok_tmp))!=EOF){
	while(1){
	if(poziom_f==-1){
		if(el_z->head_p==NULL) 
       {el_z->head_p=(przedszkolak*)malloc(sizeof(przedszkolak));
       strcpy(el_z->head_p->imie, imie_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
       strcpy(el_z->head_p->nazwisko, nazwisko_tmp);
       strcpy(el_z->head_p->grupa, grupa_tmp);
       el_z->head_p->rok=rok_tmp;
       el_z->head_p->next=NULL;
       el_z->head_p->prev=NULL;
       }
     else 
        {el_z->el_p=el_z->head_p;
        if(strcmp(nazwisko_tmp, el_z->el_p->nazwisko)<0 || (strcmp(nazwisko_tmp, el_z->el_p->nazwisko)==0 && strcmp(imie_tmp,el_z->el_p->imie)<0))
             {el_z->el_p=(przedszkolak*)malloc(sizeof(przedszkolak)); /*wpisywanie przed obecnym el, zastepowanie glowy*/
              strcpy(el_z->el_p->imie, imie_tmp);
              strcpy(el_z->el_p->nazwisko, nazwisko_tmp);
              strcpy(el_z->el_p->grupa, grupa_tmp);
              el_z->el_p->rok=rok_tmp;
              el_z->el_p->next=el_z->head_p;
              el_z->el_p->prev=NULL;
              el_z->head_p->prev=el_z->el_p;
              el_z->head_p=el_z->el_p;
             }
        else /*dodawanie przed nastepnym elementem:*/
           {while(1){el_z->temp_p=el_z->el_p;
              if(el_z->el_p->next==NULL) 
                {el_z->el_p=(przedszkolak*)malloc(sizeof(przedszkolak));
                strcpy(el_z->el_p->imie, imie_tmp); /*jesli wstawaimy na koniec listy*/
                strcpy(el_z->el_p->nazwisko, nazwisko_tmp);
                strcpy(el_z->el_p->grupa, grupa_tmp);
                el_z->el_p->rok=rok_tmp;
				el_z->el_p->next=NULL;
                el_z->temp_p->next=el_z->el_p;
                el_z->el_p->prev=el_z->temp_p;
                break;
                }
              else 
              {el_z->el_p=el_z->el_p->next;
              if(strcmp(nazwisko_tmp, el_z->el_p->nazwisko)<0 || (strcmp(nazwisko_tmp, el_z->el_p->nazwisko)==0 && strcmp(imie_tmp,el_z->el_p->imie)<0))
                {el_z->el_p=(przedszkolak*)malloc(sizeof(przedszkolak));
                strcpy(el_z->el_p->imie, imie_tmp); /*jesli wstawiamy w srodek listy*/
                strcpy(el_z->el_p->nazwisko, nazwisko_tmp);
                strcpy(el_z->el_p->grupa, grupa_tmp);
				el_z->el_p->rok=rok_tmp;
                el_z->el_p->prev=el_z->temp_p;
                el_z->el_p->next=el_z->temp_p->next;
                el_z->temp_p->next->prev=el_z->el_p;
                el_z->temp_p->next=el_z->el_p;
                break;
                }
              }
              }
           }
        }
		break;
	}
	else{	
		if(poziom_f==poziom_p){
	if(head_z==NULL){
		head_z=(zajecia*)malloc(sizeof(zajecia));
		strcpy(head_z->nazwa, nazwisko_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
		head_z->next=NULL;
		head_z->prev=NULL;
		head_z->up=NULL;
		head_z->down=NULL;
		head_z->head_p=NULL;
		el_z=head_z;
		break;
		}
	else{while(el_z->prev!=NULL) /* spr, czy w wezle element ma unikalna nazwe*/
			el_z=el_z->prev;
		pozycja=1;
		if(strcmp(el_z->nazwa, nazwisko_tmp)==0)
					pozycja=0;
		while(el_z->next!=NULL && pozycja){
			if(strcmp(el_z->nazwa, nazwisko_tmp)==0)
					pozycja=0;
			el_z=el_z->next;
			}
		if(strcmp(el_z->nazwa, nazwisko_tmp)==0)
					pozycja=0;
		if(pozycja==1){
			temp_z=el_z;
			el_z=el_z->next;
			el_z=(zajecia*)malloc(sizeof(zajecia));
			strcpy(el_z->nazwa, nazwisko_tmp);
			temp_z->next=el_z;
			el_z->prev=temp_z;
			el_z->up=NULL;
			el_z->down=NULL;
			el_z->next=NULL;
			el_z->head_p=NULL;
			break;
		}
		else{
			f=0;
			break;		
		}
		}
	}
	
	else 	if(poziom_p<poziom_f){
				poziom_p++;
				temp_z=el_z;
				el_z=el_z->down;
				el_z=(zajecia*)malloc(sizeof(zajecia));
				strcpy(el_z->nazwa, nazwisko_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
				el_z->next=NULL;
				el_z->prev=NULL;
				el_z->up=temp_z;
				el_z->down=NULL;
				el_z->head_p=NULL;
				temp_z->down=el_z;
				break;
			}
			else{
				while(el_z->prev!=NULL)
					el_z=el_z->prev;
				el_z=el_z->up;
				poziom_p--;
				}
			}
		}
}
if(f==0){
			el_z=head_z;
			tnij_wskazniki();
			fun_del_z();
			head_z=NULL;
			el_z=head_z;
			printf(" Rekordy w wezle nie maja nunikalnej nazwy. Nie zaladowano!\n");
}
else
		printf("\n Plik %s zostal zaladowany!\n", obiekt);
}
