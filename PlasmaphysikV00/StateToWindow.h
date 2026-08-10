#pragma once

#include "framework.h"



//
// Klasse: Systemzustand
//
// Nutzen: Ist der Zustand eines Systems
//
class Sim_Sys_State
{
public:
    Sim_Sys_State();
    Sim_Sys_State(const unsigned int p);
    unsigned int getID();
    void stateToWindow(HDC hdc);
    std::string stateToString();


    //====
    // Makros um die Form des Zustandes aus der System_config Datei abzulesen
    #define SET(type, name, value) \
        type name = value;

    #define FUNCTION(returnType, name, args, body) \
        returnType name args;

    #define EQUATION(returnType, name, args, body) \
            returnType name args;

    #include "System_config.inc"

    #undef EQUATION
    #undef FUNCTION
    #undef SET

private:
    unsigned int ID;
};



