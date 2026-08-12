#pragma once

#include "framework.h"


//
// Klasse: Teilchen
//
// Nutzen: entspricht einem Teilchen
//
class Teilchen
{
public:
    Teilchen();
    Teilchen(const unsigned int p);
    unsigned int getID();

    //====
    // Makros um die Form des Zustandes aus der System_config Datei abzulesen
    #define SET(type, name, value)type name = value;

    #define FUNCTION(returnType, name, args, body) \
                returnType name args;

    #define EQUATION(returnType, name, args, body) \
                    returnType name args;

    #include "Teilchen_config.inc"

    #undef EQUATION
    #undef FUNCTION
    #undef SET

private:
    unsigned int ID;
};

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
    void stateToWindow(HDC hdc, int h, int b);
    std::string stateToString();


    //====
    // Makros um die Form des Zustandes aus der System_config Datei abzulesen
    #define SET(type, name, value) type name = value;
    #define VEC(type, name, size) type name;
    #define FUNCTION(returnType, name, args, body) returnType name args;

    #define EQUATION(returnType, name, args, body) \
            returnType name args;

    #include "System_config.inc"

    #undef EQUATION
    #undef FUNCTION
    #undef VEC
    #undef SET

private:
    unsigned int ID;
};



