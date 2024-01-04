#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

// obtains user input in form of a string, continues prompting user until a proper input is given
void getUserInput(std::string location, ProgramState** states, short int* index, short int* start_index)
{
    std::cout << "command: ";
    std::string inputString;
    getline(std::cin, inputString);
    std::stringstream stream1(inputString);
    std::stringstream stream2(inputString);
    std::string word;

    // get the number of words in user input to allocate proper array size
    short int numwords = 0;
    const int MAX_WORDS = 3; // all commands will have at most 3 words, or 2 spaces
    while (getline(stream1, word, ' '))
    {
        if (numwords > MAX_WORDS) // reprompt user for another input
        {
            std::cout << "Your input contained too many words. Please try again.\n" << std::endl;
            getUserInput(location, states, index, start_index);
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

        /*
        * quit
        * help
        * home
        * undo
        * redo
        */
        if (cmd == "quit") 
        {
            // exit is normally bad practice, but this program is isolated so local memory (i.e. all used memory) is freed after termination
            exit(0);
        }
        else if (cmd == "help")
        {
            displayHelpPage(states, index, start_index);
            return;
        }
        else if (cmd == "home")
        {
            displayHomePage(states, index, start_index);
            return;
        }
        else if (cmd == "undo")
        {
            processUndoCmd(states, index, start_index, location);
            return;
        }
        else if (cmd == "redo")
        {
            processRedoCmd(states, index, start_index, location);
            return;
        }
    } // end of 1 word commands

    else if (numwords == 2) // two word commands
    {
        const std::string cmd0 = cmd_line[0];
        const std::string cmd1 = cmd_line[1];

        /*
        * list songs
        * filter none
        * filter bpm
        * filter alphabetical
        * filter date
        * page [POS INT]
        */

        if (cmd0 == "list" && cmd1 == "songs") // list songs
        {
            displaySongList(states, index, start_index, "none", 0);
            return;
        }
        else if (cmd0 == "filter") // list of all filter commands
        {
            if (location != "song_list") // invalid location
            {
                std::cout << "you must be viewing a list of songs to filter them. To view songs, type 'list songs'.\n" << std::endl;
                getUserInput(location, states, index, start_index);
                return;
            }

            else if (cmd1 == "none") // no filter
            {
                displaySongList(states, index, start_index, "none", 0);
                return;
            }
            else if (cmd1 == "bpm") // filter by bpm
            {
                displaySongList(states, index, start_index, "bpm", 0);
                return;
            }
            else if (cmd1 == "alphabetical")
            {
                displaySongList(states, index, start_index, "alphabetical", 0);
                return;
            }
            else if (cmd1 == "date")
            {
                displaySongList(states, index, start_index, "date", 0);
                return;
            }
            else // unrecognized command
            {
                std::cout << "Unrecognized filter method.\n" << std::endl;
                getUserInput(location, states, index, start_index);
                return;
            }
        } // end filter section

        else if (cmd0 == "page")
        {
            if (location != "song_list")
            {
                std::cout << "Can only switch pages while viewing a song list.\n" << std::endl;
                getUserInput(location, states, index, start_index);
                return;
            }

            // check that the next line is a number
            std::regex reg("([0-9])|([0-9])([0-9])");
            if (regex_match(cmd1, reg)) // 1 or 2 digit number
            {
                short int page = stoi(cmd1) - 1; // convert from natural index to computer index
                ProgramState currentState = (*states)[(*index) - 1];
                short int numPages = 10; // 10 pages: 0 - 9 inclusive

                // find out the max number of pages for this particular filter
                for (short int i = 0; i < NUM_CSV_LINES / NUM_SONGS_PER_PAGE; i++)
                {
                    if (currentState.display_song_indicies[NUM_SONGS_PER_PAGE * i] == -1)
                    {
                        numPages = i;
                        break;
                    }
                }

                // if the page requested has nothing on it
                if (page > numPages - 1)
                {
                    std::cout << "That page number is too large. Max pages for this filter is " << (numPages) << ".\n" << std::endl;
                    getUserInput(location, states, index, start_index);
                    return;
                }
                if (currentState.filter == "bpm") // special case with user input before songs are displayed. Do not get this input multiple times
                {
                    displaySongList(states, index, start_index, "_bpm", page);
                }
                else
                {
                    displaySongList(states, index, start_index, currentState.filter, page);
                }
                return;
            }
        }
    } // end of 2 word commands

    else if (numwords == 3) // three word commands
    {
        const std::string cmd0 = cmd_line[0];
        const std::string cmd1 = cmd_line[1];
        const std::string cmd2 = cmd_line[2];

        /*
        * filter bpm ascending
        * filter bpm descending
        * filter reverse alphabetical
        */
        if (cmd0 == "filter")
        {
            if (location != "song_list") // invalid location
            {
                std::cout << "you must be viewing a list of songs to filter them. To view songs, type 'list songs'.\n" << std::endl;
                getUserInput(location, states, index, start_index);
                return;
            }
            
            if (cmd1 == "bpm")
            {
                if (cmd2 == "ascending")
                {
                    displaySongList(states, index, start_index, "bpm_ascending", 0);
                    return;
                }

                else if (cmd2 == "descending")
                {
                    displaySongList(states, index, start_index, "bpm_descending", 0);
                    return;
                }
                else // unrecognized command
                {
                    std::cout << "Unrecognized filter bpm method.\n" << std::endl;
                    getUserInput(location, states, index, start_index);
                    return;
                }
            } // end of bpm

            if (cmd1 == "streams")
            {
                if (cmd2 == "ascending")
                {
                    displaySongList(states, index, start_index, "streams_ascending", 0);
                    return;
                }

                else if (cmd2 == "descending")
                {
                    displaySongList(states, index, start_index, "streams_descending", 0);
                    return;
                }
                else // unrecognized command
                {
                    std::cout << "Unrecognized filter streams method.\n" << std::endl;
                    getUserInput(location, states, index, start_index);
                    return;
                }
            } // end of streams

            else if (cmd1 == "reverse" && cmd2 == "alphabetical")
            {
                displaySongList(states, index, start_index, "reverse_alphabetical", 0);
                return;
            }

            else if (cmd1 == "reverse" && cmd2 == "date")
            {
                displaySongList(states, index, start_index, "reverse_date", 0);
                return;
            }

            else // unrecognized command
            {
                std::cout << "Unrecognized filter method.\n" << std::endl;
                getUserInput(location, states, index, start_index);
                return;
            }
        } // end of filter
    } // end of 3 word commands

    // unrecognized command if the program makes it here
    std::cout << "Unrecognized command.\n" << std::endl;
    getUserInput(location, states, index, start_index);
    return;
}