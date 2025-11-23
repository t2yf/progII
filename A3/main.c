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
#include "collision.h"


#define X_SCREEN     1280
#define Y_SCREEN     720
#define GROUND       550
#define RHINO_INI_POS 6400
#define KERO_INI_POS 4000
#define NUM_ENEMIES     6


#define MENU 0
#define PAUSE 1
#define WIN 2
#define LOSE 3
#define IN_GAME 4

int damage_counter = 0;
int tela = MENU;
//vetor de inimigos -> para colisão
enemie *array_enemie[NUM_ENEMIES];

int update_position(character *actor, enemie **array_enemie, int map_ajustment){
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
        retorno = DOWN;
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
    
    /*Colisão*/
    int collide;
    for(int i=0; i<NUM_ENEMIES; i++){
        collide = character_collide(actor, array_enemie[i]->basics, map_ajustment);
        if(collide){
            actor->collide = 1;
            
            if(damage_counter == 0){
                actor->hp = actor->hp - array_enemie[i]->damage;
                damage_counter = 30;
            }
           
        }
    }


    /*Permite que o personagem possa ir para os lados, mesmo que esteja no ar*/
    return retorno;
}



int main(){
    /*Inicializações básicas*/
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_image_addon(), "image addon");
    must_init(al_init_primitives_addon(), "primitives");

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

    //Cor da colisão
    ALLEGRO_COLOR collision_color;
    collision_color = al_map_rgba_f(1, 0, 0, 1);



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

    //Itens - hp e esmeraldas
    ALLEGRO_BITMAP *hp_emerald_sprites = al_load_bitmap("assets/hp_emeralds-spritesheets.png");
    must_init(hp_emerald_sprites, "hp_emerals_sprite");
    int red_emerald_x = 3500;
    int red_emerald_visible = 1;

    //Sprites de tela
    ALLEGRO_BITMAP *menu_screen = al_load_bitmap("assets/screen_menu.png");
    must_init(menu_screen, "menu");

    ALLEGRO_BITMAP *pause_screen = al_load_bitmap("assets/screen_pause.png");
    must_init(pause_screen, "pause");

    ALLEGRO_BITMAP *win_screen = al_load_bitmap("assets/screen_win.png");
    must_init(win_screen, "win");

    ALLEGRO_BITMAP *lose_screen = al_load_bitmap("assets/screen_lose.png");
    must_init(lose_screen, "lose");

    //Itens - signpost
    ALLEGRO_BITMAP *signpost = al_load_bitmap("assets/signpost_sprite.png");
    must_init(signpost, "signpost");
    int signpost_x = 6100;
    int signpost_size = 50;

    //Itens - spikes
    ALLEGRO_BITMAP *spikes = al_load_bitmap("assets/spike_sprite.png");
    must_init(spikes, "spikes");
    int spikes_x = 4450;
    int spikes_y = 100;
    int spikes_width = 198;
    int spikes_height = 500;
    


    /*Primeiros eventos*/
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));


    /*Criar personagens*/
    character *shadow = character_create(10, GROUND, 22, 30, shadow_sprite);

    /*Criar inimigos*/
    //Patrols
    enemie *gamigami = enemie_create(700, 580, 47, 41, 2, PATROL, 0, 0, badniks_sprite); //+30
    enemie *pierrot = enemie_create(3000, 550, 48, 48, 2, PATROL, 0, 74, badniks_sprite);
    //Idle
    enemie *leon = enemie_create(3600, 610, 63, 23, 1,  IDLE, 0, 192, badniks_sprite); //+60
    enemie *mogu = enemie_create(5100, 590, 31, 33, 1, IDLE, 0, 41, badniks_sprite);
    //Runners
    enemie *rhino = enemie_create(RHINO_INI_POS, 590, 39, 32, 3,RUNNER, 0, 122, badniks_sprite); //+40
    enemie *kero = enemie_create(KERO_INI_POS, 590, 40, 38, 3, RUNNER, 0,154, badniks_sprite);

    /*Colocar inimigos num array*/
    array_enemie[0] = gamigami;
    array_enemie[1] = leon;
    array_enemie[2] = rhino;
    array_enemie[3] = pierrot;
    array_enemie[4] = mogu;
    array_enemie[5] = kero;
   

    /*Atributos para plotar personagens*/
    float shadow_width = shadow->basics->width;
    float shadow_height = shadow->basics->height;
    float shadow_sourceX = 0;
    float shadow_souceY = 30;
    

    /*Criar evento*/
    ALLEGRO_EVENT event;

    int shadow_dir = 0;
    int enemie_dir = 1;
    int pier_enemie_dir = 1;


    bool done = false;
    
    /*Loop principal*/
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);
        
        if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            if(event.keyboard.keycode == ALLEGRO_KEY_P){
                if(tela == MENU){
                   
                    tela = MENU;
                   
                }
                else if(tela != IN_GAME ){
                    
                    tela = IN_GAME;
                } else tela = PAUSE;
            }

            if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                if(tela == WIN || tela == LOSE){
                    done = true;
                } else if (tela == MENU || tela == PAUSE){
                    tela = IN_GAME;
                }
            }
        }
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
                if(shadow->stamina == STAMINA){
                    shadow->crounch = 1;
                } 
                if(event.type == ALLEGRO_EVENT_KEY_UP){
                    shadow->crounch = 0;
                }
               
            }			
            /*Evitar ficar infinitamente no rolling background*/
            if(event.type == ALLEGRO_EVENT_KEY_UP)
                shadow->fix_camera = 0;
            
       
        }else if (event.type == ALLEGRO_EVENT_TIMER){
            if(tela == MENU){
                al_draw_bitmap(menu_screen, 0, 0, 0);
            } else if(tela == IN_GAME){
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
                

                /*Plotar inimigos*/
                /*GamiGami - Patrol*/
                //Cuidar para a sprite não seguir o background
                int gami_frameX;
                if(gamigami->basics->x + gamigami->basics->width > map_ajustment && gamigami->basics->x + gamigami->basics->width < map_ajustment + X_SCREEN){
                    //Movimentação patrol (de um lado para outro)
                    if((gamigami->basics->x  + gamigami->basics->width) > 900 - map_ajustment || (gamigami->basics->x  + gamigami->basics->width) < 700 - map_ajustment)
                        enemie_dir = -1*enemie_dir;
                    enemie_move(gamigami, enemie_dir, 2, rolling);
                    gami_frameX = (al_get_timer_count(timer)/10) %4;
                    al_draw_scaled_bitmap(badniks_sprite, gamigami->sourceX + (gamigami->basics->width*gami_frameX), gamigami->sourceY, gamigami->basics->width, gamigami->basics->height, gamigami->basics->x-map_ajustment, gamigami->basics->y, gamigami->basics->width*2, gamigami->basics->height*2, 0);
                }

                /*Pierrot - Patrol*/
                int pier_frameX;
                if(pierrot->basics->x + pierrot->basics->width > map_ajustment && pierrot->basics->x + pierrot->basics->width < map_ajustment + X_SCREEN){
                    if((pierrot->basics->x + pierrot->basics->width) > map_ajustment + X_SCREEN - 20 || (pierrot->basics->x) < map_ajustment + 20)
                        pier_enemie_dir = -1*pier_enemie_dir;

                    enemie_move(pierrot, pier_enemie_dir, 2, rolling);
                    pier_frameX = (al_get_timer_count(timer)/10) % 4;
                    al_draw_scaled_bitmap(badniks_sprite, pierrot->sourceX + (pierrot->basics->width*pier_frameX), pierrot->sourceY, pierrot->basics->width, pierrot->basics->height, pierrot->basics->x - map_ajustment, pierrot->basics->y, pierrot->basics->width*2, pierrot->basics->height*2, 0);
                }

                /*Rhino - Runner*/
                int rhino_frameX;
                int show_rhino = enemie_move(rhino, 0, 2, rolling);
                if(show_rhino != -1){
                    if(rhino->basics->x + rhino->basics->width > map_ajustment && rhino->basics->x + rhino->basics->width < map_ajustment + X_SCREEN){
                        rhino_frameX = (al_get_timer_count(timer)/3) %3;
                        al_draw_scaled_bitmap(badniks_sprite, rhino->sourceX + (rhino->basics->width*rhino_frameX), rhino->sourceY, rhino->basics->width, rhino->basics->height, rhino->basics->x-map_ajustment, rhino->basics->y, rhino->basics->width*2, rhino->basics->height*2, 0);
                    }
                } else {
                    //Resetar
                    rhino->basics->x = RHINO_INI_POS;
                }
                
                /*Kero - RUNNER*/
                int kero_frameX;
                int show_kero = enemie_move(kero, 0, 2, rolling);
                if(show_kero != -1){
                    if(kero->basics->x + kero->basics->width > map_ajustment && kero->basics->x + kero->basics->width < map_ajustment + X_SCREEN){
                        kero_frameX = (al_get_timer_count(timer)/3) %5;
                        al_draw_scaled_bitmap(badniks_sprite, kero->sourceX + (kero->basics->width*kero_frameX), kero->sourceY, kero->basics->width, kero->basics->height, kero->basics->x - map_ajustment, kero->basics->y, kero->basics->width*2, kero->basics->height*2, 0);
                    }
                } else{
                    kero->basics->x = KERO_INI_POS;
                }
               
                
                /*Leon - Idle*/
                int leon_frameX;
                if(leon->basics->x + leon->basics->width > map_ajustment && leon->basics->x + leon->basics->width  < map_ajustment + X_SCREEN){
                    //printf("Leon X: %d\n", leon->basics->x);
                    leon_frameX = (al_get_timer_count(timer)) %6;
                    al_draw_scaled_bitmap(badniks_sprite, leon->sourceX + (leon->basics->width*leon_frameX), leon->sourceY, leon->basics->width, leon->basics->height, leon->basics->x-map_ajustment, leon->basics->y, leon->basics->width*2, leon->basics->height*2, 0);
                }

                /*Mogu - Idle*/
                int mogu_frameX;
                if(mogu->basics->x + mogu->basics->width > map_ajustment && mogu->basics->x + mogu->basics->width < map_ajustment + X_SCREEN){
                    mogu_frameX = (al_get_timer_count(timer)/3) %7;
                    al_draw_scaled_bitmap(badniks_sprite, mogu->sourceX + (mogu->basics->width*mogu_frameX), mogu->sourceY, mogu->basics->width, mogu->basics->height, mogu->basics->x - map_ajustment, mogu->basics->y, mogu->basics->width*2, mogu->basics->height*2, 0);
                }


                /*Plotar plaquinha*/
                if(signpost_x > map_ajustment && signpost_x + signpost_size < map_ajustment + X_SCREEN){
                    al_draw_scaled_bitmap(signpost, 0, 0, signpost_size, signpost_size, signpost_x - map_ajustment, GROUND, signpost_size*2, signpost_size*2, 0);
                    
                }

                /*Plotar item de vida*/
                if(red_emerald_visible && ( red_emerald_x > map_ajustment && red_emerald_x + signpost_size < map_ajustment + X_SCREEN)){
                    al_draw_scaled_bitmap(hp_emerald_sprites, 18, 15, 15, 15, red_emerald_x - map_ajustment, GROUND, 15*2, 15*2, 0);


                    int red_emerald_collide = simple_collide(shadow->basics->x, shadow->basics->y, shadow->basics->x + shadow->basics->width*SPRITE_MULT_FACTOR, shadow->basics->y + shadow->basics->height*SPRITE_MULT_FACTOR, red_emerald_x - map_ajustment, GROUND, red_emerald_x + 15 - map_ajustment, GROUND + 15);
                    if(red_emerald_collide){
                        red_emerald_visible = 0;
                        shadow->hp += 3;
                    }
                }

                if(!red_emerald_visible)
                    al_draw_scaled_bitmap(hp_emerald_sprites, 18, 13, 15, 15, 1050, 15, 15*2, 15*2, 0);


                /*Plotar spikes*/
                //[TODO]
                if(spikes_x + spikes_width > map_ajustment && spikes_x + spikes_width < map_ajustment + X_SCREEN){
                    al_draw_scaled_bitmap(spikes, 0, 0, spikes_width, spikes_height, spikes_x - map_ajustment, spikes_y, spikes_width, spikes_height, 0);

                    
                }

                /*Nova posição do Shadow*/
                shadow->position = update_position(shadow, array_enemie, map_ajustment);
                int frameX;


                //Colisão
                int spike_collide = four_sides_collide(shadow->basics->x, shadow->basics->y, shadow->basics->x + shadow->basics->width*SPRITE_MULT_FACTOR, shadow->basics->y + shadow->basics->height*SPRITE_MULT_FACTOR, spikes_x - map_ajustment, spikes_y, spikes_x + spikes_width - map_ajustment, spikes_y + spikes_height);

                    
                if(spike_collide != 0){
                    //Se tiver agachado não acontece nada
                    if(!shadow->crounch){
                        //Se colidiu, voltar
                        if(spike_collide == 1){
                            shadow->basics->x = spikes_x  - map_ajustment - shadow->basics->width - 50;
                        } else if(spike_collide == 2){
                            shadow->basics->x = spikes_x + spikes_width - map_ajustment ;
                        }
                        shadow->collide = 1; 
                        //shadow->hp -= 1;
                        if(damage_counter == 0){
                            shadow->hp -=1;
                            damage_counter = 30;
                        }
                    }        
                }
                
                if (shadow->position == LEFT){
                    shadow_dir = ALLEGRO_FLIP_HORIZONTAL;
                } else if (shadow->position == RIGHT){
                    shadow_dir = 0;
                } 

                /*Plotar o Shadow*/
                /*Shadow parado*/
                if(shadow->position == 0){
                    frameX = (al_get_timer_count(timer)/10) % 5;
                    //Sprite parada é a primeira
                    shadow_souceY = 0;

                    //recupera stamina
                    if(shadow->stamina < STAMINA)
                        shadow->stamina ++;

                    if(shadow->collide == 1){
                        al_draw_tinted_scaled_bitmap(shadow_sprite, collision_color, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);

                        //shadow->collide = 0;
                    } else
                        al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);
                } 
                /*Shadow andar em x e estiver no chão*/ /*Shadow rastejando*/
                if((shadow->position == LEFT || shadow->position == RIGHT) && shadow->crounch && shadow->ground){
                    frameX = (al_get_timer_count(timer)/3) % 3;
                    shadow_souceY = 155;
                    shadow_sourceX = 88;

                    //Andar abaixado gasta stamina
                    shadow->stamina --;
                    if(shadow->stamina == 0){
                        shadow->crounch = !shadow->crounch;
                    }
                    
                    if(shadow->collide == 1){
                        al_draw_tinted_scaled_bitmap(shadow_sprite, collision_color, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height - 5, shadow->basics->x, shadow->basics->y + 40, shadow_width*2.8, shadow_height*2.8, shadow_dir);
                    } else
                        al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height -5, shadow->basics->x, shadow->basics->y + 40, shadow_width*2.8, shadow_height*2.8, shadow_dir);

                    shadow_sourceX = 0;
                }
                else if((shadow->position == LEFT || shadow->position == RIGHT) && shadow->ground){
                    frameX = (al_get_timer_count(timer)/3) % 14;
                    shadow_souceY = 30;

                    //recupera stamina
                    if(shadow->stamina < STAMINA)
                        shadow->stamina ++;


                    if(shadow->collide == 1){
                        al_draw_tinted_scaled_bitmap(shadow_sprite, collision_color, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);

                        //shadow->collide = 0;
                    } else
                        al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);
                } 
                /*Shadow no ar*/
                if(shadow->position == UP || !shadow->ground){
                    frameX = (al_get_timer_count(timer)/6) % 4;
                    
                    shadow_souceY = 60;
                    //recupera stamina
                    if(shadow->stamina < STAMINA)
                        shadow->stamina ++;

                    if(shadow->collide == 1){
                        al_draw_tinted_scaled_bitmap(shadow_sprite, collision_color, shadow_sourceX + (shadow_width*frameX)
                        , shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir); //
                        //shadow->collide = 0;
                    }else
                        al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX + (shadow_width*frameX), shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);

                    
                }
                /*Shadow agachado*/
                if(shadow->position == DOWN){
                    shadow_souceY = 120;
                    //recupera stamina
                    if(shadow->stamina < STAMINA)
                        shadow->stamina ++;

                    if(shadow->collide == 1){
                        al_draw_tinted_scaled_bitmap(shadow_sprite, collision_color, shadow_sourceX, shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);

                        //shadow->collide = 0;
                    } else
                        al_draw_scaled_bitmap(shadow_sprite, shadow_sourceX, shadow_souceY, shadow_width, shadow_height, shadow->basics->x, shadow->basics->y, shadow_width*SPRITE_MULT_FACTOR, shadow_height*SPRITE_MULT_FACTOR, shadow_dir);
                }
                
                
                /*Barra de stamina*/
                al_draw_filled_rectangle(10, 20, shadow->stamina + 10, 40, collision_color);

                //al_draw_filled_rectangle(500, 100, 600, 600, collision_color);
                

                /*Colisão*/
                if(shadow->collide == 1){
                    shadow->collide = 0;
                    
                }
                damage_counter --;
                if(damage_counter < 0){
                    damage_counter = 0;
                }
                /*HP*/
                al_draw_scaled_bitmap(hp_emerald_sprites, 0, 0, 18, 15, 1100, 10, 18*3, 15*3, 0);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 1150, 30, 0,  " X %d", shadow->hp);
                //al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %d Y: %d", shadow->basics->x, shadow->basics->y);

                
                /*Conferir se ainda tem vida*/
                if(shadow->hp <= 0){
                    tela = LOSE;    
                }

                /*Se encostar na plaquinha, vence*/
                int signpost_collide = simple_collide(shadow->basics->x, shadow->basics->y, shadow->basics->x + shadow->basics->width*SPRITE_MULT_FACTOR, shadow->basics->y + shadow->basics->height*SPRITE_MULT_FACTOR , signpost_x + signpost_size - map_ajustment, GROUND, signpost_x + signpost_size + 50 - map_ajustment, GROUND + signpost_size);
                    if(signpost_collide){
                        tela = WIN;
                }

                
            } else if (tela == LOSE){
                al_draw_bitmap(lose_screen, 0, 0, 0);
            } else if (tela == PAUSE){
                 al_draw_bitmap(pause_screen, 0, 0, 0);
            } else if (tela == WIN){
                 al_draw_bitmap(win_screen, 0, 0, 0);
            }
            al_flip_display();
        }else if(event.type == 42 || event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || done)
            break;

   
    }

    /*Destruir bitmaps*/
    al_destroy_bitmap(all_background);
    al_destroy_bitmap(shadow_sprite);
    al_destroy_bitmap(menu_screen);
    al_destroy_bitmap(pause_screen);
    al_destroy_bitmap(lose_screen);
    al_destroy_bitmap(win_screen);

    /*Destruir*/
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;

}