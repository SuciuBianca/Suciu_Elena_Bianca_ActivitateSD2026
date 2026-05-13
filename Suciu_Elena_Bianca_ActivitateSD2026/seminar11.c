#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}
void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//1. 
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara
typedef struct Nod Nod;
typedef struct NodSecundar NodSecundar;
struct Nod
{
	Masina info;
	Nod* next;
	NodSecundar* vecini;
};

struct NodSecundar
{
	Nod* inform;
	NodSecundar* next;
};

//2.
//functii de inserare in liste
//si in principala si in secundara

void inserareListaPrincipala(Nod** graf,Masina m)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->next = NULL;
	nou->vecini = NULL;
	nou->info = m;

	if ((*graf) == NULL)
	{
		(*graf) = nou;
	}
	else
	{
		Nod* p = (*graf);
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = nou;
	}
}

void inserrareListaSecundara(NodSecundar** cap, Nod* vecin)
{
	NodSecundar* nou = malloc(sizeof(NodSecundar));
	nou->next = NULL;
	nou->inform=vecin;

	if ((*cap) == NULL)
	{
		(*cap) = nou;
	}
	else
	{
		NodSecundar* p = (*cap);
		while(p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
}
//3.
//functie de cautarea in lista principala dupa ID
Nod* cautaNodDupaID(Nod* graf, int id)
{
	
	
		while (graf && graf->info.id != id)
		{
			graf = graf->next;
		}
		return graf;
	
}

//4.
//inserare muchie
void inserareMuchie(Nod* graf, int idStart, int idStop) 
{
	Nod* nodStart=cautaNodDupaID(graf,idStart);
	Nod* nodStop=cautaNodDupaID(graf,idStop);
	if (nodStart && nodStop)
	{
		inserrareListaSecundara(&nodStart->vecini, nodStop);
		inserrareListaSecundara(&nodStop->vecini, nodStart);
	}
}

void* citireNoduriMasiniDinFisier(const char* numeFisier) 
{
	FILE* f = fopen(numeFisier, "r");
	Nod* graf = NULL;

	while (!(feof(f)))
	{
		inserareListaPrincipala(&graf, citireMasinaDinFisier(f));
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(Nod*graf,const char* numeFisier) 
{
	FILE* f = fopen(numeFisier, "r");
	int start, stop;
	while (!feof(f))
	{
		fscanf(f, "%d %d", &start, &stop);
		inserareMuchie(graf, start, stop);
	}
	fclose(f);
	return graf;
}

void dezalocareNoduriGraf(void* listaPrincipala) 
{
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
}

void afiseazaListaSec(Nod*graf,int id)
{
	Nod* nou = cautaNodDupaID(graf, id);
	NodSecundar* aux = nou->vecini;

	while (aux)
	{
		afisareMasina(aux->inform->info);
		aux = aux->next;
	}
}

int main() {
	Nod* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");
	afiseazaListaSec(graf, 4);
	return 0;
}