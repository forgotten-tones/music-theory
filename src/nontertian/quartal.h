#ifndef __MAH_QUARTAL_H__
#define __MAH_QUARTAL_H__

#include "err/err.h"
#include "inter/inter.h"
#include "note/note.h"
#include "shared/shared.h"

// Structures //

typedef struct mah_nontertian_base
{
    char const* name;
    int size;
    int interval_steps;  // 4 for quartal, 5 for quintal
    enum mah_quality interval_quality;  // MAH_PERFECT
} mah_nontertian_base;

typedef struct mah_nontertian_chord  
{
    int size;
    int inv;
    enum mah_inversion_type inv_type;
    struct mah_note* restrict base;
    struct mah_note* restrict notes;
} mah_nontertian_chord;

// Functions //

struct mah_nontertian_chord mah_get_quartal_chord(
    struct mah_note root, 
    int num_notes,
    struct mah_note* restrict base,
    struct mah_note* restrict notes, 
    enum mah_error* err
);

void mah_invert_nontertian_chord(
    struct mah_nontertian_chord* chord, 
    int inv,
    enum mah_inversion_type inv_type,
    enum mah_error* err
);

void mah_fold_nontertian_chord(
    struct mah_nontertian_chord* chord,
    int fold_levels,
    enum mah_error* err
);

#endif