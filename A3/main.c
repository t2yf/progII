#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

/*Arquivos .h*/
#include "utils.h"
#include "element.h"
#include "character.h"

#define KEY_SEEN     1
#define KEY_DOWN     2
#define X_SCREEN     1280
#define Y_SCREEN     720



int main(){
    /*Inicializações básicas*/
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_image_addon(), "image addon");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); //30 FPS
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");


    /*Inicializar bitmaps*/
    ALLEGRO_BITMAP *background = al_load_bitmap("assets/background.png");
    ALLEGRO_BITMAP *shadow_sprite = al_load_bitmap("assets/shadow-spritesheets.png");


    /*Primeiros eventos*/
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));


    /*Criar personagens*/
    character *shadow = character_create(10, 10, 22, 30, shadow_sprite);

    /*Atributos para plotar personagens*/
    float shadow_width = shadow->basics->width;
    float shadow_height = shadow->basics->height;
    

    /*Definir teclado*/
    unsigned char key[ALLEGRO_KEY_MAX]; //Definir vetor de teclas, com tamanho de todas as teclas possíveis
    memset(key, 0, sizeof(key)); //Zerar esse vetor

    /*Criar evento*/
    bool done = false;
    bool redraw = true;
    bool active = false;
    ALLEGRO_EVENT event;

    /*Loop principal*/
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {   
            //analisa o vetor para identificar qual ação tomar
            case ALLEGRO_EVENT_TIMER:
                
                if(key[ALLEGRO_KEY_UP])
                    //
                if(key[ALLEGRO_KEY_DOWN])
                    //
                if(key[ALLEGRO_KEY_LEFT]){
                    active = true;
                    character_move_x(shadow, LEFT);
                }
                if(key[ALLEGRO_KEY_RIGHT]){
                    active = true;
                    character_move_x(shadow, RIGHT);
                }
                    

                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;

                redraw = true;
                break;
            //quando uma tecla é pressionada pela primeira vez, dois bits são definidos como 1 na entrada correspondente a tecla
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
                break;
            // quando eh solto a tecla o bit do Key_down vira 0
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= ~KEY_DOWN;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(active){
            shadow_width += al_get_bitmap_width(shadow_sprite) / 14;
        } else
            shadow_width = shadow->basics->width;

        if(shadow_width >= (al_get_bitmap_width(shadow_sprite) - 22))
            shadow_width = 0;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            //[TODO]
            /*Plotar background*/
            al_draw_bitmap(background, 0, 0, 0);

            /*Plotar shadow*/
            //[TODO] retirar hardcoded
            al_draw_bitmap_region(shadow_sprite, shadow_width, shadow_height, 22, 30, 10, 750, 0);



            al_flip_display();

            redraw = false;
        }
    }

    /*Destruir bitmaps*/
    al_destroy_bitmap(background);
    al_destroy_bitmap(shadow_sprite);

    /*Destruir*/
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;

}