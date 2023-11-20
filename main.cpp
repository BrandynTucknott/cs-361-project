#include "readCSV.cpp"
#include "userinput.cpp"
#include "undo_redo.cpp"
#include "switchpage.cpp"

#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

int main()
{
    const int NUM_CSV_LINES = 3; // only known by looking at the file beforehand 954
    const int MAX_NUM_PROGRAM_STATES = 20; // will only store information for at most 20 consecutive undos

    // allocate space for arrays that will store song info
    std::string* track_names = (std::string*) malloc(sizeof(std::string) * NUM_CSV_LINES);
    std::string* artist_names = (std::string*) malloc(sizeof(std::string) * NUM_CSV_LINES);
    int* artist_counts = (int*) malloc(sizeof(int) * NUM_CSV_LINES);
    int* released_years = (int*) malloc(sizeof(int) * NUM_CSV_LINES);
    int* released_months = (int*) malloc(sizeof(int) * NUM_CSV_LINES);
    int* released_days = (int*) malloc(sizeof(int) * NUM_CSV_LINES);
    int* in_spotify_playlists = (int*) malloc(sizeof(int) * NUM_CSV_LINES);
    int* in_spotify_charts = (int*) malloc(sizeof(int) * NUM_CSV_LINES);
    int* streams = (int*) malloc(sizeof(int) * NUM_CSV_LINES);
    int* bpms = (int*) malloc(sizeof(int) * NUM_CSV_LINES);
    std::string* keys = (std::string*) malloc(sizeof(std::string) * NUM_CSV_LINES);
    std::string* modes = (std::string*) malloc(sizeof(std::string) * NUM_CSV_LINES);

    programState* states = (programState*) malloc(sizeof(programState) * 2 * MAX_NUM_PROGRAM_STATES);
    states = new programState[2 * MAX_NUM_PROGRAM_STATES]; // initialize values in states array
    int index_in_states_array = 0;
    int valid_state_entries_start_index = 0;

    // get song info
    readCSVFile("spotify-2023.csv", track_names, artist_names, artist_counts, 
                                    released_years, released_months, released_days, 
                                    in_spotify_playlists, in_spotify_charts, streams,
                                    bpms, keys, modes, NUM_CSV_LINES);

    
    
    
    // display home page
    displayHomePage(&states, &index_in_states_array, &valid_state_entries_start_index, MAX_NUM_PROGRAM_STATES);

    // free all allocated memory
    free(track_names);
    free(artist_names);
    free(artist_counts);
    free(released_years);
    free(released_months);
    free(released_days);
    free(in_spotify_playlists);
    free(in_spotify_charts);
    free(streams);
    free(bpms);
    free(keys);
    free(modes);

    return 0;
}

// ====================================================================================
// MICROSERVICE INTERACTION
// ====================================================================================
// TODO: output to pipeline.txt
// TODO: intput from pipeline.txt