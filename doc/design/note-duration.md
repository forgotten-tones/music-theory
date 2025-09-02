# AI Prompt: Add Duration Support to Mahler Note System

## Overview
You are tasked with extending the Mahler music theory library's note system to support musical durations while maintaining 100% backwards compatibility. This involves adding comprehensive duration support including standard note values, dotted notes, and tuplets.

## Current Code Analysis

The existing `mah_note` structure is defined as:
```c
typedef struct mah_note
{
    enum mah_tone tone;
    int acci;
    int pitch;
} mah_note;
```

The library follows these conventions:
- Enum values prefixed with `MAH_`
- Header guards format: `#ifndef __MAH_[MODULE]_H__`
- Consistent naming: `mah_[module]_[function]`
- Test macros follow pattern: `ASSERT_[TYPE]` where TYPE indicates what's being tested
- Error handling via `mah_error` enum passed by pointer

## Task 1: Create Note Test Suite

**File**: `test/suites/note/mah_note_functions.test`

Create a comprehensive test suite covering existing note functionality. Based on the existing test patterns, create tests for:

1. Note creation with various tone/accidental/pitch combinations
2. Edge cases with extreme accidentals (MAH_DBFLAT, MAH_DBSHARP)
3. All seven tones (MAH_C through MAH_B)
4. Various pitch ranges
5. Any existing note comparison or utility functions

Follow the existing test macro patterns:
- Use `ASSERT_N(actual, expected)` for note comparisons
- Use `NOTE(tone, acci, pitch)` macro for note creation
- Include both valid and invalid cases with appropriate error assertions

## Task 2: Design Duration System

### Basic Duration Enum

Create `enum mah_dur` in the note header with these values representing fractional relationships to a whole note:

```c
typedef enum mah_dur
{
    // Standard durations (as fractions of whole note)
    MAH_DOTTED_WHOLE,    // 3/2 whole notes
    MAH_WHOLE,           // 1 whole note  
    MAH_DOTTED_HALF,     // 3/4 whole note
    MAH_HALF,            // 1/2 whole note
    MAH_DOTTED_QUARTER,  // 3/8 whole note
    MAH_QUARTER,         // 1/4 whole note
    MAH_DOTTED_EIGHTH,   // 3/16 whole note
    MAH_EIGHTH,          // 1/8 whole note
    MAH_DOTTED_SIXTEENTH,// 3/32 whole note
    MAH_SIXTEENTH,       // 1/16 whole note
    MAH_DOTTED_THIRTYSECOND, // 3/64 whole note
    MAH_THIRTYSECOND,    // 1/32 whole note
    MAH_SIXTYFOURTH,     // 1/64 whole note
    MAH_ONETWENTYEIGHTH, // 1/128 whole note
    
    // Special duration for tuplets
    MAH_TUPLET           // Custom duration via tuplet specification
} mah_dur;
```

### Tuplet System Design

Research indicates tuplets work as follows:
- A tuplet plays N notes in the time normally occupied by M notes of the same base duration
- Total duration = (base_duration × M) / N
- Common examples: triplets (3:2), quintuplets (5:4), septuplets (7:4)

Create a tuplet structure:
```c
typedef struct mah_tuplet
{
    int n;              // Number of notes to play
    int m;              // In the time of this many normal notes
    enum mah_dur base;  // Base duration for the tuplet
} mah_tuplet;
```

## Task 3: Backwards Compatibility Analysis

**Critical Requirement**: Analyze whether adding `enum mah_dur duration` to `mah_note` breaks existing code.

Consider:
1. Structure size changes affecting array allocations
2. Initialization patterns in existing tests
3. Function signatures that pass `mah_note` by value
4. Memory layout assumptions

If adding duration breaks compatibility, design a new structure:
```c
typedef struct mah_timed_note
{
    struct mah_note note;
    enum mah_dur duration;
    struct mah_tuplet* tuplet;  // NULL for non-tuplet notes
} mah_timed_note;
```

## Task 4: Implementation Strategy

Based on your compatibility analysis, implement ONE of these approaches:

### Option A: Extend mah_note (if compatible)
```c
typedef struct mah_note
{
    enum mah_tone tone;
    int acci;
    int pitch;
    enum mah_dur duration;      // Default: MAH_QUARTER
    struct mah_tuplet* tuplet;  // Default: NULL
} mah_note;
```

### Option B: New timed note structure (if incompatible)
Keep `mah_note` unchanged and create `mah_timed_note` as shown above.

## Task 5: Duration Calculation Functions

Implement these functions in `src/note/note.c`:

```c
// Get duration as fraction of whole note (numerator, denominator)
void mah_get_duration_fraction(struct mah_note* note, int* numerator, int* denominator, enum mah_error* err);

// Get duration in ticks (assuming whole note = 1920 ticks, standard MIDI)
int mah_get_duration_ticks(struct mah_note* note, enum mah_error* err);

// Compare durations (-1: a < b, 0: a == b, 1: a > b)
int mah_compare_durations(struct mah_note* a, struct mah_note* b, enum mah_error* err);

// Create tuplet specification
struct mah_tuplet mah_create_tuplet(int n, int m, enum mah_dur base, enum mah_error* err);
```

## Task 6: Rest Convenience Macro

Create a macro for rests that sets tone to a special rest value:

```c
// Add to mah_tone enum
typedef enum mah_tone
{
    MAH_REST = -1,  // Special value for rests
    MAH_C = 0,
    MAH_D,
    MAH_E,
    MAH_F,
    MAH_G,
    MAH_A,
    MAH_B
} mah_tone;

// Convenience macro for rests
#define REST(duration) ((struct mah_note) {MAH_REST, 0, 0, duration, NULL})
```

## Task 7: Time Signature and Measure Validation

Create time signature support:

```c
typedef struct mah_time_sig
{
    int numerator;    // Beats per measure
    int denominator;  // Note value that gets the beat (4 = quarter note)
} mah_time_sig;

// Validate that notes fit within a measure
enum mah_error mah_validate_measure(struct mah_note* notes, int count, struct mah_time_sig time_sig);
```

## Task 8: Comprehensive Test Suite

**File**: `test/suites/note/mah_duration.test`

Create tests covering:

### Basic Duration Tests
```c
// Test basic duration enum values
ASSERT_D(mah_get_duration_ticks(&NOTE(C, 0, 4, MAH_WHOLE, NULL), &ERR), 1920);
ASSERT_D(mah_get_duration_ticks(&NOTE(D, 0, 4, MAH_HALF, NULL), &ERR), 960);
ASSERT_D(mah_get_duration_ticks(&NOTE(E, 0, 4, MAH_QUARTER, NULL), &ERR), 480);

// Test dotted notes
ASSERT_D(mah_get_duration_ticks(&NOTE(F, 0, 4, MAH_DOTTED_HALF, NULL), &ERR), 1440);
ASSERT_D(mah_get_duration_ticks(&NOTE(G, 0, 4, MAH_DOTTED_QUARTER, NULL), &ERR), 720);
```

### Tuplet Tests
```c
// Triplet: 3 notes in time of 2 quarters = 480 * 2 / 3 = 320 ticks each
struct mah_tuplet triplet = mah_create_tuplet(3, 2, MAH_QUARTER, &ERR);
ASSERT_D(mah_get_duration_ticks(&NOTE(A, 0, 4, MAH_TUPLET, &triplet), &ERR), 320);

// Quintuplet: 5 notes in time of 4 quarters = 480 * 4 / 5 = 384 ticks each  
struct mah_tuplet quintuplet = mah_create_tuplet(5, 4, MAH_QUARTER, &ERR);
ASSERT_D(mah_get_duration_ticks(&NOTE(B, 0, 4, MAH_TUPLET, &quintuplet), &ERR), 384);
```

### Time Signature Validation Tests
```c
// 4/4 time: should accept 4 quarter notes
struct mah_time_sig common_time = {4, 4};
struct mah_note measure_44[] = {
    NOTE(C, 0, 4, MAH_QUARTER, NULL),
    NOTE(D, 0, 4, MAH_QUARTER, NULL), 
    NOTE(E, 0, 4, MAH_QUARTER, NULL),
    NOTE(F, 0, 4, MAH_QUARTER, NULL)
};
ASSERT_D(mah_validate_measure(measure_44, 4, common_time), MAH_ERROR_NONE);

// 3/4 time: should reject 4 quarter notes
struct mah_time_sig waltz_time = {3, 4};
ASSERT_E(mah_validate_measure(measure_44, 4, waltz_time), ERROR_INVALID_MEASURE_DURATION);

// Complex example: dotted half + quarter in 4/4
struct mah_note complex_measure[] = {
    NOTE(G, 0, 4, MAH_DOTTED_HALF, NULL),  // 3 beats
    NOTE(A, 0, 4, MAH_QUARTER, NULL)       // 1 beat
};
ASSERT_D(mah_validate_measure(complex_measure, 2, common_time), MAH_ERROR_NONE);
```

### Rest Tests  
```c
// Test rest creation and duration
ASSERT_N(REST(MAH_QUARTER), NOTE(MAH_REST, 0, 0, MAH_QUARTER, NULL));
ASSERT_D(mah_get_duration_ticks(&REST(MAH_HALF), &ERR), 960);
```

### Error Handling Tests
```c
// Invalid tuplet ratios
ASSERT_E(mah_create_tuplet(0, 2, MAH_QUARTER, &ERR), ERROR_INVALID_TUPLET);
ASSERT_E(mah_create_tuplet(3, 0, MAH_QUARTER, &ERR), ERROR_INVALID_TUPLET);

// Invalid time signatures
ASSERT_E(mah_validate_measure(measure_44, 4, (struct mah_time_sig){0, 4}), ERROR_INVALID_TIME_SIG);
```

## Task 9: Update Test Macros

Add new macros to `test/macros.h`:

```c
// Duration testing macros
#define ASSERT_DUR(act, exp) ASSERT(mah_compare_durations(&act, &exp, &ERR) == 0 && ERR == MAH_ERROR_NONE, exp)
#define TUPLET(n, m, base) ((struct mah_tuplet) {n, m, base})
#define TIMED_NOTE(tone, acci, pitch, dur, tuplet_ptr) ((struct mah_note) {MAH_ ## tone, acci, pitch, dur, tuplet_ptr})

// Time signature macro
#define TIME_SIG(num, den) ((struct mah_time_sig) {num, den})
```

## Task 10: Error Code Extensions

Add new error codes to the error enum:

```c
MAH_ERROR_INVALID_TUPLET,
MAH_ERROR_INVALID_MEASURE_DURATION, 
MAH_ERROR_INVALID_TIME_SIG,
MAH_ERROR_INVALID_DURATION
```

## Implementation Requirements

1. **Maintain all existing functionality**: Every current test must still pass
2. **Follow existing patterns**: Use the same naming conventions, error handling, and code structure
3. **Comprehensive testing**: Test all edge cases, error conditions, and valid combinations
4. **Documentation**: Add clear comments explaining duration calculations and tuplet math
5. **Memory management**: Handle tuplet pointer allocation/deallocation properly if needed

## Success Criteria

- [ ] All existing tests continue to pass (100% backwards compatibility)
- [ ] New note test suite covers all existing note functionality  
- [ ] Duration system supports all 14 specified note values
- [ ] Tuplet system handles arbitrary N:M ratios correctly
- [ ] Time signature validation prevents measure overflow
- [ ] Rest macro works as expected
- [ ] All new functionality has comprehensive test coverage
- [ ] Code follows existing library conventions exactly

## Validation Steps

1. Run existing test suite - all tests must pass
2. Run new note tests - verify existing functionality works
3. Run duration tests - verify all duration calculations are correct
4. Test measure validation with various time signatures and note combinations
5. Verify tuplet math with hand calculations for common cases (triplets, quintuplets)
6. Test error conditions and boundary cases
7. Confirm memory usage is appropriate (no leaks, proper cleanup)