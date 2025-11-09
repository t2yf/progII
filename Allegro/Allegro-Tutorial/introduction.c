//gcc introduction.c -o introduction $(pkg-config allegro-5 allegro_main-5 allegro_font-5 --libs --cflags)

/*Header Files*/
#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

int main()
{   
    if(!al_init()) /*Configurar elementos essênciais*/
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }
    if(!al_install_keyboard()) /*Permitir inputs de teclado*/
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }
  

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); /*fps*/
    if(!timer)
    {
        printf("couldn't initialize timer\n");
        return 1;
    }
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); /*lista orientadada a eventos*/
    if(!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }
    ALLEGRO_DISPLAY* disp = al_create_display(640, 480); /*tamanho do display*/
     if(!disp)
    {
        printf("couldn't initialize display\n");
        return 1;
    }
    ALLEGRO_FONT* font = al_create_builtin_font(); /*fonte feita de pixels*/
    if(!font)
    {
        printf("couldn't initialize font\n");
        return 1;
    }


    /*primeiras coisas da lista de eventos*/
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false; // verificador
    bool redraw = true;
    ALLEGRO_EVENT event;

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
            al_draw_text(font, al_map_rgb(159, 43, 104), 0, 0, 0, "Allegro Tutorial");
            al_flip_display(); /*confirma resultado > mostra*/

            redraw = false;
        }
    }
    /*shutdown*/
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}