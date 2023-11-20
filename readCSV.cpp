#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

// function which reads the csv file and stores the information
void readCSVFile(std::string filename, std::string* track_names, std::string* artist_names, int* artist_counts, 
                                    int* released_years, int* released_months, int* released_days, 
                                    int* in_spotify_playlists, int* in_spotify_charts, int* streams,
                                    int* bpms, std::string* keys, std::string* modes, const int NUM_CSV_LINES)
{
    // when reading file, the last 7 values in every line are not relevant for this project. They should be skipped over in the reading
    // the section on shazam charts should also be skipped
    // read data from file
    std::ifstream readFile(filename);
    std::string input;

    // skip first line of csv file (it is filled with headers, not information on a song)
    getline(readFile, input);

    // parse through each row of the csv file to gather information on a song
    for (int i = 0; i < NUM_CSV_LINES; i++)
    {
        getline(readFile, input); // "select" a row to parse through
        std::stringstream inputString(input);
        std::string val;

        // read track name
        // read artist names STRING 2
        getline(inputString, val, ',');
        track_names[i] = val;
        getline(inputString, val, '"'); // first quotes
        if (val == "") // there are 2+ artists
        {
            getline(inputString, val, '"'); // ending quotes, all name(s) stored in val
            artist_names[i] = val;
            getline(inputString, val, ','); // between last '"' and ',' there is no content, fix formatting so the rest of the file reading works
        }
        else // there is 1 artist
        {
            // restart the reading
            std::stringstream tempStream(input);
            inputString.swap(tempStream);
            getline(inputString, val, ',');
            getline(inputString, val, ',');
            artist_names[i] = val;
        }
        

        // read artist counts
        // read released year
        // read released month
        // read released day
        // read in_spotify_playlists
        // read in_spotify_charts
        // read streams INT 7
        getline(inputString, val, ',');
        artist_counts[i] = stoi(val);
        getline(inputString, val, ',');
        released_years[i] = stoi(val);
        getline(inputString, val, ',');
        released_months[i] = stoi(val);
        getline(inputString, val, ',');
        released_days[i] = stoi(val);
        getline(inputString, val, ',');
        in_spotify_playlists[i] = stoi(val);
        getline(inputString, val, ',');
        in_spotify_charts[i] = stoi(val);
        getline(inputString, val, ',');
        streams[i] = stoi(val);

        // skip in_apple_playlists
        // skip in_apple_charts
        // skip in_deezer_playlists
        // skip in_deezer_charts
        // skip shazam SKIP 5
        for (int j = 0; j < 5; j++)
            getline(inputString, val, ',');

        // read bpm INT 1
        getline(inputString, val, ',');
        bpms[i] = stoi(val);

        // read key
        // read mode STRING 2
        getline(inputString, val, ',');
        keys[i] = val;
        getline(inputString, val, ',');
        modes[i] = val;

        // skip next 7 values SKIP 7
        // for (int j = 0; j < 7; j++)
        //     getline(inputString, val, ',');
    }

    readFile.close();
}