#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

// "saves a user action" by keeping track of "where they have been"; stores the current state of the program
// the undo/redo buttons are based off of the Google Chrome browser undo/redo
void updateProgramStatesArray(programState** states, int* index, int* start_index, const int MAX_NUM_STATES, std::string location)
{
    programState currentState(location); // create a new program state

    // store that state in the states array; note states array stores 2 * MAX_NUM_STATES
    (*states)[*index] = currentState; // store current state
    (*index)++; // iterate next index value

    // first half of the array is filled, we will begin to ignore old states, and eventaully delete them
    if (*index >= MAX_NUM_STATES)
    {
        (*start_index)++; // ignoring old states
    }

    // when we store a state, erase the next value in line if possible. This will ensure that you cannot redo to an old action
    // at this point, *index has already been iterated, so *index - 1 stores the most recent state and *index holds the future state
    if (*index < 2 * MAX_NUM_STATES - 1) // if *index does not represent the last index in the array
    {
        programState* p = &(*states)[*index];
        p->reset();
        // (*states)[*index].reset(); // erase the next value
    }

    // resizing the array
    if (*index == 2 * MAX_NUM_STATES) // our array is filled, erase old data and make space for new data
    {
        // replace first half of array with second half values; "shift array to the left"
        for (int i = 0; i < MAX_NUM_STATES; i++)
        {
            (*states)[i] = (*states)[i + MAX_NUM_STATES]; // replace values in first half
            (*states)[i + MAX_NUM_STATES] = programState(); // erase values in second half; values are erased for clarity
        }
        (*start_index) = 0;
        (*index) = MAX_NUM_STATES; // index of MAX_NUM_STATES will be the next spot open to be changed
    }
}

// the undo/redo buttons are based off of the Google Chrome browser undo/redo
void processUndoCmd(programState** states, int* index, int* start_index, const int MAX_NUM_STATES, std::string location)
{
    // *index = 0: no input at all
    // *index = 1: index 0 filled, but not index 1. There is no action to undo
    if ((*index > *start_index + 1))
    {
        // -2 is needed bc *index is iterated by 1 to be ready to store the next state. 
        // to access the 2nd most recent state (undo our most recent state), we need to get *index - 2
        programState state_to_restore = (*states)[(*index) - 2];
        (*index)--;
        switchPage(states, index, start_index, MAX_NUM_STATES, state_to_restore.displayPage);
    }
    else
    {
        std::cout << "There is nothing to undo" << std::endl;
        getUserInput(location, states, index, start_index, MAX_NUM_STATES);
    }  
}

void processRedoCmd(programState** states, int* index, int* start_index, const int MAX_NUM_STATES, std::string location)
{
    if ((*states)[*index].displayPage == "") // there is no action to redo into
    {
        std::cout << "There is nothing to redo" << std::endl;
        getUserInput(location, states, index, start_index, MAX_NUM_STATES);
    }
    // TODO: finish redo function. Add case where there is a forward state to redo into
    else
    {
        // recall *index stores the state for the slot to be overwritten in case of user change
        programState state_to_restore = (*states)[(*index)];
        (*index)++;
        switchPage(states, index, start_index, MAX_NUM_STATES, state_to_restore.displayPage);
    }
}