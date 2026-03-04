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

void afisareMasina(Masina m) {
	
	printf("ID: %d|| Usi: %d|| Pret %5.2f||,Model: %-10s|| Sofer: %-10s|| Serie: %c\n",
		m.id,m.nrUsi,m.pret,m.model,m.numeSofer,m.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	
	if (masini == NULL || nrMasini == 0)
	{
		printf("vectorul de masini este gol");
		return;

	}
	for (int i = 0;i < nrMasini;i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	Masina* temp = (Masina*)malloc(((*nrMasini) + 1) * sizeof(Masina));
	for (int i = 0;i < *nrMasini;i++)
	{
		temp[i] = (*masini)[i];
	}
	temp[*nrMasini] = masinaNoua;
	free(*masini);
	*masini = temp;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {

	char linie[256];
	fgets(linie, 255, file);
	char delimitator[3] = ",\n";
	Masina masina;
	masina.id=atoi(strtok(linie,delimitator));
	masina.nrUsi= atoi(strtok(NULL,delimitator));
	masina.pret=atof(strtok(NULL, delimitator));
	char* aux = (strtok(NULL, delimitator));

	masina.model = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.model, aux);

	aux = (strtok(NULL, delimitator));
	masina.numeSofer = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.numeSofer, aux);
	masina.serie = strtok(NULL, delimitator)[0];
	return masina;


}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	
	FILE* file = fopen(numeFisier, "r");
	if (!file)
	{
		printf("eroare la deschidere!!\n");
		return;
	}
	else {
		Masina* vectorMasina = NULL;
		*nrMasiniCitite = 0;
		while (!feof(file))
		{	Masina masina=citireMasinaFisier(file);
			adaugaMasinaInVector(&vectorMasina, nrMasiniCitite, masina);
		}
		return vectorMasina;
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
}

int main() {
	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	

	return 0;
}