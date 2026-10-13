/**
 * Allegro Vivace - Capítulo 3 (Parte 2): Entrada por Mouse (Mouse Input)
 *
 * Conceitos apresentados:
 * - Instalação do subsistema de mouse (al_install_mouse)
 * - Captura de eventos de movimento (ALLEGRO_EVENT_MOUSE_AXES)
 * - Modo de velocidade/inércia (física suave com aceleração e atrito)
 * - Captura e ocultação do ponteiro do mouse (al_grab_mouse, al_hide_mouse_cursor)
 * - Recentralização contínua do cursor (al_set_mouse_xy)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#define KEY_SEEN     1
#define KEY_DOWN     2

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("Erro: não foi possível inicializar %s\n", description);
    exit(1);
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "teclado");
    must_init(al_install_mouse(), "mouse");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "fila de eventos");

    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "fonte");

    must_init(al_init_primitives_addon(), "addon de primitivas");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    // Posição e velocidade do objeto
    float x = 320 - 15;
    float y = 240 - 15;
    float dx = 0;
    float dy = 0;

    // Prende o mouse na janela e esconde o cursor padrão do sistema
    al_grab_mouse(disp);
    al_hide_mouse_cursor(disp);

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                // Aplica a velocidade à posição
                x += dx;
                y += dy;

                // Ricocheteia nas bordas da janela
                if(x < 0)
                {
                    x *= -1;
                    dx *= -1;
                }
                if(x > 640 - 30)
                {
                    x = 2 * (640 - 30) - x;
                    dx *= -1;
                }
                if(y < 0)
                {
                    y *= -1;
                    dy *= -1;
                }
                if(y > 480 - 30)
                {
                    y = 2 * (480 - 30) - y;
                    dy *= -1;
                }

                // Aplica atrito/fricção para desaceleração natural
                dx *= 0.90f;
                dy *= 0.90f;

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                // Converte o deslocamento relativo do mouse em impulso/aceleração
                dx += event.mouse.dx * 0.10f;
                dy += event.mouse.dy * 0.10f;

                // Recentraliza o mouse para evitar que atinja as bordas do monitor
                al_set_mouse_xy(disp, 320, 240);
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= ~KEY_DOWN;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Mova o mouse para impulsionar o bloco (inercia). ESC para sair.");

            al_draw_filled_rectangle(x, y, x + 30, y + 30, al_map_rgb(0, 200, 255));

            al_flip_display();
            redraw = false;
        }
    }

    // Libera o mouse antes de fechar
    al_ungrab_mouse();
    al_show_mouse_cursor(disp);

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
