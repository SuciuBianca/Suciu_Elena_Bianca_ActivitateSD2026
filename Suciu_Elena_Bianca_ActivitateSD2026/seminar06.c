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
//Masina citireMasinaDinFisier(FILE* file) {
//	char buffer[100];
//	char sep[3] = ",\n";
//	fgets(buffer, 100, file);
//	char* aux;
//	Masina m1;
//	aux = strtok(buffer, sep);
//	m1.id = atoi(aux);
//	m1.nrUsi = atoi(strtok(NULL, sep));
//	m1.pret = atof(strtok(NULL, sep));
//	aux = strtok(NULL, sep);
//	m1.model = malloc(strlen(aux) + 1);
//	strcpy_s(m1.model, strlen(aux) + 1, aux);
//
//	aux = strtok(NULL, sep);
//	m1.numeSofer = malloc(strlen(aux) + 1);
//	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
//
//	m1.serie = *strtok(NULL, sep);
//	return m1;
//}
//
//void afisareMasina(Masina masina) {
//	printf("Id: %d\n", masina.id);
//	printf("Nr. usi : %d\n", masina.nrUsi);
//	printf("Pret: %.2f\n", masina.pret);
//	printf("Model: %s\n", masina.model);
//	printf("Nume sofer: %s\n", masina.numeSofer);
//	printf("Serie: %c\n\n", masina.serie);
//}
//
////STACK
////Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
////putem reprezenta o stiva prin LSI, LDI sau vector
//
//
//typedef struct Nod Nod;
//struct Nod {
//	Masina masina;
//	Nod* next;
//};
//
//void pushStack(Nod** cap, Masina masina) {
//	Nod* first = (Nod*)malloc(sizeof(Nod));
//	first->masina = masina;
//	first->next = (*cap);
//	(*cap) = first;
//}
//
//Masina popStack(Nod** cap) {
//	if ((*cap) == NULL)
//	{
//		Masina m;
//		m.id = -1;
//		return m;
//	}
//	Masina rezultat = (*cap)->masina;
//	Nod* temp = (*cap)->next;
//	free(*cap);
//	*cap = temp;
//	return rezultat;
//}
//
//char IsemptyStack(Nod* cap) {
//	return cap == NULL;
//}
//
//void* citireStackMasiniDinFisier(const char* numeFisier) {
//	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
//	//prin apelul repetat al functiei citireMasinaDinFisier()
//	//ATENTIE - la final inchidem fisierul/stream-ul
//	FILE* f = fopen(numeFisier, "r");
//	if (!f) {
//		return NULL;
//	}
//	Nod* cap = NULL;
//	while (!feof(f)) {
//		Masina masina = citireMasinaDinFisier(f);
//
//		pushStack(&cap, masina);
//	}
//	fclose(f);
//	return cap;
//}
//
//
//void dezalocareStivaDeMasini(Nod** cap) {
//	while (*cap != NULL) {
//		Masina m = popStack(cap);
//		free(m.model);
//		free(m.numeSofer);
//	}
//}
//
//int size(Nod** stiva) {
//	int contor = 0;
//	Nod* stivaAux = NULL;
//	while (*stiva != NULL)
//	{
//		Masina m = popStack(stiva);
//		contor++;
//		pushStack(&stivaAux,m);
//	}
//	while (stivaAux != NULL)
//	{
//		Masina m = popStack(&stivaAux);
//		pushStack(stiva, m);
//	}
//	return contor;
//
//
//}
//typedef struct NodDublu NodDublu;
//struct NodDublu
//{
//	Masina info;
//	NodDublu* next;
//	NodDublu* prev;
//
//};
//typedef struct ListaDubluInlantuita ListaD;
//struct ListaDubluInlantuita
//{
//	NodDublu* first;
//	NodDublu* last;
//};
//
////QUEUE
////Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
////putem reprezenta o coada prin LSI, LDI sau vector
//void enqueue(ListaD* coada, Masina masina) {
//	//adauga o masina in coada
//	NodDublu* newNod = malloc(sizeof(NodDublu));
//	newNod->info=masina;
//	newNod->next = NULL;
//	newNod->prev = coada->last;
//	if (coada->last) {
//		coada->last->next = newNod;
//	}
//	else {
//		coada->first = newNod;
//	}
//
//	coada->last = newNod;
//		
//}
//
//Masina dequeue(ListaD* coada) {
//	Masina rezultat;
//	rezultat.id = -1;
//	if (coada->first)
//	{
//		rezultat = coada->first->info;
//		NodDublu* temp = coada->first;
//		coada->first = temp->next;
//		free(temp);
//
//	}
//	return rezultat;
//}
//
//ListaD citireCoadaDeMasiniDinFisier(const char* numeFisier) {
//	ListaD coada;
//	coada.first = coada.last = NULL;
//	FILE* f = fopen(numeFisier, "r");
//	if (f)
//	{
//		while (!feof(f))
//		{
//			enqueue(&coada, citireMasinaDinFisier(f));
//		}
//		fclose(f);
//	}return coada;
//}
//
//
//void dezalocareCoadaDeMasini(ListaD* coada) {
//	while (coada->first != NULL) {
//		Masina m = dequeue(coada);
//		free(m.model);
//		free(m.numeSofer);
//	}
//	coada->last = NULL;
//}
//
////metode de procesare
//Masina getMasinaByID(Nod** stiva, int id)
//{
//	Masina masinaRezultat;
//	if ((*stiva) == NULL) {
//		Masina rez;
//		rez.id = -1;
//		return rez;
//	}
//	Nod* stackNou = NULL;
//	while ((*stiva))
//	{
//		Masina masinaNoua = popStack(stiva);
//		if (masinaNoua.id == id) {
//			masinaRezultat = masinaNoua;
//			break;
//		}
//		else
//		{
//			pushStack(&stackNou, masinaNoua);
//		}
//	}
//	while (stackNou)
//	{
//		pushStack(stiva, popStack(&stackNou));
//	}
//	return masinaRezultat;
//}
//
//float calculeazaPretTotal(Nod** stiva)
//{
//	float suma = 0;
//	Nod* stivaAuxiliara = NULL;
//	while (*stiva != NULL)
//	{
//		Masina m = popStack(stiva);
//		suma += m.pret;
//		pushStack(&stivaAuxiliara, m);
//	}
//	while (stivaAuxiliara != NULL)
//	{
//		Masina m = popStack(&stivaAuxiliara);
//		pushStack(stiva, m);
//	}
//	return suma;
//}
//
//int main() {
//	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
//	printf("=== Stiva initiala ===\n");
//	afisareMasina(popStack(&stiva));
//	afisareMasina(popStack(&stiva));
//
//	afisareMasina(getMasinaByID(&stiva, 7));
//
//	printf("COADA\n");
//	ListaD coada= citireCoadaDeMasiniDinFisier("masini.txt");
//	afisareMasina(dequeue(&coada));
//	afisareMasina(dequeue(&coada));
//
//	dezalocareStivaDeMasini(&stiva);
//	dezalocareCoadaDeMasini(&coada);
//	return 0;
//}