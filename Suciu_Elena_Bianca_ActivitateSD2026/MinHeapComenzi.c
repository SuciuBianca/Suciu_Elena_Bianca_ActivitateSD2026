#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Comanda
{
	int id;
	char* numeClient;
	float valoare;
	char urgenta; // D - da, N - nu
};
typedef struct Comanda Comanda;

struct HeapMin
{
	int lungime;
	int nrElemViz;
	Comanda* vector;
};
typedef struct HeapMin HeapMin;

Comanda citireComandaDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);

	char* aux;
	Comanda c1;

	aux = strtok(buffer, sep);
	c1.id = atoi(aux);

	aux = strtok(NULL, sep);
	c1.numeClient = malloc(strlen(aux) + 1);
	strcpy(c1.numeClient, aux);

	c1.valoare = atof(strtok(NULL, sep));

	c1.urgenta = *strtok(NULL, sep);

	return c1;
}

void afisareComanda(Comanda comanda)
{
	printf("Id: %d\n", comanda.id);
	printf("Nume client: %s\n", comanda.numeClient);
	printf("Valoare: %.2f\n", comanda.valoare);
	printf("Urgenta: %c\n\n", comanda.urgenta);
}

HeapMin initializareHeap(int lungime)
{
	HeapMin heap;
	heap.lungime = lungime;
	heap.nrElemViz = 0;
	heap.vector = (Comanda*)malloc(sizeof(Comanda) * lungime);

	return heap;
}

void filtreazaHeap(HeapMin heap, int pozitieNod)
{
	int stanga = 2 * pozitieNod + 1;
	int dreapta = 2 * pozitieNod + 2;
	int pozMin = pozitieNod;

	if (stanga < heap.nrElemViz &&
		heap.vector[stanga].valoare < heap.vector[pozMin].valoare)
	{
		pozMin = stanga;
	}

	if (dreapta < heap.nrElemViz &&
		heap.vector[dreapta].valoare < heap.vector[pozMin].valoare)
	{
		pozMin = dreapta;
	}

	if (pozMin != pozitieNod)
	{
		Comanda aux = heap.vector[pozMin];
		heap.vector[pozMin] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if (pozMin <= (heap.nrElemViz - 2) / 2)
		{
			filtreazaHeap(heap, pozMin);
		}
	}
}

HeapMin citireHeapDeComenziDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");

	HeapMin heap = initializareHeap(5);

	if (f)
	{
		while (!feof(f))
		{
			Comanda comanda = citireComandaDinFisier(f);
			heap.vector[heap.nrElemViz++] = comanda;
		}

		fclose(f);
	}

	for (int i = (heap.nrElemViz - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(HeapMin heap)
{
	for (int i = 0; i < heap.nrElemViz; i++)
	{
		afisareComanda(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(HeapMin heap)
{
	for (int i = heap.lungime - 1; i >= heap.nrElemViz; i--)
	{
		afisareComanda(heap.vector[i]);
	}
}

Comanda extrageComanda(HeapMin* heap)
{
	Comanda aux;
	aux.id = -1;

	if (heap->nrElemViz > 0)
	{
		heap->nrElemViz--;

		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemViz];
		heap->vector[heap->nrElemViz] = aux;

		filtreazaHeap(*heap, 0);
	}

	return aux;
}

void dezalocareHeap(HeapMin* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].numeClient);
	}

	free(heap->vector);
	heap->vector = NULL;
	heap->nrElemViz = 0;
	heap->lungime = 0;
}
void afisareComenziUrgente(HeapMin heap)
{
	for (int i = 0; i < heap.nrElemViz; i++)
	{
		if (heap.vector[i].urgenta == 'D')
		{
			afisareComanda(heap.vector[i]);
		}
	}
}
Comanda cautaComandaDupaId(HeapMin heap, int id)
{
	Comanda c;
	c.id = -1;
	c.numeClient = NULL;
	c.valoare = 0;
	c.urgenta = '-';
	

	for (int i = 0; i < heap.nrElemViz; i++)
	{
		if (heap.vector[i].id == id)
		{
			c = heap.vector[i];
			return c;
		}
	}

	return c;
}

int main()
{
	HeapMin heap = citireHeapDeComenziDinFisier("comenzi.txt");

	afisareHeap(heap);

	printf("extrageri\n");

	afisareComanda(extrageComanda(&heap));
	afisareComanda(extrageComanda(&heap));
	afisareComanda(extrageComanda(&heap));

	printf("heap ascuns\n");

	afiseazaHeapAscuns(heap);


	printf("COMENZI URGENTE\n");
	afisareComenziUrgente(heap);

	Comanda c = cautaComandaDupaId(heap, 2);
	printf("------------\n");

	if (c.id != -1)
	{
		afisareComanda(c);
	}
	else
	{
		printf("Comanda nu a fost gasita\n");
	}

	dezalocareHeap(&heap);

	return 0;
}