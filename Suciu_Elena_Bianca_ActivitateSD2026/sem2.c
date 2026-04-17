//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<malloc.h>
//#include <string.h>
//
//
//struct Magazine {
//	int id;
//	int nrBucati;
//	char* produs;
//	float pret;
//	char marime;
//};
//
//struct Magazine initializare(int id, int nr, const char* produs, float pret, char marime) {
//	struct Magazine m;
//	m.id = id;
//	m.nrBucati = nr;
//	m.produs = (char*)malloc(sizeof(char) * (strlen(produs) + 1));
//	strcpy(m.produs, produs);
//	m.pret = pret;
//	m.marime = marime;
//	return m;
//}
//
//struct Magazine copiazaProdus(struct Magazine m) {
//	struct Magazine nou = initializare(m.id, m.nrBucati, m.produs, m.pret, m.marime);
//	return nou;
//}
//
//void afisare(struct Magazine m) {
//	printf("id:%d, nr bucati %d, produs %s, pret %5.2f, marime %d\n",
//		m.id, m.nrBucati, m.produs, m.pret, m.marime);
//}
//
//void afisareVector(struct Magazine* vector, int nrElemente) {
//	for (int i = 0; i < nrElemente; i++) {
//		afisare(vector[i]);
//	}
//}
//
//struct Magazine* copiazaPrimeleNElemente(struct Magazine* vector, int nrElemente,
//	int nrElementeCopiate) {
//	// FIX: verificarea INAINTE de malloc
//	if (nrElementeCopiate > nrElemente)
//		nrElementeCopiate = nrElemente;
//
//	struct Magazine* vectorNou = (struct Magazine*)malloc(sizeof(struct Magazine) * nrElementeCopiate);
//	for (int i = 0; i < nrElementeCopiate; i++) {
//		vectorNou[i] = copiazaProdus(vector[i]);
//	}
//	return vectorNou;
//}
//
//void dezalocare(struct Magazine** vector, int* nrElemente) {
//	for (int i = 0; i < *nrElemente; i++) {
//		free((*vector)[i].produs);
//		(*vector)[i].produs = NULL;
//	}
//	free(*vector);
//	*vector = NULL;
//	*nrElemente = 0;
//}
//
//void copiazaProduseIeftine(struct Magazine* vector, int nrElemente, float pretMax,
//	struct Magazine** vectorNou, int* dimensiune) {
//	(*dimensiune) = 0;
//	for (int i = 0; i < nrElemente; i++) {
//		if (vector[i].pret <= pretMax)
//			(*dimensiune)++;
//	}
//
//	*vectorNou = (struct Magazine*)malloc(sizeof(struct Magazine) * (*dimensiune));
//	for (int i = 0, j = 0; i < nrElemente; i++) {
//		if (vector[i].pret <= pretMax) {
//			(*vectorNou)[j] = copiazaProdus(vector[i]);
//			j++;
//		}
//	}
//}
//
//struct Magazine getPrimulElementConditionat(struct Magazine* vector, int nrElemente,
//	const char* conditie) {
//	if (vector != NULL && nrElemente > 0) {
//		for (int i = 0; i < nrElemente; i++) {
//			if (strcmp(vector[i].produs, conditie) == 0) {
//				return vector[i];
//			}
//		}
//	}
//	struct Magazine gol;
//	gol.produs = NULL;
//	gol.id = -1;
//	return gol;
//}
//
//int main() {
//	// Testare structura individuala
//	struct Magazine m;
//	m = initializare(1, 7, "rochie", 342.54, 32);
//
//	// FIX: copiazaProdus in loc de = direct
//	struct Magazine m2 = copiazaProdus(m);
//
//	afisare(m);
//	printf("\n");
//	afisare(m2);
//	printf("\n");
//
//	// Creare vector
//	int nrProduse = 3;
//	int nrProduseCopiate = 2;
//	struct Magazine* produse = (struct Magazine*)malloc(sizeof(struct Magazine) * nrProduse);
//	produse[0] = initializare(3, 256, "Tricou", 2000.8, 38);
//	produse[1] = initializare(2, 256, "Fusta", 2300.5, 44);
//	// FIX: copiazaProdus in loc de = direct ca sa avem pointeri separati
//	produse[2] = copiazaProdus(m);
//
//	printf("Vector initial:\n");
//	afisareVector(produse, nrProduse);
//	printf("\n");
//
//	// Copiere primele N elemente
//	struct Magazine* produseCopiate = copiazaPrimeleNElemente(produse, nrProduse, nrProduseCopiate);
//	printf("Primele %d produse copiate:\n", nrProduseCopiate);
//	afisareVector(produseCopiate, nrProduseCopiate);
//	printf("\n");
//
//	// Dezalocare vector copiat
//	dezalocare(&produseCopiate, &nrProduseCopiate);
//
//	printf("Vector initial dupa dezalocare copii:\n");
//	afisareVector(produse, nrProduse);
//	printf("\n");
//
//	// Copiere produse ieftine
//	struct Magazine* vectorNou;
//	int dimensiune;
//	copiazaProduseIeftine(produse, nrProduse, 2100.5, &vectorNou, &dimensiune);
//	printf("Produse ieftine (pret <= 2100.5):\n");
//	afisareVector(vectorNou, dimensiune);
//	printf("\n");
//
//	// Dezalocari finale
//	dezalocare(&produse, &nrProduse);
//	dezalocare(&vectorNou, &dimensiune);
//
//	// FIX: m si m2 au pointeri separati, se elibereaza fara probleme
//	free(m.produs);
//	m.produs = NULL;
//	free(m2.produs);
//	m2.produs = NULL;
//
//	return 0;
//}