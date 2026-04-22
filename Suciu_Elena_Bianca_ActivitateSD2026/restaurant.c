#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Rezervare
{
	unsigned int id_rezervare;
	char* nume_client;
	char* data_redervare; // "15.10.2025"
	int numar_persoane;
	char* status; // "confirmata", "in asteptare", "anulata"
};
typedef struct Rezervare Rezervare;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Nod {
	Rezervare info;
	Nod* next;
};

struct HashTable
{
	int dim;
	Nod** tabela;
};

typedef struct NodD NodD;
struct NodD
{
	Rezervare info;
	NodD* next;
	NodD* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla
{
	NodD* prim;
	NodD* ultim;
};

typedef struct NodStiva NodStiva;
struct NodStiva
{
	Rezervare info;
	NodStiva* next;
};

Rezervare citireRezervareDinFisier(FILE* f)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	char* aux;
	aux = strtok(buffer, sep);
	Rezervare r;
	r.id_rezervare = atoi(aux);

	aux = strtok(NULL, sep);
	r.nume_client = malloc(strlen(aux) + 1);
	strcpy(r.nume_client, aux);

	aux = strtok(NULL, sep);
	r.data_redervare = malloc(strlen(aux) + 1);
	strcpy(r.data_redervare, aux);

	r.numar_persoane = atoi(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	r.status = malloc(strlen(aux) + 1);
	strcpy(r.status, aux);

	return r;
}

HashTable initializareHashTable(int dimensiune)
{
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < ht.dim; i++)
	{
		ht.tabela[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* nume, int dimensiune)
{
	int suma = 0;
	for (int i = 0; i < strlen(nume); i++)
	{
		suma += nume[i];
	}
	return suma % dimensiune;
}

void adaugaRezervariInLista(Nod** cap, Rezervare r)
{
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->next = NULL;
	p->info = r;

	if (!(*cap))
	{
		*cap = p;
	}
	else
	{
		Nod* temp = *cap;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = p;
	}
}

void inserareRezervariInTabela(HashTable ht, Rezervare r)
{
	int poz = calculeazaHash(r.nume_client, ht.dim);
	adaugaRezervariInLista(&(ht.tabela[poz]), r);
}


void afisareRezervare(Rezervare r)
{
	printf("ID: %u | Client: %s | Data: %s | Persoane: %d | Status: %s\n",
		r.id_rezervare, r.nume_client, r.data_redervare, r.numar_persoane, r.status);
}

void afisareListaRezervari(Nod* cap)
{
	while (cap)
	{
		afisareRezervare(cap->info);
		cap = cap->next;
	}
}

void afisareHashTable(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.tabela[i] != NULL)
		{
			printf("Rezervarile de pe poz %d:\n", i);
			afisareListaRezervari(ht.tabela[i]);
		}
	}
}


int totalPersZi(HashTable ht, char* dataCautata)
{
	int total = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		Nod* curent = ht.tabela[i];
		while (curent)
		{
			if (strcmp(curent->info.data_redervare, dataCautata) == 0)
			{
				total += curent->info.numar_persoane;
			}
			curent = curent->next;
		}
	}
	return total;
}



void adaugaLaCoada(ListaDubla* ld, Rezervare r)
{
	NodD* nou = (NodD*)malloc(sizeof(NodD));

	// deep copy
	nou->info.id_rezervare = r.id_rezervare;
	nou->info.numar_persoane = r.numar_persoane;

	nou->info.nume_client = malloc(strlen(r.nume_client) + 1);
	strcpy(nou->info.nume_client, r.nume_client);

	nou->info.data_redervare = malloc(strlen(r.data_redervare) + 1);
	strcpy(nou->info.data_redervare, r.data_redervare);

	nou->info.status = malloc(strlen(r.status) + 1);
	strcpy(nou->info.status, r.status);

	nou->next = NULL;
	nou->prev = NULL;

	if (ld->prim == NULL) 
	{
		ld->prim = nou;
		ld->ultim = nou;
	}
	else 
	{
		nou->prev = ld->ultim;
		ld->ultim->next = nou;
		ld->ultim = nou;
	}
}

void adaugareInLdDinTabela(HashTable ht, ListaDubla* ld)
{
	for (int i = 0; i < ht.dim; i++)
	{
		Nod* curent = ht.tabela[i];
		while (curent != NULL)
		{
			if (strcmp(curent->info.status, "in asteptare") == 0)
			{
				adaugaLaCoada(ld, curent->info);
			}
			curent = curent->next;
		}
	}
}

void afisareLd(ListaDubla ld)
{
	NodD* p = ld.prim;
	while (p)
	{
		afisareRezervare(p->info);
		p = p->next;
	}
}

void afisareLdInv(ListaDubla ld)
{
	NodD* p = ld.ultim;
	while (p)
	{
		afisareRezervare(p->info);
		p = p->prev;
	}
}

void push(NodStiva** varf, Rezervare r)
{
	NodStiva* nou = malloc(sizeof(NodStiva));
	nou->info = r; 
	nou->next = *varf;
	*varf = nou;
}

void pop(NodStiva** varf)
{
	if (*varf == NULL) return;
	NodStiva* temp = *varf;
	afisareRezervare(temp->info);
	*varf = temp->next;
	free(temp->info.nume_client);
	free(temp->info.data_redervare);
	free(temp->info.status);
	free(temp);
}

void mutaInStiva(ListaDubla* ld, NodStiva** varf)
{
	NodD* curent = ld->prim;

	while (curent != NULL)
	{
		NodD* urmator = curent->next; 

		if (curent->info.numar_persoane >= 4)
		{
		
			push(varf, curent->info);

		
			if (curent->prev != NULL)
				curent->prev->next = curent->next;
			else
				ld->prim = curent->next; 

			if (curent->next != NULL)
				curent->next->prev = curent->prev;
			else
				ld->ultim = curent->prev; 

			free(curent); 
		}

		curent = urmator;
	}
}

void afisareStivaPrinPop(NodStiva** varf)
{
	printf("\n--- Afisare Stiva (pop) ---\n");
	while (*varf != NULL)
	{
		pop(varf);
	}
}

void dezalocareLista(Nod* cap)
{
	while (cap)
	{
		Nod* temp = cap;
		free(temp->info.nume_client);
		free(temp->info.data_redervare);
		free(temp->info.status);
		cap = cap->next;
		free(temp);
	}
}

void dezalocareHashTable(HashTable* ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		dezalocareLista(ht->tabela[i]);
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;

}

void dezalocareListaDubla(ListaDubla* ld)
{
	NodD* curent = ld->prim;
	while (curent != NULL)
	{
		NodD* temp = curent;
		free(temp->info.nume_client);
		free(temp->info.data_redervare);
		free(temp->info.status);
		curent = curent->next;
		free(temp);
	}
	ld->prim = NULL;
	ld->ultim = NULL;

}

int main()
{
	FILE* f = fopen("rezervari.txt", "r");
	if (f == NULL) {
		printf("Eroare la deschiderea fisierului!\n");
		return -1;
	}

	HashTable ht = initializareHashTable(7);

	while (!feof(f)) {
		Rezervare r = citireRezervareDinFisier(f);
		if (r.id_rezervare != 0) {
			inserareRezervariInTabela(ht, r);
		}
	}
	fclose(f);

	afisareHashTable(ht);

	char dataDeCautat[] = "15.10.2025";
	int total = totalPersZi(ht, dataDeCautat);
	printf("\nIn data de %s sunt %d persoane. ===\n", dataDeCautat, total);


	printf("\n=Lista Dubla ,status in asteptare ===\n");
	ListaDubla ld;
	ld.prim = NULL;
	ld.ultim = NULL;
	adaugareInLdDinTabela(ht, &ld);
	afisareLd(ld);

	afisareLdInv(ld);


	NodStiva* varf = NULL;
	mutaInStiva(&ld, &varf);
	afisareStivaPrinPop(&varf);
	afisareLd(ld);
	dezalocareHashTable(&ht);
	dezalocareListaDubla(&ld);

	return 0;
}