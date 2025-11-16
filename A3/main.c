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

int update_position(character *actor){
    if(actor->control->left){
        //printf("aqui\n");
        actor->walk(actor,LEFT);
        return LEFT;
    }
    if(actor->control->right){
        actor->walk(actor, RIGHT);
        return RIGHT;
    }

    return 0;
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
    //unsigned char key[ALLEGRO_KEY_MAX]; //Definir vetor de teclas, com tamanho de todas as teclas possíveis
    //memset(key, 0, sizeof(key)); //Zerar esse vetor

    /*Criar evento*/
    //bool done = false;
    //bool redraw = true;
    //bool active = false;
    ALLEGRO_EVENT event;

    int dir = 0;
    /*Loop principal*/
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);
        

        if(event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP){
            if (event.keyboard.keycode == ALLEGRO_KEY_A) {
                joystick_left(shadow->control);
                
            }																															
			if (event.keyboard.keycode == ALLEGRO_KEY_D) {
                joystick_right(shadow->control);
                //printf("%d\n", event.keyboard.keycode);
                				
            }																							
			if (event.keyboard.keycode == ALLEGRO_KEY_W) {
                joystick_up(shadow->control);
                
            }																														
			if (event.keyboard.keycode == ALLEGRO_KEY_S) {
                joystick_down(shadow->control);
               
            }			
       
        }else if (event.type == ALLEGRO_EVENT_TIMER){
            /*Plotar background*/
            al_draw_bitmap(background, 0, 0, 0);

            shadow->position = update_position(shadow);
            int frameX;
            
            if (shadow->position == LEFT){
                dir = ALLEGRO_FLIP_HORIZONTAL;
                //
            } else if (shadow->position == RIGHT){
                dir = 0;
            } 

            /*Sprite parada*/
            if(shadow->position == 0){
                frameX = (al_get_timer_count(timer)/10) % 5;
                shadow_souceY = 0;
                al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, dir);
            } 
            if(shadow->position == LEFT || shadow->position == RIGHT){
                frameX = (al_get_timer_count(timer)/3) % 14;
                shadow_souceY = 30;
                al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, dir);
            }


            
        

            /*Plotar shadow*/
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %d Y: %d", shadow->basics->x, shadow->basics->y);
            

            al_flip_display();
        }else if(event.type == 42 || event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
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