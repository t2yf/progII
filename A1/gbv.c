#include "gbv.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>

//TODO funções aux
    // TODO move frente
    // TODO move tras
    // TODO escrever diretório



// TODO cuidar do tamanho máximo do buffer
void move_file(FILE *file, unsigned long int start, unsigned long int end, unsigned long int reference) {
    /*Colocar file no buffer*/
    size_t size_file = end - start;
    char *buffer = malloc(size_file);

    if (!buffer) {
        perror("Erro ao alocar o buffer no move_file\n");
        return;
    }

    fseek(file, start, SEEK_SET);
    fread(buffer, 1, size_file, file);

    /*Procurar referencia e escrever*/
    fseek(file, reference, SEEK_SET);
    fwrite(buffer, 1, size_file, file);

    free(buffer);
}

//TODO talvez precise de um move_docs_back e um move_docs_forward, a depender de como funcionará o remover
//Puxar os docs para trás, mesmo que sobreponha o diretório, usar no substituir
void move_docs_back(FILE *gbv, Library *lib, unsigned long int start, unsigned long int reference) {

}
//usar no remover, usar no subsituir
void move_docs_forward() {

}


// TODO cuidar do tamanho máximo do buffer
void read_write(FILE *read_file, FILE *write_file, unsigned long int start_read, unsigned long int size_content, unsigned long int start_write) {
    /*Colocar dados no buffer*/
    //size_t size_content = end_read - start_read;
    //char *buffer = malloc(size_content);
    //TODO verificar se está certo

    char *buffer = malloc(BUFFER_SIZE);

    if (!buffer) {
        perror("Erro ao alocar buffer no read_write\n");
        return;
    }

    /*Ler conteúdo do arquivo de leitura*/
    fseek(read_file, start_read, SEEK_SET);
    fread(buffer, size_content, 1, read_file);

    //fwrite(buffer, size_content, 1, stdout);

    /*Escrever conteúdo no arquivo de escrita*/
    fseek(write_file, start_write, SEEK_SET);
    fwrite(buffer, size_content, 1, write_file);

    free(buffer);
}



//Verifica se o filername tem pelo menos 5 caracteres e se termina com .gbv
int gbv_ext_verify(const char *filername) {
    size_t size_name = strlen(filername);

    return(size_name >= 5 && strcmp(filername + size_name - 4, ".gbv") == 0);
}

//Verifica se o .gbv está vazio, ou seja, se o final é igual ao início
int gbv_empty(FILE *gbv) {
    fseek(gbv, 0, SEEK_END);
    unsigned long size = ftell(gbv);
    return size == 0;
}


//Criar um .gbv
int gbv_create(const char *filename) {
    FILE *gbv = fopen(filename, "w+b");

    if (!gbv) {
        printf("Não foi possível criar %s\n", filename);
        return 0;
    }

    fclose(gbv);
    return 1;
}

//Atribuir dados do .gbv na lib, caso não exista o .gbv, criar
int gbv_open(Library *lib, const char *filename) {
    FILE *gbv = fopen(filename, "r+b");

    if (!gbv) {
        //Verificar se o nome está certo
        int ver_ext = gbv_ext_verify(filename);
        if (!ver_ext) {
            perror("Extensão do arquivo errada: use .gbv ou crie um arquivo com pelo menos um caractere de nome\n");
            return -1;
        }

        //Criar .gbv
        int create = gbv_create(filename);
        if (!create) {
            perror("Erro ao criar o .gbv\n");
            return -1;
        }

        gbv = fopen(filename, "r+b");

        if (!gbv) {
            perror("Erro ao abrir o .gbv criado\n");
            return -1;
        }
    }

    /*Se .gbv estiver vazio, não há nada para ler*/
    if (gbv_empty(gbv)) {
        // TODO [DÚVIDA]>> mais algo ?
        lib->count = 0;

        return 0;
    }

    /*Ler info no lib*/

    //Início do .gbv estão qtde de docs (int) + offset diretório (int)
    rewind(gbv);

    /*Ler quantidade de docs*/
    fread(&lib->count, sizeof(int), 1, gbv);

    /*Ler vetor de docs*/
    unsigned long int offset_lib;
    fread(&offset_lib, sizeof(unsigned long int), 1, gbv);

    fseek(gbv, offset_lib, SEEK_SET);

    lib->docs = calloc(lib->count, sizeof(Document));
    if (!lib->docs) {
        fclose(gbv);
        return -1;
    }
    fread(lib->docs, sizeof(Document), lib->count, gbv);

    fclose(gbv);
    return 0;
}

//
int extract_data_docs(const char *docname, Document *docs) {
    struct stat info;

    /*Extrair info e conferir existência*/
    if (stat(docname, &info) != 0) {
        perror("Falha no stat\n");
        return -1;
    }

    /*Verificar se nome cabe no max*/
    // TODO teste
    // TODO se o nome for maior, cortar, não permitir
    unsigned long int tam_nome = strlen(docname);
    if (tam_nome > MAX_NAME -1) {
        perror("Nome do docs muito longo\n");
        return -1;
    }

    /*Atribuir os atributos*/
    strncpy(docs->name, docname, tam_nome +1);
    docs->name[MAX_NAME - 1] = 0;

    docs->size = info.st_size;
    docs->date = time(NULL);
    //Info a ser alterada posteriormente no gbv_add
    docs->offset = 0;

    return 0;
}

int docs_name_cmp(Library *lib, const char *docname) {
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->docs[i].name, docname) == 0) {
            return i;
        }
    }

    return -1;
}

//Substituir docs no .gbv
// int replace_docs(FILE *gbv, FILE *docs, Library *lib, int repeated_docs) {
//     long offset_repeated_docs = lib->docs[repeated_docs].offset;
//
//     /*Caso 2.1.1: Docs continua com mesmo tamanho*/
//     if (lib->docs[repeated_docs].size == size_new_docs) {
//         //TODO Substituir
//         //TODO atualizar o diretório
//     }
//     /*Caso 2.1.2: Docs aumentou*/
//     else if (lib->docs[repeated_docs].size > size_new_docs) {
//
//     }
//     /*Caso 2.1.3: Docs diminuiu*/
//     else {
//
//     }
//
//     //TODO Empurrar
//     //TODO casos do empurrar, se for o último elemento
//     //TODO atualizar offset de todos os docs subsequentes e o repetido
//     //TODO atualizar o diretório
// }

//ir colocando de um em um arquivo, pelo que está na main > for
//achar offset onde colocar, empurrar o diretório e escrever
// TODO testar com todo o tipo de arquivo, imagens e tals
// TODO !!!!! offset é unsigned long int e qtde de arquivos unsigned int
// TODO se docs é muito grande !!!
// TODO se docs repetido, substituir !!!!
// TODO fazer usando buffer estático

int gbv_add(Library *lib, const char *archive, const char *docname) {
    long offset_docs;
    long offset_lib;

    FILE *gbv = fopen(archive, "r+b");
    int empty = gbv_empty(gbv);

    /*Abrir novo docs somente leitura*/
    FILE *docs = fopen(docname, "rb");

    if (!gbv || !docs) {
        perror("Erro ao abrir arquivo no gbv_add\n");
        return -1;
    }

    /*Extrair dados do documento*/
    Document docs_info;
    int get_data = extract_data_docs(docname, &docs_info);
    if (get_data != 0) {
        perror("Erro ao coletar os dados do docs\n");
        return -1;
    }

    /*Colocar info docs no vetor do lib*/
    // Se for o primeiro elemento, temos que alocar espaço
    if (lib->count == 0) {
        lib->docs = malloc(sizeof(Document));
    } else {
        //TODO testar, consertar lib->count +1
        lib->docs = realloc(lib->docs, sizeof(Document) * (lib->count + 1));
    }

    //TODO! [AVISO] Se for o primeiro elemento ele vai estar na posição 0 do vetor
    lib->docs[lib->count] = docs_info;
    long size_new_docs = lib->docs[lib->count].size;

    /*Atualizar o lib*/
    lib->count++;

    /*Escrever count no .gbv*/
    rewind(gbv);
    fwrite(&lib->count, sizeof(int), 1, gbv);
    unsigned int offset_lib_area = sizeof(int);

    /*Caso 1: .gbv está inicialmente vazio*/
    if (empty) {
        // Library fica logo após count + offset_lib_area + primeiro docs
        offset_lib = sizeof(int) + sizeof(unsigned long int) +size_new_docs;

        // Primeiro docs fica logo depois do count + offset_lib_area
        offset_docs = sizeof(int) + sizeof(unsigned long int);

        //Escrever info do offset do diretório
        fseek(gbv, offset_lib_area, SEEK_SET);
        fwrite(&offset_lib, sizeof(unsigned long int), 1, gbv);

        /*Atualizar offset do documento*/
        lib->docs[lib->count -1].offset = offset_docs;

        //Escrever docs

        //Saber se o arquivo é muito grande
        if (size_new_docs > BUFFER_SIZE) {
            rewind(docs);
            long docs_marker = 0;
            long remaining = 0;
            long chunk = 0;

            //Cada docs grande pode ser dividido em chunk_amount blocos, se for um valor quebrado, soma mais um bloco
            int chunk_amount;
            if (size_new_docs % BUFFER_SIZE != 0) {
                chunk_amount = (int)(size_new_docs / BUFFER_SIZE) +1;
            } else {
                chunk_amount = (int)(size_new_docs / BUFFER_SIZE);
            }


            for (int i = 0; i < chunk_amount; i++) {
                //Colocar o "ponteiro" no lugar a ser lido do docs
                fseek(docs, docs_marker, SEEK_SET);

                //Quanto falta ler e escrever do documento
                remaining = size_new_docs - docs_marker;

                if (remaining < BUFFER_SIZE)
                    chunk = remaining;
                else
                    chunk = BUFFER_SIZE;
                read_write(docs, gbv, docs_marker, chunk, offset_docs);

                 docs_marker += chunk;
                 offset_docs += chunk;
            }
        } else{
            //Docs pequeno
            read_write(docs, gbv, 0, size_new_docs, offset_docs);
        }



        //Escrever diretório
        fseek(gbv, offset_lib, SEEK_SET);
        fwrite(lib->docs, sizeof(Document), lib->count, gbv);

        fclose(gbv);
        fclose(docs);
        return 0;
    }

    /*Caso 2: .gbv não vazio*/
    //TODO [DÚVIDA] perguntar se eu empurro o diretório ou só reescrevo

    /*Caso 2.1: Arquivo é repetido*/

    //Procurar docs repetido
    // int repeated_docs = docs_name_cmp(lib, docname);
    //
    // //Se for repetido
    // if (repeated_docs != -1) {
    //     //TODO substituir
    //     repeated_docs(gbv, docs, lib , repeated_docs);
    //
    //     return 0;
    // }

    /*Caso 2.2: Docs não é repetido*/

    //Pegar offset do diretório e atualizar
    unsigned long int offset_lib_original;
    fread(&offset_lib_original, sizeof(unsigned long int), 1, gbv);

    //Atualizar offset do diretório original
    offset_lib = offset_lib_original + size_new_docs;

    //Escrever info do offset do diretório
    rewind(gbv);
    fseek(gbv, offset_lib_area, SEEK_SET);
    fwrite(&offset_lib, sizeof(unsigned long int), 1, gbv);

    //Atualizar offset do documento
    offset_docs = offset_lib_original;
    lib->docs[lib->count -1].offset = offset_docs;

    //Escrever documento

    //Saber se o arquivo é muito grande
    if (size_new_docs > BUFFER_SIZE) {
        rewind(docs);
        long docs_marker = 0;
        long remaining = 0;
        long chunk = 0;

        //Cada docs grande pode ser dividido em chunk_amount blocos, se for um valor quebrado, soma mais um bloco
        int chunk_amount;
        if (size_new_docs % BUFFER_SIZE != 0) {
            chunk_amount = (int)(size_new_docs / BUFFER_SIZE) +1;
        } else {
            chunk_amount = (int)(size_new_docs / BUFFER_SIZE);
        }


        for (int i = 0; i < chunk_amount; i++) {
            //Colocar o "ponteiro" no lugar a ser lido do docs
            fseek(docs, docs_marker, SEEK_SET);

            //Quanto falta ler e escrever do documento
            remaining = size_new_docs - docs_marker;

            if (remaining < BUFFER_SIZE)
                chunk = remaining;
            else
                chunk = BUFFER_SIZE;
            read_write(docs, gbv, docs_marker, chunk, offset_docs);

            docs_marker += chunk;
            offset_docs += chunk;
        }
    } else{
        //Docs pequeno
        read_write(docs, gbv, 0, size_new_docs, offset_docs);
    }

    //Escrever diretório
    fseek(gbv, offset_lib, SEEK_SET);
    fwrite(lib->docs, sizeof(Document), lib->count, gbv);

    fclose(gbv);
    fclose(docs);

    return 0;
}

//Remover do diretório, do .gbv, arrastar os arquivos para frente, atualizar offsets do diretório
int gbv_remove(Library *lib, const char *docname){}

int gbv_list(const Library *lib) {
    // TODO pensar em casos de erro
    char buffer[20];

    for (int i = 0; i < lib->count; i++) {
        printf("##########################\n");
        printf("Nome: %s\n", lib->docs[i].name);
        printf("Tamanho: %lu\n", lib->docs[i].size);
        format_date(lib->docs[i].date, buffer, sizeof(buffer));
        printf("Data: %s\n", buffer);
        printf("Offset: %lu\n", lib->docs[i].offset);
        printf("##########################\n\n");
    }
    return 0;
}

// TODO usar sprinf com %xd para imprimir em hexadecimal 
int gbv_view(const Library *lib, const char *docname) {
    /// TODO fazer função auxiliar para ler os docs do .gbv e printar no terminal em hex tipo hexdump
    /// //https://stackoverflow.com/questions/19802940/trying-to-use-fwrite-to-write-a-string-in-binary-hex-in-c
    /// https://stackoverflow.com/questions/72717768/how-to-get-a-hex-dump-from-a-binary-file-into-c
    /// https://www.reddit.com/r/learnprogramming/comments/d686g4/read_a_binary_file_and_print_its_values_in_hex_in/
    // unsigned char buffer[8];
    // rewind(gbv);
    // fread(buffer, strlen(buffer)+1, 1, gbv);
    // fwrite(buffer, strlen(buffer)+1, 1, stdout);

}


