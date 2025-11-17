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
#include "enemies.h"


#define X_SCREEN     1280
#define Y_SCREEN     720
#define GROUND       550



int update_position(character *actor){
    int retorno = 0;
    /*Shadow no ar ou agachado*/
    if(!actor->ground){
        //Cair aos poucos (a cada frame)
        actor->vy -= GRAVITY;
        actor->jump(actor, UP);
        //Se não atingiu o chão, atingir o chão
        if(actor->basics->y  > GROUND){
            actor->basics->y = GROUND;
            actor->ground = 1;
            actor->vy = 0;
        }

        
    } else if(actor->control->down ){
        //[TODO] fazer ele andar abaixado dps
        return DOWN;
    }
    /*Shadow andando para a direita ou esquerda*/
    if(actor->control->left){
        actor->walk(actor,LEFT);

        retorno = LEFT;
    }
    else if(actor->control->right){
        actor->walk(actor, RIGHT);
        retorno = RIGHT;
    }
    /*Shadow pulando*/
    if(actor->control->up && actor->ground){
        actor->vy = 50;
        actor->jump(actor, UP);
        actor->ground =0;
        retorno = UP;
    }
    
    /*Permite que o personagem possa ir para os lados, mesmo que esteja no ar*/
    return retorno;
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
    //Background inteiro
    ALLEGRO_BITMAP *all_background = al_load_bitmap("assets/background.png");
    must_init(all_background, "background");
    //Pedaço do background
    ALLEGRO_BITMAP *sub_background = al_create_sub_bitmap(all_background, 0, 0, X_SCREEN, Y_SCREEN);
    int map_ajustment = 0;

    //Shadow
    ALLEGRO_BITMAP *shadow_sprite = al_load_bitmap("assets/shadow-spritesheets.png");
    must_init(shadow_sprite, "shadow_sprite");

    //Badniks
    ALLEGRO_BITMAP *badniks_sprite = al_load_bitmap("assets/badniks-spritesheets.png");
    must_init(badniks_sprite, "badniks_sprite");


    /*Primeiros eventos*/
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));


    /*Criar personagens*/
    character *shadow = character_create(10, GROUND, 22, 30, shadow_sprite);

    /*Criar inimigos*/
    enemie *gamigami = enemie_create(700, GROUND+30, 47, 41, 2, PATROL, 0, 0, badniks_sprite);
    //enemie *leon = enemie_create(500, GROUND, 63, 23, 1,  IDLE, 0, 192, badniks_sprite);
    //enemie *rhino = enemie_create(700, GROUND, 39, 32, 2,RUNNER, 0, 122, badniks_sprite);


    /*Atributos para plotar personagens*/
    float shadow_width = shadow->basics->width;
    float shadow_height = shadow->basics->height;
    float shadow_sourceX = 0;
    float shadow_souceY = 30;
    

    /*Criar evento*/
    ALLEGRO_EVENT event;

    int shadow_dir = 0;
    int enemie_dir = 1;
    
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
                				
            }																							
			if (event.keyboard.keycode == ALLEGRO_KEY_W) {
                joystick_up(shadow->control);
                
            }																														
			if (event.keyboard.keycode == ALLEGRO_KEY_S) {
                joystick_down(shadow->control);
               
            }			
            /*Evitar ficar infinitamente no rolling background*/
            if(event.type == ALLEGRO_EVENT_KEY_UP)
                shadow->fix_camera = 0;
       
        }else if (event.type == ALLEGRO_EVENT_TIMER){
            /*Nova posição do Shadow*/
            shadow->position = update_position(shadow);
            int frameX;
            
            if (shadow->position == LEFT){
                shadow_dir = ALLEGRO_FLIP_HORIZONTAL;
            } else if (shadow->position == RIGHT){
                shadow_dir = 0;
            } 

            
            int rolling;
            /*Ajustar Rolling Background*/
            if(shadow->fix_camera){
                /*[TODO]Mudar dps velocidade*/
                rolling = shadow->vx;
            } else rolling = 0;

            map_ajustment += rolling;
            //Evitar passar dos limites
            if(map_ajustment < MIN_MAP_BOUNDARIE_X){
                map_ajustment = 0;
                rolling = 0;
            } 
            if(map_ajustment > MAX_MAP_BOUNDARIE_X - X_SCREEN){
                map_ajustment = MAX_MAP_BOUNDARIE_X - X_SCREEN;
            }

            /*Plotar background*/
            al_reparent_bitmap(sub_background, all_background, map_ajustment, 0, X_SCREEN, Y_SCREEN);
            al_draw_bitmap(sub_background, 0, 0, 0);

            /*Plotar o Shadow*/
            /*Shadow parado*/
            if(shadow->position == 0){
                frameX = (al_get_timer_count(timer)/10) % 5;
                //Sprite parada é a primeira
                shadow_souceY = 0;
                al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);
            } 
            /*Shadow andar em x e estiver no chão*/
            if((shadow->position == LEFT || shadow->position == RIGHT) && shadow->ground){
                frameX = (al_get_timer_count(timer)/3) % 14;
                shadow_souceY = 30;
                al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);
            }
            /*Shadow no ar*/
            if(shadow->position == UP || !shadow->ground){
                frameX = (al_get_timer_count(timer)/6) % 4;
                shadow_souceY = 60;
                al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);
            }
            /*Shadow agachado*/
            if(shadow->position == DOWN){
                shadow_souceY = 120;
                al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX, shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);
            }
            /*Shadow rastejando*/
            //[TODO]
            

            /*Plotar inimigos*/

            //Cuidar para a sprite não seguir o background
            int gami_frameX;
            if(gamigami->basics->x > map_ajustment && gamigami->basics->x + gamigami->basics->width < map_ajustment + X_SCREEN){
                //[TODO] fazer mais lento
                if((gamigami->basics->x  + gamigami->basics->width) > 900 - map_ajustment || (gamigami->basics->x  + gamigami->basics->width) < 700 - map_ajustment)
                    enemie_dir = -1*enemie_dir;
                enemie_move(gamigami, enemie_dir, 2, rolling);
                gami_frameX = (al_get_timer_count(timer)/10) %4;
                al_draw_scaled_bitmap(badniks_sprite, gamigami->sourceX + (gamigami->basics->width*gami_frameX), gamigami->sourceY, gamigami->basics->width, gamigami->basics->height, gamigami->basics->x-map_ajustment, gamigami->basics->y, gamigami->basics->width*2, gamigami->basics->height*2, 0);
            }

            /*[TODO] retirar dps*/
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %d Y: %d", shadow->basics->x, shadow->basics->y);
            

            al_flip_display();
        }else if(event.type == 42 || event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            break;

   
    }

    /*Destruir bitmaps*/
    al_destroy_bitmap(all_background);
    al_destroy_bitmap(shadow_sprite);

    /*Destruir*/
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;

}