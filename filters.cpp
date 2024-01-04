#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

void applyFilter(ProgramState** states, short int* index, std::string filter)
{
    (*states)[(*index) - 1].resetDisplaySongIndicies();
    /* 
    possible vals for filter: 
        none, bpm, _bpm, bpm_ascending, bpm_descending, alphabetical, reverse_alphabetical,
        date, reverse_date, streams_ascending, streams_descending
    */
    if (filter == "none")
    {
        filterByNone(states, index);
        return;
    }

    else if (filter == "bpm")
    {
        short int lower = -1;
        short int upper = -1;

        getBpmRange(lower, upper);

        ProgramState* currentState = &((*states)[(*index) - 1]);
        currentState->lower = lower;
        currentState->upper = upper;

        filterByBpm(states, index, lower, upper);
        return;
    }

    else if (filter == "_bpm")
    {
        ProgramState* currentState = &((*states)[(*index) - 1]);
        currentState->lower = (*states)[(*index) - 2].lower;
        currentState->upper = (*states)[(*index) - 2].upper;
        filterByBpm(states, index, currentState->lower, currentState->upper);
        return;
    }

    else if (filter == "bpm_ascending")
    {
        filterByBpmAscending(states, index);
        return;
    }

    else if (filter == "bpm_descending")
    {
        filterByBpmDescending(states, index);
        return;
    }

    else if (filter == "alphabetical")
    {
        filterByAlphabetical(states, index);
        return;
    }

    else if (filter == "reverse_alphabetical")
    {
        filterByReverseAlphabetical(states, index);
        return;
    }

    else if (filter == "date")
    {
        filterByDate(states, index);
        return;
    }

    else if (filter == "reverse_date")
    {
        filterByReverseDate(states, index);
        return;
    }

    else if (filter == "streams_ascending")
    {
        filterByStreamsAscending(states, index);
        return;
    }

    else if (filter == "streams_descending")
    {
        fitlerByStreamsDescending(states, index);
        return;
    }
} // end of apply filter function

void filterByNone(ProgramState** states, short int* index)
{
    // reminder that index holds that value ready to be changed
    (*states)[(*index) - 1].setAllDisplaySongIndiciesValid();
}

void getBpmRange(short int& lower, short int& upper)
{
    // get bpm range from user
    std::regex reg("([0-9])(([0-9])+)?");
    std::string input = "";

    bool valid_input = false;
    while (!valid_input)
    {
        std::cout << "bpm lower bound: "; // prompt to user
        getline(std::cin, input);
        if (regex_match(input, reg))
        {
            valid_input = true;
        }
    } // if this loop ends, a match was found: 1 - 3 digit number only

    lower = std::stoi(input);
    valid_input = false;
    std::cout << "\n"; // makes terminal output a little easier to read

    while (!valid_input)
    {
        std::cout << "bpm upper bound: "; // prompt to user
        getline(std::cin, input);
        if (regex_match(input, reg) && lower <= std::stoi(input))
        {
            valid_input = true;
            upper = std::stoi(input);
            return;
        }
    } // if this loop ends, a match was found: 1 - 3 digit number only
}

void filterByBpm(ProgramState** states, short int* index, short int lower, short int upper)
{
    ProgramState* currentState = &((*states)[(*index) - 1]);
    currentState->resetDisplaySongIndicies();
    short int* song_indicies = currentState->display_song_indicies;

    // put into request pipeline
    std::string range = std::to_string(lower) + " " + std::to_string(upper);
    std::ofstream writeFile("request_pipeline.txt");
    writeFile << range;
    writeFile.close();

    // listen to response pipeline
    std::string input;
    std::ifstream readFile;
    short int num_checks = 0;
    while (num_checks < 12) // check for response 10 times every second (once every 100ms): wait 1200ms
    {
        readFile.open("response_pipeline.txt");
        getline(readFile, input);
        readFile.close();
        if (input != "")
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        num_checks++;
    }

    // read values and store into song_indicies
    readFile.open("response_pipeline.txt");
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        getline(readFile, input);
        if (input == "")
        {
            break;
        }
        // microservice returns the row numbers, (which start at 1, and the first row is a header row)
        // this means the first song stored at index 0 is at row 2
        song_indicies[i] = (std::stoi(input) - 2);
    }
    readFile.close();

    // clear response pipeline
    writeFile.open("response_pipeline.txt", std::ios::out | std::ios::trunc);
    writeFile.close();
}

void filterByBpmAscending(ProgramState** states, short int* index)
{
    ProgramState* state = &((*states)[(*index) - 1]);
    short int* indicies = state->display_song_indicies;
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = INCREASING_BPM_INDICIES[i];
    }
}

void filterByBpmDescending(ProgramState** states, short int* index)
{
    ProgramState* state = &((*states)[(*index) - 1]);
    short int* indicies = state->display_song_indicies;
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = INCREASING_BPM_INDICIES[NUM_CSV_LINES - i - 1];
    }
}

void filterByAlphabetical(ProgramState** states, short int* index)
{
    ProgramState* state = &((*states)[(*index) - 1]);
    short int* indicies = state->display_song_indicies;
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = ALPHABETICAL_INDICIES[i];
    }
}

void filterByReverseAlphabetical(ProgramState** states, short int* index)
{
    ProgramState* state = &((*states)[(*index) - 1]);
    short int* indicies = state->display_song_indicies;
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = ALPHABETICAL_INDICIES[NUM_CSV_LINES - i - 1];
    }
}

void filterByDate(ProgramState** states, short int* index)
{
    ProgramState* state = &((*states)[(*index) - 1]);
    short int* indicies = state->display_song_indicies;
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = DATE_INDICIES[i];
    }
}

void filterByReverseDate(ProgramState** states, short int* index)
{
    ProgramState* state = &((*states)[(*index) - 1]);
    short int* indicies = state->display_song_indicies;
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = DATE_INDICIES[NUM_CSV_LINES - i - 1];
    }
}

void filterByStreamsAscending(ProgramState** states, short int* index)
{
    ProgramState* state = &((*states)[(*index) - 1]);
    short int* indicies = state->display_song_indicies;
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = STREAM_INDICIES[i];
    }
}

void fitlerByStreamsDescending(ProgramState** states, short int* index)
{
    ProgramState* state = &((*states)[(*index) - 1]);
    short int* indicies = state->display_song_indicies;
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = STREAM_INDICIES[NUM_CSV_LINES - i - 1];
    }
}