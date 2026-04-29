#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//arbore binar de cautare- folositi pt cautare binara 
struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct NodArbore  NodArbore;
struct NodArbore {
	Masina info;
	NodArbore* left;
	NodArbore* right;

};

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


void adaugaMasinaInArbore(NodArbore** radacina ,Masina masinaNoua) {

	if (*radacina)
	{
		if ((*radacina)->info.id > masinaNoua.id)
		{
			adaugaMasinaInArbore(&(*radacina)->left, masinaNoua);

		}
		else
		{
			adaugaMasinaInArbore(&(*radacina)->right, masinaNoua);
		}
	}
	else
	{
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->info = masinaNoua;
		nou->left = nou->right = NULL;
		(*radacina) = nou;
	}

}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	NodArbore* radacina = NULL;
	
	if (file)
	{
		while (!feof(file))
		{
			Masina m = citireMasinaDinFisier(file);
			adaugaMasinaInArbore(&radacina, m);
		}fclose(file);
		return radacina;
	}
}

void afisareMasiniDinArbore(NodArbore* radacina) {
	//moduri de parcurgere
	//RSD-PREORDINE radacina stg dr
	// SRD-INORDINE stg radacina dr
	// SDR-POSTORDINE stg dr radacina

	if (radacina != NULL)
	{
		afisareMasina(radacina->info);
		afisareMasiniDinArbore(radacina->left);
		afisareMasiniDinArbore(radacina->right);
	}
	
}

void afisareInordine(NodArbore* radacina)
{
	if (radacina != NULL)
	{	
		afisareInordine(radacina->left);
		afisareMasina(radacina->info);
		afisareInordine(radacina->right);
	}
}

void dezalocareArboreDeMasini(NodArbore** radacina) {
	if (*radacina != NULL)
	{
		dezalocareArboreDeMasini(&(*radacina)->left);
		dezalocareArboreDeMasini(&(*radacina)->right);

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free((*radacina));

		*radacina = NULL;

	}
}

Masina getMasinaByID( NodArbore*radacina, int id) {
	
	if (radacina == NULL)
	{
		Masina m;
		m.id = -1;
		return m;
	}
	else if(id<radacina->info.id)
	{
		return getMasinaByID(radacina->left, id);
	}
	else if(id>radacina->info.id)
	{
		return getMasinaByID(radacina->right, id);
	}
	else
	{
		Masina m = radacina->info;
		m.model = malloc(strlen(radacina->info.model) + 1);
		strcpy(m.model, radacina->info.model);

		m.numeSofer = malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy(m.numeSofer, radacina->info.numeSofer);

		return  m;
	}
}

int determinaNumarNoduri(NodArbore* radacina) {
	
	if (radacina)
	{
		return 1 + 
			determinaNumarNoduri(radacina->right)+determinaNumarNoduri(radacina->left);
		
	}
	else
	{
		return 0;
	}
	
}

int calculeazaInaltimeArbore(NodArbore*radacina) {
	if (radacina)
	{
		return 1 + max(calculeazaInaltimeArbore(radacina->left),
			calculeazaInaltimeArbore(radacina->right));
	}
	return 0;
	
}

float calculeazaPretTotal(NodArbore*radacina) {
	
	if (radacina)
	{
		return radacina->info.pret + calculeazaPretTotal(radacina->left) + calculeazaPretTotal(radacina->right);
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore*radacina, const char* numeSofer) {

	if (radacina)
	{
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
		{
			return radacina->info.pret + calculeazaPretulMasinilorUnuiSofer(radacina->left, numeSofer) +
				calculeazaPretulMasinilorUnuiSofer(radacina->right, numeSofer);
		}
		else
		{
			return  calculeazaPretulMasinilorUnuiSofer(radacina->left, numeSofer) +
				calculeazaPretulMasinilorUnuiSofer(radacina->right, numeSofer);
		}
	}
	return 0;
}

int main() {
	
	NodArbore* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareInordine(radacina);

	afisareMasina(getMasinaByID(radacina, 3));
	printf("numar noduri: %d\n",determinaNumarNoduri(radacina));
	printf("inaltimea este %d\n ", calculeazaInaltimeArbore(radacina));
	printf("pret total %5.2f\n", calculeazaPretTotal(radacina));
	printf("pret masini sofer Ionsecu: %5.2f", calculeazaPretulMasinilorUnuiSofer(radacina, "Ionescu"));

	return 0;
}