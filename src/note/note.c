/*

| note.c |
Duration support for mahler music theory library
Provides duration calculations, tuplet support, and measure validation

*/

#include "note/note.h"
#include <stddef.h>

// Duration calculation functions //

void
mah_get_duration_fraction(struct mah_timed_note* note, int* numerator, int* denominator, enum mah_error* err)
{
    if (!note || !numerator || !denominator || !err) {
        if (err) *err = MAH_ERROR_INVALID_DURATION;
        return;
    }

    *err = MAH_ERROR_NONE;

    if (note->duration == MAH_TUPLET) {
        if (!note->tuplet || note->tuplet->n <= 0 || note->tuplet->m <= 0) {
            *err = MAH_ERROR_INVALID_TUPLET;
            return;
        }
        
        // Get base duration fraction first
        struct mah_timed_note base_note = {MAH_C, 0, 4, note->tuplet->base, NULL};
        int base_num, base_den;
        mah_get_duration_fraction(&base_note, &base_num, &base_den, err);
        if (*err != MAH_ERROR_NONE) return;
        
        // Apply tuplet ratio: (base_duration * M) / N
        *numerator = (base_num * note->tuplet->m);
        *denominator = (base_den * note->tuplet->n);
        
        // Reduce fraction
        int gcd_val = 1;
        int a = *numerator, b = *denominator;
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        gcd_val = a;
        
        *numerator /= gcd_val;
        *denominator /= gcd_val;
        
        return;
    }

    // Standard durations as fractions of whole note
    switch (note->duration) {
        case MAH_DOTTED_WHOLE:    *numerator = 3; *denominator = 2; break;
        case MAH_WHOLE:           *numerator = 1; *denominator = 1; break;
        case MAH_DOTTED_HALF:     *numerator = 3; *denominator = 4; break;
        case MAH_HALF:            *numerator = 1; *denominator = 2; break;
        case MAH_DOTTED_QUARTER:  *numerator = 3; *denominator = 8; break;
        case MAH_QUARTER:         *numerator = 1; *denominator = 4; break;
        case MAH_DOTTED_EIGHTH:   *numerator = 3; *denominator = 16; break;
        case MAH_EIGHTH:          *numerator = 1; *denominator = 8; break;
        case MAH_DOTTED_SIXTEENTH: *numerator = 3; *denominator = 32; break;
        case MAH_SIXTEENTH:       *numerator = 1; *denominator = 16; break;
        case MAH_DOTTED_THIRTYSECOND: *numerator = 3; *denominator = 64; break;
        case MAH_THIRTYSECOND:    *numerator = 1; *denominator = 32; break;
        case MAH_SIXTYFOURTH:     *numerator = 1; *denominator = 64; break;
        case MAH_ONETWENTYEIGHTH: *numerator = 1; *denominator = 128; break;
        default:
            *err = MAH_ERROR_INVALID_DURATION;
            return;
    }
}

int
mah_get_duration_ticks(struct mah_timed_note* note, enum mah_error* err)
{
    if (!note || !err) {
        if (err) *err = MAH_ERROR_INVALID_DURATION;
        return 0;
    }

    int numerator, denominator;
    mah_get_duration_fraction(note, &numerator, &denominator, err);
    if (*err != MAH_ERROR_NONE) return 0;

    // Calculate ticks: (1920 * numerator) / denominator
    // 1920 is standard MIDI ticks per whole note
    return (1920 * numerator) / denominator;
}

int
mah_compare_durations(struct mah_timed_note* a, struct mah_timed_note* b, enum mah_error* err)
{
    if (!a || !b || !err) {
        if (err) *err = MAH_ERROR_INVALID_DURATION;
        return 0;
    }

    int ticks_a = mah_get_duration_ticks(a, err);
    if (*err != MAH_ERROR_NONE) return 0;
    
    int ticks_b = mah_get_duration_ticks(b, err);
    if (*err != MAH_ERROR_NONE) return 0;

    if (ticks_a < ticks_b) return -1;
    if (ticks_a > ticks_b) return 1;
    return 0;
}

// Tuplet functions //

struct mah_tuplet
mah_create_tuplet(int n, int m, enum mah_dur base, enum mah_error* err)
{
    struct mah_tuplet tuplet = {0, 0, MAH_QUARTER};
    
    if (!err) return tuplet;
    
    if (n <= 0 || m <= 0) {
        *err = MAH_ERROR_INVALID_TUPLET;
        return tuplet;
    }
    
    if (base == MAH_TUPLET) {
        *err = MAH_ERROR_INVALID_TUPLET;
        return tuplet;
    }
    
    *err = MAH_ERROR_NONE;
    tuplet.n = n;
    tuplet.m = m;
    tuplet.base = base;
    
    return tuplet;
}

// Time signature and measure validation //

void
mah_validate_measure(struct mah_timed_note* notes, int count, struct mah_time_sig time_sig, enum mah_error* err)
{
    if (!err) return;
    
    if (!notes || count <= 0) {
        *err = MAH_ERROR_INVALID_MEASURE_DURATION;
        return;
    }
    
    if (time_sig.numerator <= 0 || time_sig.denominator <= 0) {
        *err = MAH_ERROR_INVALID_TIME_SIG;
        return;
    }
    
    // Calculate expected measure duration in ticks
    // Time signature: numerator/denominator means numerator beats of denominator note value
    // For 4/4: 4 quarter notes = 4 * 480 = 1920 ticks
    // For 3/4: 3 quarter notes = 3 * 480 = 1440 ticks
    // For 6/8: 6 eighth notes = 6 * 240 = 1440 ticks
    
    int beat_ticks;
    switch (time_sig.denominator) {
        case 1:  beat_ticks = 1920; break;  // Whole note
        case 2:  beat_ticks = 960;  break;  // Half note
        case 4:  beat_ticks = 480;  break;  // Quarter note  
        case 8:  beat_ticks = 240;  break;  // Eighth note
        case 16: beat_ticks = 120;  break;  // Sixteenth note
        case 32: beat_ticks = 60;   break;  // Thirty-second note
        case 64: beat_ticks = 30;   break;  // Sixty-fourth note
        default:
            *err = MAH_ERROR_INVALID_TIME_SIG;
            return;
    }
    
    int expected_ticks = time_sig.numerator * beat_ticks;
    
    // Sum up all note durations in the measure
    int total_ticks = 0;
    enum mah_error note_err = MAH_ERROR_NONE;
    
    for (int i = 0; i < count; i++) {
        int note_ticks = mah_get_duration_ticks(&notes[i], &note_err);
        if (note_err != MAH_ERROR_NONE) {
            *err = note_err;
            return;
        }
        total_ticks += note_ticks;
    }
    
    if (total_ticks != expected_ticks) {
        *err = MAH_ERROR_INVALID_MEASURE_DURATION;
        return;
    }
    
    *err = MAH_ERROR_NONE;
}