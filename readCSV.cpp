#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

// function which reads the csv file and stores the information
void readCSVFile(std::string filename, std::string* track_names, std::string* artist_names, 
                                    short int* released_years, short int* released_months, short int* released_days, 
                                    long int* streams, short int* bpms)
{
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
        
        // read released year
        // read released month
        // read released day
        // skip in_spotify_playlists
        // skip in_spotify_charts
        // read streams INT 7
        getline(inputString, val, ','); // read
        released_years[i] = stoi(val);
        getline(inputString, val, ','); // read
        released_months[i] = stoi(val);
        getline(inputString, val, ','); // read
        released_days[i] = stoi(val);
        getline(inputString, val, ','); // skip
        getline(inputString, val, ','); // skip
        getline(inputString, val, ','); // read
        streams[i] = stol(val); // String TO Long

        // read bpm INT 1
        getline(inputString, val, ',');
        bpms[i] = stoi(val);

        // skip key
        // skip mode
        getline(inputString, val, ',');
        getline(inputString, val, ',');
    }

    readFile.close();
}