#include "mahler.h"
#include <stdio.h>

// Demonstrate Quartal Chord Construction and Manipulation //
/*
    C4-F4-Bb4-Eb5 (4-note quartal chord on C)
    F4-Bb4-Eb5-C5 (1st inversion standard)
    F4-Bb4-Eb5-C6 (1st inversion full)
    F4-Bb4-Eb5-C5 (full inversion folded 1 level)
    F4-Bb4-Eb4-C5 (full inversion folded 2 levels)
*/

void print_chord(struct mah_nontertian_chord chord, const char* label);

int
main(void)
{
    enum mah_error err = MAH_ERROR_NONE;
    
    // Create buffers for chord notes
    struct mah_note base[5], notes[5];
    
    printf("=== Quartal Chord Examples ===\n\n");
    
    // Generate 4-note quartal chord on C4: C4-F4-Bb4-Eb5
    struct mah_nontertian_chord quartal = mah_get_quartal_chord(
        (struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4},
        4, base, notes, &err
    );
    
    if (err != MAH_ERROR_NONE) {
        printf("Error creating quartal chord: %s\n", mah_get_error(err));
        return 1;
    }
    
    print_chord(quartal, "Original 4-note quartal chord on C4");
    
    // Standard inversion - move lowest note up one octave
    mah_invert_nontertian_chord(&quartal, 1, MAH_INV_STANDARD, &err);
    print_chord(quartal, "1st inversion (standard)");
    
    // Reset and try full inversion - move lowest note up until it's highest
    quartal = mah_get_quartal_chord(
        (struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4},
        4, base, notes, &err
    );
    mah_invert_nontertian_chord(&quartal, 1, MAH_INV_FULL, &err);
    print_chord(quartal, "1st inversion (full)");
    
    // Demonstrate folding on the full inversion - bring wide chords into playable range
    mah_fold_nontertian_chord(&quartal, 1, &err);
    print_chord(quartal, "Full inversion folded 1 level");
    
    mah_fold_nontertian_chord(&quartal, 1, &err);
    print_chord(quartal, "Full inversion folded 2 levels total");
    
    printf("\n=== Different Chord Sizes ===\n\n");
    
    // Show different chord sizes
    for (int size = 2; size <= 5; size++) {
        quartal = mah_get_quartal_chord(
            (struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4},
            size, base, notes, &err
        );
        
        char label[50];
        snprintf(label, sizeof(label), "%d-note quartal chord", size);
        print_chord(quartal, label);
    }
    
    return 0;
}

void
print_chord(struct mah_nontertian_chord chord, const char* label)
{
    char disp[MAH_DISP_LEN];
    
    printf("%-30s: ", label);
    for (int i = 0; i < chord.size; i++) {
        printf("%-4s", mah_write_note(chord.notes[i], disp, MAH_DISP_LEN, NULL));
        if (i < chord.size - 1) printf("-");
    }
    printf("\n");
}