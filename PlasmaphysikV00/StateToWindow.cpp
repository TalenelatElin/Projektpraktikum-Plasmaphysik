
#include "../PlasmaphysikV00/framework.h"
#include "../PlasmaphysikV00/StateToWindow.h"




// Makros
#define SET(type, name, value)
#define VEC(type, name, size)
#define FUNCTION(returnType, name, args, body) returnType Teilchen::name args body
#define EVOLUTION(returnType, name, args, body) returnType Teilchen::evolve_##name args body

#include "../PlasmaphysikV00/Configs/Teilchen_config.inc"

#undef EVOLUTION
#undef FUNCTION
#undef VEC
#undef SET


// Makros 
#define SET(type, name, value)
#define VEC(type, name, size)
#define MAT(type, name, size1, size2)
#define FUNCTION(returnType, name, args, body) returnType Sim_Sys_State::name args body
#define TRACK(returnType, name, args, body) returnType Sim_Sys_State::name args body

#include "../PlasmaphysikV00/Configs/System_config.inc"

#undef TRACK
#undef FUNCTION
#undef MAT
#undef VEC
#undef SET



template<typename T>
std::string toString(const T& value)
{
    if constexpr (std::is_same_v<T, Teilchen>) {
        std::ostringstream ss;

        // Makros 
        #define SET(type, name, wert) ss << toString(value.name) << " ";
        #define VEC(type, name, size) ss << toString(value.name) << " ";
        #define FUNCTION(returnType, name, args, body)
        #define EVOLUTION(returnType, name, args, body)

        #include "../PlasmaphysikV00/Configs/Teilchen_config.inc"

        #undef EVOLUTION
        #undef FUNCTION
        #undef VEC
        #undef SET

        return ss.str();
    }
    else {
        std::ostringstream ss;
        ss << value;
        return ss.str();
    }
}

template<typename Q>
std::string toString(const std::vector<Q>& values)
{
    std::ostringstream ss;

    ss << "[";

    for (std::size_t i = 0; i < values.size(); ++i)
    {
        if (i > 0)
            ss << ", ";

        ss << toString(values[i]);
    }

    ss << "]";

    return ss.str();
}






void Sim_Sys_State::stateToWindow(HDC hdc, int h, int b) // TODO
{
    for (int i = 0; i < N; i++) {
        int x = particles[i].x;
        int y = particles[i].y;
        SetPixel(hdc, int(x + b / 2), int(y + h / 2), RGB(255, 0, 0));
    }
}


std::string Sim_Sys_State::stateToString() // TODO: Use Track to track
{
    std::ostringstream ss;
    ss << " stateToString noch nicht implementiert.";
    // Makros
    #define SET(type, name, value) //ss << toString(name) << " ";
    #define VEC(type, name, size) // ss << toString(name) << " ";
    #define MAT(type, name, size1, size2) // ss << toString(name) << " ";
    #define FUNCTION(returnType, name, args, body)
    #define TRACK(returnType, name, args, body) //ss << toString(name) << " ";

    #include "../PlasmaphysikV00/Configs/System_config.inc"

    #undef TRACK
    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET

    return ss.str();
}




//
// Klassen Trivia
//



Teilchen::Teilchen(const std::size_t pid) {
    id = pid;
    // Makros
    #define SET(type, name, value)
    #define VEC(type, name, size)
    #define VEC(type, name, size) name.resize(size);
    #define EVOLUTION(returnType, name, args, body)

    #include "../PlasmaphysikV00/Configs/Teilchen_config.inc"

    #undef EVOLUTION
    #undef FUNCTION
    #undef VEC
    #undef SET
}
std::size_t Teilchen::getid() {
    return id;
}


Sim_Sys_State::Sim_Sys_State(const std::vector<Teilchen> pparticles)
    :particles(pparticles)
{
    ID = 0;

    // Makros
    #define SET(type, name, value)
    #define VEC(type, name, size) name.resize(size);
    #define MAT(type, name, size1, size2) name.resize(size1, std::vector<type>(size2));
    #define FUNCTION(returnType, name, args, body)
    #define TRACK(returnType, name, args, body)

    #include "../PlasmaphysikV00/Configs/System_config.inc"

    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET
}
Sim_Sys_State::Sim_Sys_State(const std::vector<Teilchen> pparticles, const std::size_t pID)
    :particles(pparticles)
{
    ID = pID;

    // Makros
    #define SET(type, name, value)
    #define VEC(type, name, size) name.resize(size);
    #define MAT(type, name, size1, size2) name.resize(size1, std::vector<type>(size2));
    #define FUNCTION(returnType, name, args, body)
    #define TRACK(returnType, name, args, body)

    #include "../PlasmaphysikV00/Configs/System_config.inc"

    #undef TRACK
    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET
}
Sim_Sys_State::Sim_Sys_State(const std::size_t pN) {
    ID = 0;
    particles.resize(pN);

    // Makros
    #define SET(type, name, value)
    #define VEC(type, name, size) name.resize(size);
    #define MAT(type, name, size1, size2) name.resize(size1, std::vector<type>(size2));
    #define FUNCTION(returnType, name, args, body)
    #define TRACK(returnType, name, args, body)

    #include "../PlasmaphysikV00/Configs/System_config.inc"

    #undef TRACK
    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET
}
Sim_Sys_State::Sim_Sys_State(const std::size_t pN, const std::size_t pID) {
    ID = pID;
    particles.resize(pN);

    // Makros
    #define SET(type, name, value)
    #define VEC(type, name, size) name.resize(size);
    #define MAT(type, name, size1, size2) name.resize(size1, std::vector<type>(size2));
    #define FUNCTION(returnType, name, args, body)
    #define TRACK(returnType, name, args, body)

    #include "../PlasmaphysikV00/Configs/System_config.inc"

    #undef TRACK
    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET
}
std::size_t Sim_Sys_State::getID() {
    return ID;
}



context::context(const float ph, const unsigned short pdepth, const std::size_t pN, const unsigned short pNumPara)
    : h(ph)
{
    prevs.resize(pdepth);
    probs.resize(pN, std::vector<double>(pNumPara));
}