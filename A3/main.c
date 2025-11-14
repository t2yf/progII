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
#include "joystick.h"


#define X_SCREEN     1280
#define Y_SCREEN     720

#define SPRITE_MULT_FACTOR 3.5

void update_position(character *actor){
    if(actor->control->left){
        //printf("aqui\n");
        actor->walk(actor,LEFT);
    }
    if(actor->control->right){
        actor->walk(actor, RIGHT);
    }
}



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
    must_init(shadow_sprite, "shadow_sprite");

    /*[TODO] alegro keyboard*/
    ALLEGRO_KEYBOARD_STATE keystate;

    /*Primeiros eventos*/
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));


    /*Criar personagens*/
    character *shadow = character_create(10, 550, 22, 30, shadow_sprite);


    /*Atributos para plotar personagens*/
    float shadow_width = shadow->basics->width;
    float shadow_height = shadow->basics->height;
    float shadow_sourceX = 0;
    float shadow_souceY = 30;
    

    /*Definir teclado*/
    unsigned char key[ALLEGRO_KEY_MAX]; //Definir vetor de teclas, com tamanho de todas as teclas possíveis
    memset(key, 0, sizeof(key)); //Zerar esse vetor

    /*Criar evento*/
    //bool done = false;
    //bool redraw = true;
    //bool active = false;
    ALLEGRO_EVENT event;

    /*Loop principal*/
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);
        /*[TODO] alegro keyboard*/
        al_get_keyboard_state(&keystate);

        	

       if(event.type == 30){
            update_position(shadow);

            al_clear_to_color(al_map_rgb(0, 0, 0));
            /*Plotar background*/
            al_draw_bitmap(background, 0, 0, 0);

            shadow_sourceX += (al_get_bitmap_width(shadow_sprite) - 22) % 14;	
            
            /*Plotar shadow*/
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %d Y: %d", shadow->basics->x, shadow->basics->y);
            al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX, shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, 0);

            al_flip_display();
       }else if(event.type == 10 || (event.type == 12)){
            if (event.keyboard.keycode == 1) joystick_left(shadow->control);																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda) (!)
			else if (event.keyboard.keycode == 4) {
                joystick_right(shadow->control);
                				
            }																							//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita) (!)
			else if (event.keyboard.keycode == 23) joystick_up(shadow->control);																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima) (!)
			else if (event.keyboard.keycode == 19) joystick_down(shadow->control);							
       } else if(event.type == 42 || al_key_down(&keystate, ALLEGRO_KEY_ESCAPE))
            break;

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