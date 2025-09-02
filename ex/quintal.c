#include "mahler.h"
#include <stdio.h>

// Compare Quartal vs Quintal Chord Construction //
/*
    Quartal (P4 intervals): C4-F4-Bb4-Eb5-Ab5
    Quintal (P5 intervals):  C4-G4-D5-A5-E6
    
    Shows how different interval types create different harmonic textures
*/

void print_chord_comparison(struct mah_note quartal_notes[], struct mah_note quintal_notes[], int size, const char* label);

int
main(void)
{
    enum mah_error err = MAH_ERROR_NONE;
    
    printf("=== Quartal vs Quintal Chord Comparison ===\n\n");
    
    // Create buffers for both chord types
    struct mah_note q4_base[5], q4_notes[5];  // quartal
    struct mah_note q5_base[5], q5_notes[5];  // quintal
    
    // Compare different chord sizes
    for (int size = 2; size <= 5; size++) {
        // Create quartal chord (P4 intervals)
        struct mah_nontertian_chord quartal = mah_get_quartal_chord(
            (struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4},
            size, q4_base, q4_notes, &err
        );
        
        if (err != MAH_ERROR_NONE) {
            printf("Error creating quartal chord: %s\n", mah_get_error(err));
            return 1;
        }
        
        // Create quintal chord (P5 intervals)  
        struct mah_nontertian_chord quintal = mah_get_quintal_chord(
            (struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4},
            size, q5_base, q5_notes, &err
        );
        
        if (err != MAH_ERROR_NONE) {
            printf("Error creating quintal chord: %s\n", mah_get_error(err));
            return 1;
        }
        
        char label[50];
        snprintf(label, sizeof(label), "%d-note chords", size);
        print_chord_comparison(quartal.notes, quintal.notes, size, label);
    }
    
    printf("\n=== Inversion Comparison ===\n\n");
    
    // Demonstrate how inversions work differently for each type
    struct mah_nontertian_chord quartal_3 = mah_get_quartal_chord(
        (struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4},
        3, q4_base, q4_notes, &err
    );
    
    struct mah_nontertian_chord quintal_3 = mah_get_quintal_chord(
        (struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4},
        3, q5_base, q5_notes, &err
    );
    
    print_chord_comparison(quartal_3.notes, quintal_3.notes, 3, "Original 3-note chords");
    
    // Standard inversions
    mah_invert_nontertian_chord(&quartal_3, 1, MAH_INV_STANDARD, &err);
    mah_invert_nontertian_chord(&quintal_3, 1, MAH_INV_STANDARD, &err);
    print_chord_comparison(quartal_3.notes, quintal_3.notes, 3, "1st inversion (standard)");
    
    // Reset and do 2nd inversion
    quartal_3 = mah_get_quartal_chord((struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4}, 3, q4_base, q4_notes, &err);
    quintal_3 = mah_get_quintal_chord((struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4}, 3, q5_base, q5_notes, &err);
    mah_invert_nontertian_chord(&quartal_3, 2, MAH_INV_STANDARD, &err);
    mah_invert_nontertian_chord(&quintal_3, 2, MAH_INV_STANDARD, &err);
    print_chord_comparison(quartal_3.notes, quintal_3.notes, 3, "2nd inversion (standard)");
    
    return 0;
}

void
print_chord_comparison(struct mah_note quartal_notes[], struct mah_note quintal_notes[], int size, const char* label)
{
    char disp[MAH_DISP_LEN];
    
    printf("%-25s:\n", label);
    
    // Print quartal chord
    printf("  Quartal (P4): ");
    for (int i = 0; i < size; i++) {
        printf("%-4s", mah_write_note(quartal_notes[i], disp, MAH_DISP_LEN, NULL));
        if (i < size - 1) printf("-");
    }
    printf("\n");
    
    // Print quintal chord
    printf("  Quintal (P5): ");
    for (int i = 0; i < size; i++) {
        printf("%-4s", mah_write_note(quintal_notes[i], disp, MAH_DISP_LEN, NULL));
        if (i < size - 1) printf("-");
    }
    printf("\n\n");
}