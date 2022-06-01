/*
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "ldr.h"
#include "../adc.h"

#define MIN_VERSCHIL 30
#define MIN_LICHT 80

// ADC Initialisatie
void init_ldr(void)
{
    ADMUX = (0 << REFS1) | (1 << REFS0);
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN);
}

//int map(int x, int in_min, int in_max, int out_min, int out_max)
//{
//    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
//}
//
//// Voor LDR boven (vanaf bovenkant)
//uint16_t get_LightLevel_1()
//{
//    _delay_ms(10);
//    ADCSRA |= (1 << ADSC);
//
//    while (ADCSRA & (1 << ADSC))
//        ;
//
//    _delay_ms(10);
//    return (ADC); // Return 10-bit result
//}
//
//// Voor LDR beneden (vanaf bovenkant)
//uint16_t get_LightLevel_2()
//{
//    _delay_ms(10);
//    ADCSRA |= (1 << ADSC);
//
//    while (ADCSRA & (1 << ADSC))
//        ;
//
//    _delay_ms(10);
//    return (ADC); // Return 10-bit result
//}
//
//void ldr_vergelijken()
//{
//    // LDR 1 < 2
//    if ((map(get_LightLevel_1(), 0, 1023, 0, 3)) < (map(get_LightLevel_2(), 0, 1023, 0, 3)))
//    {
//        // Rechtsom draaien
//
//        while (1)
//        {
//            if ((map(get_LightLevel_1(), 0, 1023, 0, 3)) == (map(get_LightLevel_2(), 0, 1023, 0, 3)))
//            {
//                // Stop motoren
//                // Vooruit rijden
//
//                break;
//            }
//        }
//    }
//    // LDR 1 > 2
//    else if ((map(get_LightLevel_1(), 0, 1023, 0, 3)) > (map(get_LightLevel_2(), 0, 1023, 0, 3)))
//    {
//        // Linksom draaien
//
//        while (1)
//        {
//            if ((map(get_LightLevel_1(), 0, 1023, 0, 3)) == (map(get_LightLevel_2(), 0, 1023, 0, 3)))
//            {
//                // Stop motoren
//                // Vooruit rijden
//
//                break;
//            }
//        }
//    }
//}

int get_light(int LDR) // choose which ldr to read
{
    ADMUX = 0b01000000; // read from A0: LDR boven (1)
    if (LDR)
    {
        ADMUX = 0b01000001; // read from A1: LDR beneden (2)
    }
    ADCSRA |= (1 << ADSC);                 // start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC); // wait for conversion to finish

    return ADC;
}

void ldr_vergelijken()
{
    // verschil bestaat
    if (abs(get_light(0) - get_light(1)) > MIN_VERSCHIL)
    {
        // LDR 1 < 2
        if (get_light(0) < get_light(1))
        {
            // Rechtsom draaien
            printf("ldr: motors rechts\n");

            while (1)
            {
                // LDR 1 == 2
                if (abs(get_light(0) - get_light(1)) < MIN_VERSCHIL)
                {
                    // motoren stoppen
                    printf("ldr: motors uit\n");
                    break;
                }
            }
        }
        // LDR 1 > 2
        else
        {
            // Linksom draaien

            printf("ldr: motors links\n");

            while (1)
            {
                // LDR 1 == 2
                if (abs(get_light(0) - get_light(1)) < MIN_VERSCHIL) /// waarden geven wss niet precies dezelfde waarde, dus ff nog oplossing
                {
                    // motoren stoppen
                    printf("ldr: motors uit\n");
                    break;
                }
            }
        }
    }

}

void ldr_volgen()
{
    while (1)
    {
        // LDR > 100
        if (get_light(0) < MIN_LICHT || get_light(1) < MIN_LICHT)
        {
            // ToF 1 en 2 > 20
            if (1)
            {
                printf("ldr: vergelijken\n");
                while (1)
                {
                    ldr_vergelijken();
                }
            }
            // ToF 1 en 2 < 20
            else
            {
                // Motoren uit
                printf("ldr: motors uit\n");
            }
        }
        printf("get_light0: %i\n", adc_convert(MEGA_PIN_A0_ANALOG));
        printf("get_light1: %i\n", adc_convert(MEGA_PIN_A1_ANALOG));
        _delay_ms(100);
    }
}
