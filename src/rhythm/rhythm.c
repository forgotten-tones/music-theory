/*

| rhythm.c |
Rhythm and timing support for mahler music theory library
Provides time signature validation and measure analysis

*/

#include "rhythm/rhythm.h"
#include <stddef.h>

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