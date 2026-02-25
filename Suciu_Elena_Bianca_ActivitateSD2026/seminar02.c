#include<stdio.h>
#include<malloc.h>

struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};
struct Telefon initializare(int id, int ram, const char* producator, float pret, char serie) {
		struct Telefon t;
		t.id = id;
		t.RAM = ram;
		t.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1));
		//strcpy_s(t.producator, strlen(producator) + 1, producator);
		strcpy(t.producator, producator);
		t.pret = pret;
		t.serie = serie;
		return t;
	}
struct Telefon copiazaTelefon(struct Telefon t) {
	struct Telefon telefon = initializare(t.id, t.RAM, t.producator, t.pret, t.serie);
	return telefon;
}

void afisare(struct Telefon t) {
	printf("id: %d,ram: %d, producator: %s, pret: %5.2f\n", t.id, t.RAM, t.producator, t.pret);
}

void afisareVector(struct Telefon* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		
		afisare(vector[i]);
	
	}
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	if (nrElementeCopiate > nrElemente) {
		nrElementeCopiate = nrElemente;
	}
		
		struct Telefon* vectorNou = (struct Telfon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
		for (int i = 0;i < nrElementeCopiate;i++)
		{
			vectorNou[i] = copiazaTelefon(vector[i]);
		}
		return vectorNou;
	
	
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
	for (int i = 0;i < *nrElemente; i++)
	{
		free((*vector)[i].producator);
		(*vector)[i].producator = NULL;
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaTelefoaneScumpe(struct Telefon* vector, int nrElemente, float pretMin, 
	struct Telefon** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	*dimensiune = 0;
	for (int i = 0;i < nrElemente;i++)
	{	
		if (vector[i].pret >= pretMin)
		{
			(*dimensiune)++;
		}
	}
	*vectorNou = malloc(*dimensiune * sizeof(struct Telefon));

	for (int i = 0,j=0 ;i< nrElemente;i++)
	{
		if (vector[i].pret >= pretMin)
		{
			(*vectorNou)[j] = copiazaTelefon(vector[i]);
			j += 1;
			
		}
	}

}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon t;
	t.id = 1;

	return t;
}
	

int main() {
	struct Telefon t;
	t = initializare(1, 256, "Samsung", 2000.8, 'A');
	afisare(t);
	int nrTelefoane = 3;
	int nrTelefoaneCopiate = 2;
	struct Telefon* telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0]= initializare(3, 256, "Samsung", 2000.8, 'A');
	telefoane[1]= initializare(2, 256, "Samsung", 2300.5, 'D');
	telefoane[2] = t;
	afisareVector(telefoane, nrTelefoane);
	struct Telefon* telefoaneCopiate = copiazaPrimeleNElemente(telefoane, nrTelefoane, 2);
	printf("vect afisare telefoane copiate:\n");
	afisareVector(telefoaneCopiate, nrTelefoaneCopiate);
	dezalocare(&telefoaneCopiate, &nrTelefoaneCopiate);
	afisareVector(telefoane, nrTelefoane);

	struct Telefon *vectorNou;
	int dimensiune;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 2100.5, &vectorNou, &dimensiune);
	printf("afisare telefoane scumpe: \n");
	afisareVector(vectorNou, dimensiune);



	return 0;
}