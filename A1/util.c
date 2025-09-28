#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "util.h"
#include "gbv.h"


void format_date(time_t t, char *buffer, int max) {
    struct tm *info = localtime(&t);
    strftime(buffer, max, "%d/%m/%Y %H:%M:%S", info);
}


/*Manipulação do Library ==============================================================================================*/
void free_lib(Library *lib) {
    if(!lib->docs) //lib->count == 0
        return;
    free(lib->docs);
}

void lib_count_write(FILE *gbv, Library *lib) {
    rewind(gbv);
    fwrite(&lib->count, sizeof(int), 1, gbv);
}

void lib_offset_write(FILE *gbv, Library *lib, unsigned long int offset_lib) {
    //Escrever no .gbv o offset do diretório
    rewind(gbv);
    fseek(gbv, sizeof(int), SEEK_SET);
    fwrite(&offset_lib, sizeof(unsigned long int), 1, gbv);
}

void lib_write(FILE *gbv, Library *lib, unsigned long int offset_lib) {
    //No diretório tem apenas as info de cada doc
    rewind(gbv);
    fseek(gbv, offset_lib, SEEK_SET);
    fwrite(lib->docs, sizeof(Document), lib->count, gbv);
}

/*Funções auxiliares =================================================================================================*/

int docs_name_cmp(const Library *lib, const char *docname) {
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->docs[i].name, docname) == 0) {
            return i;
        }
    }
    return -1;
}

int gbv_ext_verify(const char *filername) {
    size_t size_name = strlen(filername);

    return(size_name >= 5 && strcmp(filername + size_name - 4, ".gbv") == 0);
}

int gbv_empty(FILE *gbv) {
    fseek(gbv, 0, SEEK_END);
    unsigned long size = ftell(gbv);
    return size == 0;
}