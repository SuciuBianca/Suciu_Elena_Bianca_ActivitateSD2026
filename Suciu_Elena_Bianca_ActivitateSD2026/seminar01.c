#include<stdio.h>
#include<stdlib.h>

struct Joc {
	int id;
	int nrJucatoriMin;
	char* denumire;
	float pret;
	char rating;
};

struct Joc initializare(int _id, int _nrjucatoriMin, char* _denumire, float _pret, char _rating) {
	struct Joc s;
	s.id = _id;
	s.nrJucatoriMin = _nrjucatoriMin;
	s.denumire = malloc(sizeof(char) * (strlen(_denumire) + 1));
	strcpy(s.denumire, _denumire);
	s.pret = _pret;
	s.rating = _rating;
	return s;
}

void afisare(struct  Joc  j) {
	printf("id: %d\n", j.id);
	printf("nume: %s\n", j.denumire);
	printf("nr jucatori: %d\n", j.nrJucatoriMin);
	printf("rating: %c\n", j.rating);
	printf("pret: %5.2f\n", j.pret);
}

void modifica_Atribut(struct  Joc* s, char* _den) {
	free((*s).denumire);
	(*s).denumire = malloc((strlen(_den) + 1) * sizeof(char));
	strcpy((*s).denumire, _den);
}
float calcPretIndividual(struct Joc s)
{
	float pret;
	pret = s.pret / s.nrJucatoriMin;
	return pret;

}
void dezalocare(struct Joc* s) {
	free((*s).denumire);

}

int main() {
	struct  Joc  s;
	s = initializare(1, 3, "monopoly", 120, 'A');
	afisare(s);
	modifica_Atribut(&s, "ceva");
	afisare(s);
	int pret;
	pret = calcPretIndividual(s);
	printf("pret per pers: %d\n", pret);
	int nrJocuri = 3;
	struct Joc* jocuri = malloc(sizeof(struct Joc) * nrJocuri);
	jocuri[0] = s;
	jocuri[1] = initializare(2, 2, "catan", 60, 'A');
	jocuri[2] = initializare(3, 2, "activity", 100, 'A');
	afisare(*(jocuri + 1));
	//-> dereferentiere+ accesare
	//[] deplasare+derferentiere


	dezalocare(&s);

	return 0;
}