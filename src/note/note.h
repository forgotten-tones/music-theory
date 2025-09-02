#ifndef __MAH_NOTE_H__
#define __MAH_NOTE_H__

// Enums //

typedef enum mah_tone
{
    MAH_REST = -1,
    MAH_C = 0,
    MAH_D,
    MAH_E,
    MAH_F,
    MAH_G,
    MAH_A,
    MAH_B
} mah_tone;

typedef enum mah_acci
{
    MAH_DBFLAT  = -2,
    MAH_FLAT    = -1,
    MAH_NATURAL = 0,
    MAH_SHARP   = 1,
    MAH_DBSHARP = 2
} mah_acci;

typedef enum mah_dur
{
    MAH_DOTTED_WHOLE,
    MAH_WHOLE,
    MAH_DOTTED_HALF,
    MAH_HALF,
    MAH_DOTTED_QUARTER,
    MAH_QUARTER,
    MAH_DOTTED_EIGHTH,
    MAH_EIGHTH,
    MAH_DOTTED_SIXTEENTH,
    MAH_SIXTEENTH,
    MAH_DOTTED_THIRTYSECOND,
    MAH_THIRTYSECOND,
    MAH_SIXTYFOURTH,
    MAH_ONETWENTYEIGHTH,
    MAH_TUPLET
} mah_dur;

// Structures //

typedef struct mah_tuplet
{
    int n;
    int m;
    enum mah_dur base;
} mah_tuplet;

// There's not a good place for this right now; tehnically it's not neccassary until we start
// dealing with measures. Since this was really only created to do testing, we should probably
// move it to the test suite.
typedef struct mah_time_sig
{
    int numerator;
    int denominator;
} mah_time_sig;

typedef struct mah_note
{
    enum mah_tone tone;
    int acci;
    int pitch;
} mah_note;

typedef struct mah_timed_note
{
    enum mah_tone tone_timed;
    int acci_timed;
    int pitch_timed;
    enum mah_dur duration;
    struct mah_tuplet* tuplet;
} mah_timed_note;

// Functions //

#include "err/err.h"

// Duration calculation functions
void mah_get_duration_fraction(struct mah_timed_note* note, int* numerator, int* denominator, enum mah_error* err);
int mah_get_duration_ticks(struct mah_timed_note* note, enum mah_error* err);
int mah_compare_durations(struct mah_timed_note* a, struct mah_timed_note* b, enum mah_error* err);

// Tuplet functions
struct mah_tuplet mah_create_tuplet(int n, int m, enum mah_dur base, enum mah_error* err);

// Time signature and measure validation
void mah_validate_measure(struct mah_timed_note* notes, int count, struct mah_time_sig time_sig, enum mah_error* err);

// Convenience macros
#define REST(duration) ((struct mah_timed_note) {MAH_REST, 0, 0, duration, NULL})

#endif
