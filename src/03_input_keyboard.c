/**
 * Allegro Vivace - Capítulo 3 (Parte 1): Entrada por Teclado (Keyboard Input)
 *
 * Conceitos apresentados:
 * - O problema do atraso (key repeat) do sistema operacional
 * - Vetor de estado de teclas com máscara de bits (KEY_SEEN e KEY_DOWN)
 * - Captura contínua de teclas pressionadas simultaneamente
 * - Movimento suave e responsivo em tempo real
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
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // Vetor que armazena o estado de cada tecla
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    // Posição inicial do quadrado controlado pelo jogador
    float x = 320 - 15;
    float y = 240 - 15;

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
                // Se pressionar ESC, fecha o programa
                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                // Movimentação suave contínua (5 pixels por frame)
                if(key[ALLEGRO_KEY_UP])
                    y -= 5;
                if(key[ALLEGRO_KEY_DOWN])
                    y += 5;
                if(key[ALLEGRO_KEY_LEFT])
                    x -= 5;
                if(key[ALLEGRO_KEY_RIGHT])
                    x += 5;

                // Limpa o bit KEY_SEEN para o próximo ciclo
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                // Quando uma tecla é pressionada, marca como SEEN e DOWN
                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                // Quando uma tecla é solta, remove o bit DOWN
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

            // Instruções na tela
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Use as SETAS para mover o bloco. ESC para sair.");

            // Desenha o quadrado vermelho do jogador
            al_draw_filled_rectangle(x, y, x + 30, y + 30, al_map_rgb(255, 0, 0));

            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
