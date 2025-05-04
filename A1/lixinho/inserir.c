void inserir_membros(Archiver *archiver, char **membros, int num_arq){
    /*IMPORTANTE: Como essa operação vem após o verifica_existe_archiver o *archiver já possui o arquivo.vc carregado*/

    //Abrir arquivo binário
    FILE *arq = fopen(archiver->nome, "r+b");
    if(!arq){
        perror("Erro ao abrir o arquivo\n");
        return;
    }

    int qtde_membros_arq = archiver->dir.qtde_membros;

    //!!! Pensar se for o primeiro membro a ser inserido -> não tem o tamanho do maior membro -> VER SE FAZ SENTIDO!!
    //!!!VERIFICAR SE O ARQUIVO JÁ EXISTE -> SUBSTITUIÇÃO
        /*[CASO 3] Um dos arquivos já existe, ou seja, precisa de substituição*/
    //!!!Como ajusto offset de cada membro


    //Fazer buffer estático de 1024

    //Realocar vetor do diretório de acordo com a quantidade de membros
    int nova_cap = archiver->dir.qtde_membros + num_arq;
    Membro *vetor_realloc = realloc(archiver->dir.membros, sizeof(Membro)*nova_cap);
    if(!vetor_realloc){
        perror("Erro ao realocar o diretório\n");
        return;
    }
    archiver->dir.membros = vetor_realloc; 


    //Extrair informação dos membros novos
    for(int i=0; i<num_arq; i++){
        const char *nome_membro = membros[i];
        Membro new_membro; 
        extrair_info_membro(archiver, nome_membro, &new_membro);

        //Adicionar informações do novo membro no diretório
        archiver->dir.membros[archiver->dir.qtde_membros] = new_membro;
        archiver->dir.qtde_membros++;
    }

    //Colocar diretório em um arquivo temporário
    FILE *tmp = fopen("tmp.vc", "wb");
    if(!tmp){
        perror("[Inserir] Erro ao criar arquivo temporário\n");
        return;
    }

    //Escrever diretório no arquivo temporário
    fwrite(&archiver->dir.qtde_membros, sizeof(int), 1, tmp);
    fwrite(&archiver->dir.cap, sizeof(int), 1, tmp);
    fwrite(&archiver->dir.membros, sizeof(Membro), archiver->dir.qtde_membros, tmp);

    //Fazer o buffer (tamanho do maior membro)
    size_t max_tam = 0;
    for(int i = 0; i< archiver->dir.qtde_membros; i++){
        if(archiver->dir.membros[i].tamanho_comprimido > max_tam)
            max_tam = archiver->dir.membros[i].tamanho_comprimido;
    }

    char *buffer = malloc(max_tam);
    if(!buffer){
        perror("Erro ao alocar buffer\n");
        return;
    }

    /*IMPORTANTE: Caso seja os primeiros membros a serem inseridos, não tem membros antigos para ler*/
    if(qtde_membros_arq > 0){
        //Ir para parte dos membros no original
        //!!!Se tirar cap, mudar sizeof(int)*2 para sizeof(int)
        fseek(arq, sizeof(int)*2 + sizeof(Membro)*qtde_membros_arq, SEEK_SET);
        //Ir para o fim do arquivo temporário
        fseek(tmp, 0, SEEK_END);

        //Para cada membro, copiar do arquivo original para o temporário !!!Escreve os novos tbm???
        for(int i=0; i< qtde_membros_arq; i++){
            Membro *m = &archiver->dir.membros[i];

            fseek(arq, m->offset, SEEK_SET);

            fread(buffer, 1, m->tamanho_comprimido, arq);
            //Atualizar novo offset com a posição atual do tmp
            m->offset = ftell(tmp);

            fwrite(buffer, 1, m->tamanho_comprimido, tmp);
        }
    }
    

    //Escrever os membros novos em tmp
    int arq_novos = qtde_membros_arq;
    for(int i = 0; i < num_arq; i++){
        Membro *m = &archiver->dir.membros[arq_novos + i];

        FILE *membro_arq = fopen(m->nome, "rb");
        if(!membro_arq){
            perror("Erro ao abrir o membro novo");
            continue;
        }

        fread(buffer, 1, m->tamanho_comprimido, membro_arq);

        //Atualiza o offset com a posição do tmp
        m->offset = ftell(tmp);
        fwrite(buffer, 1, m->tamanho_comprimido, tmp);

        fclose(membro_arq);
    }

    //Substituir o arquivo antigo pelo novo
    fclose(arq);
    fclose(tmp);
    rename("tmp.vc", archiver->nome);





}


//Tamanho do diretório
unsigned long tam_dir = sizeof(int) + archiver->dir.qtde_membros * sizeof(Membro);

//Novo final do diretório
unsigned long fim_dir = tam_dir + tam_info; 

 //Descobrir o byte do final do arquivo
 fseek(arq, 0, SEEK_END);
 unsigned long fim_arq = ftell(arq);

  //Atualizar informações no diretório
  archiver->dir.membros[archiver->dir.qtde_membros + i].offset = pos_offset;
  archiver->dir.membros[archiver->dir.qtde_membros + i].tamanho_comprimido = tamanho_membro;
  archiver->dir.membros[archiver->dir.qtde_membros + i].tamanho = tamanho_membro;


//ACHAR O TAMANHO DO DIRETORIO
fseek(archiver, 0, SEEK_END);
long tam = ftell(archiver);