#include "inserir.h"
#include "manip_arc.h"
/*!!!!SE O MEMBRO JÁ EXISTIR*/
void inserir_membros(Archiver *archiver, char **membros, int num_arq){

    /*IMPORTANTE: Como essa operação vem após o verifica_existe_archiver o *archiver já possui o arquivo.vc carregado*/

    //Abrir arquivo binário
    FILE *arq = fopen(archiver->nome, "r+b");
    if(!arq){
        perror("Erro ao abrir o arquivo\n");
        return;
    }

    /*Afastar os membros para escrever as informações no diretorio*/
    //Tamanho das informações a serem colocadas no diretório
    unsigned long tam_info = num_arq * sizeof(Membro);


    
    //Ir puxando os membros de trás para frente
    if(archiver->dir.qtde_membros != 0){ // Se não é o primeiro membro
        for(int i = archiver->dir.qtde_membros - 1; i >= 0; i--){
            Membro *m = &archiver->dir.membros[i];
    
            unsigned long ini = m->offset;
            unsigned long fim = m->offset + m->tamanho_comprimido;
            unsigned long ref = ini + tam_info;
    
            mover_membros(arq, ini, fim, ref);
    
            //Atualizar o offset
            m->offset = ref; 
        }
    }
    


    /*Achar final do diretório antigo*/
    unsigned long indice = archiver->dir.qtde_membros;
    unsigned long tam_dir = sizeof(int) + archiver->dir.qtde_membros * sizeof(Membro);

    /*Realocar diretório*/
    unsigned long novo_tam_dir = archiver->dir.qtde_membros + num_arq;
    archiver->dir.membros = realloc(archiver->dir.membros, sizeof(Membro)*novo_tam_dir);
    if (!archiver->dir.membros) {
        perror("Erro ao realocar diretório\n");
        fclose(arq);
        return;
    }

    

    /*Inserir os membros novos no final e informações no diretório*/
    //Procurar final do arquivo
    fseek(arq, 0, SEEK_END);

    for(int i=0; i<num_arq; i++){
        FILE *novo_membro = fopen(membros[i], "rb");

        if(!novo_membro){
            perror("Erro ao abrir o novo membro\n");
            continue;
        }

        //Descobrir tamanho do arquivo
        fseek(novo_membro, 0, SEEK_END);
        size_t tam_membro = ftell(novo_membro);
        
        //Retornar ao início do arquivo
        rewind(novo_membro);

        //Ler membro com um buffer fixo
        char buffer[TAM_BUFFER];

        size_t membro_lido;
        long pos_offset = ftell(arq);

        //Ler e escrever o membro
        while((membro_lido = fread(buffer, 1, TAM_BUFFER, novo_membro)) > 0){
            //Se membro tiver menos de 1024bytes ele escreve o membro inteiro, caso contrário ele corta em pedaços menores
            fwrite(buffer, 1, membro_lido, arq);
        }

        fclose(novo_membro);

        //Extrair informações no novo membro 
        const char *nome_membro = membros[i];
        Membro new_membro;
        extrair_info_membro(archiver, nome_membro, &new_membro);

        //Atualizar a posição do offset
        new_membro.offset = pos_offset;

        archiver->dir.membros[indice] = new_membro;
        indice++;
    }

    //Atualizar tamanho do diretório
    archiver->dir.qtde_membros = novo_tam_dir;

    //Escrever informações do diretório do arquivo, acaba reescrevendo o diretório inteiro
    fseek(arq, 0, SEEK_SET);
    fwrite(&archiver->dir.qtde_membros, sizeof(int), 1, arq);
    fwrite(archiver->dir.membros, sizeof(Membro), archiver->dir.qtde_membros, arq);
    
    fclose(arq);
}