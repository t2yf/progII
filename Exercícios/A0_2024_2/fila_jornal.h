#ifndef __FILA_JORNAL__
#define __FILA_JORNAL__

typedef struct noticia{
    char titulo[33]; //guardar \0 por isso 33 caracteres
    char texto[513];
    char idade; //economizar 1byte

    struct noticia *prox;
} noticia;

typedef struct fila_jornal{
    noticia *inicio;
    noticia *final;
} fila_jornal;


noticia* criar_noticia();
int inserir_noticia(noticia *nova, fila_jornal *fila);
noticia* remover_noticia(fila_jornal *fila);
void atualizar_fila(fila_jornal *fila);
void destruir_fila(fila_jornal *fila);


#endif