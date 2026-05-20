#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Statie
{
	int idStatie;
	char* nume;
	char* magistrala;
	int nrCalatori;
};
typedef struct Statie Statie;

typedef struct Nod Nod;
typedef struct NodSecundar NodSecundar;

struct Nod
{
	Statie info;
	NodSecundar* vecini;
	Nod* next;
};

struct NodSecundar
{
	NodSecundar* next;
	Nod* nodInfo;
};

Statie citireStatieDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);

	char* aux;
	Statie s;

	aux = strtok(buffer, sep);
	s.idStatie = atoi(aux);

	aux = strtok(NULL, sep);
	s.nume = malloc(strlen(aux) + 1);
	strcpy(s.nume, aux);

	aux = strtok(NULL, sep);
	s.magistrala = malloc(strlen(aux) + 1);
	 strcpy(s.magistrala, aux);

	s.nrCalatori = atoi(strtok(NULL, sep));

	return s;
}

void afisareStatie(Statie s)
{
	printf("Id statie: %d\n", s.idStatie);
	printf("Nume  %s\n", s.nume);
	printf("Magistrala: %s\n", s.magistrala);
	printf("Nr calatori: %d\n\n", s.nrCalatori);
}

void inserareListaPrincipala(Nod** cap, Statie statieNoua)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->info = statieNoua;
	nou->next  = NULL;
	nou->vecini = NULL;

	if (*cap)
	{
		Nod* p = (*cap);

		while (p->next)
		{
			 p = p->next;
		}

		p->next = nou;
	}
	else
	{
		*cap = nou;
	}
}

Nod* getStatieById(Nod* cap, int id)
{
	while (cap && cap->info.idStatie != id)
	{
		 cap = cap->next;
	}

	return cap;
}

void inserareListaSecundara(NodSecundar** cap, Nod* nodInfo)
{
	NodSecundar*nou = malloc(sizeof(NodSecundar));
	nou->nodInfo = nodInfo;
	nou->next = NULL;

	if (*cap)
	{
		NodSecundar* p = *cap;

		while (p->next)
		{
			p = p->next;
		}

		p->next = nou;
	}
	else
	{
		*cap = nou;
	}
}

void inserareMuchie(Nod* graf, int idStart, int idStop)
{
	Nod* nodStart= getStatieById(graf, idStart);
	Nod* nodStop= getStatieById(graf, idStop);

	if (nodStart && nodStop)
	{
		inserareListaSecundara(&(nodStart->vecini), nodStop);
		inserareListaSecundara(&(nodStop->vecini), nodStart);
	}
}

Nod* citireNoduriStatiiDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Nod* cap = NULL;

	if (f)
	{
		while (!feof(f))
		{
			inserareListaPrincipala(&cap, citireStatieDinFisier(f));
		}

		fclose(f);
	}

	return cap;
}

void citireMuchiiDinFisier(Nod* graf, const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");

	if (f)
	{
		int idStart;
		int idStop;

		while (fscanf(f, "%d %d", &idStart, &idStop) == 2)
		{
			 inserareMuchie(graf, idStart, idStop);
		}

		fclose(f);
	}
}

void afisareVeciniStatie(Nod* graf, int id)
{
	Nod* statie = getStatieById(graf, id);

	if (statie)
	{
		NodSecundar* p = statie->vecini;

		while (p)
		{
			afisareStatie(p->nodInfo->info);
			p = p->next;
		}
	}
}

int numarVeciniStatie(Nod* graf, int id)
{
	Nod* statie = getStatieById(graf, id);

	if (statie)
	{
		int nr = 0;
		NodSecundar* p = statie->vecini;

		while (p)
		{
			nr++;
			p = p->next;
		}

		return nr;
	}

	return 0;
}

void afisareStatiiMagistrala(Nod* graf, const char* magistrala)
{
	while (graf)
	{
		if (strcmp(graf->info.magistrala, magistrala) == 0)
		{
			afisareStatie(graf->info);
		}

		graf = graf->next;
	}
}

void dezalocareListaSecundara(NodSecundar** cap)
{
	while (*cap)
	{
		NodSecundar* aux = *cap;
		(*cap) = (*cap)->next;
		free(aux);
	}
}

void dezalocareGraf(Nod** graf)
{
	while (*graf)
	{
		Nod* aux = *graf;
		(*graf) = (*graf)->next;

		dezalocareListaSecundara(&aux->vecini);

		free(aux->info.nume);
		free(aux->info.magistrala);

		free(aux);
	}
}

int main()
{
	Nod* graf = citireNoduriStatiiDinFisier("statii.txt");

	citireMuchiiDinFisier(graf, "conexiuni.txt");

	afisareVeciniStatie(graf, 1);

	printf("Numar vecini statie 1: %d\n\n", numarVeciniStatie(graf, 1));

	afisareStatiiMagistrala(graf, "M1");

	dezalocareGraf(&graf);

	return 0;
}