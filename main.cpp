#include "readCSV.cpp"
#include "userinput.cpp"
#include "undo_redo.cpp"
#include "switchpage.cpp"
#include "filters.cpp"
#include "sorting.cpp"

#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

int main()
{
    // erase all content in files before start to be safe
    std::ofstream writeFile("request_pipeline.txt", std::ios::out | std::ios::trunc);
    writeFile.close();
    writeFile.open("response_pipeline.txt", std::ios::out | std::ios::trunc);
    writeFile.close();

    // allocate space for arrays that will store song info
    std::string* track_names = (std::string*) malloc(sizeof(std::string) * NUM_CSV_LINES);
    std::string* artist_names = (std::string*) malloc(sizeof(std::string) * NUM_CSV_LINES);
    short int* released_years = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES); // ####
    short int* released_months = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES); // 1 - 12
    short int* released_days = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES); // 1 - 31
    short int* in_spotify_playlists = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES);
    short int* in_spotify_charts = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES);
    long int* streams = (long int*) malloc(sizeof(long int) * NUM_CSV_LINES);
    short int* bpms = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES);
    std::string* keys = (std::string*) malloc(sizeof(std::string) * NUM_CSV_LINES);
    std::string* modes = (std::string*) malloc(sizeof(std::string) * NUM_CSV_LINES);

    ProgramState* states = (ProgramState*) malloc(sizeof(ProgramState) * 2 * MAX_NUM_STATES);
    states = new ProgramState[2 * MAX_NUM_STATES]; // initialize values in states array
    short int index_in_states_array = 0;
    short int valid_state_entries_start_index = 0;

    // get song info
    readCSVFile("spotify-2023.csv", track_names, artist_names, 
                                    released_years, released_months, released_days, 
                                    streams, bpms);

    TRACK_NAMES = track_names;
    ARTIST_NAMES = artist_names;
    RELEASED_YEARS = released_years;
    RELEASED_MONTHS = released_months;
    RELEASED_DAYS = released_days;
    STREAMS = streams;
    BPMS = bpms;

    short int* alphabetical_indicies = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES);
    short int* increasing_bpm_indicies = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES);
    short int* date_indicies = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES);
    short int* stream_indicies = (short int*) malloc(sizeof(short int) * NUM_CSV_LINES);

    sortByBpm(increasing_bpm_indicies);
    sortByAlphabetical(alphabetical_indicies);
    sortByReleaseDate(date_indicies);
    sortByStreams(stream_indicies);

    ALPHABETICAL_INDICIES = alphabetical_indicies;
    INCREASING_BPM_INDICIES = increasing_bpm_indicies;
    DATE_INDICIES = date_indicies;
    STREAM_INDICIES = stream_indicies;
    
    // display home page
    displayHomePage(&states, &index_in_states_array, &valid_state_entries_start_index);

    // free all allocated memory
    free(track_names);
    free(artist_names);
    free(released_years);
    free(released_months);
    free(released_days);
    free(streams);
    free(bpms);

    free(alphabetical_indicies);
    free(increasing_bpm_indicies);
    free(date_indicies);
    free(stream_indicies);

    return 0;
}