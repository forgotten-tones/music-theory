# AI Implementation Prompt: Nontertian Module

## Overview
You are tasked with implementing a new module called `nontertian` for the mahler music theory library. This module will handle quartal and quintal chord construction and manipulation. You must follow the exact same patterns, coding style, and conventions as the existing `chord` module in this library.

## Module Structure
Create the following files:
- `nontertian/quartal.h` - Header file for quartal chord functionality
- `nontertian/quartal.c` - Implementation file for quartal chord functionality  
- `nontertian/quintal.h` - Header file for quintal chord functionality
- `nontertian/quintal.c` - Implementation file for quintal chord functionality

## Coding Style Requirements
You must exactly follow the coding style and conventions used throughout this library:

1. **Naming Conventions:**
   - All public functions start with `mah_`
   - All public structs start with `mah_`
   - All public enums start with `mah_`
   - Use snake_case for all identifiers
   - Enum values use `MAH_` prefix in ALL_CAPS

2. **Function Signatures:**
   - Follow the exact pattern used by `mah_get_chord()`: take root note, chord parameters, buffer pointers, and error pointer
   - Use `enum mah_error* err` as the last parameter for error handling
   - Use `restrict` keyword for pointer parameters where appropriate
   - Return structs by value, not by pointer

3. **Error Handling:**
   - Use the existing `enum mah_error` types
   - Use `RETURN_EMPTY_STRUCT_ERR(type, error)` macro for early returns with errors
   - Use `SET_ERR(error)` macro to set error values
   - Check for invalid parameters and set appropriate errors

4. **Memory Management:**
   - Follow the existing pattern where the caller provides memory buffers
   - Never allocate memory internally
   - Use the same buffer pattern as `mah_get_chord()` with separate `base` and `notes` arrays

## Required Data Structures

Define these structures following the existing patterns:

```c
// Base structure for nontertian chord definitions (similar to mah_chord_base)
typedef struct mah_nontertian_base
{
    char const* name;
    int size;
    enum mah_interval_type interval_type;  // P4 for quartal, P5 for quintal
} mah_nontertian_base;

// Runtime nontertian chord structure (similar to mah_chord)
typedef struct mah_nontertian_chord  
{
    int size;
    int inv;
    enum mah_inversion_type inv_type;  // Two inversion types as specified
    struct mah_note* restrict base;
    struct mah_note* restrict notes;
} mah_nontertian_chord;
```

## Required Functionality

### 1. Chord Construction Functions
Implement these core functions following the exact pattern of `mah_get_chord()`:

```c
// Generate quartal chord (P4 intervals)
struct mah_nontertian_chord mah_get_quartal_chord(
    struct mah_note root, 
    int num_notes,  // 2-5 inclusive
    struct mah_note* restrict base,
    struct mah_note* restrict notes, 
    enum mah_error* err
);

// Generate quintal chord (P5 intervals)  
struct mah_nontertian_chord mah_get_quintal_chord(
    struct mah_note root,
    int num_notes,  // 2-5 inclusive  
    struct mah_note* restrict base,
    struct mah_note* restrict notes,
    enum mah_error* err
);
```

### 2. Inversion Support
Implement inversion functions following the pattern of `mah_invert_chord()`. Support two distinct inversion methods:

```c
// Invert nontertian chord with specified inversion type
void mah_invert_nontertian_chord(
    struct mah_nontertian_chord* chord, 
    int inv,
    enum mah_inversion_type inv_type,
    enum mah_error* err
);
```

**Inversion Types:**
1. **Standard Inversion (`MAH_INV_STANDARD`)**: Move lowest note up one octave (like existing `mah_invert_chord`)
2. **Full Inversion (`MAH_INV_FULL`)**: Move lowest note up by octaves until it becomes the highest note

### 3. Folding Operations  
Implement chord folding to bring wide-spread chords into playable range:

```c
// Fold down the highest notes by specified number of levels
void mah_fold_nontertian_chord(
    struct mah_nontertian_chord* chord,
    int fold_levels,  // Number of notes to fold down
    enum mah_error* err
);
```

**Folding Logic:**
- `fold_levels = 1`: Lower the highest note down one octave
- `fold_levels = 2`: Lower the two highest notes down one octave each  
- Continue pattern for higher fold levels
- Validate that fold_levels doesn't exceed chord size - 1

### 4. Error Handling
Add new error types to `enum mah_error` (you'll need to extend the existing enum):
- `MAH_ERROR_INVALID_NONTERTIAN_SIZE` - for invalid number of notes (not 2-5)
- `MAH_ERROR_INVALID_FOLD_LEVEL` - for invalid folding levels

## Implementation Requirements

1. **Use Existing Interval System:** 
   - Use `mah_get_inter()` with `MAH_PERFECT` quality
   - For quartal: use intervals of 4 steps with `MAH_PERFECT` quality
   - For quintal: use intervals of 5 steps with `MAH_PERFECT` quality

2. **Follow Existing Buffer Management:**
   - Caller provides `base` and `notes` arrays like in `mah_get_chord()`
   - `base` array stores the original chord notes (never modified after creation)
   - `notes` array stores the current state (modified by inversions/folding)

3. **Validation:**
   - Check `num_notes` is between 2-5 inclusive
   - Check inversion number doesn't exceed `chord->size - 1`  
   - Check fold levels doesn't exceed `chord->size - 1`
   - Check for null pointers

4. **File Organization:**
   - Put quartal functions in `quartal.c/.h`
   - Put quintal functions in `quintal.c/.h`  
   - Include appropriate headers following existing patterns
   - Use same include guard naming convention: `#ifndef __MAH_QUARTAL_H__`

## Expected Usage Examples

```c
// Create 4-note quartal chord on C
struct mah_note base[4], notes[4];
struct mah_nontertian_chord quartal = mah_get_quartal_chord(
    (struct mah_note){MAH_C, MAH_NATURAL, MAH_OCTAVE_4},
    4, base, notes, &err
);
// Result: C4-F4-Bb4-Eb5

// Apply full inversion (move C up to be highest)
mah_invert_nontertian_chord(&quartal, 1, MAH_INV_FULL, &err);
// Result: F4-Bb4-Eb5-C6

// Fold down 2 levels to make it playable
mah_fold_nontertian_chord(&quartal, 2, &err);  
// Result: F4-Bb4-Eb4-C5
```

## Testing
Follow the existing test patterns in the test suite. Create comprehensive tests covering:
- Chord construction with various root notes and sizes
- Both inversion types
- Folding operations  
- Error conditions
- Edge cases with extreme accidentals and octaves

Ensure your implementation passes all tests and integrates seamlessly with the existing codebase architecture.