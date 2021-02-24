// Assignment 2 20T1 COMP1511: CS bEats
// beats.c
//
// This program was written by YOUR-NAME-HERE (z5555555)
// on INSERT-DATE-HERE
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Fix default return value of add_musical_note_to_beat.
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// Add any extra #includes your code needs here.
#include "beats.h"
// Add your own #defines here.
////////////////////////////////////////////////////////////////////////
// You don't have to use the provided struct track, you are free to
// make your own struct instead.
// If you use the provided struct track, you will have to add fields
// to store other information.
struct track {
    struct beat *head;
    struct track *next;
};
// You don't have to use the provided struct beat, you are free to
// make your own struct instead.
// If you use the provided struct beat, you may add fields
// to it to store other information.
struct beat {
    // You may choose to add or change fields in this struct.
    struct note *notes;
    struct beat *next;
};
// You don't have to use the provided struct note, you are free to
// make your own struct instead.
// If you use the provided struct note, you add fields
// to it to store other information.
struct note {
    // You may choose to add or change fields in this struct.
    int octave;
    int key;
    struct note *next;
};
// Add any other structs you define here.
////////////////////////////////////////////////////////////////////////
// Add prototypes for any extra functions you create here.


// Return a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);
    new_beat->next = NULL;
    new_beat->notes = NULL;
    // You do not need to change this function, unless you change
    // the implementation of struct beat.
    return new_beat;
}
// You need to implement the following functions.
// You can find descriptions of what each function should do in beats.h
////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////
// Add a note to the end of a beat.
int add_note_to_beat(Beat beat, int octave, int key) {
    struct note *head = beat->notes;
    struct note *current = NULL;
    struct note *p = NULL;
    // check for invalid octave input
    if (octave < 0 || octave >= 10) { 
        return INVALID_OCTAVE;
    } 
    // check for invalid key input
    if (key < 0 || key >= 12) { 
        return INVALID_KEY;
    } 
    // create new note and assign user input values
    struct note *new_note = malloc(sizeof(struct note));
    new_note->key = key;
    new_note->octave = octave;
    new_note->next = NULL;
    // check if there are other notes
    if (head == NULL) {
        beat->notes = new_note; // new entry in empty list
        return VALID_NOTE;
    } else if (head->next == NULL) { // when only 1 note is present in list
        current = head;
    } else { // when there are more than 1 note in list
        p = head;
        while (p->next != NULL) {
            p = p->next;
        }
        current = p;
    }
    int key_diff = new_note->key - current->key;
    int octave_diff = new_note->octave - current->octave;
    if (octave_diff < 0) {
        return NOT_HIGHEST_NOTE;
    } else if (octave_diff == 0) {
        if (key_diff <= 0) {
            return NOT_HIGHEST_NOTE;
        } else {
            current->next = new_note;
            return VALID_NOTE;
        }
    } else {
        current->next = new_note;
        return VALID_NOTE;
    }

    
}
// Print the contents of a beat.
void print_beat(Beat beat) {
    struct note *head = beat->notes;
    struct note *p = NULL;
    if (beat->notes == NULL) { // when there are no notes in beat
        printf("\n");
    } else if (beat->notes->next == NULL){ // when there is one note in beat
        printf("%d %02d\n", beat->notes->octave, beat->notes->key);
    } else { // when beat has many notes
        p = head;
        while (p != NULL) {
            printf("%d %02d", p->octave, p->key);
            if (p->next != NULL) {
                printf(" | ");
            } else {
                printf("\n");
            }
            p = p->next;
        }
    }   
    return;
}
// Count the number of notes in a beat that are in a given octave.
int count_notes_in_octave(Beat beat, int octave) {
    struct note *current = beat->notes;
    int total = 0;
    while (current != NULL) {
        if (octave == current->octave) {
            total++;
        }
        current = current->next;
    }
    return total;
}
////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////
// Return a malloced track with fields initialized.
Track create_track(void) {
    Track new_track = malloc(sizeof(struct track));
    new_track->head = NULL;
    new_track->next = NULL;
    // Note: there is no fprintf in this function, as the
    // Stage 1 autotests call create_track but expect it to return NULL
    // (so this function should do nothing in Stage 1).
    return new_track;
}
// Add a beat after the current beat in a track.
void add_beat_to_track(Track track, Beat beat) {
    struct beat *head = track->head;
    struct beat *p = NULL;
    struct beat *new_beat = malloc(sizeof(struct beat));
    new_beat->notes = NULL;
    new_beat->next = NULL;
    if (head == NULL) {
        head = new_beat;
    } else {
        p = head;
        while (head->next != NULL) {
            p = p->next;
        }
        p->next = new_beat;
    }
    return;
}
// Set a track's current beat to the next beat.
int select_next_beat(Track track) {
    printf("select_next_beat not implemented yet.\n");
    return TRACK_STOPPED;
}
// Print the contents of a track.
void print_track(Track track) {
    printf("print_track not implemented yet.\n");
    return;
}
// Count beats after the current beat in a track.
int count_beats_left_in_track(Track track) {
    printf("count_beats_left_in_track not implemented yet.\n");
    return 0;
}
////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////
// Free the memory of a beat, and any memory it points to.
void free_beat(Beat beat) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_beat but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than exit).
    return;
}
// Free the memory of a track, and any memory it points to.
void free_track(Track track) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_track but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than print an error).
    return;
}
// Remove the currently selected beat from a track.
int remove_selected_beat(Track track) {
    printf("remove_selected_beat not implemented yet.");
    return TRACK_STOPPED;
}
////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////
// Add note to beat, given its 'musical notation'.
int add_musical_note_to_beat(Beat beat, char *musical_notation) {
    printf("add_musical_note_to_beat not implemented yet.");
    return VALID_NOTE;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////
// Cut a range of beats to the end of a track.
void cut_range_to_end(Track track, int range_length) {
    printf("cut_range_to_end not implemented yet.");
    return;
}
// Reverse a list of beats within a range of a track.
int reverse_range(Track track, int range_length) {
    printf("reverse_range not implemented yet.");
    return 0;
}
