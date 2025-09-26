#include "gbv.h"

#include <ctype.h>

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

// TODO [ALTERAÇÕES FEITAS NOS ARQUIVO BASE]
    // TODO [MAIN.C] Dar free no lib docs após o loop do for, visto que se eu der free dentro do gbv_add não vai mais existir uma lib para acessar, dando erro
    // TODO [GBV.H] No gbv_remove adicionar o parâmetro "const char *archive", para que eu possa abrir o gbv e remover o docs do .gbv, já que se eu alterasse apenas
    // TODO a lib, eu poderia ter problemas na movimentação e no substitui

//TODO
    // TODO testar nome de arquivo muito grande, não deixar ser criado
    // TODO reorganizar as funções no utils

//TODO funções aux

void free_lib(Library *lib) {
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


void move_file(FILE *file, unsigned long int start, unsigned long int size_content, unsigned long int reference) {
    /*Colocar file no buffer*/
    char *buffer = malloc(size_content);

    if (!buffer) {
        perror("Erro ao alocar o buffer no move_file\n");
        return;
    }

    fseek(file, start, SEEK_SET);
    fread(buffer, 1, size_content, file);


    /*Procurar referencia e escrever*/
    fseek(file, reference, SEEK_SET);
    fwrite(buffer, 1, size_content, file);

    free(buffer);
}

//Mover um bloco do .gbv para trás, o diretório DEVE ser reescrito
void move_docs_back(FILE *gbv, Library *lib, int idx_first, int idx_last, long size_to_move) {
    // Tratar caso de erro nos índices
    if (idx_last < idx_first || idx_first < 0 || idx_last < 0 || idx_first >= lib->count || idx_last >= lib->count) {
        printf("Erro nos índices em move_docs_back\n");
        return;
    }

    //Localizar o primeiro docs
    long offset_first = lib->docs[idx_first].offset;

    //Localizar o último docs
    //TODO [AVISO] nomalmente idx_last será lib->count -1 (último), fiz assim para deixar mais genérico
    long offset_last = lib->docs[idx_last].offset + lib->docs[idx_last].size;

    //Qual "blocão" eu devo puxar
    long chunk_to_move = offset_last - offset_first;

    //Para onde mover
    long where_to_move = offset_first + size_to_move;

    //Puxar a partir do último docs

    //Se o bloco é muito grande, cortar em partes
    if (chunk_to_move > BUFFER_SIZE) {
        long aux_pointer = 0;
        long aux_last = offset_last;
        long aux_where_to_move = 0;

        //O blocão pode ser dividido em blocos menores
        int mini_chunk_amount;
        if (chunk_to_move % BUFFER_SIZE != 0) {
            mini_chunk_amount = (int)(chunk_to_move / BUFFER_SIZE) + 1;
        } else {
            mini_chunk_amount = (int)(chunk_to_move / BUFFER_SIZE);
        }

        long mini_chunk_size = 0;

        for (int i = 0; i < mini_chunk_amount; i++) {

            aux_pointer = aux_last - BUFFER_SIZE;
            aux_where_to_move = aux_pointer + size_to_move;

            if (aux_pointer >= offset_first) {
                mini_chunk_size = BUFFER_SIZE;
            } else {

                mini_chunk_size = aux_last - offset_first;
                aux_pointer = offset_first;
                aux_where_to_move = offset_first + size_to_move;
            }
            move_file(gbv, aux_pointer, mini_chunk_size, aux_where_to_move);
            aux_last = aux_pointer;
        }

    } else {
        //Puxar bloco inteiro
        move_file(gbv, offset_first, chunk_to_move, where_to_move);
    }

    //Atualizar offsets
    for (int i = idx_first; i <= idx_last; i++) {
        lib->docs[i].offset += size_to_move;
    }

}


//Mover um bloco do .gbv para frente, o diretório DEVE ser reescrito
void move_docs_forward(FILE *gbv, Library *lib, int idx_first, int idx_last, long size_to_move) {
    // Tratar caso de erro nos índices
    if (idx_last < idx_first || idx_first < 0 || idx_last < 0 || idx_first >= lib->count || idx_last >= lib->count) {
        printf("Erro nos índices em move_docs_forward\n");
        return;
    }
    //Localizar o primeiro docs
    long offset_first = lib->docs[idx_first].offset;

    //Localizar o último docs
    //TODO [AVISO] nomalmente idx_last será lib->count -1 (último), fiz assim para deixar mais genérico
    long offset_last = lib->docs[idx_last].offset + lib->docs[idx_last].size;

    //Qual "blocão" eu devo puxar
    long chunk_to_move = offset_last - offset_first;

    //Para onde mover
    //TODO [AVISO] Lógica contrária ao move_docs_back, aqui está puxando para frente
    long where_to_move = offset_first - size_to_move;

    //Se invadir espaço onde está a quantidade de docs e offset do diretório (int + long), retornar
    if (where_to_move < 12) {
        printf("Erro no move_docs_forward, where_to_move invadindo espaço indevido\n");
        return;
    }

    //Puxar a partir do último docs

    //Se o bloco é muito grande, cortar em partes
    if (chunk_to_move > BUFFER_SIZE) {
        long aux_first = offset_first;
        long aux_pointer = aux_first;
        long aux_where_to_move = 0;

        //O blocão pode ser dividido em blocos menores
        int mini_chunk_amount;
        if (chunk_to_move % BUFFER_SIZE != 0) {
            mini_chunk_amount = (int)(chunk_to_move / BUFFER_SIZE) + 1;
        } else {
            mini_chunk_amount = (int)(chunk_to_move / BUFFER_SIZE);
        }

        long mini_chunk_size = 0;
        //TODO [AVISO] alterações da lógica do move_docs_back
        for (int i = 0; i < mini_chunk_amount; i++) {
            aux_where_to_move = aux_pointer - size_to_move;

            if (aux_pointer <= offset_last) {
                mini_chunk_size = BUFFER_SIZE;
            } else {
                aux_pointer -= BUFFER_SIZE;
                mini_chunk_size = offset_last - aux_pointer;
                aux_where_to_move = aux_pointer - size_to_move;
                //aux_pointer = offset_last;
            }

            move_file(gbv, aux_pointer, mini_chunk_size, aux_where_to_move);
            aux_pointer = aux_first + BUFFER_SIZE;
            aux_first = aux_pointer;
        }

    } else {
        //Puxar bloco inteiro
        move_file(gbv, offset_first, chunk_to_move, where_to_move);
    }

    //Atualizar offsets
    for (int i = idx_first; i <= idx_last; i++) {
        //TODO [AVISO] Lógica contrária ao move_docs_back, aqui está puxando para frente
        lib->docs[i].offset -= size_to_move;
    }


}

void read_write(FILE *read_file, FILE *write_file, unsigned long int start_read, unsigned long int size_content, unsigned long int start_write) {
    char *buffer = malloc(BUFFER_SIZE);

    if (!buffer) {
        perror("Erro ao alocar buffer no read_write\n");
        return;
    }

    /*Ler conteúdo do arquivo de leitura*/
    fseek(read_file, start_read, SEEK_SET);
    fread(buffer, size_content, 1, read_file);

    /*Escrever conteúdo no arquivo de escrita*/
    fseek(write_file, start_write, SEEK_SET);
    fwrite(buffer, size_content, 1, write_file);

    free(buffer);
}

//Se arquivo é grande, escreve de pouco a pouco
void write_docs(FILE *gbv, FILE *docs, long size_new_docs, long offset_docs) {
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

        fclose(gbv);
        return 0;
    }

    /*Ler info no lib*/

    //Início do .gbv estão qtde de docs (int) + offset diretório (int)
    rewind(gbv);

    /*Ler quantidade de docs*/
    fread(&lib->count, sizeof(int), 1, gbv);

    /*Se foi deletado todos os docs*/
    if (lib->count == 0) {
        fclose(gbv);
        return 0;
    }

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


int extract_data_docs(const char *docname, Document *docs) {
    struct stat info;

    /*Extrair info e conferir existência*/
    if (stat(docname, &info) != 0) {
        perror("Falha no stat\n");
        return -1;
    }

    /*Verificar se nome cabe no max*/
    unsigned long int tam_nome = strlen(docname);
    if (tam_nome > MAX_NAME -1) {
        perror("Nome do docs muito longo\n");
        return -1;
    }

    /*Atribuir os atributos*/
    //Inicializar vetor do nome com espaços
    memset(docs->name, 0, MAX_NAME);

    strncpy(docs->name, docname, tam_nome +1);
    docs->name[MAX_NAME - 1] = 0;

    docs->size = info.st_size;
    docs->date = time(NULL);
    //Info a ser alterada posteriormente no gbv_add
    docs->offset = 0;

    return 0;
}

int docs_name_cmp(const Library *lib, const char *docname) {
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->docs[i].name, docname) == 0) {
            return i;
        }
    }

    return -1;
}


//Substituir docs no .gbv, idx do docs que será substituido
unsigned long int replace_docs(FILE *gbv, FILE *docs, Library *lib, int idx, Document docs_info, unsigned long int offset_lib_original) {
    int idx_next_doc = idx + 1;
    int idx_max = lib->count -1;

    long offset_docs = lib->docs[idx].offset;
    long size_old_docs = lib->docs[idx].size;
    long size_new_docs = docs_info.size;
    unsigned long int offset_lib = offset_lib_original;
    //Pegar valor absoluto
    long diff_size = labs(size_old_docs - size_new_docs);

    /*Caso 1: Docs aumentou de tamanho*/
    if (size_new_docs > size_old_docs) {
        //Empurrar
        move_docs_back(gbv, lib, idx_next_doc, idx_max, diff_size);

        //Novo valor offset do diretório
        offset_lib = offset_lib + diff_size;
    }

    /*Caso 2: Docs diminuiu de tamanho */
    if (size_new_docs < size_old_docs) {
        //Puxar
        move_docs_forward(gbv, lib, idx_next_doc, idx_max, diff_size);

        //Novo valor offset do diretório
        offset_lib = offset_lib - diff_size;

        //Truncar no ponto do diretório
        fseek(gbv, offset_lib, SEEK_SET);
        int size_to_truncate = fileno(gbv);
        ftruncate(size_to_truncate, ftell(gbv));

    }
    /*Caso 3: Docs permanece do mesmo tamanho, ou seja, apenas reescrever o diretório*/
    //Escrever docs
    write_docs(gbv, docs, size_new_docs, offset_docs);

    return offset_lib;
}


//Adiciona docs, trata caso de ser repetido

int gbv_add(Library *lib, const char *archive, const char *docname) {
    long offset_docs;
    long offset_lib;

    FILE *gbv = fopen(archive, "r+b");

    /*Abrir novo docs somente leitura*/
    FILE *docs = fopen(docname, "rb");

    if (!gbv || !docs) {
        perror("Erro ao abrir arquivo no gbv_add\n");
        return -1;
    }

    /*Se está vazio ou apenas com qtde docs e offset lib*/
    int empty = gbv_empty(gbv);
    int empty_docs = 0;

    //Pegar offset do diretório e atualizar
    unsigned long int offset_lib_original;
    fseek(gbv, sizeof(int), SEEK_SET);
    fread(&offset_lib_original, sizeof(unsigned long int), 1, gbv);

    /*Extrair dados do documento*/
    Document docs_info;
    //memset(&docs_info, 0, sizeof(docs_info));

    int get_data = extract_data_docs(docname, &docs_info);
    if (get_data != 0) {
        perror("Erro ao coletar os dados do docs\n");
        return -1;
    }

    /*Caso 1: Arquivo é repetido*/
    //Procurar docs repetido
    int repeated_docs = docs_name_cmp(lib, docname);

    //Se for repetido
    if (repeated_docs != -1) {
        unsigned long int new_offset_lib = replace_docs(gbv, docs, lib,repeated_docs, docs_info,offset_lib_original);

        //Atualizar informações do docs, exceto o offset, que permanece
        long offset_repeated_docs = lib->docs[repeated_docs].offset;
        lib->docs[repeated_docs] = docs_info;
        lib->docs[repeated_docs].offset = offset_repeated_docs;


        //Escrever diretório
        lib_offset_write(gbv, lib, new_offset_lib);
        lib_write(gbv, lib, new_offset_lib);

        //Liberar memória
        fclose(gbv);
        fclose(docs);
        return 0;
    }

    /*Colocar docs_info (novo) no vetor do lib*/
    // Se for o primeiro elemento, temos que alocar espaço
    if (lib->count == 0) {
        lib->docs = malloc(sizeof(Document));
        empty_docs = 1;
    } else {
        lib->docs = realloc(lib->docs, sizeof(Document) * (lib->count + 1));
    }

    //TODO! [AVISO] Se for o primeiro elemento ele vai estar na posição 0 do vetor
    lib->docs[lib->count] = docs_info;
    long size_new_docs = lib->docs[lib->count].size;

    /*Atualizar o lib*/
    lib->count++;

    /*Escrever count no .gbv*/
    lib_count_write(gbv, lib);

    /*Caso 2: .gbv está inicialmente vazio ou com apenas qtde de docs e offset lib*/
    if (empty || empty_docs) {
        // Library fica logo após count + offset_lib_area + primeiro docs
        offset_lib = sizeof(int) + sizeof(unsigned long int) +size_new_docs;

        // Primeiro docs fica logo depois do count + offset_lib_area
        offset_docs = sizeof(int) + sizeof(unsigned long int);

        //Escrever info do offset do diretório
        lib_offset_write(gbv, lib, offset_lib);


        /*Atualizar offset do documento*/
        lib->docs[lib->count -1].offset = offset_docs;

        //Escrever docs

        //Saber se o arquivo é muito grande
        write_docs(gbv, docs, size_new_docs, offset_docs);

        //Escrever diretório
        lib_write(gbv, lib, offset_lib);

        fclose(docs);
        fclose(gbv);

        return 0;
    }

    /*Caso 3: .gbv não vazio e Docs não é repetido*/

    //Atualizar offset do diretório original
    offset_lib = offset_lib_original + size_new_docs;

    //Escrever info do offset do diretório
    lib_offset_write(gbv, lib, offset_lib);


    //Atualizar offset do documento
    offset_docs = offset_lib_original;
    lib->docs[lib->count -1].offset = offset_docs;

    //Escrever documento

    //Saber se o arquivo é muito grande
    write_docs(gbv, docs, size_new_docs, offset_docs);


    //Escrever diretório
    lib_write(gbv, lib, offset_lib);

    fclose(gbv);
    fclose(docs);

    return 0;
}

//Remover do diretório, .gbv, arrastar os arquivos para frente, atualizar offsets do diretório
int gbv_remove(Library *lib, const char *archive, const char *docname){
    //Abrir gbv
    FILE *gbv = fopen(archive, "r+b");
    if (!gbv) {
        return -1;
    }

    //Pegar offset do diretório
    unsigned long int new_offset_lib = 0;
    unsigned long int offset_lib_original;
    long where_truncate = 0;
    fseek(gbv, sizeof(int), SEEK_SET);
    fread(&offset_lib_original, sizeof(unsigned long int), 1, gbv);

    //Localizar offset do docname
    int idx_to_remove = docs_name_cmp(lib, docname);
    if (idx_to_remove == -1) {
        perror("Erro: Docs não foi encontrado\n");
        return -1;
    }

    //long offset_doc_to_remove = lib->docs[idx_to_remove].offset;
    long size_doc_to_remove = lib->docs[idx_to_remove].size;
    int idx_max = lib->count -1;

    /*Caso 1: É o primeiro e único docs, ou seja, não existirá mais vetor de docs*/
    if (lib->count == 1) {
        where_truncate = sizeof(int) + sizeof(unsigned long int);
        fseek(gbv, where_truncate, SEEK_SET);

        //Truncar logo depois do offset do diretório
        fseek(gbv, where_truncate, SEEK_SET);
        int size_to_truncate = fileno(gbv);
        ftruncate(size_to_truncate, ftell(gbv));

        new_offset_lib = where_truncate;
    }

    /*Caso 2: É o último docs*/
    else if (idx_to_remove == lib->count - 1) {
        //Truncar no final do último
        long offset_next_to_last = lib->docs[idx_to_remove - 1].offset;
        long size_next_to_last = lib->docs[idx_to_remove - 1].size;

        where_truncate = offset_next_to_last + size_next_to_last;
        fseek(gbv, where_truncate, SEEK_SET);
        int size_to_truncate = fileno(gbv);
        ftruncate(size_to_truncate, ftell(gbv));

        //Atualizar offset lib
        new_offset_lib = where_truncate;

        //Atualizar vetor de docs
        lib->docs = realloc(lib->docs, sizeof(Document) * (lib->count - 1));

    }
    /*Caso 3: É um docs no meio do vetor*/
    else {
        //Truncar no final do último
        long offset_last = lib->docs[idx_max].offset;
        long size_last = lib->docs[idx_max].size;

        //Sobrescrever o docs a ser retirado
        move_docs_forward(gbv, lib, idx_to_remove +1, idx_max, size_doc_to_remove);

        //Truncar
        where_truncate = offset_last + size_last;
        fseek(gbv, where_truncate, SEEK_SET);
        int size_to_truncate = fileno(gbv);
        ftruncate(size_to_truncate, ftell(gbv));

        //Atualizar vetor de docs do lib
        for (int i = idx_to_remove; i < idx_max; i++) {
            lib->docs[i] = lib->docs[i + 1];
        }

        lib->docs = realloc(lib->docs, sizeof(Document) * (lib->count - 1));

        //Atualizar offset lib
        new_offset_lib = offset_lib_original - size_doc_to_remove;
    }

    lib->count--;
    //Atualizar o count
    lib_count_write(gbv, lib);

    //Atualizar offset do lib
    lib_offset_write(gbv, lib, new_offset_lib);

    //Atualizar lib
    lib_write(gbv, lib, new_offset_lib);

    fclose(gbv);

    return 0;
}

int gbv_list(const Library *lib) {
    if (lib->count == 0) {
        printf("Erro: Não há o que listar\n");
        return -1;
    }
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

    //Liberar memória
    free(lib->docs);
    return 0;
}

int is_text_docs(const char *docname) {
    //Pegar o último ponto
    const char *ext = strrchr(docname, '.');
    if (ext == NULL) return -1;

    ext++;

    char lower[16];
    int i = 0;
    while (ext[i] && i < 15) {
        lower[i] = (char)tolower(ext[i]);
        i++;
    }
    lower[i] = '\0';

    int amount_ext = 7;
    const char *text_ext[] = {"txt", "c", "h", "md", "json", "csv", "html"};

    for (int j = 0; j < amount_ext; j++) {
        if (strcmp(lower, text_ext[j]) == 0)
            return 1;
    }

    return 0;
}

void view_docs(char *buffer, int text, long size_read) {
    char *read_buffer = malloc(size_read);
    if (text == 1) {
        fwrite(buffer, size_read, 1, stdout);
        printf("\n");
    } else {
        for (unsigned char *p = (unsigned char *)buffer; *p != '\0'; p++) {
            printf("%02X ", *p);
        }
    }

    free(read_buffer);
}

char read_only_one_char() {
    char op;
    scanf(" %c", &op);
    int ln;
    while ((ln = getchar()) != '\n' && ln != EOF);
    return op;
}

int gbv_view(const Library *lib, const char *archive, const char *docname) {
    if (lib->count == 0) {
        printf("Nada foi inserido no .gbv\n");
        return -1;
    }

    //Pegar tamanho e offset do docs
    int idx = docs_name_cmp(lib, docname);
    if (idx == -1) {
        printf("Docs não inserido no .gbv\n");
        free(lib->docs);
        return -1;
    }
    long size_doc_to_view = lib->docs[idx].size;
    long offset_doc_to_view = lib->docs[idx].offset;

    //Abrir .gbv
    FILE *gbv = fopen(archive, "rb");

    if (!gbv) {
        perror("Erro: Não foi possível abrir o .gbv no view\n");
        return -1;
    }

    //Identificar se docs é texto
    int text_doc = is_text_docs(docname);

    //TODO TODO alterar valor
    long size_buffer = 10;
    char *buffer = malloc(size_buffer);

    long start_doc = offset_doc_to_view;
    long end_doc = start_doc + size_doc_to_view;

    //Começa no início do arquivo
    long mini_size_buffer = size_buffer;
    long start = start_doc;
    long end = start + size_buffer;

    rewind(gbv);
    fseek(gbv, start, SEEK_SET);
    fread(buffer, size_buffer, 1, gbv);
    view_docs(buffer, text_doc, size_buffer);

    //Ler operação
    char op = read_only_one_char();

    while (op != 'q') {
        switch (op) {
            case 'n': {
                start = end;
                if (start == end_doc || size_doc_to_view <= size_buffer) {
                    printf("----------------\nFim do documento\n----------------\n");
                    break;
                }
                end = end + size_buffer;

                if (end <= end_doc) {
                    //Imprimir
                    fseek(gbv, start, SEEK_SET);
                    fread(buffer, size_buffer, 1, gbv);
                    view_docs(buffer, text_doc, size_buffer);
                } else {
                    mini_size_buffer = end_doc - start;
                    end = end_doc;

                    char *mini_buffer = malloc(mini_size_buffer);

                    fseek(gbv, start, SEEK_SET);
                    fread(mini_buffer, mini_size_buffer, 1, gbv);
                    view_docs(mini_buffer, text_doc, mini_size_buffer);

                    free(mini_buffer);

                    break;
                }
                break;
            }
            case 'p': {
                end = start;
                if (end == start_doc || size_doc_to_view <= size_buffer) {
                    printf("-------------------\nInicio do documento\n-------------------\n");
                    break;
                }
                start = start - size_buffer;

                if (start > start_doc) {
                    //Imprimir
                    fseek(gbv, start, SEEK_SET);
                    fread(buffer, size_buffer, 1, gbv);
                    view_docs(buffer, text_doc, size_buffer);
                } else {
                    start = start_doc;
                    mini_size_buffer = end - start;

                    char *mini_buffer = malloc(mini_size_buffer);

                    fseek(gbv, start, SEEK_SET);
                    fread(mini_buffer, mini_size_buffer, 1, gbv);
                    view_docs(mini_buffer, text_doc, mini_size_buffer);

                    free(mini_buffer);

                    break;
                }

                break;
            }
            default: {
                printf("Entre com uma opção válida: 'n' para o próximo bloco; 'p' para o bloco anterior; 'q' para sair\n");
                break;
            }
        }
        op = read_only_one_char();
    }
    //Liberar memória
    free(buffer);
    free(lib->docs);
    fclose(gbv);

    return 0;
}


