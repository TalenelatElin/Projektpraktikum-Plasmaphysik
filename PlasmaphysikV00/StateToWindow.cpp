
#include "framework.h"
#include "StateToWindow.h"
#include "Konstanten_config.inc"

#include <string>
#include <sstream>

template<typename T>
std::string toString(const T& value)
{
    if constexpr (std::is_same_v<T, Teilchen>) {
        std::ostringstream ss;

        //====
        // Makros für die Variablennamen aus der System_config datei
        #define SET(type, name, wert) ss << toString(value.name) << " ";

        #define FUNCTION(returnType, name, args, body)
        #define EQUATION(returnType, name, args, body)
        // nichts erzeugen, Varialblen sind bereits in der Header Datei gesetzt

        #include "Teilchen_config.inc"

        #undef EQUATION
        #undef FUNCTION
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



//====
// Makros zur Implementierung aus der Teilchen_config datei
#define SET(type, name, value)
// nichts erzeugen, Varialblen sind bereits in der Header Datei gesetzt

#define FUNCTION(returnType, name, args, body) \
    returnType Teilchen::name args body

#define EQUATION(returnType, name, args, body) \
    returnType Teilchen::name args body

#include "Teilchen_config.inc"

#undef EQUATION
#undef FUNCTION
#undef SET


//====
// Makros zur Implementierung aus der System_config datei
#define SET(type, name, value)
// nichts erzeugen, Varialblen sind bereits in der Header Datei gesetzt
#define VEC(type, name, size)

#define FUNCTION(returnType, name, args, body) \
    returnType Sim_Sys_State::name args body

#define EQUATION(returnType, name, args, body) \
    returnType Sim_Sys_State::name args body

#include "System_config.inc"

#undef EQUATION
#undef FUNCTION
#undef VEC
#undef SET





std::string Sim_Sys_State::stateToString()
{
    std::ostringstream ss;
    //====
    // Makros für die Variablennamen aus der System_config datei
    #define SET(type, name, value) \
        ss << toString(name) << " ";    // printet einfach den wert der aktuellen Variable aus?
    #define VEC(type, name, size) \
        ss << toString(name) << " ";

    #define FUNCTION(returnType, name, args, body)
    #define EQUATION(returnType, name, args, body)
    // nichts erzeugen, Varialblen sind bereits in der Header Datei gesetzt

    #include "System_config.inc"

    #undef EQUATION
    #undef FUNCTION
    #undef VEC
    #undef SET

    return ss.str();
}

void Sim_Sys_State::stateToWindow(HDC hdc, int h, int b)
{
    for (int i = 0; i < N; i++) {
        int x = 1;//particles[i].x;
        int y = 2;//particles[i].y;
        SetPixel(hdc, int(x + b / 2), int(y + h / 2), RGB(255, 0, 0));
    }
}

//
// Klassen Trivia
//

Teilchen::Teilchen() {
    ID = 0;
}
Teilchen::Teilchen(const unsigned int p) {
    ID = p;
}
unsigned int Teilchen::getID()
{
    return ID;
}


Sim_Sys_State::Sim_Sys_State() {
    ID = 0;
    //====
    // Makros zur Implementierung aus der System_config datei
    #define SET(type, name, value)
    // nichts erzeugen, Varialblen sind bereits in der Header Datei gesetzt
    #define VEC(type, name, size) name.resize(size);

    #define FUNCTION(returnType, name, args, body)

    #define EQUATION(returnType, name, args, body)

    #include "System_config.inc"

    #undef EQUATION
    #undef FUNCTION
    #undef VEC
    #undef SET
}
Sim_Sys_State::Sim_Sys_State(const unsigned int p) {
    ID = p;
}
unsigned int Sim_Sys_State::getID() {
    return ID;
}

