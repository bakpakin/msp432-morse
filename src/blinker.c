#include "msp.h"
#include <stdint.h>

/*
 * This is the speed at which we playback the code.
 */
#define TIMESCALE 40000

#define MORSE_1(F) (1 + 8*(F))
#define MORSE_2(F) (2 + 8*(F))
#define MORSE_3(F) (3 + 8*(F))
#define MORSE_4(F) (4 + 8*(F))
#define MORSE_5(F) (5 + 8*(F))

/*
 * Each letter is stored as single byte. The first 3 bytes
 * Indicate how many dots and dashes in the letter, and the next 5
 * bytes indicate the dots vs dashes. A 1 is a dash, and 0 is a dot.
 * Using international morse.
 *
 * This keeps the generated code compact, although a switch is easier to read.
 * Probably a moot point on the msp432p401r.
 */
static const uint8_t MORSE_LETTERS[] = {
    MORSE_2(0x2), /* A */
    MORSE_4(0x1), /* B */
    MORSE_4(0x5), /* C */
    MORSE_3(0x1), /* D */
    MORSE_1(0x0), /* E */
    MORSE_4(0x4), /* F */
    MORSE_3(0x3), /* G */
    MORSE_4(0X0), /* H */
    MORSE_2(0x0), /* I */
    MORSE_4(0xE), /* J */
    MORSE_3(0x5), /* K */
    MORSE_4(0x2), /* L */
    MORSE_2(0x3), /* M */
    MORSE_2(0x1), /* N */
    MORSE_3(0x7), /* O */
    MORSE_4(0x6), /* P */
    MORSE_4(0xB), /* Q */
    MORSE_3(0x2), /* R */
    MORSE_3(0x0), /* S */
    MORSE_1(0x1), /* T */
    MORSE_3(0x4), /* U */
    MORSE_4(0x8), /* V */
    MORSE_3(0x6), /* W */
    MORSE_4(0x9), /* X */
    MORSE_4(0xD), /* Y */
    MORSE_4(0x3), /* z */
};

static const uint8_t MORSE_DIGITS[] = {
    MORSE_5(0xE), /* 0 */
    MORSE_5(0x1E),/* 1 */
    MORSE_5(0x1C),/* 2 */
    MORSE_5(0x18),/* 3 */
    MORSE_5(0x10),/* 4 */
    MORSE_5(0x0), /* 5 */
    MORSE_5(0x1), /* 6 */
    MORSE_5(0x3), /* 7 */
    MORSE_5(0x7), /* 8 */
    MORSE_5(0xF), /* 9 */
};

/*
 * Pause for some number of time units. This should probably be
 * implemented in a better way than a dumb loop.
 */
void pause(uint32_t time) {
    volatile uint32_t i;
    for (i = time; i > 0; i--);
}

/*
 * To 'play' our morse code, we use a standard time scale.
 * A dot is one unit, a dash is 3 units, and the time between
 * dots and dashes is also 1 unit. The time between letters is 3 units,
 * and the time between words is 5 units.
 */

void play_byte(uint8_t morse_byte) {
    uint8_t i;
    uint8_t count = morse_byte & 0x7;
    for (i = 0; i < count; i++) {
        P1->OUT |= BIT0;
        /* Check the (i + 3)th byte. If it is set, play a dash, otherwise play a dot. */
        pause(((morse_byte >> i) & 0x8) ? 3 * TIMESCALE : TIMESCALE);
        P1->OUT &= ~BIT0;
        pause(TIMESCALE);
    }
    pause(2 * TIMESCALE);
}

void play_string(const char * str) {
    const char * c = str;
    while (*c) {
        if (*c >= 'a' && *c <= 'z') {
            play_byte(MORSE_LETTERS[*c - 'a']);
        } else if (*c >= 'A' && *c <= 'Z') {
            play_byte(MORSE_LETTERS[*c - 'A']);
        } else if (*c >= '0' && *c <= '9') {
            play_byte(MORSE_DIGITS[*c - '0']);
        } else if (*c == ' ') {
            pause(5 * TIMESCALE); /* Word spacing */
        }
        c++;
    }
}

int main(void) {

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    P1->DIR |= BIT0; /* P1 is output */

    play_string("Hello, Calvin!");
}

