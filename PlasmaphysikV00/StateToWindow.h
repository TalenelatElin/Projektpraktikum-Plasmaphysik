#pragma once

#include "../PlasmaphysikV00/framework.h"

class Sim_Sys_State;

//
// Klasse: context
//
// Nutzen: ist nötig um die evolution eines Teilchens zu berechnen.
//         context enthält randomness, sowie alle wichtigen Größen die aus der Zustandsklasse kommen können
//
//
class context
{
public:
    context(const float ph, const unsigned short pdepth, const std::size_t pN, const unsigned short pNumPara);

    float h;
    std::vector<Sim_Sys_State*> prevs;
    std::vector<std::vector<double>> probs;

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
    Teilchen(const std::size_t pid);
    std::size_t getid();

    // Makros
    #define SET(type, name, value) type name = value;
    #define VEC(type, name, size) type name;
    #define FUNCTION(returnType, name, args, body) returnType name args;
    #define EVOLUTION(returnType, name, args, body) returnType evolve_##name args;

    #include "../PlasmaphysikV00/Configs/Teilchen_config.inc"

    #undef EVOLUTION
    #undef FUNCTION
    #undef VEC
    #undef SET

private:
    std::size_t id;
};




//
// Klasse: Systemzustand
//
// Nutzen: Ist der Zustand eines Systems
//
class Sim_Sys_State
{
public:
    Sim_Sys_State(const std::vector<Teilchen> pparticles);
    Sim_Sys_State(const std::vector<Teilchen> pparticles, const std::size_t pID);
    Sim_Sys_State(const std::size_t pN);
    Sim_Sys_State(const std::size_t pN, const std::size_t pID);
    std::size_t getID();

    void stateToWindow(HDC hdc, int h, int b);
    std::string stateToString();

    std::vector<Teilchen> particles;

        
    // Makros
    #define SET(type, name, value) type name = value;
    #define VEC(type, name, size) type name;
    #define MAT(type, name, size1, size2) std::vector<std::vector<type>> name;
    #define FUNCTION(returnType, name, args, body) returnType name args;
    #define TRACK(returnType, name, args, body) returnType name args;

    #include "../PlasmaphysikV00/Configs/System_config.inc"

    #undef TRACK
    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET

private:
    std::size_t ID;
};




