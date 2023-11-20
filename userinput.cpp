#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

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
    } // end of 1 word commands

    else if (numwords == 2) // two word commands
    {
        
    } // end of 2 word commands
    else // unrecognized command
    {
        std::cout << "unrecognized command. Please try again.\n" << std::endl;
        getUserInput(location, states, index, start_index, MAX_NUM_STATES);
        return;
    }
}