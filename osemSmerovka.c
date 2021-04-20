#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define N 3
#define SUBOR "osemsmerovka.txt"

/* ROMAN BITAROVSKY
	VISUAL STUDIO 2019 
		ZPRPR2 Projekt 1 praca s 2d poliami */

int ries_slovo(int* riadky, int* stlpce, int** tajnicka, int* h_riadok, int* h_stlpec, char slovo[]);

void nacitaj_tajnicku_zo_suboru(FILE* fr, int* riadky, int* stlpce, int** tajnicka) {
	char c = 0;
	for (int m = 0; m < *riadky; m++) {
		for (int n = 0; n < *stlpce+1; n++) {
			c = getc(fr);
			if (isupper(c)) { //vsetky velke pismena sa ulozia do 2d pola tajnicka
				tajnicka[m][n] = c;
			}
		}
	}
}

void zapis_indexy(int* riadky, int* stlpce, int** tajnicka, int* indexy[], int dlzka_stlpcov_indexy_alock[], int dlzka_stlpcov_indexy_realpocet[]) {

	int pismenko = 'A';
	for (int index = 0; index < 26; index++) {
		indexy[index][0] = pismenko;
		pismenko++;
		dlzka_stlpcov_indexy_realpocet[index] = 1;
	}
	//proste to do pola indexy zapise jendotlive vyskyty pre kazde pismenko kt. je v poli tajnicka
	for (int i_riadok = 0; i_riadok < *riadky; i_riadok++) {
		for (int i_stlpec = 0; i_stlpec < *stlpce; i_stlpec++) {
			for (int i_indexy = 0; i_indexy < 26; i_indexy++) {
				if (tajnicka[i_riadok][i_stlpec] == indexy[i_indexy][0]) {
					if (dlzka_stlpcov_indexy_alock[i_indexy] - dlzka_stlpcov_indexy_realpocet[i_indexy] < 2) {
						//reallock pola indexy ak je poèet vo¾ných zápisov menší ako 2
						if ((indexy[i_indexy] = (int*)realloc(indexy[i_indexy], sizeof(int) * (dlzka_stlpcov_indexy_alock[i_indexy] + (2 * N)))) == NULL) {
							printf("Error! memory not reallocated.");
							exit(0);
						}
						dlzka_stlpcov_indexy_alock[i_indexy] += (2 * N);
					}
					indexy[i_indexy][dlzka_stlpcov_indexy_realpocet[i_indexy]] = i_riadok;
					indexy[i_indexy][dlzka_stlpcov_indexy_realpocet[i_indexy] + 1] = i_stlpec;
					dlzka_stlpcov_indexy_realpocet[i_indexy] += 2;

					//na nezapisane miesta v pole indexy sa da -1
					for (int jednicky = dlzka_stlpcov_indexy_realpocet[i_indexy] + 1; jednicky < dlzka_stlpcov_indexy_alock[i_indexy]; jednicky++) {
						indexy[i_indexy][jednicky] = -1;
					}
				}
			}
		}
	}
}

void nacitaj_slovo(FILE* fr, int* riadky, int* stlpce, int** tajnicka, int* indexy[], int dlzka_stlpcov_indexy_realpocet[]) {

	char slovo[101] = { 0 };
	while (fscanf(fr, "%s", slovo) == 1) { 	//teraz zoberiem slovo zo suboru
		//printf("slove je: %s \n", slovo);
		int i_indexy;
		for (i_indexy = 0; i_indexy < 26; i_indexy++) { //najdem si prve pismenko slova v indexe
			if (slovo[0] == indexy[i_indexy][0])
				break;
		}

		int nachadza = dlzka_stlpcov_indexy_realpocet[i_indexy];
		int h_riadok = 0, h_stlpec = 0, zapis = 1;
		while (zapis < dlzka_stlpcov_indexy_realpocet[i_indexy] -1) {
			h_riadok = indexy[i_indexy][zapis];
			h_stlpec = indexy[i_indexy][zapis + 1];
			zapis += 2;

			nachadza -= (ries_slovo(riadky, stlpce, tajnicka, &h_riadok, &h_stlpec, slovo));
		}
		if (nachadza == dlzka_stlpcov_indexy_realpocet[i_indexy]) {
			printf("Slovo %s sa nepodarilo najst \n \n", slovo);
		}
		else { //print tajnicka
			for (int riadoktest = 0; riadoktest < *riadky; riadoktest++) {
				for (int stlpectest = 0; stlpectest < *stlpce; stlpectest++) {
					printf("%c", tajnicka[riadoktest][stlpectest]);
				}
				printf("\n");
			}
			printf("\n");
		}
	}
}

int ries_slovo(int* riadky, int* stlpce, int** tajnicka, int* h_riadok, int* h_stlpec, char slovo[]) {
	//funkcia hlada slovo vo vsetkych 8 smeroch
	int dlzka_slova = strlen(slovo);
	int skumany_r = *h_riadok, skumany_s = *h_stlpec;
	int flag = 0, cislo_pismenka = 0, vyskyt_slova = 0;

	//sever
	while (flag != 1) {
		for (skumany_r = *h_riadok; skumany_r > * h_riadok - dlzka_slova; skumany_r--) {
			if ((skumany_r < *riadky && skumany_r >= 0 && cislo_pismenka < dlzka_slova) && (tajnicka[skumany_r][skumany_s] == slovo[cislo_pismenka] || tajnicka[skumany_r][skumany_s] == tolower(slovo[cislo_pismenka]))) {
				cislo_pismenka++;
			}
			else {
				flag = 1;
				break;
			}
		}
		if (flag != 1) { //tak to slovo je na severe
			for (skumany_r = *h_riadok; skumany_r > * h_riadok - dlzka_slova; skumany_r--) {
				tajnicka[skumany_r][skumany_s] = tolower(tajnicka[skumany_r][skumany_s]);
			}
			vyskyt_slova += 1;
			flag = 1;
		}
	}

	//juh
	flag = 0, cislo_pismenka = 0;
	skumany_s = *h_stlpec;
	while (flag != 1) {
		for (skumany_r = *h_riadok; skumany_r - *h_riadok < dlzka_slova; skumany_r++) {
			if ((skumany_r < *riadky && skumany_r >= 0 && cislo_pismenka < dlzka_slova) && (tajnicka[skumany_r][skumany_s] == slovo[cislo_pismenka] || tajnicka[skumany_r][skumany_s] == tolower(slovo[cislo_pismenka]))) {
					cislo_pismenka++;
			}
			else {
				flag = 1;
				break;
			}
		}
		if (flag != 1) { //tak to slovo je na juhu
			for (skumany_r = *h_riadok; skumany_r - *h_riadok < dlzka_slova; skumany_r++) {
				tajnicka[skumany_r][skumany_s] = tolower(tajnicka[skumany_r][skumany_s]);
			}
			vyskyt_slova += 1;
			flag = 1;
		}
	}

	//vychod
	flag = 0, cislo_pismenka = 0;
	skumany_r = *h_riadok;
	while (flag != 1) {
		for (skumany_s = *h_stlpec; skumany_s - *h_stlpec < dlzka_slova; skumany_s++) {
			if ((skumany_s < *stlpce && skumany_r >= 0 && cislo_pismenka < dlzka_slova) && (tajnicka[skumany_r][skumany_s] == slovo[cislo_pismenka] || tajnicka[skumany_r][skumany_s] == tolower(slovo[cislo_pismenka]))) {
					cislo_pismenka++;
			}
			else {
				flag = 1;
				break;
			}
		}
		if (flag != 1) { //tak to slovo je na vychode
			for (skumany_s = *h_stlpec; skumany_s - *h_stlpec < dlzka_slova; skumany_s++) {
				tajnicka[skumany_r][skumany_s] = tolower(tajnicka[skumany_r][skumany_s]);
			}
			vyskyt_slova += 1;
			flag = 1;
		}
	}

	//zapad
	flag = 0, cislo_pismenka = 0;
	skumany_r = *h_riadok;
	while (flag != 1) {
		for (skumany_s = *h_stlpec; skumany_s > * h_stlpec - dlzka_slova; skumany_s--) {
			if ((skumany_s < *stlpce && skumany_r >= 0 && cislo_pismenka < dlzka_slova) && (tajnicka[skumany_r][skumany_s] == slovo[cislo_pismenka] || tajnicka[skumany_r][skumany_s] == tolower(slovo[cislo_pismenka]))) {
					cislo_pismenka++;
			}
			else {
				flag = 1;
				break;
			}
		}
		if (flag != 1) { //tak to slovo je na zapade
			for (skumany_s = *h_stlpec; skumany_s > * h_stlpec - dlzka_slova; skumany_s--) {
				tajnicka[skumany_r][skumany_s] = tolower(tajnicka[skumany_r][skumany_s]);
			}
			vyskyt_slova += 1;
			flag = 1;
		}
	}

	//SeveroVychod
	flag = 0, cislo_pismenka = 0;
	skumany_s = *h_stlpec;
	skumany_r = *h_riadok;
	while (flag != 1) {
		for (int diag = 0; diag < dlzka_slova; diag++) {
			if ((skumany_r - diag >= 0 && skumany_s - diag < *stlpce) && (tajnicka[skumany_r - diag][skumany_s + diag] == slovo[cislo_pismenka] || tajnicka[skumany_r - diag][skumany_s + diag] == tolower(slovo[cislo_pismenka]))) {
					cislo_pismenka++;
			}
			else {
				flag = 1;
				break;
			}
		}
		if (flag != 1) { //tak to slovo je na SV
			for (int diag = 0; diag < dlzka_slova; diag++) {
				tajnicka[skumany_r - diag][skumany_s + diag] = tolower(tajnicka[skumany_r - diag][skumany_s + diag]);
			}
			vyskyt_slova += 1;
			flag = 1;
		}
	}

	//SeveroZapad
	flag = 0, cislo_pismenka = 0;
	skumany_s = *h_stlpec;
	skumany_r = *h_riadok;
	while (flag != 1) {
		for (int diag = 0; diag < dlzka_slova; diag++) {
			if ((skumany_r - diag >= 0 && skumany_s - diag >= 0) && (tajnicka[skumany_r - diag][skumany_s - diag] == slovo[cislo_pismenka] || tajnicka[skumany_r - diag][skumany_s - diag] == tolower(slovo[cislo_pismenka]))) {
					cislo_pismenka++;
			}
			else {
				flag = 1;
				break;
			}
		}
		if (flag != 1) { //tak to slovo je na SZ
			for (int diag = 0; diag < dlzka_slova; diag++) {
				tajnicka[skumany_r - diag][skumany_s - diag] = tolower(tajnicka[skumany_r - diag][skumany_s - diag]);
			}
			vyskyt_slova += 1;
			flag = 1;
		}
	}

	//JuhoZapad
	flag = 0, cislo_pismenka = 0;
	skumany_s = *h_stlpec;
	skumany_r = *h_riadok;
	while (flag != 1) {
		for (int diag = 0; diag < dlzka_slova; diag++) {
			if ((skumany_r + diag < *riadky && skumany_s - diag >= 0) && (tajnicka[skumany_r + diag][skumany_s - diag] == slovo[cislo_pismenka] || tajnicka[skumany_r + diag][skumany_s - diag] == tolower(slovo[cislo_pismenka]))) {
					cislo_pismenka++;
			}
			else {
				flag = 1;
				break;
			}
		}
		if (flag != 1) { //tak to slovo je na JZ
			for (int diag = 0; diag < dlzka_slova; diag++) {
				tajnicka[skumany_r + diag][skumany_s - diag] = tolower(tajnicka[skumany_r + diag][skumany_s - diag]);
			}
			vyskyt_slova += 1;
			flag = 1;
		}
	}

	//JuhoVychod
	flag = 0, cislo_pismenka = 0;
	skumany_s = *h_stlpec;
	skumany_r = *h_riadok;
	while (flag != 1) {
		for (int diag = 0; diag < dlzka_slova; diag++) {
			if ((skumany_r + diag < *riadky && skumany_s + diag < *stlpce) && (tajnicka[skumany_r + diag][skumany_s + diag] == slovo[cislo_pismenka] || tajnicka[skumany_r + diag][skumany_s + diag] == tolower(slovo[cislo_pismenka]))) {
					cislo_pismenka++;
			}
			else {
				flag = 1;
				break;
			}
		}
		if (flag != 1) { //tak to slovo je na JV
			for (int diag = 0; diag < dlzka_slova; diag++) {
				tajnicka[skumany_r + diag][skumany_s + diag] = tolower(tajnicka[skumany_r + diag][skumany_s + diag]);
			}
			vyskyt_slova += 1;
			flag = 1;
		}
	}
	
	if (vyskyt_slova == 0) 
		return 0;
	
	else if (vyskyt_slova > 0)
		return 2;
	
	return 0;
}

void vypis_tajnicku(int** tajnicka, int* riadky, int* stlpce) {
	//funckie po vysrkani vsetkych slov vypise spravu z tajnicky
	int flag = 0;
	for (int i_riadok = 0; i_riadok < *riadky; i_riadok++) {
		for (int i_stlpec = 0; i_stlpec < *stlpce; i_stlpec++) {
			if ( tajnicka[i_riadok][i_stlpec] >= 'A' && tajnicka[i_riadok][i_stlpec] <= 'Z' ) {
				printf("%c", tajnicka[i_riadok][i_stlpec]);
				flag = 1;
			}
		}
	}
	printf("\n");
	if (flag == 0) {
		printf("Tajnicka je prazdna \n");
	}
}

int ukonci_program(int** tajnicka, int* indexy[], FILE* fr, int* riadky, int* dlzka_stlpcov_indexy_alock) {

	//uvolnennie 2d pola tajnicka
	for (int i = 0; i < *riadky; i++) {
		if (tajnicka[i] != NULL)
			free(tajnicka[i]);
	}
	free(tajnicka);

	//tu je problem s uvolnenim
	for (int i = 0; i < 26; i++) {
		if(indexy[i] != NULL)
			free(indexy[i]);
	}
	indexy = NULL;
	free(indexy);

	//zatvorenie súboru
	if (fclose(fr) == EOF) {
		printf("Subor sa nepodarilo zatvorit\n");
		return 0;
	}
	return 0;
}


int main() {

	//otvorenie suboru
	FILE* fr;
	if ((fr = fopen(SUBOR, "r+")) == NULL) {
		printf("Nepodarilo sa otvorit subor\n");
		return 0;
	}

	//nacitanie prvych dvoch cisel zo suboru, teda riadky a stlpce ktore bude mat tajnicka
	int riadky = 0, stlpce = 0;
	fscanf(fr, "%d %d ", &riadky, &stlpce);

	//vytvorenie 2d dynamickeho pola TAJNICKA podla riadkov a stlpcov
	int** tajnicka = { 0 };
	tajnicka = (int**)calloc(riadky, sizeof(int*)); //tu si urobím riadky
	for (int i = 0; i < riadky; i++) { //tu prechádzam riadkami aby som do nich mohol robi ståpce
		tajnicka[i] = (int*)calloc(stlpce, sizeof(int)); //tu do každého riadku vytvorím stlpce
	}

	nacitaj_tajnicku_zo_suboru(fr, &riadky, &stlpce, tajnicka);

	int dlzka_stlpcov_indexy_alock[26] = { 0 }; //staticke pole pre alockute dlzky pola indexy
	int dlzka_stlpcov_indexy_realpocet[26] = { 0 }; //staticke pole pre pocet realne zapisanych veci v pole indexy
	int* indexy[26] = { 0 }; //staticke pole ukazovatelov pre vyskyt vsetkych pismen v 2d poli 
	for (int index = 0; index < 26; index++) {
		if ((indexy[index] = (int*)calloc((N + 1), sizeof(int))) == NULL) {
			printf("Error! memory not allocated.");
			exit(0);
		}
		//zapisanie zaciatocnych dlzok do pola dlzka_indexy
		dlzka_stlpcov_indexy_alock[index] = N + 1;
	}

	//funckia zapise vyskyty pismen do pola indexy
	zapis_indexy(&riadky, &stlpce, tajnicka, indexy, dlzka_stlpcov_indexy_alock, dlzka_stlpcov_indexy_realpocet); 

	nacitaj_slovo(fr, &riadky, &stlpce, tajnicka, indexy, dlzka_stlpcov_indexy_realpocet);
	//medzitym je este volana funkcia na lustenie 4 a 8 smerovky ale ta je volana vo funkcii nacitaj_slovo

	vypis_tajnicku(tajnicka, &riadky, &stlpce);

	//uvolni alokovanu pamat a zavrie subor
	ukonci_program(tajnicka, indexy, fr, &riadky, dlzka_stlpcov_indexy_alock);

	return 0;
}
