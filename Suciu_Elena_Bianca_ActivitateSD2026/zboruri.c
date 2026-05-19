#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Zbor
{
	int id;
	float durata;
	char* companie;
	float pret; // folosit ca prioritate in HeapMin: zborul cel mai ieftin se extrage primul
	char bagaj; // D,N
};
typedef struct Zbor Zbor;

typedef struct HeapMin HeapMin;
struct HeapMin
{
	int lungime;
	Zbor* vector;
	int nrZboruri;
};

typedef struct NodArbore NodArbore;
struct NodArbore
{
	Zbor info;
	NodArbore* dreapta;
	NodArbore* stanga;
};

typedef struct Nod Nod;
typedef struct NodSecundar NodSecundar;

struct Nod
{
	Zbor info;
	Nod* next;
	NodSecundar* vecini;
};

struct NodSecundar
{
	Nod* inform;
	NodSecundar* next;
};

Zbor citireZborDinFiser(FILE* f)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);

	char* aux;
	Zbor z;

	aux = strtok(buffer, sep);
	z.id = atoi(aux);

	z.durata = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	z.companie = malloc(strlen(aux) + 1);
	strcpy(z.companie, aux);

	z.pret = atof(strtok(NULL, sep));

	z.bagaj = *strtok(NULL, sep);

	return z;
}

void afisareZbor(Zbor z)
{
	printf("ID: %d\n", z.id);
	printf("Durata: %.2f\n", z.durata);
	printf("Companie: %s\n", z.companie);
	printf("Pret: %.2f\n", z.pret);
	printf("Bagaj: %c\n\n", z.bagaj);
}

HeapMin initializareHeap(int lungime)
{
	HeapMin heap;
	heap.lungime = lungime;
	heap.nrZboruri = 0;
	heap.vector = (Zbor*)malloc(sizeof(Zbor) * lungime);
	return heap;
}

void filtrareHeap(HeapMin heap, int pozitieNod)
{
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMin = pozitieNod;

	if (pozFiuSt < heap.nrZboruri && heap.vector[pozMin].pret > heap.vector[pozFiuSt].pret)
	{
		pozMin = pozFiuSt;
	}

	if (pozFiuDr < heap.nrZboruri && heap.vector[pozMin].pret > heap.vector[pozFiuDr].pret)
	{
		pozMin = pozFiuDr;
	}

	if (pozMin != pozitieNod)
	{
		Zbor aux = heap.vector[pozMin];
		heap.vector[pozMin] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if (pozMin <= (heap.nrZboruri - 2) / 2)
		{
			filtrareHeap(heap, pozMin);
		}
	}
}

HeapMin citireHeapDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	HeapMin heap = initializareHeap(7);

	if (file)
	{
		while (!feof(file) && heap.nrZboruri < heap.lungime)
		{
			heap.vector[heap.nrZboruri++] = citireZborDinFiser(file);
		}

		fclose(file);
	}

	for (int i = (heap.nrZboruri - 2) / 2; i >= 0; i--)
	{
		filtrareHeap(heap, i);
	}

	return heap;
}

Zbor extrageZborIeftin(HeapMin* heap)
{
	Zbor aux;
	aux.id = -1;
	aux.companie = NULL;

	if (heap->nrZboruri > 0)
	{
		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrZboruri - 1];
		heap->vector[heap->nrZboruri - 1] = aux;
		heap->nrZboruri--;

		if (heap->nrZboruri > 0)
		{
			filtrareHeap(*heap, 0);
		}
	}

	return aux;
}

void afisareHeapAscuns(HeapMin heap)
{
	for (int i = heap.nrZboruri; i < heap.lungime; i++)
	{
		afisareZbor(heap.vector[i]);
	}
}

void adaugaZborInArbore(NodArbore** root, Zbor zborNou)
{
	if ((*root) != NULL)
	{
		if (zborNou.id < (*root)->info.id)
		{
			adaugaZborInArbore(&(*root)->stanga, zborNou);
		}
		else
		{
			adaugaZborInArbore(&(*root)->dreapta, zborNou);
		}
	}
	else
	{
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->info = zborNou;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		(*root) = nou;
	}
}

NodArbore* citireArboreDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	NodArbore* root = NULL;

	if (file)
	{
		while (!feof(file))
		{
			adaugaZborInArbore(&root, citireZborDinFiser(file));
		}

		fclose(file);
	}

	return root;
}

int nrZboruriCompanie(NodArbore* root, const char* companie)
{
	if (root)
	{
		int nr = 0;

		if (strcmp(root->info.companie, companie) == 0)
		{
			nr = 1;
		}

		return nr
			+ nrZboruriCompanie(root->stanga, companie)
			+ nrZboruriCompanie(root->dreapta, companie);
	}
	else
	{
		return 0;
	}
}

void afisareInordine(NodArbore* root)
{
	if (root)
	{
		afisareInordine(root->stanga);
		afisareZbor(root->info);
		afisareInordine(root->dreapta);
	}
}

Zbor getZborById(NodArbore* root, int id)
{
	Zbor z;
	z.id = -1;
	z.durata = 0;
	z.pret = 0;
	z.bagaj = '-';
	z.companie = NULL;

	if (root)
	{
		if (id < root->info.id)
		{
			return getZborById(root->stanga, id);
		}
		else if (id > root->info.id)
		{
			return getZborById(root->dreapta, id);
		}
		else
		{
			z.id = root->info.id;
			z.durata = root->info.durata;
			z.pret = root->info.pret;
			z.bagaj = root->info.bagaj;

			z.companie = malloc(strlen(root->info.companie) + 1);
			strcpy(z.companie, root->info.companie);
		}
	}

	return z;
}

int calculeazaInaltimea(NodArbore* root)
{
	if (root == NULL)
	{
		return 0;
	}

	int dr = calculeazaInaltimea(root->dreapta);
	int st = calculeazaInaltimea(root->stanga);

	return 1 + (st > dr ? st : dr);
}

void rotireStanga(NodArbore** root)
{
	NodArbore* aux = (*root)->dreapta;
	(*root)->dreapta = aux->stanga;
	aux->stanga = (*root);
	(*root) = aux;
}

void rotireDreapta(NodArbore** root)
{
	NodArbore* aux = (*root)->stanga;
	(*root)->stanga = aux->dreapta;
	aux->dreapta = (*root);
	(*root) = aux;
}

int getGradEchilibru(NodArbore* root)
{
	int dr = calculeazaInaltimea(root->dreapta);
	int st = calculeazaInaltimea(root->stanga);

	return st - dr;
}

void adaugaZborInArboreEchilibrat(NodArbore** root, Zbor zborNou)
{
	if ((*root) != NULL)
	{
		if (zborNou.id > (*root)->info.id)
		{
			adaugaZborInArboreEchilibrat(&(*root)->dreapta, zborNou);
		}
		else
		{
			adaugaZborInArboreEchilibrat(&(*root)->stanga, zborNou);
		}

		int gradEchilibru = getGradEchilibru(*root);

		if (gradEchilibru == -2)
		{
			if (getGradEchilibru((*root)->dreapta) == -1)
			{
				rotireStanga(root);
			}
			else
			{
				rotireDreapta(&(*root)->dreapta);
				rotireStanga(root);
			}
		}

		if (gradEchilibru == 2)
		{
			if (getGradEchilibru((*root)->stanga) == 1)
			{
				rotireDreapta(root);
			}
			else
			{
				rotireStanga(&(*root)->stanga);
				rotireDreapta(root);
			}
		}
	}
	else
	{
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->stanga = NULL;
		nou->dreapta = NULL;
		nou->info = zborNou;
		(*root) = nou;
	}
}

NodArbore* citireAVLZboruriDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	NodArbore* root = NULL;

	if (file)
	{
		while (!feof(file))
		{
			adaugaZborInArboreEchilibrat(&root, citireZborDinFiser(file));
		}

		fclose(file);
	}

	return root;
}

void afisarePreordine(NodArbore* root)
{
	if (root)
	{
		afisareZbor(root->info);
		afisarePreordine(root->stanga);
		afisarePreordine(root->dreapta);
	}
}

void inserareListaPrincipala(Nod** graf, Zbor z)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->next = NULL;
	nou->vecini = NULL;
	nou->info = z;

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

Nod* cautaNodDupaID(Nod* graf, int id)
{
	while (graf && graf->info.id != id)
	{
		graf = graf->next;
	}

	return graf;
}

void inserareListaSecundara(NodSecundar** cap, Nod* vecin)
{
	NodSecundar* nou = malloc(sizeof(NodSecundar));
	nou->next = NULL;
	nou->inform = vecin;

	if ((*cap) == NULL)
	{
		(*cap) = nou;
	}
	else
	{
		NodSecundar* p = (*cap);

		while (p->next)
		{
			p = p->next;
		}

		p->next = nou;
	}
}

void inserareMuchie(Nod* graf, int idStart, int idStop)
{
	Nod* nodStart = cautaNodDupaID(graf, idStart);
	Nod* nodStop = cautaNodDupaID(graf, idStop);

	if (nodStart && nodStop)
	{
		inserareListaSecundara(&nodStart->vecini, nodStop);
		inserareListaSecundara(&nodStop->vecini, nodStart);
	}
}

Nod* citireNoduriZboruriDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Nod* graf = NULL;

	if (f)
	{
		while (!feof(f))
		{
			inserareListaPrincipala(&graf, citireZborDinFiser(f));
		}

		fclose(f);
	}

	return graf;
}

void citireMuchiiDinFisier(Nod* graf, const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	int start, stop;

	if (f)
	{
		while (!feof(f))
		{
			fscanf(f, "%d %d", &start, &stop);
			inserareMuchie(graf, start, stop);
		}

		fclose(f);
	}
}

void afiseazaVeciniZbor(Nod* graf, int id)
{
	Nod* nod = cautaNodDupaID(graf, id);

	if (nod)
	{
		NodSecundar* aux = nod->vecini;

		while (aux)
		{
			afisareZbor(aux->inform->info);
			aux = aux->next;
		}
	}
}

void dezalocareZbor(Zbor* z)
{
	if (z->companie != NULL)
	{
		free(z->companie);
		z->companie = NULL;
	}
}

void dezalocareArbore(NodArbore** root)
{
	if (*root)
	{
		dezalocareArbore(&(*root)->stanga);
		dezalocareArbore(&(*root)->dreapta);

		dezalocareZbor(&(*root)->info);
		free(*root);
		*root = NULL;
	}
}

void dezalocareHeap(HeapMin* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		dezalocareZbor(&heap->vector[i]);
	}

	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrZboruri = 0;
}

void dezalocareListaSecundara(NodSecundar** cap)
{
	while (*cap)
	{
		NodSecundar* p = (*cap);
		(*cap) = (*cap)->next;
		free(p);
	}
}

void dezalocareListaPrincipala(Nod** graf)
{
	while (*graf)
	{
		Nod* p = (*graf);
		(*graf) = (*graf)->next;

		dezalocareListaSecundara(&(p->vecini));
		dezalocareZbor(&p->info);
		free(p);
	}
}

int main()
{
	HeapMin heap = citireHeapDinFisier("zboruri.txt");

	printf("EXTRAGERI HEAP:\n");
	afisareZbor(extrageZborIeftin(&heap));
	afisareZbor(extrageZborIeftin(&heap));
	afisareZbor(extrageZborIeftin(&heap));

	printf("HEAP ASCUNS:\n");
	afisareHeapAscuns(heap);

	NodArbore* rootABC = citireArboreDinFisier("zboruri.txt");

	printf("ZBORURI INORDINE ABC:\n");
	afisareInordine(rootABC);

	printf("Nr zboruri Tarom: %d\n\n", nrZboruriCompanie(rootABC, "Tarom"));

	Zbor z = getZborById(rootABC, 10);

	printf("ZBOR CAUTAT DUPA ID:\n");
	if (z.id != -1)
	{
		afisareZbor(z);
		dezalocareZbor(&z);
	}
	else
	{
		printf("Zborul nu a fost gasit.\n\n");
	}

	NodArbore* rootAVL = citireAVLZboruriDinFisier("zboruri.txt");

	printf("ZBORURI PREORDINE AVL:\n");
	afisarePreordine(rootAVL);

	Nod* graf = citireNoduriZboruriDinFisier("zboruri.txt");
	citireMuchiiDinFisier(graf, "muchii2.txt");

	printf("VECINII ZBORULUI CU ID 10:\n");
	afiseazaVeciniZbor(graf, 10);

	dezalocareHeap(&heap);
	dezalocareArbore(&rootABC);
	dezalocareArbore(&rootAVL);
	dezalocareListaPrincipala(&graf);

	return 0;
}