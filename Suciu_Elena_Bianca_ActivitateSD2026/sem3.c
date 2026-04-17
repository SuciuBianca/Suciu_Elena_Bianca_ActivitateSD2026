//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
////trebuie sa folositi fisierul masini.txt
////sau va creati un alt fisier cu alte date
//
//struct StructuraMasina {
//	int id;
//	int nrUsi;
//	float pret;
//	char* model;
//	char* numeSofer;
//	unsigned char serie;
//};
//typedef struct StructuraMasina Masina;
//
//void afisareMasina(Masina m) {
//	printf("Id: %d ||NrUsi: %d ||Pret: %5.2f ||Model: %s ||NumeSofer: %s ||Serie: %c\n"
//		, m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
//}
//
//void afisareVectorMasini(Masina* masini, int nrMasini) {
//	for (int i = 0;i < nrMasini;i++)
//	{
//		afisareMasina(masini[i]);
//	}
//}
//
//void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
//	Masina* temp = malloc(sizeof(nrMasini) + 1);
//}
//
//Masina citireMasinaFisier(FILE* file) {
//	//functia citeste o masina dintr-un strceam deja deschis
//	//masina citita este returnata;
//}
//
//Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
//	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
//	//prin apelul repetat al functiei citireMasinaFisier()
//	//numarul de masini este determinat prin numarul de citiri din fisier
//	//ATENTIE - la final inchidem fisierul/stream-ul
//}
//
//void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
//	//este dezalocat intreg vectorul de masini
//}
//
//int main() {
//
//
//	return 0;
//}