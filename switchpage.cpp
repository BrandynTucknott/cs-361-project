#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

// helper function for the undo command; switches to appropriate page without overriding page state values
void switchPage(ProgramState** states, short int* index, short int* start_index, std::string page)
{
    if (page == "home")
    {
        printHomePage();
        getUserInput("home", states, index, start_index);
    }
    else if (page == "help")
    {
        printHelpPage();
        getUserInput("help", states, index, start_index);
    }
    else if (page == "song_list")
    {
        printSongList(states, index);
        getUserInput("song_list", states, index, start_index);
    }
}

// =============================================================================================
// WRITE ALL DISPLAY PAGES HERE
// =============================================================================================

// displays a list of commands and descriptions on how to use them
void printHelpPage()
{
    // std::cout << "\033[2J\033[1;1H"; // functions like the linux 'clear'
    system("clear");
    std::cout << "\n========================================" << std::endl;
    std::cout << "Help with Commands\n" << std::endl;

    std::cout << "filter [CATEGORY] - applys CATEGORY filter to song list; must be done viewing songs" << std::endl;
    std::cout << "   NOTE: only one filter can be applied at a time; leaving this page will remove the filter" << std::endl;
    std::cout << "         CATEGORY - alphabetical" << std::endl;
    std::cout << "                  - bpm" << std::endl;
    std::cout << "                  - bpm ascending" << std::endl;
    std::cout << "                  - bpm descending" << std::endl;
    std::cout << "                  - date" << std::endl;
    std::cout << "                  - none" << std::endl;
    std::cout << "                  - reverse alphabetical" << std::endl;
    std::cout << "                  - reverse date" << std::endl;
    std::cout << "                  - streams ascending" << std::endl;
    std::cout << "                  - streams descending" << std::endl;
    std::cout << "help - brings this page up; can be done anywhere" << std::endl;
    std::cout << "home - returns to the home page; can be done anywhere" << std::endl;
    std::cout << "list songs - lists all songs in database; can be done anywhere" << std::endl;
    std::cout << "page [POSITIVE INTEGER] - switches pages while viewing songs; must be done viewing songs" << std::endl;
    std::cout << "quit - exits the program; can be done anywhere" << std::endl;
    std::cout << "redo - cancels the revert; can be done anywhere" << std::endl;
    std::cout << "undo - reverts your last command; can be done anywhere" << std::endl;
    std::cout << std::endl; // give some space to make it easier on the eyes
}

void displayHelpPage(ProgramState** states, short int* index, short int* start_index)
{
    printHelpPage();
    updateProgramStatesArray(states, index, start_index, "help", -1, "none");
    getUserInput("help", states, index, start_index);
}

void printHomePage()
{
    system("clear");
    std::cout << "========================================" << std::endl;
    std::cout << "Most Popular Spotify Songs 2023" << std::endl;
    std::cout << "\n\n\nType 'help' for a list of commands" << std::endl;
}

// prints the home page to console when called
void displayHomePage(ProgramState** states, short int* index, short int* start_index)
{
    printHomePage();
    updateProgramStatesArray(states, index, start_index, "home", -1, "none");
    getUserInput("home", states, index, start_index);
}


// ===================================================================================================
// PRINTING SONG FUNCTIONALITY HERE
// ===================================================================================================

// helper function for printSongList: adds 'num_spaces' spaces to the end of the given string 's'
void addSpaces(std::string* s, const short int num_spaces)
{
    for (short int i = 0; i < num_spaces; i++)
    {
        (*s) += " ";
    }
}

// prints all songs that satisfy filter when called
void printSongList(ProgramState** states, short int* index)
{
    system("clear");
    ProgramState current_state = (*states)[(*index) - 1];
    const short int* song_indicies = current_state.display_song_indicies;
    short int num_songs = NUM_SONGS_PER_PAGE;
    const short int page_num = current_state.page_num;

    if (song_indicies[0] == -1) // no songs match requirements such as in user specificied bpm range where no songs exist
    {
        std::cout << "\n\nThere are no songs that match your requirements!\n" << std::endl;
        return;
    }

    const std::string filter = current_state.filter;
    if (filter == "streams_ascending" || filter == "streams_descending")
    {
        printSongListStreams(states, index);
        return;
    }

    // what to display
    short int max_length_track_name = 10; // "Track Name" has 10 chars (header)
    short int max_length_artist_name = 9; // "Artist(s)" has 9 chars (header)

    // release date MM-DD-YYYY
    const short int date_length = 12; // max("MM-DD-YYYY".length(), "Release Date".length()(Header)) = max(10, 12)
    const short int bpm_digits = 3; // nothing will go higher than 3 (bc bpm <= 999 is guaranteed)

    // first loop to gather info in order to print it nicely
    for (int i = 0; i < NUM_SONGS_PER_PAGE; i++)
    {
        if (page_num * NUM_SONGS_PER_PAGE + i >= NUM_CSV_LINES)
        {
            num_songs = i;
            break;
        }
        const short int song_index = song_indicies[page_num * NUM_SONGS_PER_PAGE + i];
        if (song_index == -1 || song_index >= NUM_CSV_LINES) // stop, there are no more songs to get
        {
            num_songs = i;
            break;
        }
        // check for new max lengths
        if (TRACK_NAMES[song_index].length() > max_length_track_name)
            max_length_track_name = TRACK_NAMES[song_index].length();
        if (ARTIST_NAMES[song_index].length() > max_length_artist_name)
            max_length_artist_name = ARTIST_NAMES[song_index].length();
    }

    // print header
    // track name --> artist name --> release date --> bpm
    std::string row_to_print = "\n";
    row_to_print += "Track Name";
    addSpaces(&row_to_print, max_length_track_name - 10 + MIN_NUM_SPACES);
    row_to_print += "Artist(s)";
    addSpaces(&row_to_print, max_length_artist_name - 9 + MIN_NUM_SPACES);
    row_to_print += "Release Date";
    addSpaces(&row_to_print, MIN_NUM_SPACES);
    row_to_print += "BPM";
    addSpaces(&row_to_print, MIN_NUM_SPACES);

    std::cout << row_to_print << std::endl;
    const int num_dashes_in_divider = row_to_print.length();

    for (int i = 0; i < num_dashes_in_divider; i++) // print divider
    {
        std::cout << "-";
    }
    std::cout << "\n";

    // second loop here to do the actual printing for each song
    for (int i = 0; i < num_songs; i++)
    {
        const short int song_index = song_indicies[page_num * NUM_SONGS_PER_PAGE + i];
        // track name --> artist name --> release date
        row_to_print = "";
        row_to_print += TRACK_NAMES[song_index];
        addSpaces(&row_to_print, max_length_track_name - TRACK_NAMES[song_index].length() + MIN_NUM_SPACES);

        row_to_print += ARTIST_NAMES[song_index];
        addSpaces(&row_to_print, max_length_artist_name - ARTIST_NAMES[song_index].length() + MIN_NUM_SPACES);

        // construct and add date string
        std::string date = "";
        date += std::to_string(RELEASED_MONTHS[song_index]);
        date += "-";
        date += std::to_string(RELEASED_DAYS[song_index]);
        date += "-";
        date += std::to_string(RELEASED_YEARS[song_index]);
        row_to_print += date;
        addSpaces(&row_to_print, date_length - date.length() + MIN_NUM_SPACES);

        const std::string bpm = std::to_string(BPMS[song_index]);
        row_to_print += bpm;
        addSpaces(&row_to_print, bpm_digits - bpm.length() + MIN_NUM_SPACES);
        std::cout << row_to_print << std::endl;
    }

    // print the footer
    for (int i = 0; i < num_dashes_in_divider / 2 - 4; i++) // print divider
    {
        std::cout << "-";
    }

    std::cout << " Page " << (page_num + 1) << " "; // pages are kept like indicies, starting at 0. Conventionally in UI, it starts at 1
    for (int i = 0; i < num_dashes_in_divider / 2 - 4; i++) // print divider
    {
        std::cout << "-";
    }
    std::cout << "\n\n";
} // end of printSongList function

// returns the number of digits in stream: 100 --> 3; 1000 --> 4; 3 --> 1
short int getNumDigits(long int stream)
{
    short int digits = 0;
    while (stream > 0)
    {
        stream /= 10;
        digits++;
    }

    return digits;
}

// str is a number is string form: "10000" --> "10,000", "23535635" --> "23,535,635", "134465" --> "134,465"
void addCommas(std::string* str, const short int num_commas)
{
    for (short int i = 0; i < num_commas; i++)
    {
        const short index = str->length() - 3 * (i + 1) - i;
        str->insert(index, ",");
    }
}

// the UI changes when printing streams, so a new function is needed to neatly print it
void printSongListStreams(ProgramState** states, short int* index)
{
    ProgramState current_state = (*states)[(*index) - 1];
    const short int* song_indicies = current_state.display_song_indicies;
    short int num_songs = NUM_SONGS_PER_PAGE;
    const short int page_num = current_state.page_num;

    // what to display
    short int max_length_track_name = 10; // "Track Name" has 10 chars (header)
    short int max_length_artist_name = 9; // "Artist(s)" has 9 chars (header)
    short int max_length_streams = 7; // "Streams" has 7 chars (header)

    // first loop to gather info in order to print it nicely
    for (int i = 0; i < NUM_SONGS_PER_PAGE; i++)
    {
        if (page_num * NUM_SONGS_PER_PAGE + i >= NUM_CSV_LINES)
        {
            num_songs = i;
            break;
        }
        const short int song_index = song_indicies[page_num * NUM_SONGS_PER_PAGE + i];
        if (song_index == -1 || song_index >= NUM_CSV_LINES) // stop, there are no more songs to get
        {
            num_songs = i;
            break;
        }
        // check for new max lengths
        if (TRACK_NAMES[song_index].length() > max_length_track_name)
            max_length_track_name = TRACK_NAMES[song_index].length();
        if (ARTIST_NAMES[song_index].length() > max_length_artist_name)
            max_length_artist_name = ARTIST_NAMES[song_index].length();
        const short int digits = getNumDigits(STREAMS[song_index]);
        const short int commas = std::ceil(digits / 3.0) - 1;
        if (digits + commas > max_length_streams)
            max_length_streams = digits;
    }

    // print header
    // track name --> artist name --> Streams
    std::string row_to_print = "\n";
    row_to_print += "Track Name";
    addSpaces(&row_to_print, max_length_track_name - 10 + MIN_NUM_SPACES);
    row_to_print += "Artist(s)";
    addSpaces(&row_to_print, max_length_artist_name - 9 + MIN_NUM_SPACES);
    row_to_print += "Streams";
    addSpaces(&row_to_print, max_length_streams - 7 + MIN_NUM_SPACES);

    std::cout << row_to_print << std::endl;
    const int num_dashes_in_divider = row_to_print.length();

    for (int i = 0; i < num_dashes_in_divider; i++) // print divider
    {
        std::cout << "-";
    }
    std::cout << "\n";

    // second loop here to do the actual printing for each song
    for (int i = 0; i < num_songs; i++)
    {
        const short int song_index = song_indicies[page_num * NUM_SONGS_PER_PAGE + i];
        // track name --> artist name --> release date
        row_to_print = "";
        row_to_print += TRACK_NAMES[song_index];
        addSpaces(&row_to_print, max_length_track_name - TRACK_NAMES[song_index].length() + MIN_NUM_SPACES);

        row_to_print += ARTIST_NAMES[song_index];
        addSpaces(&row_to_print, max_length_artist_name - ARTIST_NAMES[song_index].length() + MIN_NUM_SPACES);

        // construct and add date string
        std::string streams = std::to_string(STREAMS[song_index]);
        const short int digits = getNumDigits(STREAMS[song_index]); // generally better to divide than convert to string and use str.length()
        const short int num_commas = std::ceil(digits / 3.0) - 1;
        addCommas(&streams, num_commas);
        row_to_print += streams;
        addSpaces(&row_to_print, max_length_streams - (digits + num_commas) + MIN_NUM_SPACES);
        std::cout << row_to_print << std::endl;
    }

    // print the footer
    for (int i = 0; i < num_dashes_in_divider / 2 - 4; i++) // print divider
    {
        std::cout << "-";
    }

    std::cout << " Page " << (page_num + 1) << " "; // pages are kept like indicies, starting at 0. Conventionally in UI, it starts at 1
    for (int i = 0; i < num_dashes_in_divider / 2 - 4; i++) // print divider
    {
        std::cout << "-";
    }
    std::cout << "\n\n";
} // end of printSongListStreams function

void displaySongList(ProgramState** states, short int* index, short int* start_index, std::string filter, short int pageNum)
{
    // update vs print order doesn't matter for other functions, but it does for this one
    updateProgramStatesArray(states, index, start_index, "song_list", pageNum, filter);
    applyFilter(states, index, filter);
    printSongList(states, index);
    getUserInput("song_list", states, index, start_index);
}