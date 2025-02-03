#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUMERO_DE_PIXELS 25
#define WS2812_PIN 7

#define LED_RED_PIN 13
#define BUTTON_A 5
#define BUTTON_B 6

#define DEBOUCE_DELAY_US 30000 // Debouce delay (em microsegundos)

volatile int counter = 0; // contador Global
        
        //Matriz para cada número de 0 a 9.
bool numeros[10][NUMERO_DE_PIXELS] = {
    {// Número 0
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0},
    {// Número 1
     0, 1, 1, 1, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 1,
     0, 1, 1, 0, 0,
     0, 0, 1, 0, 0},
    {// Número 2
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0},
    {// Número 3
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0},
    {// Número 4
     0, 1, 0, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0},
    {// Número 5
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0},
    {// Número 6
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0},
    {// Número 7
     0, 1, 0, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 0, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0},
    {// Número 8
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0},
    {// Número 9
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0}
     };

// Função relacionada a Matriz de LED
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void display_numero(bool *buffer, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = urgb_u32(r, g, b);
    for (int i = 0; i < NUMERO_DE_PIXELS; i++)
    {
        put_pixel(buffer[i] ? color : 0);
    }
}

void setup()
{
    // Configuração do LED Vermelho do LED RGB
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    // CONFIGURAÇÕES DE BOTÕES A E B
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

void button_callback(uint gpio, uint32_t events)
{
    // Funçao de interrupção para ambos os botões. Callback
    static volatile uint64_t last_interrupt_time_a = 0;
    static volatile uint64_t last_interrupt_time_b = 0;
    uint64_t current_time = time_us_64();

    if (gpio == BUTTON_A)
    {   //Verificando se foi o botão A 
        if (current_time - last_interrupt_time_a > DEBOUCE_DELAY_US)
        {
            counter = (counter + 1) % 10;
            last_interrupt_time_a = current_time;
        }
    }else if (gpio == BUTTON_B){ //Verificando se foi o botão B 
        if (current_time - last_interrupt_time_b > DEBOUCE_DELAY_US)
        {
            counter = (counter - 1 + 10) % 10;
            last_interrupt_time_b = current_time;
        }
    }
}
void led_red_blink()
{   // Pisca o LED vermelho, e aguarda 100ms ligando e desligando o LED
    while (1)
    {
        gpio_put(LED_RED_PIN, 1); // o Led acende durante 100 ms true ou 1
        sleep_ms(100);

        gpio_put(LED_RED_PIN, 0); // após 100 ms o led desliga false ou 0
        sleep_ms(100);
    }
}

int main()
{
    
    stdio_init_all();
    setup();
    // Inicia a Thread para o LED VERMELHO no núcleo 1
    multicore_launch_core1(led_red_blink);

    //Configuração inicial do PIO e LEDs
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_RISE, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_RISE, true, &button_callback);

    while (true)
    {                               //    R    G     B
        display_numero(numeros[counter], 15, 5 , 5  );
        sleep_ms(100);
    }
    return 0;
}