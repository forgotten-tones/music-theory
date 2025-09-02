#ifndef __MAH_RHYTHM_H__
#define __MAH_RHYTHM_H__

#include "err/err.h"
#include "note/note.h"

// Structures //

typedef struct mah_time_sig
{
    int numerator;
    int denominator;
} mah_time_sig;

// Functions //

// Time signature and measure validation
void mah_validate_measure(struct mah_timed_note* notes, int count, struct mah_time_sig time_sig, enum mah_error* err);

#endif