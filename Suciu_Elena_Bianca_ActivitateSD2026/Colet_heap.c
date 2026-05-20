#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Colet
{
	int idColet;
	char* destinatar;
	float greutate;
	int prioritate;
	char* codZona; 
};
typedef struct Colet Colet;

typedef struct HeapMin HeapMin;
struct HeapMin
{
	Colet* vector;
	int lungime;
	int nrColete;
};

Colet citireColetDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);

	char* aux;
	Colet c;

	aux = strtok(buffer, sep);
	c.idColet = atoi(aux);

	aux = strtok(NULL, sep);
	c.destinatar = malloc(strlen(aux) + 1);
	strcpy(c.destinatar, aux);

	c.greutate = atof(strtok(NULL, sep));

	c.prioritate = atoi(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.codZona = malloc(strlen(aux) + 1);
	strcpy(c.codZona, aux);

	return c;
}

void afisareColet(Colet c)
{
	printf("Id colet: %d\n", c.idColet);
	printf("Destinatar: %s\n", c.destinatar);
	printf("Greutate: %.2f\n", c.greutate);
	printf("Prioritate: %d\n", c.prioritate);
	printf("Cod zona: %s\n\n", c.codZona);
}

HeapMin initializareHeap(int lungime)
{
	HeapMin heap;
	heap.vector = malloc(sizeof(Colet) * lungime);
	heap.lungime = lungime;
	heap.nrColete = 0;
	return heap;
}

void filtreazaHeap(HeapMin heap, int pozNod)
{
	int pozFiuSt = 2 * pozNod + 1;
	int pozFiuDr = 2 * pozNod + 2;
	int pozMin = pozNod;

	if (pozFiuSt < heap.nrColete &&
		heap.vector[pozFiuSt].prioritate < heap.vector[pozMin].prioritate)
	{
		pozMin = pozFiuSt;
	}

	if (pozFiuDr < heap.nrColete &&
		heap.vector[pozFiuDr].prioritate < heap.vector[pozMin].prioritate)
	{
		pozMin = pozFiuDr;
	}

	if (pozMin != pozNod)
	{
		Colet aux = heap.vector[pozMin];
		heap.vector[pozMin] = heap.vector[pozNod];
		heap.vector[pozNod] = aux;

		if (pozMin <= (heap.nrColete - 2) / 2)
		{
			filtreazaHeap(heap, pozMin);
		}
	}
}

HeapMin citireHeapDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	HeapMin heap = initializareHeap(7);

	if (f)
	{
		while (!feof(f) && heap.nrColete < heap.lungime)
		{
			heap.vector[heap.nrColete++] = citireColetDinFisier(f);
		}

		fclose(f);
	}

	for (int i = (heap.nrColete - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(HeapMin heap)
{
	for (int i = 0; i < heap.nrColete; i++)
	{
		afisareColet(heap.vector[i]);
	}
}

Colet extrageColet(HeapMin* heap)
{
	Colet c;
	c.idColet = -1;
	c.destinatar = NULL;
	c.codZona = NULL;
	c.greutate = 0;
	c.prioritate = 0;

	if (heap->nrColete > 0)
	{
		c.idColet = heap->vector[0].idColet;
		c.greutate = heap->vector[0].greutate;
		c.prioritate = heap->vector[0].prioritate;

		c.destinatar = malloc(strlen(heap->vector[0].destinatar) + 1);
		strcpy(c.destinatar, heap->vector[0].destinatar);

		c.codZona = malloc(strlen(heap->vector[0].codZona) + 1);
		strcpy(c.codZona, heap->vector[0].codZona);

		Colet aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrColete - 1];
		heap->vector[heap->nrColete - 1] = aux;

		heap->nrColete--;

	}

	return c;
}

int numaraColeteZona(HeapMin heap, const char* codZona)
{
	int nr = 0;

	for (int i = 0; i < heap.nrColete; i++)
	{
		if (strcmp(heap.vector[i].codZona, codZona) == 0)
		{
			nr++;
		}
	}

	return nr;
}

void salveazaColeteZonaInVector(HeapMin heap, Colet* vector, int* nr, const char* codZona)
{
	for (int i = 0; i < heap.nrColete; i++)
	{
		if (strcmp(heap.vector[i].codZona, codZona) == 0)
		{
			vector[*nr].idColet = heap.vector[i].idColet;
			vector[*nr].greutate = heap.vector[i].greutate;
			vector[*nr].prioritate = heap.vector[i].prioritate;

			vector[*nr].destinatar = malloc(strlen(heap.vector[i].destinatar) + 1);
			strcpy(vector[*nr].destinatar, heap.vector[i].destinatar);

			vector[*nr].codZona = malloc(strlen(heap.vector[i].codZona) + 1);
			strcpy(vector[*nr].codZona, heap.vector[i].codZona);

			(*nr)++;
		}
	}
}

void afisareVectorColete(Colet* vector, int nr)
{
	for (int i = 0; i < nr; i++)
	{
		afisareColet(vector[i]);
	}
}

void dezalocareColet(Colet* c)
{
	if (c->destinatar != NULL)
	{
		free(c->destinatar);
		c->destinatar = NULL;
	}

	if (c->codZona != NULL)
	{
		free(c->codZona);
		c->codZona = NULL;
	}
}

void dezalocareHeap(HeapMin* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		dezalocareColet(&heap->vector[i]);
	}

	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrColete = 0;
}

void dezalocareVectorColete(Colet** vector, int nr)
{
	for (int i = 0; i < nr; i++)
	{
		dezalocareColet(&(*vector)[i]);
	}

	free(*vector);
	*vector = NULL;
}

int main()
{
	HeapMin heap = citireHeapDinFisier("colet.txt");

	afisareHeap(heap);

	Colet c = extrageColet(&heap);

	afisareColet(c);
 
	afisareHeap(heap);

	const char* zonaCautata = "B101";
	int nrColete = numaraColeteZona(heap, zonaCautata);

	Colet* vector = malloc(sizeof(Colet) * nrColete);
	int nr = 0;

	salveazaColeteZonaInVector(heap, vector, &nr, zonaCautata);

	printf("COLETE DIN ZONA %s:\n", zonaCautata);
	afisareVectorColete(vector, nr);

	dezalocareColet(&c);
	dezalocareVectorColete(&vector, nr);
	dezalocareHeap(&heap);

	return 0;
}