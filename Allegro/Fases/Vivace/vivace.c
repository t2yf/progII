#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

int main(){
    #define X_SCREEN 1200
    #define Y_SCREEN 920

    al_init();                      //Inicializações básicas
    al_install_keyboard();          //Inicialização do teclado

    ALLEGRO_TIMER* timer = al_create_timer(1.0/ 30.0);          // 30 fps
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue;         //Lista de eventos
    ALLEGRO_DISPLAY* disp = al_create_display()
}