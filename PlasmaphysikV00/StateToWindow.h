#pragma once

#include "../PlasmaphysikV00/framework.h"






class Sim_Sys_State;

//
// Struct: context
//
// Nutzen: ist nötig um die evolution eines Teilchens zu berechnen.
//         context enthält randomness, sowie alle wichtigen Größen die aus der Zustandsklasse kommen können
//
//
struct context
{
    context(const float ph, const unsigned short pdepth, const std::size_t pN, const unsigned short pNumPara)
        : h(ph)
    {
        prevs.resize(pdepth);
        probs.resize(pN, std::vector<double>(pNumPara));
    }

    float h;
    std::vector<Sim_Sys_State*> prevs;
    std::vector<std::vector<double>> probs;
};




//
// Klasse: Teilchen
//
// Nutzen: entspricht einem Teilchen
//
class Teilchen
{
public:
    Teilchen() { id = 0; }
    Teilchen(const std::size_t pid) : id(pid)
    {
    // Makros
    #define SET(type, name, value)
    #define VEC(type, name, size) name.resize(size);
    #define FUNCTION(returnType, name, args, body)
    #define EVOLUTION(name, args, body)

    #include "../PlasmaphysikV00/Configs/Teilchen_config.inc"

    #undef EVOLUTION
    #undef FUNCTION
    #undef VEC
    #undef SET
}
    std::size_t getid() { return id;  }
    
    // Makros
    #define SET(type, name, value) type name = value;
    #define VEC(type, name, size) type name;
    #define FUNCTION(returnType, name, args, body) returnType name args;
    #define EVOLUTION(name, args, body) decltype(name) evolve_##name args;

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
    Sim_Sys_State() { ID = 0; }
#define SET(type, name, value)
#define FUNCTION(returnType, name, args, body)
#define TRACKSET(type, name, value)
#define TRACKFUNC(returnType, name, args, body)
#define TRACKPARTICLE(type, name, size) 
    explicit Sim_Sys_State(std::size_t pN, std::size_t pID = 0)
        :  ID(pID)
    {
        particles.reserve(pN);
        for (std::size_t i = 0; i < pN; i++) {
            particles.emplace_back(i);
        }
        // Makros
        #define VEC(type, name, size) name.resize(size);
        #define MAT(type, name, size1, size2) name.resize(size1, std::vector<type>(size2));

        #include "../PlasmaphysikV00/Configs/System_config.inc"

        #undef MAT
        #undef VEC
    }
    explicit Sim_Sys_State(std::vector<Teilchen> pparticles, std::size_t pID = 0)
      : particles(std::move(pparticles)), ID(pID)
    {
        // Makros
        #define VEC(type, name, size) name.resize(size);
        #define MAT(type, name, size1, size2) name.resize(size1, std::vector<type>(size2));

        #include "../PlasmaphysikV00/Configs/System_config.inc"
    }
#undef TRACKSET
#undef TRACKFUNC
#undef TRACKPARTICLE
#undef FUNCTION
#undef MAT
#undef VEC
#undef SET


    std::size_t getID() { return ID; }

    void stateToWindow(HDC hdc, int h, int b);

    std::vector<Teilchen> particles;

        
    // Makros
    #define SET(type, name, value) type name = value;
    #define VEC(type, name, size) type name;
    #define MAT(type, name, size1, size2) std::vector<std::vector<type>> name;
    #define FUNCTION(returnType, name, args, body) returnType name args;
    #define TRACKSET(type, name, value) type name = value;
    #define TRACKFUNC(returnType, name, args, body) returnType name args;
    #define TRACKPARTICLE(type, name, size)
    #define VERTFUNC(name, args, expression) decltype(Teilchen::name) vert_##name;

    #include "../PlasmaphysikV00/Configs/System_config.inc"
    #undef VERTFUNC
    #undef TRACKPARTICLE
    #undef TRACKFUNC
    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET
    
private:
    unsigned short ID;
};