#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <regex>
#include <chrono>
#include <thread>
#include <algorithm>
#include <limits>
#include <cmath>

// only known by looking at the file beforehand: 953 songs in csv file
#define NUM_CSV_LINES 953

// will only store information for at most 20 consecutive undos
#define MAX_NUM_STATES 20

// how many spaces to put at the end of each data piece when printing to terminal (minimum)
#define MIN_NUM_SPACES 2

// how many songs will be displayed at most per page
#define NUM_SONGS_PER_PAGE 100

const std::string* TRACK_NAMES;
const std::string* ARTIST_NAMES;
const short int* RELEASED_YEARS;
const short int* RELEASED_MONTHS;
const short int* RELEASED_DAYS;
const long int* STREAMS;
const short int* BPMS;

const short int* ALPHABETICAL_INDICIES;
const short int* INCREASING_BPM_INDICIES;
const short int* DATE_INDICIES;
const short int* STREAM_INDICIES;

struct ProgramState
{
    
    std::string displayPage; // type of page
    short int display_song_indicies[NUM_CSV_LINES];
    short int page_num; // page in list songs if applicable
    std::string filter; // filter mode

    short int lower; // bounds on bpm filter input by user
    short int upper;

    ProgramState()
    {
        displayPage = "init. val";
        filter = "init. val";
        page_num = -10;
        lower = -10;
        upper = -10;
        setAllDisplaySongIndiciesValid();
    }

    ProgramState(std::string location, short int pageNum, std::string fltr)
    {
        displayPage = location;
        page_num = pageNum;
        filter = fltr;

        lower = -10;
        upper = -10;
        setAllDisplaySongIndiciesValid();
    }

    void reset()
    {
        displayPage = "";
        filter = "";
        page_num = -1;
        lower = -1;
        upper = -1;
        resetDisplaySongIndicies();
    }

    void resetDisplaySongIndicies()
    {
        for (int i = 0; i < NUM_CSV_LINES; i++)
        {
            display_song_indicies[i] = -1; // no indicies are valid unless otherwise specified
        }
    }

    void setAllDisplaySongIndiciesValid()
    {
        for (int i = 0; i < NUM_CSV_LINES; i++)
        {
            display_song_indicies[i] = i;
        }
    }
};

void readCSVFile(std::string, std::string*, std::string*, short int*, short int*, short int*, long int*, short int*);

void swap(short int*, short int, short int);
void sortByBpm(short int*);
void sortByAlphabetical(short int*);
void sortByReleaseDate(short int*);
void sortByStreams(short int*);

void getUserInput(std::string, ProgramState**, short int*, short int*);

void switchPage(ProgramState**, short int*, short int*, std::string);

void printHelpPage();
void displayHelpPage(ProgramState**, short int*, short int*);
void printHomePage();
void displayHomePage(ProgramState**, short int*, short int*);
void addSpaces(std::string*, const short int);
void applyFilter(ProgramState**, short int*, std::string);
void printSongList(ProgramState**, short int*);
short int getNumDigits(long int);
void addCommas(std::string*, const short int);
void printSongListStreams(ProgramState**, short int*);
void displaySongList(ProgramState**, short int*, short int*, std::string, short int);

void processUndoCmd(ProgramState**, short int*, short int*, std::string);
void processRedoCmd(ProgramState**, short int*, short int*, std::string);
void updateProgramStatesArray(ProgramState**, short int*, short int*, std::string, short int, std::string);

void filterByNone(ProgramState**, short int*);
void getBpmRange(short int&, short int&);
void filterByBpm(ProgramState**, short int*, short int, short int);
void filterByBpmAscending(ProgramState**, short int*);
void filterByBpmDescending(ProgramState**, short int*);
void filterByAlphabetical(ProgramState**, short int*);
void filterByReverseAlphabetical(ProgramState**, short int*);
void filterByDate(ProgramState**, short int*);
void filterByReverseDate(ProgramState**, short int*);
void filterByStreamsAscending(ProgramState**, short int*);
void fitlerByStreamsDescending(ProgramState**, short int*);