#pragma once

#include "framework.h"

class Sim_Sys_State;

//
// Klasse: context
//
// Nutzen: ist nötig um die evolution eines Teilchens zu berechnen.
//         context enthält randomness, sowie alle wichtigen Größen die aus der Zustandsklasse kommen können
//
//
class context // TODO: Allgemeiner machen
{
public:
    context(unsigned int d);
    double px = 0;
    double py = 0;
    double pz = 0;
    std::vector<Sim_Sys_State*> prevs;

private:
};




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
    unsigned int id;

    //====
    // Makros um die Form des Zustandes aus der System_config Datei abzulesen
    #define SET(type, name, value) type name = value;
    #define VEC(type, name, size) type name;
    #define FUNCTION(returnType, name, args, body) returnType name args;
    #define EVOLUTION(returnType, name, args, body) returnType evolve_##name args;

    #include "Teilchen_config.inc"

    #undef EVOLUTION
    #undef FUNCTION
    #undef VEC
    #undef SET

private:
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
    unsigned int ID;
    void stateToWindow(HDC hdc, int h, int b);
    std::string stateToString();

        
    //====
    // Makros um die Form des Zustandes aus der System_config Datei abzulesen
    #define SET(type, name, value) type name = value;
    #define VEC(type, name, size) type name;
    #define MAT(type, name, size1, size2) std::vector<std::vector<type>> name;
    #define FUNCTION(returnType, name, args, body) returnType name args;

    #include "System_config.inc"

    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET

private:
};




