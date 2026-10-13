/**
 * Allegro Vivace - Capítulo 4: Áudio e Sons (Sound)
 *
 * Conceitos apresentados:
 * - Instalação de áudio e codecs (al_install_audio, al_init_acodec_addon)
 * - Reserva de canais de reprodução (al_reserve_samples)
 * - Efeitos sonoros curtos em memória (ALLEGRO_SAMPLE e al_play_sample)
 * - Música de fundo com streaming contínuo (ALLEGRO_AUDIO_STREAM e al_attach_audio_stream_to_mixer)
 * - Modos de reprodução (ALLEGRO_PLAYMODE_ONCE e ALLEGRO_PLAYMODE_LOOP)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

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

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "fonte");

    must_init(al_init_primitives_addon(), "primitives");

    // 1. Inicializa o subsistema de áudio e codecs
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");

    // 2. Reserva 16 canais virtuais para tocar múltiplos efeitos simultaneamente
    must_init(al_reserve_samples(16), "reserva de samples");

    // 3. Carrega efeito sonoro na memória RAM (WAV ou FLAC)
    ALLEGRO_SAMPLE* elephant = al_load_sample("elephant.wav");
    must_init(elephant, "elephant.wav");

    // 4. Carrega música por streaming de disco (OPUS ou OGG)
    // Parâmetros: arquivo, número de buffers (2), tamanho da amostra do buffer (2048)
    ALLEGRO_AUDIO_STREAM* music = al_load_audio_stream("music.opus", 2, 2048);
    must_init(music, "music.opus");

    // Configura para tocar em loop infinito e anexa ao mixer padrão
    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer());

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

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
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                // Tecla 'E': toca o som do elefante
                // Parâmetros: sample, volume (1.0), pan (0.0 centro), velocidade (1.0), modo, id
                if(event.keyboard.keycode == ALLEGRO_KEY_E)
                    al_play_sample(elephant, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    done = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(20, 20, 30));

            al_draw_text(font, al_map_rgb(255, 255, 255), 640 / 2, 480 / 2 - 20, ALLEGRO_ALIGN_CENTER, "Musica de fundo tocando em loop...");
            al_draw_text(font, al_map_rgb(255, 220, 0), 640 / 2, 480 / 2 + 10, ALLEGRO_ALIGN_CENTER, "Pressione [E] para tocar o efeito sonoro!");
            al_draw_text(font, al_map_rgb(180, 180, 180), 640 / 2, 480 / 2 + 40, ALLEGRO_ALIGN_CENTER, "Pressione [ESC] para sair.");

            al_flip_display();
            redraw = false;
        }
    }

    // Liberação de áudio e outros recursos
    al_destroy_audio_stream(music);
    al_destroy_sample(elephant);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
