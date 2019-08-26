#include <stdio.h>
#include <stdlib.h>
#include "conio.c"
#include <time.h>
#define ARROW_UP 72 
#define ARROW_DOWN 80 
#define ARROW_LEFT 75 
#define ARROW_RIGHT 77
int proba[10][10];
/****** TEXTES ******/
int menu()
{
	int choix;
	do{ system("cls"); textcolor(LIGHTCYAN); printf("\n\n\n\t\t\tMENU");
		textcolor(WHITE); printf("\n\n\n\tQue souhaitez-vous faire:\n\t0: Afficher les r%cgle du jeu\n",138);
		printf("\t1: Jouer %c la bataille navale\n\t2: Quitter \n\nVotre choix: ", 133);
	 	scanf("%d",&choix);} while (choix<0 || choix>2);
	return choix;
}

int menu_jeu()
{	int choix;
	do{	textcolor(LIGHTCYAN); printf("\n\n\t\t Comment voulez vous jouer ?");
		textcolor(WHITE);printf("\n\t0: 1 VS 1\n\t1: 1 VS Ordi\n\t2: Ordi VS Ordi\n\nVotre choix: ");
		scanf("%d",&choix);} while (choix<0 || choix>2);
	return choix;
}

void regle()
{	printf("Ce jeu de soci%ct%c se joue %c deux, l'un contre l'autre sur deux grilles o%c sont plac%cs 5 navires mis en place par les joueurs.",130,130,133,151,130);
	printf("\n\nLe but %ctant de faire couler tous les navires de l'adversaire.",130);
	printf("\n\nVos 5 navires sont de tailles diff%cntes, il y en %c un de 2 cases, deux de 3 cases, un de 4 cases et un de 5 cases.",130,133);
	printf("\n\nAu d%cbut du jeu, il vous faudra placer les bateaux sur votre espace de jeu.",130);
	printf("Lors du placement des bateaux, vous ne placerez que la t%cte du bateau, pour le placement %c l'horizontale, il s'agira de la case la plus %c gauche.",136,133,133); 
	printf("Pour le placement %c la verticale, il s'agira de la case la plus haute.",133);
	printf("\n\nPour s%cl%cctionner l'une des cases de la grille, il faut appuyer sur ESPACE.", 130,130);
	printf("\n\nLe jeu se termine lorsque l'une des flottes de l'un des joueur a %ct%c compl%ctement d%ctruite\n\n", 130,130,138, 130);
}

/****** DEFINITION BATEAU, INITIALISATION ******/
typedef struct{int vertical; int x_tete; int y_tete; int taille; int cpt;} bateau; 

void init_flotte(bateau* flotte) 
{	flotte[0].cpt=flotte[1].cpt=flotte[2].cpt=flotte[3].cpt=flotte[4].cpt=0; 
	flotte[2].taille=flotte[4].taille=3, flotte[0].taille=5, flotte[1].taille=4, flotte[3].taille=2;}

void init_memo(int memo[5]) {int i;for(i=0; i<5; i++) memo[i]=-1;}

/***** FONCTIONS GENERALES *****/
void affichage_placement(int tab[10][10])
{	int i,j;
	printf("\n\n");
	printf("    A  B  C  D  E  F  G  H  I  J\n");
	printf("    __ __ __ __ __ __ __ __ __ __\n");
	for(i=0;i<10;i++)
	{	printf("   |");
      	for(j=0;j<10;j++) 
      	{	if(tab[i][j]==1) printf(" O|");
      		else if(tab[i][j]==9) printf(" X|");
      		else printf("  |");	
	  	}
      printf("\n %d |__|__|__|__|__|__|__|__|__|__|\n",i);
	}
	printf("\n\n\n Les X sont les cases o%c l'on ne peut pas placer  de bateau et les O sont l%c o%c il y a d%cj%c un bateau.", 151,133,151,130,133);
	printf("\nAppuyez sur espace pour valider la case (ce sera toujours avec cette touche, y compris pour tirer)");
}

void affichage(int tab[10][10])
{	int i,j;
	printf("\n\n");
	printf("    A  B  C  D  E  F  G  H  I  J\n");
	printf("    __ __ __ __ __ __ __ __ __ __\n");
	for(i=0;i<10;i++)
	{ printf("   |");
      for(j=0;j<10;j++) 
	  {	if(tab[i][j]==2) printf(" O|");
		else if(tab[i][j]==3) textcolor(LIGHTRED),printf(" X"),textcolor(WHITE),printf("|");  
		else printf("  |");
	  }
      printf("\n %d |__|__|__|__|__|__|__|__|__|__|\n",i);
	}
}

/***** PLACEMENT *****/
void bateau_fantome(bateau *bat, int jeu[10][10]) 
{	int i;
	if (bat->vertical)
	{	if (bat->x_tete-1>=0) jeu[(bat->x_tete)-1][bat->y_tete]=9;
		if (bat->x_tete+bat->taille<10) jeu[(bat->x_tete)+bat->taille][bat->y_tete]=9;
		for (i=0; i<bat->taille; i++) 
		{ if (bat->x_tete+i<10 && bat->y_tete+1<10) jeu[(bat->x_tete)+i][(bat->y_tete)+1]=9;
		  if (bat->x_tete+i<10 && bat->y_tete-1>=0) jeu[(bat->x_tete)+i][(bat->y_tete)-1]=9;
		}
	}
	else
	{	if (bat->y_tete-1>=0) jeu[bat->x_tete][(bat->y_tete)-1]= 9;
		if ((bat->y_tete)+bat->taille<10) jeu[bat->x_tete][(bat->y_tete)+bat->taille]=9;
		for (i=0; i<bat->taille; i++)
		{	if (bat->y_tete+i<10 && bat->x_tete-1>=0) jeu[(bat->x_tete)-1][(bat->y_tete)+i]=9;
			if (bat->y_tete+i<10 && bat->x_tete+1<10) jeu[(bat->x_tete)+1][(bat->y_tete)+i]=9;
		} 
	}
}

void supprime_9(int jeu[10][10])
{	int i, j;
	for (i=0; i<10;i++) for (j=0; j<10; j++) if (jeu[i][j]==9) jeu[i][j]=0;
}

	// Placement Joueur
void curseur_placement(int tab[10][10], bateau* navire)  
{   int x=5, y=5, ligne=0, colonne=0, continuer=0, i, rate;
    gotoxy(x,y);printf("*");	
	while(continuer==0) if(kbhit()) // conserve la valeur entrée par l'utilisateur
		switch(getch())
		{
			case ARROW_RIGHT:
						  gotoxy(x,y);printf(" "); 
						  if (navire->vertical) if(x>=32) x=5,colonne=0; else x+=3,colonne++;
						  else                  if(x>=32-3*(navire->taille-1)) x=5,colonne=0; else x+=3,colonne++;
						  gotoxy(x,y);printf("*");break;
     	    case ARROW_LEFT:
    		   	       	  gotoxy(x,y);printf(" ");
    		   	       	  if (navire->vertical) if(x<=7) x=32,colonne=9; else x-=3,colonne--;
    		   	       	  else                  if(x<=7) x=32-3*(navire->taille-1),colonne=10-navire->taille; else x-=3,colonne--;
					      gotoxy(x,y);printf("*");break;
			case ARROW_DOWN: 	  	
                          gotoxy(x,y);printf(" ");
                          if (navire->vertical) if(y>=23-2*(navire->taille-1)) y=5,ligne=0; else y+=2,ligne++;
                          else if(y>=23) y=5,ligne=0; else y+=2,ligne++;
      					  
	                      gotoxy(x,y);printf("*");break;
           	case ARROW_UP:	  	
      					  gotoxy(x,y);printf(" ");
      					  if (navire->vertical) if(y<=6) y=23-2*(navire->taille-1),ligne=10-navire->taille; else y-=2,ligne--;
      					  else if(y<=6) y=23,ligne=9; else y-=2,ligne--;
						  gotoxy(x,y);printf("*");break;
			case ' ': 
			    rate=0;
				if (!navire->vertical) for (i=0; i<navire->taille; i++) rate|=tab[ligne][colonne+i];
				else                   for (i=0; i<navire->taille; i++) rate|=tab[ligne+i][colonne];
				
				if(rate) break; 
				if (!navire->vertical) for (i=0; i<navire->taille; i++) tab[ligne][colonne+i]=1;
				else                   for (i=0; i<navire->taille; i++) tab[ligne+i][colonne]=1;
				navire->x_tete=ligne; navire->y_tete=colonne; bateau_fantome(navire, tab); continuer++;			  
		}
}		

void placement_bateaux(int tab[10][10], bateau* flotte) 
{	int i;
	for (i=0; i<4; i++)	
	{	
		do{	printf("\n Voulez-vous placer le bateau %c %d cases %c l'horizontale (tapez 0) ou %c la verticale (tapez 1) ?", 133,flotte[i].taille,133,133); 
	       	scanf("%d", &flotte[i].vertical);} while (flotte[i].vertical<0 || flotte[i].vertical>1);
		printf("\nPlacez la t%cte de ce bateau : \n Pour s%clectionner une case, veuillez appuyer sur la barre espace.",136,130);
		printf("\n Pour un placement %c la verticale, la t%cte se situe sur la case la plus %c haute du bateau.", 133,136,133);
		printf("\n Pour un placement %c l'horizontale, la t%cte se trouve sur la case la plus %c gauche du bateau.",133,136,133);
		printf("Vous ne pouvez pas coller les bateaux!");
		system("PAUSE"); system("cls");
		affichage_placement(tab); curseur_placement(tab, &flotte[i]); system("cls");
	}
	do{	printf("\nVoulez-vous placer le 2e bateau %c 3 cases %c l'horizontale (tapez 0) ou %c la verticale (tapez 1) ?", 133,133,133); 
		scanf("%d", &flotte[4].vertical);} while (flotte[4].vertical<0 || flotte[4].vertical>1);
	printf("\nPlacez la t%cte de ce bateau : \n Pour s%clectionner une case, veuillez appuyer sur la barre espace.",136,130);
	printf("\n Pour un placement %c la verticale, la t%cte se situe sur la case la plus %c haute du bateau.", 133,136,133);
	printf("\n Pour un placement %c l'horizontale, la t%cte se trouve sur la case la plus %c gauche du bateau.",133,136,133);
	printf("Vous ne pouvez pas coller les bateaux et donc placer de bateaux sur les croix rouges");
	system("PAUSE"), system("cls"),	affichage_placement(tab), curseur_placement(tab, &flotte[4]), system("cls");
	supprime_9(tab);
	
}
	// Placement Ordinateur

void placement_ordi(int tab[10][10], bateau* flotte)
{	int i,ligne,colonne,rate,cpt; 
	for (cpt=0; cpt<5;cpt++)
	{	
		if (rand()%2) 
		{
			do {rate=0,ligne=rand()%9, colonne=rand()%(10-flotte[cpt].taille); 
				for(i=colonne; i<flotte[cpt].taille+colonne; i++) rate|=tab[ligne][i];} while(rate); 
			flotte[cpt].vertical=0; for(i=colonne; i<flotte[cpt].taille+colonne; i++) tab[ligne][i]=1;
		}
		else
		{
			do {rate=0,ligne=rand()%(10-flotte[cpt].taille),colonne=rand()%9;
		 		for(i=ligne; i<flotte[cpt].taille+ligne; i++) rate|=tab[i][colonne];} while (rate);
			flotte[cpt].vertical=1; for(i=ligne; i<flotte[cpt].taille+ligne; i++) tab[i][colonne]=1;
		}
		flotte[cpt].x_tete=ligne; flotte[cpt].y_tete=colonne; bateau_fantome(&flotte[cpt], tab);
	}	  
	supprime_9(tab);
}


/***** TIRER *****/
int indice_bateau(int x, int y, bateau *flotte)
{
	int i, j;
	for (i=0; i<5; i++)	{if (flotte[i].vertical==1 && flotte[i].y_tete==y) for (j=0; j<flotte[i].taille; j++) if (flotte[i].x_tete+j==x) return i;
						 if (flotte[i].vertical==0 && flotte[i].x_tete==x) for (j=0; j<flotte[i].taille; j++) if (flotte[i].y_tete+j==y) return i;}
	return -1;
}

int win (bateau* flotte)
{
	int i;
	for (i=0; i<5; i++) if (flotte[i].cpt!=flotte[i].taille) return 0;
	return 1;
}

void etat(int ind, bateau* flotte)
{
	if (ind==-1) {printf("\n Rien touch%c!", 130); return;}
	if (flotte[ind].cpt==flotte[ind].taille) {printf("\n Bateau coul%c !", 130); return;}
	printf("\n Bateau touch%c!", 130); return;
}


//Tirer JOUEUR
void curseur_tirer(int tab[10][10], bateau* flotte)
{
    int x=5, y=5, indice, ligne=0,colonne=0, continuer=0;
    printf("Choisissez o%c vous voulez tirer\n",151);gotoxy(x,y);printf("*");	 	
	while(continuer!=1)	if(kbhit()) // conserve la valeur entrée par l'utilisateur
		switch(getch())
		{
			case ARROW_RIGHT:
						  gotoxy(x,y);printf(" "); 
						  if(x>=32) x=5,colonne=0; else x+=3,colonne++;
						  gotoxy(x,y);printf("*");break;
     	    case ARROW_LEFT:
    		   	       	  gotoxy(x,y);printf(" ");
    		   	       	  if(x<=7) x=32,colonne=9; else x-=3,colonne--;
					      gotoxy(x,y);printf("*");break;
			case ARROW_DOWN: 	  	
                          gotoxy(x,y);printf(" ");
      					  if(y>=23) y=5,ligne=0; else y+=2,ligne++;
	                      gotoxy(x,y);printf("*");break;
           	case ARROW_UP:	  	
      					  gotoxy(x,y);printf(" ");
      					  if(y<=6) y=23,ligne=9; else y-=2,ligne--;
						  gotoxy(x,y);printf("*");break;
			case ' ': 
				while(y<24 && x<33)	{gotoxy(x+1,y);
					if (tab[ligne][colonne]==1)	indice=indice_bateau(ligne, colonne, flotte), flotte[indice].cpt++,tab[ligne][colonne]+=2,continuer++;
					else if (tab[ligne][colonne]==0) tab[ligne][colonne]+=2, continuer++,indice=-1;
					break;}
		}
		gotoxy(40, 25); etat(indice, flotte);
}


void tirer_joueur(int tab[10][10], bateau* flotte)
{ 	
	affichage(tab);
	curseur_tirer(tab, flotte);	
	system("PAUSE");
}

// Tirer Ordinateur ("IA")
void couler_ordi(int jeu[10][10], bateau* flotte, int memo[5])
{	int i=1;
	if (memo[4]==-1) {// On ne connait pas l'orientation et l'ordinateur ne doit pas savoir avant de tirer (d'où la forme des if)
		if (jeu[memo[2]+1][memo[3]]<2 && memo[2]+1<10) {memo[0]=memo[2]+1, memo[1]=memo[3]; if (jeu[memo[0]][memo[1]]==1) memo[4]=1;}	
		else if (jeu[memo[2]][memo[3]+1]<2 && memo[3]+1<10) {memo[0]=memo[2], memo[1]=memo[3]+1; if (jeu[memo[0]][memo[1]]==1) memo[4]=0;}
		else if (jeu[memo[2]-1][memo[3]]<2 && memo[2]-1>0)  {memo[0]=memo[2]-1, memo[1]=memo[3]; if (jeu[memo[0]][memo[1]]==1) memo[4]=1;}
		else memo[0]=memo[2],memo[1]=memo[3]-1, memo[4]=0;}
	else{	
		if(memo[4]){ // vertical
			while(memo[2]+i<10) {if(jeu[memo[2]+i][memo[3]]==2) {i=-1; break;} if(jeu[memo[2]+i][memo[3]]<2) break; else i++;}
			if (i==-1) while(jeu[memo[2]+i][memo[3]]>1)     {if(jeu[memo[2]+i][memo[3]]<2) break; i--;}
			memo[0]=memo[2]+i;}
		else{
			while(memo[3]+i<10) {if(jeu[memo[2]][memo[3]+i]==2) {i=-1; break;} if(jeu[memo[2]][memo[3]+i]<2) break; i++; }
			if (i==-1) while(jeu[memo[2]][memo[3]+i]>1) {if(jeu[memo[2]][memo[3]+i]<2) break; i--; }
			memo[1]=memo[3]+i;}}
}

void tab_proba(int jeu[10][10], bateau* flotte, int memo[5]) 
{
	int i,j,l,indice;
	for (i=0; i<10; i++) for (j=0;j<10; j++) proba[i][j]=0;
	for(indice=0; indice<5; indice++) for (i=0; i<10; i++) for (j=0; j<10; j++)
	{
		if (flotte[indice].cpt!=flotte[indice].taille) 
		{
			for (l=0; l<flotte[indice].taille; l++) if (jeu[i][j+l]>1 || j+l>10) break;
			if (l==flotte[indice].taille) for (l=0; l<flotte[indice].taille; l++) proba[i][j+l]++;
			for (l=0; l<flotte[indice].taille; l++) if (jeu[i+l][j]>1 || i+l>10) break;
			if (l==flotte[indice].taille) for (l=0; l<flotte[indice].taille; l++) proba[i+l][j]++;
		}
	}
	memo[0]=memo[1]=0; 
	for (i=0; i<10; i++) for(j=0; j<10; j++) if (proba[i][j]>proba[memo[0]][memo[1]]) memo[0]=i, memo[1]=j;
	if(jeu[memo[0]][memo[1]]==1) memo[2]=memo[0], memo[3]=memo[1];
}

void i_alea(int jeu[10][10], int memo[5])
{
	do {memo[0]=rand()%9, memo[1]=rand()%9;} while (jeu[memo[0]][memo[1]]>1);
}

void tirer_ordi(int jeu[10][10], bateau* flotte, int memo[5], int facile)
{
	if (facile) i_alea(jeu, memo);
	else
	{	if (memo[2]==-1) tab_proba(jeu,flotte,memo);
	 	else couler_ordi(jeu, flotte, memo);
	}		
    int indice=indice_bateau(memo[0], memo[1], flotte); 
	jeu[memo[0]][memo[1]]+=2;
	if(jeu[memo[0]][memo[1]]==3) flotte[indice].cpt++;
	if (flotte[indice].cpt==flotte[indice].taille) init_memo(memo);
	affichage(jeu); gotoxy(40, 25); etat(indice, flotte); system("PAUSE"); system("cls");	
}

//Jouer
void joue() 
{
	int i,j1,j2,lvl1,lvl2,choix,memo1[5],memo2[5], jeu1[10][10], jeu2[10][10];
	bateau* flotte1=(bateau*)malloc(sizeof(bateau)*5); bateau* flotte2=(bateau*)malloc(sizeof(bateau)*5);
	init_flotte(flotte1), init_flotte(flotte2); for (j1=0; j1<10; j1++) for (j2=0; j2<10; j2++) jeu1[j1][j2]=jeu2[j1][j2]=0; 
	init_memo(memo1), init_memo(memo2);
	choix=menu_jeu();
	if (choix==0) j1=j2=1; if (choix==1) j1=1, j2=0; if(choix==2) j1=j2=0;	
	if (!j2) do printf("\nQuel niveau voulez-vous pour l'ordinateur 1? \n\t\t0:Imbattable\n\t\t1:Facile\nVotre choix:"),
	            scanf("%d", &lvl1);while(lvl1<0 || lvl1>1);
	if (!j1) do printf("\nQuel niveau voulez-vous pour l'ordinateur 2? \n\t\t0:Imbattable\n\t\t1:Facile\nVotre choix:"), 
				scanf("%d", &lvl2);while(lvl2<0 || lvl1>2);
	if (j1) placement_bateaux(jeu1, flotte1); else placement_ordi(jeu1, flotte1);
	if (j2) placement_bateaux(jeu2, flotte2); else placement_ordi(jeu2, flotte2);
	for (i=1;;i++)
	{	system("cls"); printf("\n Tour %d du Joueur 1\n",i); system("PAUSE");system("cls");
		if (j1) tirer_joueur(jeu2, flotte2);else tirer_ordi(jeu2, flotte2, memo2, lvl2);
		if (win(flotte2)) break; 
		system("cls"); printf("\n Tour %d du Joueur 2\n",i); system("PAUSE"); system("cls");
		if (j2) tirer_joueur(jeu1, flotte1); else tirer_ordi(jeu1, flotte1, memo1, lvl1);
		if (win(flotte1)) break;	
	}
	system("cls"); textcolor(LIGHTRED);
	if (win(flotte2)) printf("\n\n\n\t\tLe joueur 1 gagne!");
	else  printf("\n\n\n\t\tLe joueur 2 gagne!");
	textcolor(WHITE);
	free(flotte1);free(flotte2);
}

int main()
{ 	
	srand(time(0));
	system("mode con LINES=50 COLS=50");
	int choix;
	textcolor(LIGHTCYAN); printf("\n\n\n\n\tBienvenue dans la Bataille Navale\n\n\n"); textcolor(WHITE); system("PAUSE");
	for (;;)
	{	choix=menu();
		if(choix==0) regle(); 
		if(choix==1) joue();
		if(choix==2) break;
		printf("\n"); system("PAUSE");system("cls");
	}
  	
   	printf("A bient%ct!", 147);
	fflush(stdin); getchar();
	return 0;
}
