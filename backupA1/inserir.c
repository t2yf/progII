#include "inserir.h"
#include "manip_arc.h"
#include "lz.h"
/*!!!!SE O MEMBRO JÁ EXISTIR -> SUBSTITUIR*/

int verifica_membro_existe(Archiver *archiver, char *nome_membro){
    //printf("Entrou Verifica membro existe\n");
    for(int i = 0; i < archiver->dir.qtde_membros; i++){
        /*Se o membro já existir, retorna em qual posição*/
       // printf("Nome do membro subs: %s\n", nome_membro);
        //printf("Nome do membro dir: %s\n", archiver->dir.membros[i].nome);
        if(strcmp(archiver->dir.membros[i].nome, nome_membro) == 0){
           // printf("Ordem repetida: %d\n", i);
            return i;
        }
    }
    /*Se membro for novo, retornar -1*/
    return -1;
}

/*TODO VER PARA -IC + SE FOR SÓ SUBSTITUIR UM MEMBRO*/
void substituicao_membro(FILE *arq, FILE *novo_membro, Archiver *archiver, int ordem_membro, size_t novo_tam_membro){
    printf("Entrou substituição\n");

    //Verificar se o tamanho mudou
    unsigned long dif_tam_membro = novo_tam_membro - archiver->dir.membros[ordem_membro].tamanho_comprimido;

    if(dif_tam_membro > 0){
        //Shiftar outros membros para a direita, se existir outros membros depois dele
        if(ordem_membro != archiver->dir.qtde_membros -1){
            //Do último para o primeiro
            for(int i = archiver->dir.qtde_membros -1; i = ordem_membro + 1; i--){
                Membro *m = &archiver->dir.membros[i];

                unsigned long ini = m->offset;
                printf("ini: %ld\n", ini);
                unsigned long fim = m->offset + m->tamanho_comprimido;
                // TODO TODO AJUSTAR REFERENCIA
                unsigned long ref = ini + dif_tam_membro;
                printf("ref: %ld\n", ref);
                printf("%lu", sizeof(Membro));
                //unsigned long ref = ini + sizeof(Membro) + sizeof(int); //considera o diretório também
        
                mover_membros(arq, ini, fim, ref);
                
                //TODO VERIFICAR SE ELE REALMENTE ATUALIZA O OFFSET PARA FORA
                //Atualizar o offset
                m->offset = ref;
                //Atualizar tamanho do membro
                //TODO CUIDAR TAMANHO COMPRIMIDO, OUTRA FUNÇÃO TALVEZ
                m->tamanho_original = novo_tam_membro;

                printf("Informações extraídas do membro:\n");
                printf("  Nome: %s\n", m->nome);
                printf("  UID: %u\n", m->UID);
                printf("  Tamanho original: %ld bytes\n", m->tamanho_original);
                printf("  Tamanho compactado: %ld bytes\n", m->tamanho_comprimido);
                printf("  Data de modificação: %s", ctime(&m->data_modificacao)); // já inclui \n
                printf("  Ordem no diretório: %d\n", m->ordem);
                printf("  Offset: %ld\n", m->offset);
            }
            
        }
        
    } else if(dif_tam_membro < 0){
        //Shiftar outros membros para a esquerda
        if(ordem_membro != archiver->dir.qtde_membros -1){
            //Do primeiro para o último
            for(int i = ordem_membro + 1 ; i = archiver->dir.qtde_membros -1; i++){
                Membro *m = &archiver->dir.membros[i];

                unsigned long ini = m->offset;
                printf("ini: %ld\n", ini);
                unsigned long fim = m->offset + m->tamanho_comprimido;
                // TODO TODO AJUSTAR REFERENCIA
                unsigned long ref = ini + dif_tam_membro;
                printf("ref: %ld\n", ref);
                //unsigned long ref = ini + sizeof(Membro) + sizeof(int); //considera o diretório também
        
                mover_membros(arq, ini, fim, ref);
                
                //TODO VERIFICAR SE ELE REALMENTE ATUALIZA O OFFSET PARA FORA
                //Atualizar o offset
                m->offset = ref;
                //Atualizar tamanho do membro
                m->tamanho_original = novo_tam_membro;

                printf("Informações extraídas do membro:\n");
                printf("  Nome: %s\n", m->nome);
                printf("  UID: %u\n", m->UID);
                printf("  Tamanho original: %ld bytes\n", m->tamanho_original);
                printf("  Tamanho compactado: %ld bytes\n", m->tamanho_comprimido);
                printf("  Data de modificação: %s", ctime(&m->data_modificacao)); // já inclui \n
                printf("  Ordem no diretório: %d\n", m->ordem);
                printf("  Offset: %ld\n", m->offset);
            }
        }
    }
    //Se não o tamanho é o mesmo, então sobrescrever
    unsigned long offset_membro_subs = archiver->dir.membros[ordem_membro].offset;
        
    //Aloca buffer
    char *buffer = aloca_buffer_dinamico(novo_tam_membro); 

    //Colocar membro num buffer
    fread(buffer, novo_tam_membro, 1, novo_membro);

    //Ir para onde membro estava
    fseek(arq, offset_membro_subs, SEEK_SET);

    //Reescrever
    fwrite(buffer, novo_tam_membro, 1, arq);

    //Atualizar tamanho do membro
    archiver->dir.membros[ordem_membro].tamanho_original = novo_tam_membro;

    // fseek(arq, 0, SEEK_SET);
    // fwrite(&archiver->dir.qtde_membros, sizeof(int), 1, arq);
    // fwrite(archiver->dir.membros, sizeof(Membro), archiver->dir.qtde_membros, arq);       
}

void inserir_membros(Archiver *archiver, char **membros, int num_arq){
    /*IMPORTANTE: Como essa operação vem após o verifica_existe_archiver o *archiver já possui o arquivo.vc carregado*/

    //Abrir arquivo binário
    // fprintf(stderr, archiver->nome);
    FILE *arq = fopen(archiver->nome, "r+b");
    if(!arq){
        perror("Erro ao abrir o arquivo\n");
        return;
    }

    // TODO ARRUMAR UM LUGAR DECENTE PARA ESSAS INCIALIZAÇÕES E VERIFICAR SE ELAS SÃO REALMENTE NECESSÁRIAS
    //Tamanho das informações a serem colocadas no diretório
    unsigned long tam_info = num_arq * sizeof(Membro);

    /*Achar final do diretório antigo*/
    unsigned long pos_novo_mem = archiver->dir.qtde_membros;
    unsigned long tam_dir_antigo = sizeof(int) + archiver->dir.qtde_membros * sizeof(Membro);

    /*Realocar diretório*/
    unsigned long novo_tam_dir = archiver->dir.qtde_membros + num_arq;
    archiver->dir.membros = realloc(archiver->dir.membros, sizeof(Membro)*novo_tam_dir);
    if (!archiver->dir.membros) {
        perror("Erro ao realocar diretório\n");
        fclose(arq);
        return;
    }

    char buffer[TAM_BUFFER];

    /*Inserir os membros novos no final e informações no diretório*/
    for(int i=0; i<num_arq; i++){
        FILE *novo_membro = fopen(membros[i], "rb");

        if(!novo_membro){
            perror("Erro ao abrir o novo membro\n");
            return;
            //continue;
        }

        //Descobrir tamanho do novo membro e lendo para o buffer
        fseek(novo_membro, 0, SEEK_END);
        size_t tam_membro = ftell(novo_membro);
        rewind(novo_membro);


        //TODO  VERIFICAR SE O MEMBRO JÁ EXISTE
        const char *nome_membro = membros[i];
        //Se membro já existe, inserir ele na posição certinha e shiftar se necessário
        int ordem_membro_subs = verifica_membro_existe(archiver, nome_membro);
        if(ordem_membro_subs != -1){
            printf("Ir para substituição\n");
            substituicao_membro(arq, novo_membro, archiver, ordem_membro_subs, tam_membro);
            //TODO, ELE ESCREVE O DIRETÓRIO DE NOVO, ACHO QUE É UM PROBLEMA -> RETORNAR SÓ O QUE FOI MODIFICADO?
            continue; //Já substituiu um membro, logo ir para o próximo
        
        }

        //Se o arquivo for muito grande
        if(tam_membro > TAM_BUFFER){
            size_t aux_tam_membro = tam_membro;
            while(aux_tam_membro > 0){
                size_t tam_ler;
                if(aux_tam_membro < TAM_BUFFER)
                    tam_ler = aux_tam_membro;
                else 
                    tam_ler = TAM_BUFFER;

                fread(buffer, tam_ler, 1, novo_membro);
                //Escrever parte do novo membro no archiver
                fseek(arq, 0, SEEK_END);
                long final_arq = ftell(arq);
                fwrite(buffer, tam_ler, 1, arq);

                //Pegar outro pedaço do arquivo
                aux_tam_membro = aux_tam_membro - tam_ler;

            }
        } else{
            fread(buffer, tam_membro, 1, novo_membro);
            // Escrevendo o novo membro no archiver
            fseek(arq, 0, SEEK_END);
            long final_arq = ftell(arq);
            fwrite(buffer, tam_membro, 1, arq);
        }
        

        fclose(novo_membro);

        //Extrair informações no novo membro
        //const char *nome_membro = membros[i];

        //Se for o primeiro membro a ser inserido, offset é o fim do diretório
        long pos_offset;
        if(archiver->dir.qtde_membros == 0){
            pos_offset = sizeof(int) + archiver->dir.qtde_membros * sizeof(Membro); // 4bytes
        } else {
            //Offset do arquivo anterior + tam
            long offset_mem_anterior = archiver->dir.membros[pos_novo_mem - 1].offset;
            long tam_mem_anterior = archiver->dir.membros[pos_novo_mem - 1].tamanho_comprimido;
            pos_offset = offset_mem_anterior + tam_mem_anterior;
            //pos_offset = final_arq;
        }

        //long pos_offset = 0;

        Membro new_membro;
        extrair_info_membro(archiver, nome_membro, &new_membro);

        //Atualizar a posição do offset
        new_membro.offset = pos_offset;

        archiver->dir.membros[pos_novo_mem] = new_membro;
        pos_novo_mem++;
        //Atualizar tamanho do diretório
        archiver->dir.qtde_membros += 1;

        // printf("Informações extraídas do membro:\n");
        // printf("  Nome: %s\n", new_membro.nome);
        // printf("  UID: %u\n", new_membro.UID);
        // printf("  Tamanho original: %ld bytes\n", new_membro.tamanho_original);
        // printf("  Tamanho compactado: %ld bytes\n", new_membro.tamanho_comprimido);
        // printf("  Data de modificação: %s", ctime(&new_membro.data_modificacao)); // já inclui \n
        // printf("  Ordem no diretório: %d\n", new_membro.ordem);
        // printf("  Offset: %ld\n", new_membro.offset);
    }

    /*Afastar os membros para escrever as informações no diretorio*/
    //Ir puxando os membros de trás para frente
    for(int i = archiver->dir.qtde_membros - 1; i >= 0; i--){
        Membro *m = &archiver->dir.membros[i];

        unsigned long ini = m->offset;
        //printf("ini: %ld\n", ini);
        unsigned long fim = m->offset + m->tamanho_comprimido;
        // TODO TODO AJUSTAR REFERENCIA
        unsigned long ref = ini + sizeof(Membro) * num_arq;
        //printf("ref: %ld\n", ref);
        //printf("%lu", sizeof(Membro));
        //unsigned long ref = ini + sizeof(Membro) + sizeof(int); //considera o diretório também

        mover_membros(arq, ini, fim, ref);

        //Atualizar o offset
        m->offset = ref;

        // printf("Informações extraídas do membro:\n");
        // printf("  Nome: %s\n", m->nome);
        // printf("  UID: %u\n", m->UID);
        // printf("  Tamanho original: %ld bytes\n", m->tamanho_original);
        // printf("  Tamanho compactado: %ld bytes\n", m->tamanho_comprimido);
        // printf("  Data de modificação: %s", ctime(&m->data_modificacao)); // já inclui \n
        // printf("  Ordem no diretório: %d\n", m->ordem);
        // printf("  Offset: %ld\n", m->offset);
    }

    //TODO arrumar offset, empurrar muda o offset

    //Escrever informações do diretório do arquivo, acaba reescrevendo o diretório inteiro
    fseek(arq, 0, SEEK_SET);
    fwrite(&archiver->dir.qtde_membros, sizeof(int), 1, arq);
    fwrite(archiver->dir.membros, sizeof(Membro), archiver->dir.qtde_membros, arq);

    fclose(arq);
}

/*TRATAR CASO DE SUBSTITUIÇÃO*/
void inserir_membros_comprimidos(Archiver *archiver, char **membros, int num_arq){
    /*IMPORTANTE: Como essa operação vem após o verifica_existe_archiver o *archiver já possui o arquivo.vc carregado*/

    //Abrir arquivo binário
    FILE *arq = fopen(archiver->nome, "r+b");
    if(!arq){
        perror("Erro ao abrir o arquivo\n");
        return;
    }
    
    //Tamanho das informações a serem colocadas no diretório
       //Tamanho das informações a serem colocadas no diretório
       unsigned long tam_info = num_arq * sizeof(Membro);

       /*Achar final do diretório antigo*/
       unsigned long pos_novo_mem = archiver->dir.qtde_membros;
       unsigned long tam_dir_antigo = sizeof(int) + archiver->dir.qtde_membros * sizeof(Membro);
   
       /*Realocar diretório*/
       unsigned long novo_tam_dir = archiver->dir.qtde_membros + num_arq;
       archiver->dir.membros = realloc(archiver->dir.membros, sizeof(Membro)*novo_tam_dir);
       if (!archiver->dir.membros) {
           perror("Erro ao realocar diretório\n");
           fclose(arq);
           return;
       }
   
    //char buffer[TAM_BUFFER];

    /*Comprimir e inserir os membros novos no final. Realocar e colocar informações no diretório*/
    for(int i=0; i<num_arq; i++){
        FILE *novo_membro = fopen(membros[i], "rb");

        if(!novo_membro){
            /*TODO Tratar esse erro melhor, continue passa para próxima parte da função e pode dar erro*/
            perror("Erro ao abrir o novo membro\n");
            return;
            //continue;
        }

        //Descobrir tamanho original do novo membro
        fseek(novo_membro, 0, SEEK_END);
        size_t tam_membro_sem_compr = ftell(novo_membro);
        rewind(novo_membro);

        //Alocar buffer
        char *buffer = aloca_buffer_dinamico(tam_membro_sem_compr);
    
        //Colocar membro num buffer
        fread(buffer, tam_membro_sem_compr, 1, novo_membro);

        //Comprimir novo membro
        char buffer_comprimido[TAM_BUFFER*2]; //out pode acabar sendo maior então deixar um buffer 2 vezes maior
        unsigned long tam_mem_compr;
        tam_mem_compr = LZ_Compress(buffer, buffer_comprimido, tam_membro_sem_compr);
        
        // Escrevendo o novo membro no archiver
        fseek(arq, 0, SEEK_END);
        long final_arq = ftell(arq);
        fwrite(buffer_comprimido, tam_mem_compr, 1, arq);

        fclose(novo_membro);

        //Extrair informações no novo membro
        const char *nome_membro = membros[i];

        //Se for o primeiro membro a ser inserido, offset é o fim do diretório
        long pos_offset;
        if(archiver->dir.qtde_membros == 0){
            pos_offset = sizeof(int) + archiver->dir.qtde_membros * sizeof(Membro); // 4bytes
        } else {
            //Offset do arquivo anterior + tam
            long offset_mem_anterior = archiver->dir.membros[pos_novo_mem - 1].offset;
            long tam_mem_anterior = archiver->dir.membros[pos_novo_mem - 1].tamanho_comprimido;
            pos_offset = offset_mem_anterior + tam_mem_anterior;
            //pos_offset = final_arq;
        }

        //TODO Ver se ele realmente atualiza isso no memebro, talvez fazer um ponteiro
        Membro new_membro;
        extrair_info_membro(archiver, nome_membro, &new_membro);

        //Atualizar a posição do offset
        new_membro.offset = pos_offset;
        new_membro.tamanho_comprimido = tam_mem_compr;

        archiver->dir.membros[pos_novo_mem] = new_membro;
        pos_novo_mem++;
        //Atualizar tamanho do diretório
        archiver->dir.qtde_membros += 1;

        printf("Informações extraídas do membro:\n");
        printf("  Nome: %s\n", new_membro.nome);
        printf("  UID: %u\n", new_membro.UID);
        printf("  Tamanho original: %ld bytes\n", new_membro.tamanho_original);
        printf("  Tamanho compactado: %ld bytes\n", new_membro.tamanho_comprimido);
        printf("  Data de modificação: %s", ctime(&new_membro.data_modificacao)); // já inclui \n
        printf("  Ordem no diretório: %d\n", new_membro.ordem);
        printf("  Offset: %ld\n", new_membro.offset);
    }

    /*Afastar os membros para escrever as informações no diretorio*/
    //Ir puxando os membros de trás para frente
    for(int i = archiver->dir.qtde_membros - 1; i >= 0; i--){
        Membro *m = &archiver->dir.membros[i];

        unsigned long ini = m->offset;
        printf("ini: %ld\n", ini);
        unsigned long fim = m->offset + m->tamanho_comprimido;
        // TODO TODO AJUSTAR REFERENCIA
        unsigned long ref = ini + sizeof(Membro) * num_arq;
        printf("ref: %ld\n", ref);
        printf("%lu", sizeof(Membro));
        //unsigned long ref = ini + sizeof(Membro) + sizeof(int); //considera o diretório também

        mover_membros(arq, ini, fim, ref);

        //Atualizar o offset
        m->offset = ref;

        printf("Informações extraídas do membro:\n");
        printf("  Nome: %s\n", m->nome);
        printf("  UID: %u\n", m->UID);
        printf("  Tamanho original: %ld bytes\n", m->tamanho_original);
        printf("  Tamanho compactado: %ld bytes\n", m->tamanho_comprimido);
        printf("  Data de modificação: %s", ctime(&m->data_modificacao)); // já inclui \n
        printf("  Ordem no diretório: %d\n", m->ordem);
        printf("  Offset: %ld\n", m->offset);
    }


    //Escrever informações do diretório do arquivo, acaba reescrevendo o diretório inteiro
    fseek(arq, 0, SEEK_SET);
    fwrite(&archiver->dir.qtde_membros, sizeof(int), 1, arq);
    fwrite(archiver->dir.membros, sizeof(Membro), archiver->dir.qtde_membros, arq);
    
    fclose(arq);
}

