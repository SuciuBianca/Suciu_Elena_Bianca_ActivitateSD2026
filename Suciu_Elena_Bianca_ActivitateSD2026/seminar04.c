#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//lista simpla inlantuita este o structura omogena, liniara ..?
//vect este un tablou unidim cu el omogene, care pot fi accesate in mod direct 
struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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

void afisareListaMasini(Nod* cap) {
	while (cap)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap,Masina masinaNoua) {
	Nod* temp = malloc(sizeof(Nod));
	temp->info = masinaNoua;
	temp->next = NULL;
	if (!(*cap))
	{
		*cap = temp;
	}
	else {
		Nod* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = temp;
	}
	
}
//ADAUGAT
void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = *cap;
	*cap = nodNou;

}

void* citireListaMasiniDinFisier(const char* numeFisier) {

	FILE* file = fopen(numeFisier, "r");
	Nod* cap = NULL;
	while (!feof(file))
	{	
		adaugaMasinaInLista(&cap,citireMasinaDinFisier(file));
	}
	fclose(file);
	return cap;
}

void dezalocareListaMasini(Nod** temp) {
	while (*temp)
	{
		free((*temp)->info.numeSofer);
		free((*temp)->info.model);
		Nod* nou=*temp;
		nou=(*temp)->next;
		free(*temp);
		*temp = nou;
	}
}
//ADAUGAT
float calculeazaPretMediu(Nod*cap) {
	float sum = 0;
	int contor = 0;
	while (cap)
	{
		sum += cap->info.pret;
		contor++;
		cap=cap->next;
	}
	if(contor==0)
	{
		return 0;
	}
		
	return sum / contor;
}
//ADAUGAT
void stergeMasiniDinSeria(Nod** cap,char serieCautata) {
	while ((*cap) != NULL && (*cap)->info.serie == serieCautata)
	{
		Nod* temp = *cap;
		*cap = (*cap)->next;

		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}

	if ((*cap) == NULL)
	{
		return;
	}
	Nod* curent = *cap;
	while (curent->next != NULL)
	{
		if (curent->next->info.serie == serieCautata)
		{
			Nod* temp = curent->next;
			curent->next = temp->next;

			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
		else {
			curent = curent->next;
		}
	}
}
//ADAUGAT
float calculeazaPretulMasinilorUnuiSofer(Nod*cap,const char* numeSofer) {
	float suma = 0;
	while (cap!=NULL)
	{
		if (strcmp(cap->info.numeSofer, numeSofer) == 0)
		{
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

int main() {
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	printf("lista masini:\n");
	afisareListaMasini(cap);
	printf("Pret mediu total: %.2f\n", calculeazaPretMediu(cap));
	printf("Pret masini 'Ionescu': %.2f\n\n", calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));

	Masina mNoua;
	mNoua.id = 99; mNoua.nrUsi = 4; mNoua.pret = 15000; mNoua.serie = 'X';
	mNoua.model = malloc(20); strcpy(mNoua.model, "ModelTest");
	mNoua.numeSofer = malloc(20); strcpy(mNoua.numeSofer, "Ion");

	adaugaLaInceputInLista(&cap, mNoua);
	printf("dupa adaugare masina \n");
	afisareListaMasini(cap); 
	stergeMasiniDinSeria(&cap, 'X');
	printf("dupa stergere seria X \n");
	afisareListaMasini(cap);

	dezalocareListaMasini(&cap);
	

	return 0;
}