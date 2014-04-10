/*Name: Zadanie 2.3 - projekt bazy przedszkola
  Author: 1I1 - Piotr Jastrzebski
  Date: 15-03-09 14:15
  Description: Program zapewnia mozliwosc dodania przedszkolakow, wraz z podstawowymi danymi na ich temat, do bazy przedszkola.
               Tworzy on 3 listy, jedna posortowana wg grup, nazwisk i imion, druga posortowana wg nazwisk i imion, trzecia zas, pomocnicza - przechowuje unikatowe zajecia.
               Dla wieloelementowych baz danych, jest to lepsze rozwiazanie, niz sortowanie kazdorazowo przed wypisaniem.
               Istnieje mozliwosc wyswietlenia bazy calego przedszkola wg grup, lub jego czesci wg konkretnych zajec.*/
  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct przedszkolak{ /*lista dwukierunkowa - baza posortowana wg. grupa/nazwisko/imie*/
       char imie[20], nazwisko[20], grupa[20], zajecia[20];
       int rok;
       struct przedszkolak *next, *prev;
       }*head=NULL, *el=NULL, *temp=NULL;
       
struct przedszkolak_2{ /*lista dwukierunkowa - baza posortowana wg. nazwisko/imie*/
       char imie[20], nazwisko[20], grupa[20], zajecia[20];
       int rok;
       struct przedszkolak_2 *next, *prev;
       }*head_2=NULL, *el_2=NULL, *temp_2=NULL;
       
struct lista{ /*lista jednokierunkowa - unikalne rekordy zajec*/
       char zajecia[20];
       struct lista *next;
       }*head_l=NULL, *el_l=NULL, *temp_l=NULL;
  
char bufor; /*bufor znakow i licznik ilosci dodanych pozycji do bazy*/
int licznik; 

void menu(void);
void dod_dziecko(void);
void pok_grupy(void);
void pok_zajecia(void);
void help(void);
void wroc(void);
void linia(void);
void lista_push(char imie[], char nazwisko[], char grupa[], char zajecia[], int rok);
void lista_print(char flaga[]);
void listuj(char zajecia[]);
       
int main(void){
   menu();
   return 0;
}

void menu(void){/*menu glowne programu*/
   int poz;
   system("tput clear");
   printf("\n ::Radosny Przedszkolak::\n"); 
   linia();
      printf(" 1. Dodaj przedszkolaka [%d/19]\n", licznik);  
      printf(" 2. Wyswietl dzieci grupami\n"); 
      printf(" 3. Wyswietl uczestnikow zajec\n\n");
      printf(" 0. Pomoc\n"); 
   linia();
   printf(" Pozycja: ");

while(1)/*sprawdzenie poprawnosci wprowadzanych danych z zkresu 0-3*/
if(scanf("%d", &poz)!=1 || poz>3 || poz <0 || (bufor=getchar())!='\n')
                {printf(" Podano niepoprawna pozycje z menu. By wprowadzic ponownie");
                do bufor=getchar();
                while (bufor!='\n');
                wroc();
                }
else break;

switch(poz)
   {case 1 : dod_dziecko(); break;
    case 2 : pok_grupy(); break;
    case 3 : pok_zajecia(); break;
    case 0 : help(); break;
   }
}

void dod_dziecko(void){/*dodawanie do bazy nowej pozycji*/
   char imie[20], nazwisko[20], grupa[20], zajecia[20];
   int rok;
   system("tput clear");
   printf("\n Menu / Dodaj przedszkolaka\n");
   linia();
   printf(" Witaj w module dodawania przedszkolaka do bazy.\n\n");
   if(licznik>=19) /*zabezpiecza przed dodaniem za duzej ilosci przedszkolakow*/
      {printf(" Baza jest juz pelna. Aby powrocic do menu");
       wroc();
      }
   else
      {printf(" Podaj:\n");

printf(" - nazwisko dziecka: ");
while(1)
if(scanf("%[A-Z]s",nazwisko)!=1 || (bufor=getchar())!='\n') /*spr, czy tylko litery*/
                {printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - nazwisko dziecka: ");
                do bufor=getchar();
                while (bufor!='\n');
                }
else break;

printf(" - imie dziecka: ");
while(1)
if(scanf("%[A-Z]s",imie)!=1 || (bufor=getchar())!='\n')
                {printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - imie dziecka: ");
                do bufor=getchar();
                while (bufor!='\n');
                }
else break;

printf(" - grupe: ");
while(1)
if(scanf("%[A-Z]s",grupa)!=1 || (bufor=getchar())!='\n')
                {printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - grupe: ");
                do bufor=getchar();
                while (bufor!='\n');
                }
else break;

printf(" - rok urodzenia: ");
while(1)
if(scanf("%d",&rok)!=1 || rok<2000 || rok>2009 || (bufor=getchar())!='\n') /*spr, czy liczba*/
                {printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - rok: ");
                do bufor=getchar();
                while (bufor!='\n');
                }
else break;

printf(" - zajecia dodatkowe, w ktorych uczestniczy: ");
while(1)
if(scanf("%[A-Z]s",zajecia)!=1 || (bufor=getchar())!='\n')
                {printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie:\n - zajecia: ");
                do bufor=getchar();
                while (bufor!='\n');
                }
else break;

lista_push(imie, nazwisko, grupa, zajecia, rok);
printf("\n Dodanie przedszkolaka do bazy zakonczone sukcesem!\n");
linia();
printf(" Aby powrocic do menu");
wroc();
   }
}

void pok_grupy(void){/*wyswietlanie bazy wg grup, nazwiska i imienia*/
   system("tput clear");
   printf("\n Menu / Wyswietl dzieci grupami\n");
   linia();
   lista_print("0");/*funkcja print listy wg grup*/
   linia();
   printf(" Aby powrocic do menu");
   wroc();
}

void pok_zajecia(void){/*wyswietlanie przedszkolakow spelniajacych kryteria dot. zajec*/
   char zajecia[20];
   system("tput clear");
   printf("\n Menu / Wyswietl uczestnikow zajec\n"); 
   linia();
   if(head==NULL)
      {printf(" Baza jest pusta. By powrocic do menu i dodac przedszkolaka"); wroc();}
   else
      {printf(" Wprowadz nazwe zajecia, ktorego uczestnikow chesz wyswietlic:\n");
      el_l=head_l;
      while(el_l)
         {printf(" - %s\n", el_l->zajecia);
         el_l=el_l->next;
         }
      }
while(1)
if(scanf("%[A-Z]s",zajecia)!=1 || (bufor=getchar())!='\n')
                {printf(" Niepoprawny zakres wprowadzanych danych. Podaj ponownie: ");
                do bufor=getchar();
                while (bufor!='\n');
                }
else break;

lista_print(zajecia);/*funkcja print listy wg zajec*/
linia();
printf(" Aby powrocic do menu");
wroc();}

void help(void){/*pomoc programu*/
   system("tput clear");
   printf("\n Menu / Pomoc\n");
   linia();
   printf(" UWAGA! Wszystkie dane tekstowe wprowadzaj wielkimi literami! UWAGA!\n\n\n");
   printf(" ::Pozycja 1::\n Wczytuje do 20 pozycji zawierajacych:\n");
   printf(" - nazwisko i imie\n");
   printf(" - grupe\n");
   printf(" - rok urodzenia\n");
   printf(" - zajecia dodatkowe np.: rytmika, angielski, korektywa\n\n");
   printf(" ::Pozycja 2::\n Wyswietla calosc alfabetycznie wg grup,\n");
   printf(" a w ramach kazdej grupy wg alfabetycznej kolejnosci nazwisk i imion.\n\n");
   printf(" ::Pozycja 3::\n Wyswietla wybrane zajecia dodatkowe, np. tylko rytmika,\n");
   printf(" albo dzieci nie uczeszczajace na zadne zajecia.\n");
   printf(" W ramach kazdego zajecia pozycje uporzadkowane wg alfabetycznej\n");
   printf(" kolejnosci nazwisk i imion.\n");
   linia();
   printf(" Aby powrocic do menu");
   wroc();
}

void wroc(void){/*wraca do glownego menu*/
        printf(" nacisnij ENTER. ");
        while(1)
           {if((bufor=getchar())=='\n')
              menu();
           }
}
     

void linia(void){/*wyswietla stala linie*/
   printf(" ______________________________________________________________________________\n\n");
}

void lista_push (char imie_tmp[], char nazwisko_tmp[], char grupa_tmp[], char zajecia_tmp[], int rok_tmp){
     /*zapisuje zajecia do listy*/
     listuj(zajecia_tmp);
    /*dodaje do listy przedszkolak*/
     if(head==NULL) 
       {head=(struct przedszkolak*)malloc(sizeof(struct przedszkolak));
       strcpy(head->imie, imie_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
       strcpy(head->nazwisko, nazwisko_tmp);
       strcpy(head->grupa, grupa_tmp);
       strcpy(head->zajecia, zajecia_tmp);
       head->rok=rok_tmp;
       head->next=NULL;
       head->prev=NULL;
       }
     else 
     {el=head;
     if(strcmp(grupa_tmp,el->grupa)<0 || (strcmp(grupa_tmp,el->grupa)==0 && (strcmp(nazwisko_tmp, el->nazwisko)<0 || (strcmp(nazwisko_tmp, el->nazwisko)==0 && strcmp(imie_tmp,el->imie)<0))))
       {el=(struct przedszkolak*)malloc(sizeof(struct przedszkolak)); /*wpisywanie przed obecnym el, zastepowanie glowy*/
       strcpy(el->imie, imie_tmp);
       strcpy(el->nazwisko, nazwisko_tmp);
       strcpy(el->grupa, grupa_tmp);
       strcpy(el->zajecia, zajecia_tmp);
       el->rok=rok_tmp;
       el->next=head;
       el->prev=NULL;
       head->prev=el;
       head=el;
       }
     else /*dodawanie przed nastepnym elementem:*/
        {while(1)
                {temp=el;
                 if(el->next==NULL) 
                   {el=(struct przedszkolak*)malloc(sizeof(struct przedszkolak));
                   strcpy(el->imie, imie_tmp); /*a) jesli wstawaimy na koniec listy*/
                   strcpy(el->nazwisko, nazwisko_tmp);
                   strcpy(el->grupa, grupa_tmp);
                   strcpy(el->zajecia, zajecia_tmp);
                   el->rok=rok_tmp;
                   el->next=NULL;
                   temp->next=el;
                   el->prev=temp;
                   break;
                   }
                 else 
                   {el=el->next;
                       if(strcmp(grupa_tmp,el->grupa)<0 || (strcmp(grupa_tmp,el->grupa)==0 && (strcmp(nazwisko_tmp, el->nazwisko)<0 || (strcmp(nazwisko_tmp, el->nazwisko)==0 && strcmp(imie_tmp,el->imie)<0))))
                          {el=(struct przedszkolak*)malloc(sizeof(struct przedszkolak));
                           strcpy(el->imie, imie_tmp); /*b) jesli wstawiamy w srodek listy*/
                           strcpy(el->nazwisko, nazwisko_tmp);
                           strcpy(el->grupa, grupa_tmp);
                           strcpy(el->zajecia, zajecia_tmp);
                           el->prev=temp;
                           el->next=temp->next;
                           temp->next->prev=el;
                           temp->next=el;
                           break;
                          }
                   }
                 }
        }
     }
     
     /*dodaje do listy przedszkolak_2*/
     if(head_2==NULL) 
       {head_2=(struct przedszkolak_2*)malloc(sizeof(struct przedszkolak_2));
       strcpy(head_2->imie, imie_tmp); /*jesli lista jest pusta, tworzy nowa i el glowy*/
       strcpy(head_2->nazwisko, nazwisko_tmp);
       strcpy(head_2->grupa, grupa_tmp);
       strcpy(head_2->zajecia, zajecia_tmp);
       head_2->rok=rok_tmp;
       head_2->next=NULL;
       head_2->prev=NULL;
       }
     else 
        {el_2=head_2;
        if(strcmp(nazwisko_tmp, el_2->nazwisko)<0 || (strcmp(nazwisko_tmp, el_2->nazwisko)==0 && strcmp(imie_tmp,el_2->imie)<0))
             {el_2=(struct przedszkolak_2*)malloc(sizeof(struct przedszkolak_2)); /*wpisywanie przed obecnym el, zastepowanie glowy*/
              strcpy(el_2->imie, imie_tmp);
              strcpy(el_2->nazwisko, nazwisko_tmp);
              strcpy(el_2->grupa, grupa_tmp);
              strcpy(el_2->zajecia, zajecia_tmp);
              el_2->rok=rok_tmp;
              el_2->next=head_2;
              el_2->prev=NULL;
              head_2->prev=el_2;
              head_2=el_2;
             }
        else /*dodawanie przed nastepnym el_2ementem:*/
           {while(1){temp_2=el_2;
              if(el_2->next==NULL) 
                {el_2=(struct przedszkolak_2*)malloc(sizeof(struct przedszkolak_2));
                strcpy(el_2->imie, imie_tmp); /*a) jesli wstawaimy na koniec listy*/
                strcpy(el_2->nazwisko, nazwisko_tmp);
                strcpy(el_2->grupa, grupa_tmp);
                strcpy(el_2->zajecia, zajecia_tmp);
                el_2->rok=rok_tmp;
                el_2->next=NULL;
                temp_2->next=el_2;
                el_2->prev=temp_2;
                break;
                }
              else 
              {el_2=el_2->next;
              if(strcmp(nazwisko_tmp, el_2->nazwisko)<0 || (strcmp(nazwisko_tmp, el_2->nazwisko)==0 && strcmp(imie_tmp,el_2->imie)<0))
                {el_2=(struct przedszkolak_2*)malloc(sizeof(struct przedszkolak_2));
                strcpy(el_2->imie, imie_tmp); /*b) jesli wstawiamy w srodek listy*/
                strcpy(el_2->nazwisko, nazwisko_tmp);
                strcpy(el_2->grupa, grupa_tmp);
                strcpy(el_2->zajecia, zajecia_tmp);
                el_2->prev=temp_2;
                el_2->next=temp_2->next;
                temp_2->next->prev=el_2;
                temp_2->next=el_2;
                break;
                }
              }
              }
           }
        }
licznik++;
}
     
void lista_print(char flaga[]){/*wyswietla liste na ekran, flaga=0 - wg grup, flaga=zajecia - wg zajec*/
   int olsnienie=0;
   el=head;
   el_2=head_2;
   if(head==NULL)
      {printf(" Baza jest pusta. By powrocic do menu i dodac przedszkolaka"); wroc();}
   else
      {while(el_2)
          {if (strcmp(flaga, "0")==0)/*lista wg grup*/
              {printf("\n Nazwisko: %s\n Imie: %s\n Grupa: %s\n Zajecia: %s\n Rok urodzenia: %d\n", el->nazwisko, el->imie, el->grupa, el->zajecia, el->rok);
              olsnienie=1;
              }
           else if(strcmp(flaga, el_2->zajecia)==0)/*lista wg zajec*/
              {olsnienie=1;
              printf("\n Nazwisko: %s\n Imie: %s\n Grupa: %s\n Zajecia: %s\n Rok urodzenia: %d\n", el_2->nazwisko, el_2->imie, el_2->grupa, el_2->zajecia, el_2->rok);
              }
           el_2=el_2->next;
           el=el->next;
           if(el_2==NULL && olsnienie==0)
              printf(" Zaden przedszkolak nie uczeszcza na podane zajecia.\n");
           }
      }
}
           
void listuj(char zajecia[]){ /*dodaje do jednokierunkowej listy unikalne rekordy zajec*/
     int flaga=0;
     if(head_l==NULL)
        {head_l=(struct lista*)malloc(sizeof(struct lista));
        strcpy(head_l->zajecia, zajecia);
        head_l->next=NULL;
        }
     else
        {el_l=head_l;
         while(el_l)
            {if(strcmp(zajecia, el_l->zajecia)==0)
               {flaga=1; break;}
            else
             {temp_l=el_l; el_l=el_l->next;}
            } 
             if(el_l==NULL && flaga==0)
                {el_l=(struct lista*)malloc(sizeof(struct lista));
                strcpy(el_l->zajecia, zajecia);
                el_l->next=NULL;
                temp_l->next=el_l;
                }
       }
}
