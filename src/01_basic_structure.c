/**
 * Allegro Vivace - Capítulo 1: Estrutura Básica do Jogo (Basic Game Structure)
 *
 * Conceitos apresentados:
 * - Inicialização do Allegro e verificação com must_init()
 * - Criação de Display (janela) e Timer (temporizador)
 * - Fila de Eventos (Event Queue) e registro de fontes
 * - Game Loop clássico: Timer -> Lógica -> Redraw
 * - Limpeza correta de memória ao encerrar
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("Erro: não foi possível inicializar %s\n", description);
    exit(1);
}

int main()
{
    // 1. Inicializa o núcleo da biblioteca Allegro
    must_init(al_init(), "allegro");

    // 2. Inicializa o subsistema de teclado
    must_init(al_install_keyboard(), "teclado");

    // 3. Cria um timer a 30 quadros por segundo (1/30s)
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    // 4. Cria a fila de eventos central
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "fila de eventos");

    // 5. Cria a janela do jogo (640x480)
    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    must_init(disp, "display");

    // 6. Cria a fonte embutida para desenhar texto
    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "fonte");

    // 7. Registra as fontes de eventos na fila
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    // Inicia o timer antes do loop principal
    al_start_timer(timer);

    // ==========================================
    // LOOP PRINCIPAL (Game Loop)
    // ==========================================
    while(1)
    {
        // Bloqueia a execução até que um evento chegue na fila
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // Aqui é processada a lógica do jogo a cada tick do relógio (30 FPS)
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                // Pressionar qualquer tecla ou fechar a janela encerra o loop
                done = true;
                break;
        }

        if(done)
            break;

        // Desenha somente se o timer disparou E a fila de eventos foi esvaziada
        if(redraw && al_is_event_queue_empty(queue))
        {
            // Limpa a tela com preto
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Desenha o texto "Hello world!" em branco nas coordenadas (0, 0)
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");

            // Envia o que foi desenhado para a tela (double buffering)
            al_flip_display();

            redraw = false;
        }
    }

    // Liberação de recursos na ordem correta
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
