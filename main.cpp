#include "header.h"

int main()
{
    const int NUM_CSV_LINES = 3; // only known by looking at the file beforehand 954
    const int MAX_NUM_PROGRAM_STATES = 20; // will only store 20 different program states at one point

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

// obtains user input in form of a string, continues prompting user until a proper input is given
void getUserInput(std::string location, programState** states, int* index, int* start_index, const int MAX_NUM_STATES)
{
    std::cout << "command: ";
    std::string inputString;
    getline(std::cin, inputString);
    std::stringstream stream1(inputString);
    std::stringstream stream2(inputString);
    std::string word;

    // get the number of words in user input to allocate proper array size
    int numwords = 0;
    const int MAX_WORDS = 3; // all commands will have at most 3 words, or 2 spaces
    while (getline(stream1, word, ' '))
    {
        if (numwords > MAX_WORDS) // reprompt user for another input
        {
            std::cout << "Your input contained too many words. Please try again.\n" << std::endl;
            getUserInput(location, states, index, start_index, MAX_NUM_STATES);
            return;
        }
        numwords++;
    }

    // store the words into an array of strings
    std::string cmd_line[numwords];
    for (int i = 0; i < numwords; i++)
    {
        getline(stream2, word, ' ');
        cmd_line[i] = word;
    }


    // =====================================================
    // HANDLE COMMANDS HERE
    // =====================================================

    if (numwords == 1) // one word commands
    {
        const std::string cmd = cmd_line[0];
        if (cmd == "quit") 
        {
            // exit is normally bad practice, but this program is isolated so local memory (i.e. all used memory) is freed after termination
            exit(0);
        }
        else if (cmd == "help")
        {
            displayHelpPage(states, index, start_index, MAX_NUM_STATES);
            return;
        }
        else if (cmd == "home")
        {
            displayHomePage(states, index, start_index, MAX_NUM_STATES);
            return;
        }
        else if (cmd == "undo")
        {
            processUndoCmd(states, index, start_index, MAX_NUM_STATES, location);
            return;
        }
        else if (cmd == "redo")
        {
            processRedoCmd(states, index, start_index, MAX_NUM_STATES, location);
            return;
        }
        else // unrecognized 1 word command
        {
            std::cout << "unrecognized command. Please try again.\n" << std::endl;
            getUserInput(location, states, index, start_index, MAX_NUM_STATES);
            return;
        }
    }
    else // unrecognized command
    {
        std::cout << "unrecognized command. Please try again.\n" << std::endl;
        getUserInput(location, states, index, start_index, MAX_NUM_STATES);
        return;
    }
}

// error check user input to assure proper input was given
bool validateUserInput(std::string location, std::string userInput)
{
    return true;
}

// helper function for the undo command; switches to appropriate page without overriding page state values
void switchPage(programState** states, int* index, int* start_index, const int MAX_NUM_STATES, std::string page)
{
    if (page == "home")
    {
        printHomePage();
        getUserInput("home", states, index, start_index, MAX_NUM_STATES);
    }
    else if (page == "help")
    {
        printHelpPage();
        getUserInput("help", states, index, start_index, MAX_NUM_STATES);
    }
}

// =============================================================================================
// WRITE ALL DISPLAY PAGES HERE
// =============================================================================================

// displays a list of commands and descriptions on how to use them
void printHelpPage()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "Help with Commands\n" << std::endl;
    std::cout << "help - brings this page up; can be done anywhere" << std::endl;
    std::cout << "home - returns to the home page; can be done anywhere" << std::endl;
    std::cout << "quit - exits the program; can be done anywhere" << std::endl;
    std::cout << "redo - redoes your last undo call; can be done anywhere" << std::endl;
    std::cout << "undo - undoes your last command; can be done anywhere" << std::endl;
    std::cout << std::endl; // give some space to make it easier on the eyes
}

void displayHelpPage(programState** states, int* index, int* start_index, const int MAX_NUM_STATES)
{
    printHelpPage();
    updateProgramStatesArray(states, index, start_index, MAX_NUM_STATES, "help");
    getUserInput("help", states, index, start_index, MAX_NUM_STATES);
}

void printHomePage()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Most Popular Spotify Songs 2023" << std::endl;
    std::cout << "\n\n\nType 'help' for a list of commands" << std::endl;
}

// prints the home page to console when called
void displayHomePage(programState** states, int* index, int* start_index, const int MAX_NUM_STATES)
{
    printHomePage();
    updateProgramStatesArray(states, index, start_index, MAX_NUM_STATES, "home");
    getUserInput("home", states, index, start_index, MAX_NUM_STATES);
}




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
    // if (*index == 2 * MAX_NUM_STATES) // our array is filled, erase old data and make space for new data
    // {
    //     // replace first half of array with second half values; "shift array to the left"
    //     for (int i = 0; i < MAX_NUM_STATES; i++)
    //     {
    //         (*states)[i] = (*states)[i + MAX_NUM_STATES]; // replace values in first half
    //         (*states)[i + MAX_NUM_STATES] = programState(); // erase values in second half; values are erased for clarity
    //     }
    //     (*start_index) = 0;
    //     (*index) = MAX_NUM_STATES; // index of MAX_NUM_STATES will be the next spot open to be changed
    // }
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
}


// ====================================================================================
// MICROSERVICE INTERACTION
// ====================================================================================
// TODO: output to pipeline.txt
// TODO: intput from pipeline.txt

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