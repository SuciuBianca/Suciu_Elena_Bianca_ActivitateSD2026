#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct JocVideo {
	int id;
	int anLansare;
	float pret;
	char* titlu;
	char* dezvoltator;
	char clasificareVarsta;
	char* gen;
};typedef struct JocVideo JocVideo;


typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Nod  
{
    JocVideo info;
    Nod* next;
    Nod* prev;

};

struct HashTable {
    Nod** vector;
    int dim;
};


JocVideo citireJocDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    JocVideo v;
    char* aux;
    v.id = atoi(strtok(buffer, sep));
    v.anLansare = atoi(strtok(NULL, sep));
    v.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    v.titlu = malloc(strlen(aux) + 1);
    strcpy(v.titlu, aux);
    aux = strtok(NULL, sep);
    v.dezvoltator = malloc(strlen(aux) + 1);
    strcpy(v.dezvoltator, aux);
    v.clasificareVarsta = *strtok(NULL, sep);
    aux = strtok(NULL, sep);
    v.gen = malloc(strlen(aux) + 1);
    strcpy(v.gen, aux);
    return v;
}

void afisareJoc(JocVideo joc) {
   printf("ID: %d | Titlu: %s | Lansare: %d | Pret: %0.2f | Dev: %s | Varsta: %c | Gen: %s\n",
    joc.id, joc.titlu, joc.anLansare, joc.pret, joc.dezvoltator, joc.clasificareVarsta, joc.gen);
    
}

void afisareLista(Nod*cap) {
    while (cap)
    {
        afisareJoc(cap->info);
        cap = cap->next;
   }
}

void adaugaJocInLista(Nod*cap, JocVideo jocNou) {
    Nod* p = cap;
    while (p->next)
    {
        p = p->next;
    }

    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->next = NULL;
    nou->info = jocNou;
    nou->prev = p;
    p->next = nou;
}

int stergeJocDupaTitlu(Nod**cap, const char* titlu) {
 
    Nod* p = (*cap);
    while (p && strcmp(p->info.titlu, titlu) != 0)
    {
        p = p->next;
    }
    if (p == NULL)
    {
        return;
    }
    if (p->prev == NULL)
    {
        p->next->prev = NULL;
    }
    else if (p->next == NULL)
    {
        
        p->prev->next = NULL;
    }
    else
    {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    }
    free(p->info.titlu);
    free(p->info.gen);
    free(p->info.dezvoltator);
    free(p);
    return 1;
}

void sorteazaListaDupaPret(Nod** cap) {
   int s = 0;
    
    JocVideo aux = (*cap)->info;
    do
    {
        s = 0;
        Nod* p = (*cap);
        while (p && p->next != NULL)
        {
            if (p->info.pret > p->next->info.pret)
            {
                s = 1;
                aux = p->info;
                p->info = p->next->info;
                p->next->info = aux;
            }
            p = p->next;
        }
    } while (s == 1);
  
}


HashTable initializareHashTable(int dimensiune)
{
    HashTable ht;
    ht.dim = dimensiune;
    ht.vector = (Nod**)malloc(sizeof(Nod*)*dimensiune);
    if (ht.vector != NULL)
    {
        for (int i = 0;i < dimensiune;i++)
        {
            ht.vector[i] = NULL;
        }
    }
   

    return ht;
}

int calculeazaHash(const char* gen, int dimensiune) {
    int suma = 0;
    for (int i = 0;i < strlen(gen);i++)
    {
        suma += gen[i];
    }
    return suma % dimensiune;
}

void inserareJocInTabela(HashTable* ht, JocVideo joc) {
    int pozitie = calculeazaHash(joc.gen, ht->dim);
    if (ht->vector[pozitie] == NULL)
    {
        ht->vector[pozitie] = (Nod*)malloc(sizeof(Nod));
        ht->vector[pozitie]->info = joc;
        ht->vector[pozitie]->next = NULL;
    }
    else
    {
        Nod* curent = ht->vector[pozitie];
        while (curent->next)
        {
            curent = curent->next;
        }
        Nod* p = (Nod*)malloc(sizeof(Nod));
        p->next = NULL;
        p->info = joc;
        p->prev = curent;
        curent->next = p;
    }
 }

HashTable citireJocuriDinFisier(const char* numeFisier,int dim) {
    HashTable hash = initializareHashTable(dim);
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL)
    {
        printf("eroare la deschiderer!!!1");
        return hash;
    }
    while (!feof(file))
    {
        JocVideo joc = citireJocDinFisier(file);
        inserareJocInTabela(&hash, joc);
    }fclose(file);
   
    return hash;

}

void afisareTabelaDeJocuri(HashTable ht) {
    for (int i = 0;i < ht.dim;i++)
    {
        if (ht.vector[i] != NULL)
        {
            printf("jocurile de pe poz %d sunt:\n ", i);
            Nod* curent = ht.vector[i];
            while (curent!=NULL)
            {
                afisareJoc(curent->info);
                curent = curent->next;
          }
        }
        else printf("\npe poz %d nu sunt jocuri\n", i);
    }
 
}

void dezalocareListaDeJocuri(Nod** cap)
{
    Nod* p = (*cap);
    while (p)
    {
        Nod* curent = p;
        p = p->next;
        if (curent->info.dezvoltator != NULL)
        {
            free(curent->info.dezvoltator);
        }
        if (curent->info.gen != NULL)
        {
            free(curent->info.gen);

        }
        if (curent->info.titlu != NULL)
        {
            free(curent->info.titlu);
        }
        free(curent);
    }(*cap) = NULL;
}
void dezalocareTabelaDeJocuri(HashTable* ht) {
    for (int i = 0;i < ht->dim;i++)
    {
        dezalocareListaDeJocuri(&ht->vector[i]);
  }
    free(ht->vector);
    ht->dim = 0;
    ht->vector = NULL;
}


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
     float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
         float* preturi = NULL;
         (*nrClustere) = 0;

         for (int i = 0;i < ht.dim;i++)
         {
             if (ht.vector[i] != NULL)
             {
                 (*nrClustere)++;
             }
         }
         preturi = (float*)malloc(sizeof(float) * (*nrClustere));
         int contor = 0;
         for (int i = 0;i < ht.dim;i++)
         {
             if (ht.vector[i] != NULL)
             {
                 preturi[contor] = calculeazaPretMediu(ht.vector[i]);
                 contor++;
             }
         }
         return preturi;
     }
   

JocVideo getCelMaiScumpJoc(HashTable ht) {
    JocVideo maxim;
    for (int i = 0; i < ht.dim; i++)
    {
        if (ht.vector[i] != NULL)
        {
            maxim = ht.vector[i]->info;
            break;
        }
    }
    for (int i = 0;i < ht.dim;i++)
    {
        Nod* curent = ht.vector[i];
        while (curent != NULL)
        {
            if (curent->info.pret > maxim.pret)
            {
                maxim = curent->info;
            }
            curent = curent->next;
        }
       
    }
    return maxim;
}

JocVideo* cautaJocDupaTitlu(HashTable ht, const char* titlu) {
    JocVideo* v = NULL;
    for (int i = 0;i < ht.dim;i++)
    {
        Nod* curent = ht.vector[i];

        while (curent != NULL)
        {

            if (strcmp(curent->info.titlu, titlu) == 0)
            {
                v = &curent->info;
                return v;
            }

            curent = curent->next;
        }

    }
    return NULL;
}


int main() {
 
    HashTable ht = citireJocuriDinFisier("jocVideo.txt", 5);

   
    printf("===== TABELA DE JOCURI =====\n");
    afisareTabelaDeJocuri(ht);

    int nrClustere = 0;
    float* medii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
    printf("\n===== PRETURI MEDII PER CLUSTER =====\n");
    for (int i = 0; i < nrClustere; i++)
    {
        printf("Cluster %d: %.2f RON\n", i, medii[i]);
    }
    free(medii);
 
    printf("\n===== CEL MAI SCUMP JOC =====\n");
    JocVideo maxim = getCelMaiScumpJoc(ht);
    afisareJoc(maxim);

    printf("\n===== CAUTARE =====\n");
    JocVideo* gasit = cautaJocDupaTitlu(ht, "Elden Ring");
    if (gasit != NULL)
        afisareJoc(*gasit);
    else
        printf("Jocul nu a fost gasit.\n");
    dezalocareTabelaDeJocuri(&ht);

    return 0;
}