
#include "framework.h"
#include "StateToWindow.h"
#include "Konstanten_config.inc"

#include <string>
#include <sstream>

template<typename T>
std::string toString(const T& value)
{
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

template<typename T>
std::string toString(const std::vector<T>& values)
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

//
// Klasse: Systemzustand
//
// Nutzen: Ist der Zustand eines Systems
//

Sim_Sys_State::Sim_Sys_State()
{
    ID = 0;
}

Sim_Sys_State::Sim_Sys_State(const unsigned int p)
{
    ID = p;
}

unsigned int Sim_Sys_State::getID()
{
    return ID;
}

//====
// Makros zur Implementierung aus der System_config datei
#define SET(type, name, value)
// nichts erzeugen, Varialblen sind bereits in der Header Datei gesetzt

#define FUNCTION(returnType, name, args, body) \
    returnType Sim_Sys_State::name args body

#define EQUATION(returnType, name, args, body) \
    returnType Sim_Sys_State::name args body

#include "System_config.inc"

#undef EQUATION
#undef FUNCTION
#undef SET


void Sim_Sys_State::stateToWindow(HDC hdc) // TODO
{

}

std::string Sim_Sys_State::stateToString() // TODO
{
    std::ostringstream ss;
    //====
    // Makros für die Variablennamen aus der System_config datei
    #define SET(type, name, value) \
        ss << toString(name) << " ";    // printet einfach den wert der aktuellen Variable aus.

    #define FUNCTION(returnType, name, args, body)
    #define EQUATION(returnType, name, args, body)
    // nichts erzeugen, Varialblen sind bereits in der Header Datei gesetzt

    #include "System_config.inc "

    #undef EQUATION
    #undef FUNCTION
    #undef SET

    return ss.str();
}