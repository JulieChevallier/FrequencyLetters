#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


FILE *ouvreFichierLecture(const char *path){
    return fopen(path, "r");
}

char lireCaractere(FILE *file){
    return fgetc(file);
}

int index_caractere(char c){
    return isalpha(c) ? tolower(c) - 'a' : -1;
}



typedef struct{
    char *path;
} info_fichier_t;

typedef struct{
    char caractere;
    unsigned int nb_occurence;
} info_caractere_t;

typedef struct{
    unsigned int nb_caractere_total;
    info_caractere_t occurence_info[26];
} info_frequence_t;



info_frequence_t info_frequence;



void *traitementFichier(void * Fic){
    info_fichier_t* data = (info_fichier_t* )Fic;
    FILE *f = ouvreFichierLecture(data->path);
    if (f){
        char c;
        while ((c = lireCaractere(f)) != EOF){
            pthread_mutex_lock(&mutex);
            int i = index_caractere(c);
            if (i >= 0 && i < 26){
                info_frequence.nb_caractere_total++;
                info_frequence.occurence_info[i].nb_occurence++;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}


void init(){
    info_frequence.nb_caractere_total = 0;
    for (char i = 'a'; i <= 'z'; i++){
        info_frequence.occurence_info[i - 'a'].caractere = i;
        info_frequence.occurence_info[i - 'a'].nb_occurence = 0;
    }
}


void affiche(){
    printf("nb total caracteres: %d\n", info_frequence.nb_caractere_total);
    for (char i = 'a'; i <= 'z'; i++){
        int occurrence = info_frequence.occurence_info[i -'a'].nb_occurence;
        float freq = ((float)occurrence / info_frequence.nb_caractere_total) * 100;
        printf("%c %d %.2f %%\n", info_frequence.occurence_info[i -'a'].caractere, occurrence, freq);
    }
}


int main(int argc, char **argv){
    if (argc < 2){
        printf("%s fichier1 fichier2 fichier3 ...", argv[0]);
        exit(0);
    }
    init();
    int nbfichiers = argc - 1;

    //---allocation mémoire---//
    pthread_t* threads = malloc(sizeof(pthread_t) * nbfichiers);
    info_fichier_t *fichiers = malloc(sizeof(info_fichier_t) *(nbfichiers));


    //---implémentation---//
    for (int i = 0; i < nbfichiers; i++){
        fichiers[i].path = argv[i + 1];
        pthread_create(&threads[i],NULL,traitementFichier,&fichiers[i]);
    }

    //---attente des threads---//
    for (int i = 0; i < nbfichiers; i++) {
        pthread_join(threads[i], NULL);
    }
    affiche();

    //---libération de la mémoire allouée---//
    free(threads);
    free(fichiers);

    exit(0);
}
