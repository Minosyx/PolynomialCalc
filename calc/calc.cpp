#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void freeArray(int* tab) {
	free(tab);
}

int* realArray(int* w, int su) { // realokacja pamieci
	int* tmp;
	tmp = (int*)realloc(w, su * sizeof(int));
	if (tmp == NULL)
	{
		exit(-1);
	}
	else
	{
		w = tmp;
	}
	return w;
}

int* createArray(int su) {
	int* tab;
	tab = (int*)malloc(sizeof(int) * su);
	return tab;
}

int* saveToReg(int* w, int* sw, int* reg, int* sreg) { // zapis rejestru w do jednego z dostepnych rejestrow badz odwrotnie
	reg = realArray(reg, *sw + 1);
	*sreg = *sw;
	
	memcpy(reg, w, sizeof(int) * (*sreg + 1));

	return reg;
}

void printOptions() {
	printf("\nWybierz: 0 + - * / %% > < ?\n");
}

void printArray(int* w, int* sw) { // funkcja wyswietlajaca wielomian przechowywany w rejestrze
	int switcher = 0;
	int i;
	for (i = *sw; i >= 0; i--) {
		if (i < *sw && w[i] > 0) printf("  +  ");
		else if (i < *sw && w[i] < 0) printf("  -  ");
		if (i > 0 && w[i] > 0) {
			printf("%dX^%d", w[i], i);
			switcher = 1;
		}
		else if (i == *sw && w[i] < 0 && *sw > 0) {
			printf("%dX^%d", w[i], i);
			switcher = 1;
		}
		else if (i > 0 && i < *sw && w[i] < 0) {
			printf("%dX^%d", -1 * w[i], i);
			switcher = 1;
		}
		if ((i == 0 && w[i] > 0) || (i == 0 && switcher == 0)) {
			printf("%d", w[i]);
		}
		else if (i == 0 && w[i] < 0) {
			printf("%d", -1 * w[i]);
		}
	}
	printf("\n");
}

void showRegs(int* a, int* sa, int* b, int* sb, int* c, int* sc, int* d, int* sd) { // wyswietlanie obecnego stanu rejestrow a, b, c, d
	system("CLS");
	printf("	Rejestr a(x): ");
	printArray(a, sa);
	printf("	Rejestr b(x): ");
	printArray(b, sb);
	printf("	Rejestr c(x): ");
	printArray(c, sc);
	printf("	Rejestr d(x): ");
	printArray(d, sd);
	printf("\n");
}

int* zero(int* w, int* sw) { // zerowanie rejestru
	*sw = 0;
	w = realArray(w, *sw + 1);
	w[0] = 0;

	return w;
}

int* c_frac(int* w, int* sw, int p, int modSwitch, int* r, int* sr) { // implementacja dzielenia pisemnego
	int i, j, scalar = -1, k;
	int su;
	int* u;
	int check;
	int last = -1;
	int zeros = 0;

	int* outcome, sout;
	int* tmp;

	if (sr == NULL) {
		printf("Podaj stopien wielomianu u(x): ");
		scanf("%d", &su);

		u = createArray(su + 1);
		tmp = createArray(*sw + 1);
		memcpy(tmp, w, sizeof(int) * (*sw + 1));

		outcome = createArray(*sw - su + 1);
		sout = *sw - su;

		for (i = su; i >= 0; i--) {
			printf("Wspolczynnik u[%d]: ", i);
			scanf("%d", &u[i]);
			if (u[i] == 0) zeros++;
		}
		if (zeros == su + 1) {
			system("CLS");
			printf("	Nie mozna dzielic przez 0!!!\n"); // wykluczam przypadek dzielenia przez 0
			freeArray(u);
			freeArray(outcome);
			freeArray(tmp);
			return w;
		}

		if (*sw == 0 && w[0] == 0) {
			return w;
		}

		if (*sw < su) {
			printf("Dzielenie nie jest wykonalne dla w < u!\n"); // podzielenie wielomianu mniejszego przez wiekszy jest niemozliwe, poniewaz operujemy na tablicach intow
			freeArray(u);
			return w;
		}

		if (p) {
			for (i = *sw; i >= su; i--) {
				j = 0;
				while (j < p) {
					check = (u[su] * j) % p; // szukam odpowiednich wspolczynnikow, tak jak w dzieleniu pisemnym
					if (check == tmp[i]) {
						scalar = j;
						outcome[i - su] = scalar; // znaleziony wspolczynnik zapisuje do tablicy outcome
						break;
					}
					j++;
				}
				if (scalar == -1) {
					printf("	Dzialanie niewykonalne!!!\n");
					freeArray(u);
					freeArray(outcome);
					freeArray(tmp);
					return w;
				}
				for (k = su; k >= 0; k--) { // dokonuje mnozenia przez znaleziony wspolczynnik i odejmuje od wielomianu, ktory dzielimy
					tmp[i - (su - k)] = (tmp[i - (su - k)] - scalar * u[k]) % p; // wstawiam reszte w odpowiednie miejsce w wielomianie ktory dzielimy, by moc kontynuowac dzielenie
					if (tmp[i - (su - k)] < 0) tmp[i - (su - k)] += p;
				}
			}

			if (modSwitch == 0) { // wywolano funkcje jako dzielenie (modSwitch == 0)
				system("CLS");
				for (i = su; i >= 0; i--) {
					if (tmp[i] != 0) { // sprawdzam czy zostaje reszta z dzielenia 
						printf("	UWAGA! Z dzielenia pozostaje reszta!!!\n");
						break;
						//return w; //można zwrócić rejestr jeżeli nie obługujemy dzielenia z resztą
					}
				}
				w = realArray(w, sout + 1);
				memcpy(w, outcome, sizeof(int) * (sout + 1));
				*sw = sout;
			}

			if (modSwitch == 1) { // wywolano funkcje jako funkcje modulo (modSwitch == 1)
				for (i = su; i >= 0; i--) {
					if (tmp[i] != 0) { // sprawdzam czy zostaje jakas reszta z dzielenia
						last = i;
						break;
					}
					else {
						last = -1;
					}
				}
				if (last != -1) {
					system("CLS");
					memcpy(w, tmp, sizeof(int) * (*sw + 1));
					w = realArray(w, last + 1); // odpowiednio realokuje pamiec
					*sw = last;
				}
				else {
					system("CLS");
					printf("	Wielomian jest podzielny bez reszty!\n	W rejestrze pozostaje wprowadzony wczesniej wielomian!\n");
					freeArray(u);
					freeArray(outcome);
					freeArray(tmp);
					return w;
				}

			}
		}
		if (!p) {
			for (i = *sw; i >= su; i--) {
				scalar = tmp[i] / u[su]; // szukam odpowiednich wspolczynnikow, tak jak w dzieleniu pisemnym
				if (scalar * u[su] != tmp[i]) {
					printf("	Dzialanie niewykonalne!!!\n");
					freeArray(u);
					freeArray(outcome);
					freeArray(tmp);
					return w;
				}
				else outcome[i - su] = scalar;
				for (k = su; k >= 0; k--) { // dokonuje mnozenia przez znaleziony wspolczynnik i odejmuje od wielomianu, ktory dzielimy
					tmp[i - (su - k)] = (tmp[i - (su - k)] - scalar * u[k]); // wstawiam reszte w odpowiednie miejsce w wielomianie ktory dzielimy, by moc kontynuowac dzielenie
				}
			}

			if (modSwitch == 0) { // wywolano funkcje jako dzielenie (modSwitch == 0)
				system("CLS");
				for (i = su; i >= 0; i--) {
					if (tmp[i] != 0) { // sprawdzam czy zostaje reszta z dzielenia 
						printf("	UWAGA! Z dzielenia pozostaje reszta!!!\n");
						break;
						//return w; //można zwrócić rejestr jeżeli nie obługujemy dzielenia z resztą
					}
				}
				w = realArray(w, sout + 1);
				memcpy(w, outcome, sizeof(int) * (sout + 1));
				*sw = sout;
			}

			if (modSwitch == 1) { // wywolano funkcje jako funkcje modulo (modSwitch == 1)
				for (i = su; i >= 0; i--) {
					if (tmp[i] != 0) { // sprawdzam czy zostaje jakas reszta z dzielenia
						last = i;
						break;
					}
					else {
						last = -1;
					}
				}
				if (last != -1) {
					system("CLS");
					memcpy(w, tmp, sizeof(int) * (*sw + 1));
					w = realArray(w, last + 1); // odpowiednio realokuje pamiec
					*sw = last;
				}
				else {
					system("CLS");
					printf("	Wielomian jest podzielny bez reszty!\n	W rejestrze pozostaje wprowadzony wczesniej wielomian!\n");
					freeArray(u);
					freeArray(outcome);
					freeArray(tmp);
					return w;
				}

			}
		}
		freeArray(u);
	}
	else {
		
		tmp = createArray(*sw + 1);
		memcpy(tmp, w, sizeof(int) * (*sw + 1));

		outcome = createArray(*sw - *sr + 1);
		sout = *sw - *sr;

		for (i = *sr; i >= 0; i--) {
			if (r[i] == 0) zeros++;
		}
		if (zeros == *sr + 1) {
			system("CLS");
			printf("	Nie mozna dzielic przez 0!!!\n"); // wykluczam przypadek dzielenia przez 0
			return w;
		}

		if (*sw == 0 && w[0] == 0) {
			return w;
		}

		if (*sw < *sr) {
			printf("Dzielenie nie jest wykonalne dla w < u!\n"); // podzielenie wielomianu mniejszego przez wiekszy jest niemozliwe, poniewaz operujemy na tablicach intow
			return w;
		}

		if (p) {
			for (i = *sw; i >= *sr; i--) {
				j = 0;
				while (j < p) {
					check = (r[*sr] * j) % p; // szukam odpowiednich wspolczynnikow, tak jak w dzieleniu pisemnym
					if (check == tmp[i]) {
						scalar = j;
						outcome[i - *sr] = scalar; // znaleziony wspolczynnik zapisuje do tablicy outcome
						break;
					}
					j++;
				}
				if (scalar == -1) {
					printf("	Dzialanie niewykonalne!!!\n");
					return w;
				}
				for (k = *sr; k >= 0; k--) { // dokonuje mnozenia przez znaleziony wspolczynnik i odejmuje od wielomianu, ktory dzielimy
					tmp[i - (*sr - k)] = (tmp[i - (*sr - k)] - scalar * r[k]) % p; // wstawiam reszte w odpowiednie miejsce w wielomianie ktory dzielimy, by moc kontynuowac dzielenie
					if (tmp[i - (*sr - k)] < 0) tmp[i - (*sr - k)] += p;
				}
			}

			if (modSwitch == 0) { // wywolano funkcje jako dzielenie (modSwitch == 0)
				system("CLS");
				for (i = *sr; i >= 0; i--) {
					if (tmp[i] != 0) { // sprawdzam czy zostaje reszta z dzielenia 
						printf("	UWAGA! Z dzielenia pozostaje reszta!!!\n");
						break;
						//return w; //można zwrócić rejestr jeżeli nie obługujemy dzielenia z resztą
					}
				}
				w = realArray(w, sout + 1);
				memcpy(w, outcome, sizeof(int) * (sout + 1));
				*sw = sout;
			}

			if (modSwitch == 1) { // wywolano funkcje jako funkcje modulo (modSwitch == 1)
				for (i = *sr; i >= 0; i--) {
					if (tmp[i] != 0) { // sprawdzam czy zostaje jakas reszta z dzielenia
						last = i;
						break;
					}
					else {
						last = -1;
					}
				}
				if (last != -1) {
					system("CLS");
					memcpy(w, tmp, sizeof(int) * (*sw + 1));
					w = realArray(w, last + 1); // odpowiednio realokuje pamiec
					*sw = last;
				}
				else {
					system("CLS");
					printf("	Wielomian jest podzielny bez reszty!\n	W rejestrze pozostaje wprowadzony wczesniej wielomian!\n");
					freeArray(outcome);
					freeArray(tmp);
					return w;
				}
			}
		}
		if (!p) {
			for (i = *sw; i >= *sr; i--) {
				scalar = tmp[i] / r[*sr]; // szukam odpowiednich wspolczynnikow, tak jak w dzieleniu pisemnym
				if (scalar * r[*sr] != tmp[i]) {
					printf("	Dzialanie niewykonalne!!!\n");
					freeArray(outcome);
					freeArray(tmp);
					return w;
				}
				else outcome[i - *sr] = scalar;
				for (k = *sr; k >= 0; k--) { // dokonuje mnozenia przez znaleziony wspolczynnik i odejmuje od wielomianu, ktory dzielimy
					tmp[i - (*sr - k)] = (tmp[i - (*sr - k)] - scalar * r[k]); // wstawiam reszte w odpowiednie miejsce w wielomianie ktory dzielimy, by moc kontynuowac dzielenie
				}
			}

			if (modSwitch == 0) { // wywolano funkcje jako dzielenie (modSwitch == 0)
				system("CLS");
				for (i = *sr; i >= 0; i--) {
					if (tmp[i] != 0) { // sprawdzam czy zostaje reszta z dzielenia 
						printf("	UWAGA! Z dzielenia pozostaje reszta!!!\n");
						break;
						//return w; //można zwrócić rejestr jeżeli nie obługujemy dzielenia z resztą
					}
				}
				w = realArray(w, sout + 1);
				memcpy(w, outcome, sizeof(int) * (sout + 1));
				*sw = sout;
			}

			if (modSwitch == 1) { // wywolano funkcje jako funkcje modulo (modSwitch == 1)
				for (i = *sr; i >= 0; i--) {
					if (tmp[i] != 0) { // sprawdzam czy zostaje jakas reszta z dzielenia
						last = i;
						break;
					}
					else {
						last = -1;
					}
				}
				if (last != -1) {
					system("CLS");
					memcpy(w, tmp, sizeof(int) * (*sw + 1));
					w = realArray(w, last + 1); // odpowiednio realokuje pamiec
					*sw = last;
				}
				else {
					system("CLS");
					printf("	Wielomian jest podzielny bez reszty!\n	W rejestrze pozostaje wprowadzony wczesniej wielomian!\n");
					freeArray(outcome);
					freeArray(tmp);
					return w;
				}

			}
		}
	}
	freeArray(outcome);
	freeArray(tmp);
	return w;
}

int* c_mul(int* w, int* sw, int p, int* r, int* sr) { // implementacja standardowego mnozenia wielomianow, zmieniamy ilosc alokowanej pamieci dla rejestru w
	int i, j;
	int su;
	int* u;
	int* tmp;
	int stmp;
	int insert;
	int size = -1;
	if (sr == NULL) {
		printf("Podaj stopien wielomianu u(x): ");
		scanf("%d", &su);

		u = createArray(su + 1);
		tmp = createArray(su + *sw + 1);
		stmp = su + *sw;

		memset(tmp, 0, sizeof(int)*(stmp + 1));

		for (i = su; i >= 0; i--) {
			j = *sw;
			printf("Wspolczynnik u[%d]: ", i);
			scanf("%d", &u[i]);
			for (j = *sw; j >= 0; j--) {
				insert = i + j;
				tmp[insert] += w[j] * u[i];
			}
		}

		if (p) {
			for (i = stmp; i >= 0; i--) {
				tmp[i] %= p;
				if (tmp[i] < 0) tmp[i] += p;
			}
		}

		if (su != 0 || *sw != 0) {
			w = realArray(w, stmp + 1);
		}
		freeArray(u);
	}
	else {
		tmp = createArray(*sr + *sw + 1);
		stmp = *sr + *sw;

		memset(tmp, 0, sizeof(int) * (stmp + 1));

		for (i = *sr; i >= 0; i--) {
			j = *sw;
			for (j = *sw; j >= 0; j--) {
				insert = i + j;
				tmp[insert] += w[j] * r[i];
			}
		}

		if (p) {
			for (i = stmp; i >= 0; i--) {
				tmp[i] %= p;
				if (tmp[i] < 0) tmp[i] += p;
			}
		}

		if (*sr != 0 || *sw != 0) {
			w = realArray(w, stmp + 1);
			
		}
	
	}
	memcpy(w, tmp, sizeof(int) * (stmp + 1));
	*sw = stmp;
	for (i = *sw; i >= 0; i--) {
		if (w[i] != 0) size = i;
	}
	if (size == -1) {
		w = realArray(w, 1);
		*sw = 0;
	}
	freeArray(tmp);
	return w;
}

int* c_sub(int* w, int* sw, int p, int* r, int* sr) { // odejmujemy odpowiednie komorki tablic, w razie potrzeby zmieniamy ilosc alokowanej pamieci dla rejestru w
	int i;
	int su;
	int* u;
	if (sr == NULL) {
		printf("Podaj stopien wielomianu u(x): ");
		scanf("%d", &su);

		u = createArray(su + 1);

		if (su > * sw) {
			w = realArray(w, su + 1);
		}
		for (i = su; i >= 0; i--) {
			if (i > * sw) {
				w[i] = 0;
			}
			printf("Wspolczynnik u[%d]: ", i);
			scanf("%d", &u[i]);
			if (p) {
				w[i] = (w[i] - u[i]) % p;
				if (w[i] < 0) w[i] += p;
			}
			else {
				w[i] -= u[i];
			}

		}
		if (*sw < su) *sw = su;
		freeArray(u);
	}
	else {
		if (*sr > * sw) {
			w = realArray(w, *sr + 1);
		}
		for (i = *sr; i >= 0; i--) {
			if (i > * sw) {
				w[i] = 0;
			}
			if (p) {
				w[i] = (w[i] - r[i]) % p;
				if (w[i] < 0) w[i] += p;
			}
			else {
				w[i] -= r[i];
			}

		}
		if (*sw < *sr) *sw = *sr;
	}
	return w;
}

int* c_sum(int* w, int* sw, int p, int* r, int* sr) { // dodajemy odpowiednie komorki tablic, w razie potrzeby zmieniamy ilosc alokowanej pamieci dla rejestru w
	int i;
	int su;
	int* u;
	if (sr == NULL) {
		printf("Podaj stopien wielomianu u(x): ");
		scanf("%d", &su);

		u = createArray(su + 1);

		if (su > * sw) {
			w = realArray(w, su + 1);
		}
		for (i = su; i >= 0; i--) {
			if (i > * sw) {
				w[i] = 0;
			}
			printf("Wspolczynnik u[%d]: ", i);
			scanf("%d", &u[i]);
			if (p) {
				w[i] = (w[i] + u[i]) % p;
				if (w[i] < 0) w[i] += p;
			}
			else {
				w[i] += u[i];
			}

		}
		if (*sw < su) *sw = su;
		freeArray(u);
	}
	else {
		if (*sr > * sw) {
			w = realArray(w, *sr + 1);
		}
		for (i = *sr; i >= 0; i--) {
			if (i > * sw) {
				w[i] = 0;
			}
			if (p) {
				w[i] = (w[i] + r[i]) % p;
				if (w[i] < 0) w[i] += p;
			}
			else {
				w[i] += r[i];
			}

		}
		if (*sw < *sr) *sw = *sr;
	}
	return w;
}

int main()
{
	int p;
	char choice[2];
	int* w, sw = 0;
	int is_active = 1;
	int modSwitch = 0;

	int* a, sa = 0;
	int* b, sb = 0;
	int* c, sc = 0;
	int* d, sd = 0;

	a = createArray(sa + 1);
	if (a == NULL)
		exit(-1);
	a[0] = 0;
	b = createArray(sb + 1);
	if (b == NULL)
		exit(-1);
	b[0] = 0;
	c = createArray(sc + 1);
	if (c == NULL)
		exit(-1);
	c[0] = 0;
	d = createArray(sd + 1);
	if (d == NULL)
		exit(-1);
	d[0] = 0;

	printf("Dostepne rejestry (a, b, c, d)\n");
	printf("Podaj podstawe modularna (0 = brak mod): ");
	scanf("%d", &p);

	w = createArray(sw + 1);
	if (w == NULL)
		exit(-1);
	w[0] = 0;
	
	while (is_active) { // menu glowne
		printOptions();
		scanf("%s", &choice);
		switch (choice[0])
		{
		case '0':
			if (choice[1] == 'a') a = zero(a, &sa);
			else if (choice[1] == 'b') b = zero(b, &sb);
			else if (choice[1] == 'c') c = zero(c, &sc);
			else if (choice[1] == 'd') d = zero(d, &sd);
			else w = zero(w, &sw);
			break;
		case '+':
			if (choice[1] == 'a') w = c_sum(w, &sw, p, a, &sa);
			else if (choice[1] == 'b') w = c_sum(w, &sw, p, b, &sb);
			else if (choice[1] == 'c') w = c_sum(w, &sw, p, c, &sc);
			else if (choice[1] == 'd') w = c_sum(w, &sw, p, d, &sd);
			else w = c_sum(w, &sw, p, NULL, NULL);
			break;
		case '-':
			if (choice[1] == 'a') w = c_sub(w, &sw, p, a, &sa);
			else if (choice[1] == 'b') w = c_sub(w, &sw, p, b, &sb);
			else if (choice[1] == 'c') w = c_sub(w, &sw, p, c, &sc);
			else if (choice[1] == 'd') w = c_sub(w, &sw, p, d, &sd);
			else w = c_sub(w, &sw, p, NULL, NULL);
			break;
		case '*':
			if (choice[1] == 'a') w = c_mul(w, &sw, p, a, &sa);
			else if (choice[1] == 'b') w = c_mul(w, &sw, p, b, &sb);
			else if (choice[1] == 'c') w = c_mul(w, &sw, p, c, &sc);
			else if (choice[1] == 'd') w = c_mul(w, &sw, p, d, &sd);
			else w = c_mul(w, &sw, p, NULL, NULL);
			break;
		case '/':
			modSwitch = 0;
			if (choice[1] == 'a') w = c_frac(w, &sw, p, modSwitch, a, &sa);
			else if (choice[1] == 'b') w = c_frac(w, &sw, p, modSwitch, b, &sb);
			else if (choice[1] == 'c') w = c_frac(w, &sw, p, modSwitch, c, &sc);
			else if (choice[1] == 'd') w = c_frac(w, &sw, p, modSwitch, d, &sd);
			else w = c_frac(w, &sw, p, modSwitch, NULL, NULL);
			break;
		case '%':
			modSwitch = 1;
			if (choice[1] == 'a') w = c_frac(w, &sw, p, modSwitch, a, &sa);
			else if (choice[1] == 'b') w = c_frac(w, &sw, p, modSwitch, b, &sb);
			else if (choice[1] == 'c') w = c_frac(w, &sw, p, modSwitch, c, &sc);
			else if (choice[1] == 'd') w = c_frac(w, &sw, p, modSwitch, d, &sd);
			else w = c_frac(w, &sw, p, modSwitch, NULL, NULL);
			break;
		case '>':
			system("CLS");
			if (choice[1] == 'a') a = saveToReg(w, &sw, a, &sa);
			else if (choice[1] == 'b') b = saveToReg(w, &sw, b, &sb);
			else if (choice[1] == 'c') c = saveToReg(w, &sw, c, &sc);
			else if (choice[1] == 'd') d = saveToReg(w, &sw, d, &sd);
			else printf("Wybierz poprawny rejestr!\n");
			break;
		case '<':
			system("CLS");
			if (choice[1] == 'a') w = saveToReg(a, &sa, w, &sw);
			else if (choice[1] == 'b') w = saveToReg(b, &sb, w, &sw);
			else if (choice[1] == 'c') w = saveToReg(c, &sc, w, &sw);
			else if (choice[1] == 'd') w = saveToReg(d, &sd, w, &sw);
			else printf("Wybierz poprawny rejestr!\n");
			break;
		case '?':
			showRegs(a, &sa, b, &sb, c, &sc, d, &sd);
			break;
		}
		if (choice[0] != '/' && choice[0] != '%' && choice[0] != '>' && choice[0] != '<' && choice[0] != '?')	system("CLS");
		printf("	Rejestr w(X): ");
		printArray(w, &sw);
		
	}

	freeArray(w);
	freeArray(a);
	freeArray(b);
	freeArray(c);
	freeArray(d);
}