#ifndef __MAH_QUINTAL_H__
#define __MAH_QUINTAL_H__

#include "err/err.h"
#include "inter/inter.h"
#include "note/note.h"
#include "shared/shared.h"
#include "nontertian/quartal.h"  // For shared structs

// Functions //

struct mah_nontertian_chord mah_get_quintal_chord(
    struct mah_note root, 
    int num_notes,
    struct mah_note* restrict base,
    struct mah_note* restrict notes, 
    enum mah_error* err
);

#endif