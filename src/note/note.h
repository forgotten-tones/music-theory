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

typedef enum mah_octave
{
    // Theoretical sub-MIDI range for music theory analysis
    MAH_OCTAVE_NEG5 = -5,  // Theoretical: ~0.5 Hz fundamental
    MAH_OCTAVE_NEG4 = -4,  // Theoretical: ~1 Hz fundamental  
    MAH_OCTAVE_NEG3 = -3,  // Theoretical: ~2 Hz fundamental
    MAH_OCTAVE_NEG2 = -2,  // Theoretical: ~4 Hz fundamental
    
    // MIDI standard range (MIDI notes 0-127)
    MAH_OCTAVE_NEG1 = -1,  // MIDI: C-1 to B-1 (notes 0-11)
    MAH_OCTAVE_0 = 0,      // MIDI: C0 to B0 (notes 12-23)
    MAH_OCTAVE_1 = 1,      // MIDI: C1 to B1 (notes 24-35)
    MAH_OCTAVE_2 = 2,      // MIDI: C2 to B2 (notes 36-47)
    MAH_OCTAVE_3 = 3,      // MIDI: C3 to B3 (notes 48-59)
    MAH_OCTAVE_4 = 4,      // MIDI: C4 to B4 (notes 60-71) - Middle C octave
    MAH_OCTAVE_5 = 5,      // MIDI: C5 to B5 (notes 72-83)
    MAH_OCTAVE_6 = 6,      // MIDI: C6 to B6 (notes 84-95)
    MAH_OCTAVE_7 = 7,      // MIDI: C7 to B7 (notes 96-107)
    MAH_OCTAVE_8 = 8,      // MIDI: C8 to B8 (notes 108-119)
    MAH_OCTAVE_9 = 9,      // MIDI: C9 to G9 (notes 120-127) - MIDI limit
    
    // Theoretical super-MIDI range for music theory analysis
    MAH_OCTAVE_10 = 10,    // Theoretical: ~16,000 Hz range
    MAH_OCTAVE_11 = 11,    // Theoretical: ~32,000 Hz range
    MAH_OCTAVE_12 = 12,    // Theoretical: ~64,000 Hz range (beyond human hearing)
    MAH_OCTAVE_13 = 13,    // Theoretical: ~128,000 Hz range
    MAH_OCTAVE_14 = 14,    // Theoretical: ~256,000 Hz range
    MAH_OCTAVE_15 = 15     // Theoretical: ~512,000 Hz range
} mah_octave;

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


typedef struct mah_note
{
    enum mah_tone tone;
    int acci;
    enum mah_octave octave;
} mah_note;

typedef struct mah_timed_note
{
    enum mah_tone tone_timed;
    int acci_timed;
    enum mah_octave octave_timed;
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

// Convenience macros
#define REST(duration) ((struct mah_timed_note) {MAH_REST, 0, MAH_OCTAVE_0, duration, NULL})

#endif
