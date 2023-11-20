#ifndef HEADER_H
#define HEADER_H
#include "header.h"
#endif

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