#include "inserir.h"
#include "manip_arc.h"
/*!!!!SE O MEMBRO JÁ EXISTIR -> SUBSTITUIR*/
/*TRATAR CASO SE O ARQUIVO FOR MTO MAIOR QUE 1024*/
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
            continue;
        }

        //Descobrir tamanho do novo membro e lendo para o buffer
        fseek(novo_membro, 0, SEEK_END);
        size_t tam_membro = ftell(novo_membro);
        rewind(novo_membro);
        fread(buffer, tam_membro, 1, novo_membro);

        // Escrevendo o novo membro no archiver
        fseek(arq, 0, SEEK_END);
        long final_arq = ftell(arq);
        fwrite(buffer, tam_membro, 1, arq);

        fclose(novo_membro);

        //Extrair informações no novo membro
        const char *nome_membro = membros[i];

        // TODO PENSAR QUE O DIRETÓRIO NÃO EXISTE, O PRIMEIRO OFFSET SERÁ 0, OS SUBSEQUENTES SERÃO OFFSET + TAMANHO GUARDADO
        // PROVAVELMENTE FAZER UM IF, NÃO SEI DA UM JEITO THAMIKO

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

    //TODO arrumar offset, empurrar muda o offset

    //Escrever informações do diretório do arquivo, acaba reescrevendo o diretório inteiro
    fseek(arq, 0, SEEK_SET);
    fwrite(&archiver->dir.qtde_membros, sizeof(int), 1, arq);
    fwrite(archiver->dir.membros, sizeof(Membro), archiver->dir.qtde_membros, arq);

    fclose(arq);
}

/*TRATAR CASO DE SUBSTITUIÇÃO*/
void inserir_membros_comprimidos(Archiver *archiver, char **membros, int num_arq){

}

