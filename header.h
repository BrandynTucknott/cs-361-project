#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>

struct programState
{
    std::string displayPage = "this value has not been initialized";

    programState()
    {
        displayPage = "";
    }

    programState(std::string location)
    {
        displayPage = location;
    }

    void reset()
    {
        displayPage = "";
    }
};

void readCSVFile(std::string, std::string*, std::string*, int*, int*, int*, int*, int*, int*, int*, int*, std::string*, std::string*, const int);
void getUserInput(std::string, programState**, int*, int*, const int);
bool validateUserInput(std::string, std::string);
void switchPage(programState**, int*, int*, const int, std::string);

void printHelpPage();
void displayHelpPage(programState**, int*, int*, const int);
void printHomePage();
void displayHomePage(programState**, int*, int*, const int);

void processUndoCmd(programState**, int*, int*, const int, std::string);
void processRedoCmd(programState**, int*, int*, const int, std::string);

void updateProgramStatesArray(programState**, int*, int*, const int, std::string);