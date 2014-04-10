/*Name: 		Zadanie 3.3 - projekt rozbudowanej bazy przedszkola
  Author: 		1I1 - Piotr Jastrzebski
  Date: 		20-04-09 19:15
  Description: 	Napisać komputerowy spis dzieci przedszkola (lub odpowiednio rozbudować
				poprzedni program (zad. 2.3)), zawierający dowolnie wiele pozycji,
				zorganizowanych w drzewie zajęć dodatkowych (np. angielski, pływanie,
				rysunek, itp.). Każda grupa (zajęcie dod.) może zawierać w sobie podrzędne
				grupy, przy czym nazwy grup równorzędnych w danym węźle drzewa nie mogą się
				powtarzać (w innym przypadku możliwe jest powtarzanie nazw). W ramach każdej
				grupy pozycje opisywane są jak w zad 2.3, tzn.: nazwisko, imię, grupa
				przedszkolna, rok urodzenia. Program powinien umożliwiać:
				-    	wpisywanie nowych i modyfikację już istniejących  pozycji,
				-       tworzenie  nowej grupy zajęć dodatkowych w dowolnym miejscu drzewa,
				-       usunięcie istniejącej grupy z drzewa,
				-       zmianę nazwy dowolnej grupy zajęć ,
				-       przenoszenie fragmentu drzewa,
				-       wyszukiwanie i wyświetlanie listy dzieci wg zadanych parametrów,
				takich jak w zad. 2.3 (np.: wyświetl wszystkich z grupy przedszkolnej
				"skrzaty", albo: znajdź dzieci urodzone w 2002 roku) ,
				-       wyświetlanie całego drzewa,
				-       wyświetlanie zawartości dowolnie wybranego zajęcia łącznie z innymi
				zajęciami w nim zawartymi (pozycje w kolejności alfabetycznej nazwisk).
				
				Program powinien być odporny i reagować na nielogiczne wprowadzanie danych
				(np. rok urodzenia: ABCD).
				
				Program przyjmuje 20 znakowe ciagi znakow zlozone z wielkich liter alfabetu lacinskiego wraz z
				znakiem podkreslenia. Minimalny i maksymalny rok urodzenia dziecka moze byc zmieniany przez
				predefiniowane stale YMIN, YMAX, ktore wynosza odpowiednio 2000 i 2009*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 20 /*dlugosc stringow*/
#define YMIN 2000 /*minimalny rok urodzenia przedszkolaka*/
#define YMAX 2009 /*maksymalny rok urodzenia przedszkolaka*/

char bufor, nazwa_tmp[N], imie_tmp[N], nazwisko_tmp[N], grupa_tmp[N], nazwa_tmp2[N], imie_tmp2[N], nazwisko_tmp2[N], grupa_tmp2[N]; /*bufor, tablice pomocnicze*/
int rok_tmp, rok_tmp2, pozycja, war; /*zmienne pomocnicze roku urodzenia, zmienna wyboru pozycji, zmienna warunku unikalnosci rekordu*/

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

		
/*TODO*/ /*TODO*/
		
/*FUNKCJE PROGRAMU*/
/*OK*/ void menu(void); /*wyswietla menu*/
/*OK*/ void linia(void); /*wyswietla stala linie*/
/*OK*/ void wroc(void); /*wraca do menu*/
/*OK*/ void monit(void); /*wyswietla aktualna grupe*/

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


int main(){
	menu();
	return 0;
}

void menu(void){
	system("tput clear");
	printf("\n ::Radosny Przedszkolak 2.0::\n");
	monit();
	linia();
	printf(" :: Zajecia\t\t\t\t\t\t\t:: Przedszkolak\n\n");
	printf(" 0. dodaj grupe\t\t\t\t\t\t\t10. dodaj\n");
	printf(" 1. dodaj podgrupe\t\t\t\t\t\t11. edytuj\n");
	printf(" 2. grupa nizej\t\t\t\t\t\t\t12. wyszukaj\n");
	printf(" 3. zmien nazwe\n");
	printf(" 4. grupa na lewo\n");
	printf(" 5. usun\n");
	printf(" 6. grupa po prawo\n");
	printf(" 7. przenies\n");
	printf(" 8. grupa wyzej\n");
	printf(" 9. wyswietl grupe z podgrupami\n\n\n");
	printf("\t\t\t\t   [8] GORA\n\n");
	printf("\t\t\t   [4] LEWO\t   [6] PRAWO\n\n");
	printf("\t\t\t\t   [2] DOL\n");
	linia();
	printf(" Pozycja: ");

while(1){/*sprawdzenie poprawnosci wprowadzanych danych z zkresu + nie zezwala operowac, gdy nie ma grup*/
	if(scanf("%d", &pozycja)!=1 || (head_z==NULL && pozycja!=0) || pozycja>12 || pozycja<0 || (bufor=getchar())!='\n'){
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
int i=0;
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
