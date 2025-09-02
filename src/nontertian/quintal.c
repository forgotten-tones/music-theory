/*

| quintal.c |
Defines quintal chord functionality
Handles quintal chord construction using P5 intervals

*/

#include "nontertian/quintal.h"

// Functions //

struct mah_nontertian_chord
mah_get_quintal_chord(
    struct mah_note const root, 
    int const num_notes,
    struct mah_note* restrict base,
    struct mah_note* restrict notes, 
    enum mah_error* err
)
{
    // Validate num_notes is between 2-5 inclusive
    if (num_notes < 2 || num_notes > 5)
    {
        RETURN_EMPTY_STRUCT_ERR(mah_nontertian_chord, MAH_ERROR_INVALID_NONTERTIAN_SIZE);
    }

    // Validate buffer pointers
    if (base == NULL || notes == NULL)
    {
        RETURN_EMPTY_STRUCT_ERR(mah_nontertian_chord, MAH_ERROR_INVALID_RANGE);
    }

    notes[0] = base[0] = root;

    // Build quintal chord using P5 intervals
    for (int i = 1; i < num_notes; i++)
    {
        enum mah_error inter_err = MAH_ERROR_NONE;
        struct mah_interval p5_interval = { 5, MAH_PERFECT };

        notes[i] = base[i] = mah_get_inter(notes[i - 1], p5_interval, &inter_err);
        if (inter_err != MAH_ERROR_NONE)
        {
            RETURN_EMPTY_STRUCT_ERR(mah_nontertian_chord, inter_err);
        }
    }

    return (struct mah_nontertian_chord) {
        .size = num_notes,
        .inv = 0,
        .inv_type = MAH_INV_STANDARD,
        .base = base,
        .notes = notes,
    };
}