#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Comanda {
	int id;
	char* numeClient;
	float valoare;
	char urgenta;//D-da N-nu

};typedef struct Comanda Comanda;
typedef struct Nod Nod;
struct Nod {
	Comanda info;
	Nod* next;
	Nod* prev;
};
typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	Nod* prim;
	Nod* ultim;

};
typedef struct NodCoada NodCoada;
struct NodCoada
{
	Comanda info;
	NodCoada* next;

};
struct Coada 
{
	NodCoada* prim;
	NodCoada* ultim;
};typedef struct Coada Coada;

Comanda citireComandaDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Comanda c1;
	aux=strtok(buffer, sep);
	c1.id = atoi(aux);
	aux = strtok(NULL, sep);
	c1.numeClient = malloc(strlen(aux) + 1);
	strcpy(c1.numeClient, aux);
	c1.valoare = atof(strtok(NULL, sep));
	c1.urgenta = *strtok(NULL, sep);

	return c1;
}

void afisareComanezi(Comanda c) 
	{
		printf("id:%d\n", c.id);
		printf("Nume client %s \n", c.numeClient);
		printf("Este urgent %c \n", c.urgenta);
		printf("Valoarea comenzii %5.2f\n\n", c.valoare);
	} 
void afisareListaComenzi(ListaDubla lista)
{
	Nod* nou = lista.prim;
	while (nou)
	{
		afisareComanezi(nou->info);
		nou = nou->next;
	}

}

 void insereazaLaFinal(ListaDubla* ld, Comanda comandaNoua)
{	
	Nod* nou = malloc(sizeof(Nod));
	nou->info = comandaNoua;
	nou->next = NULL;
	nou->prev = ld->ultim;

	if (ld->ultim)
	{
		ld->ultim->next = nou;
	}
	else {
		ld->prim = nou;
	}ld->ultim = nou;

}

ListaDubla citireFisierComenzi(const char* numeFisier) {
	ListaDubla ld;
	ld.prim = NULL;
	ld.ultim = NULL;
	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Comanda c = citireComandaDinFisier(f);
			insereazaLaFinal(&ld, c);
		}
		fclose(f); 
	}
	else {
		printf("eroare la deschidere\n");
	}return ld; 
}


void pushCoada(Coada* c, Comanda comandaNoua)
{
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->info = comandaNoua;
	nou->next = NULL; 

	if (c->ultim != NULL) {
		c->ultim->next = nou;
	}
	else {
		c->prim = nou;
	}
	c->ultim = nou;
}
Comanda popCoada(Coada* c) 
{
	if (c->prim == NULL) {
		Comanda nula;
		nula.id = -1;
		return nula;
	}
	Comanda rezultat = c->prim->info;
	NodCoada* temp = c->prim;
	c->prim = c->prim->next;

	if (c->prim == NULL) {
		c->ultim = NULL;
	}

	free(temp);

	return rezultat;
}

void extrageUrgente(ListaDubla lista, Coada* coadaUrgente)
{
	Nod* nou = lista.prim;
	while (nou != NULL) 
	{
		if (nou->info.urgenta == 'D') {
			pushCoada(coadaUrgente, nou->info);
		}
		nou = nou->next;
	}
}
int main()
{
	ListaDubla listaComenzi = citireFisierComenzi("comenzi.txt");
	afisareListaComenzi(listaComenzi);
	Coada coadaUrgente;
	coadaUrgente.prim = NULL;
	coadaUrgente.ultim = NULL;

	extrageUrgente(listaComenzi, &coadaUrgente);

	printf("extragere din comenziile urgente \n");
	Comanda primaUrgenta = popCoada(&coadaUrgente);

	if (primaUrgenta.id != -1) {
		afisareComanezi(primaUrgenta);
	}
	else {
		printf("Nu exista comenzi urgente.\n");
	}
	return 0;
}