#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

// helper function: swaps the two indicies of array: assumes valid input
void sw ap(short int* array, short int i, short int j)
{
    if (i == j)
        return;
    const short int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

void sortByBpm(short int* indicies)
{
    // initialize
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = i;
    }

    // sort
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        // bpms are positive integers (no larger than triple digits in given data)
        short int current_smallest = 1000;
        short int current_smallest_index = -1;

        for (short int j = i; j < NUM_CSV_LINES; j++)
        {
            const short int bpm = BPMS[indicies[j]];
            if (bpm < current_smallest)
            {
                current_smallest = bpm;
                current_smallest_index = j;
            }
        }
        swap(indicies, i, current_smallest_index);
    }
}

void sortByAlphabetical(short int* indicies)
{
    // initialize
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = i;
    }

    // sort
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        // guarantees any song compared to this will go before it
        std::string current_smallest = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
        short int current_smallest_index = -1;

        for (short int j = i; j < NUM_CSV_LINES; j++)
        {
            std::string str = TRACK_NAMES[indicies[j]];
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            const int comp = str.compare(current_smallest);
            if (comp < 0)
            {
                current_smallest = str;
                current_smallest_index = j;
            }
        }
        swap(indicies, i, current_smallest_index);
    }
}

void sortByReleaseDate(short int* indicies)
{
    // initialize
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = i;
    }

    // sort
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        short int current_smallest_year = 2024; // all songs were made in 2023 or before
        short int current_smallest_month = 13; // only 12 months in a year
        short int current_smallest_day = 32; // never more than 31 days in a month
        short int current_smallest_index = -1;

        for (short int j = i; j < NUM_CSV_LINES; j++)
        {
            const short int year = RELEASED_YEARS[indicies[j]];
            const short int month = RELEASED_MONTHS[indicies[j]];
            const short int day = RELEASED_DAYS[indicies[j]];

            const bool same_year_and_month = year == current_smallest_year && month == current_smallest_month;
            if (year < current_smallest_year)
            {
                current_smallest_year = year;
                current_smallest_month = month;
                current_smallest_day = day;
                current_smallest_index = j;
            }

            else if (month < current_smallest_month && year == current_smallest_year) // years are the same, but months are not
            {
                current_smallest_year = year;
                current_smallest_month = month;
                current_smallest_day = day;
                current_smallest_index = j;
            }

            else if (day < current_smallest_day && same_year_and_month) // years and months are the same, but days are not
            {
                current_smallest_year = year;
                current_smallest_month = month;
                current_smallest_day = day;
                current_smallest_index = j;
            }
            // else the date is the same, do nothing
        }
        // move smallest date index to current front (index i) of the array
        swap(indicies, i, current_smallest_index);
    }
}

void sortByStreams(short int* indicies)
{
    // initialize
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        indicies[i] = i;
    }

    // sort
    for (short int i = 0; i < NUM_CSV_LINES; i++)
    {
        // bpms are positive integers (no larger than triple digits in given data)
        long int current_smallest = std::numeric_limits<long int>::max();
        short int current_smallest_index = -1;

        for (short int j = i; j < NUM_CSV_LINES; j++)
        {
            const long int streams = STREAMS[indicies[j]];
            if (streams < current_smallest)
            {
                current_smallest = streams;
                current_smallest_index = j;
            }
        }
        swap(indicies, i, current_smallest_index);
    }
}