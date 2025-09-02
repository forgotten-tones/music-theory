/*

| quartal.c |
Defines quartal chord functionality
Handles quartal chord construction, inversion, and folding

*/

#include "nontertian/quartal.h"
#include <string.h>

// Functions //

struct mah_nontertian_chord
mah_get_quartal_chord(
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

    // Build quartal chord using P4 intervals
    for (int i = 1; i < num_notes; i++)
    {
        enum mah_error inter_err = MAH_ERROR_NONE;
        struct mah_interval p4_interval = { 4, MAH_PERFECT };

        notes[i] = base[i] = mah_get_inter(notes[i - 1], p4_interval, &inter_err);
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

void
mah_invert_nontertian_chord(
    struct mah_nontertian_chord* chord, 
    int const inv,
    enum mah_inversion_type const inv_type,
    enum mah_error* err
)
{
    if (chord == NULL)
    {
        SET_ERR(MAH_ERROR_INVALID_RANGE);
        return;
    }

    if (inv >= chord->size || inv < 0)
    {
        SET_ERR(MAH_ERROR_INVALID_INVERSION);
        return;
    }

    // Reset to base before inversion
    memcpy(chord->notes, chord->base, sizeof(struct mah_note) * chord->size);

    for (int j = 0; j < inv; j++)
    {
        struct mah_note temp = chord->notes[0]; // save lowest note

        if (inv_type == MAH_INV_STANDARD)
        {
            // Standard inversion: move lowest note up one octave
            temp.octave += 1;
        }
        else if (inv_type == MAH_INV_FULL)
        {
            // Full inversion: move lowest note up by octaves until it becomes highest
            // Find the actual highest note in the current state
            int highest_octave = chord->notes[1].octave; // Start with second note
            for (int k = 2; k < chord->size; k++)
            {
                if (chord->notes[k].octave > highest_octave)
                {
                    highest_octave = chord->notes[k].octave;
                }
            }
            while (temp.octave <= highest_octave)
            {
                temp.octave += 1;
            }
        }

        // Shift all other notes down one position
        for (int i = 0; i < chord->size - 1; i++)
        {
            chord->notes[i] = chord->notes[i + 1];
        }
        chord->notes[chord->size - 1] = temp;
    }

    chord->inv = inv;
    chord->inv_type = inv_type;
}

void
mah_fold_nontertian_chord(
    struct mah_nontertian_chord* chord,
    int const fold_levels,
    enum mah_error* err
)
{
    if (chord == NULL)
    {
        SET_ERR(MAH_ERROR_INVALID_RANGE);
        return;
    }

    if (fold_levels < 0 || fold_levels >= chord->size)
    {
        SET_ERR(MAH_ERROR_INVALID_FOLD_LEVEL);
        return;
    }

    // Fold down the highest notes by specified number of levels
    for (int i = 0; i < fold_levels; i++)
    {
        int highest_idx = chord->size - 1 - i;
        chord->notes[highest_idx].octave -= 1;
    }
}