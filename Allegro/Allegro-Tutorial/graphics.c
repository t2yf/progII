//gcc graphics.c -o graphics $(pkg-config allegro-5 allegro_font-5 allegro_image-5 --libs --cflags)
//gcc graphics.c -o graphics $(pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags)



/*Header Files*/
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h> /*image addon*/
#include <allegro5/allegro_primitives.h> /*permitir desenhar formas simples*/

#define X_Screen 640
#define Y_Screen 480


void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{   
    /*Inicializações*/
    must_init(al_init(), "allegro"); /*Configurar elementos essênciais*/
    must_init(al_install_keyboard(), "keyboard"); /*Permitir inputs de teclado*/
    must_init(al_init_image_addon(), "image addon"); /*Permitir bitmaps (reconhecer imagens)*/
    must_init(al_init_primitives_addon(), "primitives"); /*Permitir desenhar formas simples*/


    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); /*fps*/
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); /*lista orientadada a eventos*/
    must_init(queue, "queue");

    /*smooth > deixar mais suave os elementos primitivos e imagens*/
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(X_Screen, Y_Screen); /*tamanho do display*/
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font(); /*fonte feita de pixels*/
    must_init(font, "font");

    /*IMAGEM*/
    ALLEGRO_BITMAP* jax = al_load_bitmap("jax.jpeg");
    must_init(jax, "jax");


    /*primeiras coisas da lista de eventos*/
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false; // verificador
    bool redraw = true;
    ALLEGRO_EVENT event;

    //Onde começa recorte do bitmap
    float SourceX = 70;
    float SourceY = 250;
    //Largura e altura de corte da imagem
    float SourceW = 300;
    float SourceH = 150;
    //Posição destido no bitmap
    float x_jax = 0;
    float y_jax = 0;

    al_start_timer(timer);
    /*Jogo orientado à frames, é calculado o que vai acontecer no próximo frame e mostrado na tela*/
    while(1)
    {
        al_wait_for_event(queue, &event); /*o programa é travado até que ocorra um evento*/

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER: /*se temporizador disparar um evento (a cada 1/30 seg) avança um quadro*/
                // game logic goes here.
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN: /*como o loop termina (apertar um botão (press any key to exit) ou apertar no X)*/
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        

        /*renderizar (se não rednderizou um quadro e não tem mais eventos para tratar)*/
        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(242, 210, 189));
            al_draw_bitmap_region(jax, SourceX, SourceY, SourceW, SourceH, x_jax, y_jax, 0);/*render imagem*/
            //al_draw_tinted_bitmap(jax, al_map_rgba_f(1, 0, 0, 1), x_jax, y_jax, 0); /*change color*/
            al_draw_text(font, al_map_rgb(159, 43, 104), 0, 0, 0, "Allegro Tutorial"); /*render texto*/
            
            /*desenhar formas simples*/
            al_draw_filled_triangle(35, 350, 85, 375, 35, 400, al_map_rgb_f(0, 1, 0));
            al_draw_filled_rectangle(240, 260, 340, 340, al_map_rgba_f(0, 0, 0.5, 0.5)); //translucido
            al_draw_circle(450, 370, 30, al_map_rgb_f(1, 0, 1), 2);
            al_draw_line(440, 110, 460, 210, al_map_rgb_f(1, 0, 0), 1);
            al_draw_line(500, 220, 570, 200, al_map_rgb_f(1, 1, 0), 1);
            
            //al_draw_bitmap(jax, 50, 70, 0); /*render imagem*/
            al_flip_display(); /*confirma resultado > mostra*/

            redraw = false;
        }
    }
    /*shutdown*/

    //IMAGEM
    al_destroy_bitmap(jax);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}