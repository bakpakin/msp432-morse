#include "msp.h"
#include <stdint.h>

/*
 * This is the speed at which we playback the code.
 */
#define TIMESCALE 20000

/*
 * Write a byte of our custom morse encoding.
 * L is the number of dots and dashes (length), and BITS is
 * the unshifted encoding of dots and dashes.
 */
#define MORSE(L, BITS) ((0x1 << (7 - (L))) | ((BITS) << (8 - (L))))

/*
 * Each letter is stored as single byte. The leading 0s are padding
 * up to the frist 1. The remaining bits then encode dot and dashes,
 * with 0s as dots and 1s as dashes.
 *
 * Using international morse.
 *
 * This keeps the generated code compact, although a switch statement is easier to read.
 * Probably a moot point on the msp432p401r.
 */
static const uint8_t MORSE_LETTERS[] = {
    MORSE(2, 0x2), /* A */
    MORSE(4, 0x1), /* B */
    MORSE(4, 0x5), /* C */
    MORSE(3, 0x1), /* D */
    MORSE(1, 0x0), /* E */
    MORSE(4, 0x4), /* F */
    MORSE(3, 0x3), /* G */
    MORSE(4, 0X0), /* H */
    MORSE(2, 0x0), /* I */
    MORSE(4, 0xE), /* J */
    MORSE(3, 0x5), /* K */
    MORSE(4, 0x2), /* L */
    MORSE(2, 0x3), /* M */
    MORSE(2, 0x1), /* N */
    MORSE(3, 0x7), /* O */
    MORSE(4, 0x6), /* P */
    MORSE(4, 0xB), /* Q */
    MORSE(3, 0x2), /* R */
    MORSE(3, 0x0), /* S */
    MORSE(1, 0x1), /* T */
    MORSE(3, 0x4), /* U */
    MORSE(4, 0x8), /* V */
    MORSE(3, 0x6), /* W */
    MORSE(4, 0x9), /* X */
    MORSE(4, 0xD), /* Y */
    MORSE(4, 0x3), /* Z */
};

static const uint8_t MORSE_DIGITS[] = {
    MORSE(5, 0xE), /* 0 */
    MORSE(5, 0x1E),/* 1 */
    MORSE(5, 0x1C),/* 2 */
    MORSE(5, 0x18),/* 3 */
    MORSE(5, 0x10),/* 4 */
    MORSE(5, 0x0), /* 5 */
    MORSE(5, 0x1), /* 6 */
    MORSE(5, 0x3), /* 7 */
    MORSE(5, 0x7), /* 8 */
    MORSE(5, 0xF), /* 9 */
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
 * Play a buzzing tone
 */
void buzz(uint32_t time, uint32_t halfperiod) {
    volatile uint32_t i, j;
    uint32_t periods = time / halfperiod;
    for (i = periods; i > 0; i--) {
        for (j = halfperiod; j > 0; j--);
        P2->OUT ^= BIT7;
    }
}

/*
 * To 'play' our morse code, we use a standard time scale.
 * A dot is one unit, a dash is 3 units, and the time between
 * dots and dashes is also 1 unit. The time between letters is 3 units,
 * and the time between words is 5 units.
 *
 * This function plays a single byte, followed by a 3 unit pause.
 */
void play_byte(uint8_t morse_byte) {
    uint8_t i = 0;
    while (!((morse_byte >> i++) & 0x1)); /* Skip the leading 0s and the first 1 */
    for (; i < 8; i++) {
        /* Check the ith byte. If it is set, play a dash, otherwise play a dot. */
        buzz(((morse_byte >> i) & 0x1) ? 3 * TIMESCALE : TIMESCALE, 400);
        pause(TIMESCALE);
    }
    pause(2 * TIMESCALE);
}

/*
 * Play a string.
 */
void play_string(const char * c) {
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
    P2->DIR |= BIT7;

    play_string("Hello, Calvin!");
}

