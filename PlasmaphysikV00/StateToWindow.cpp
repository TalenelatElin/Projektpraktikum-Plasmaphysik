
#include "../PlasmaphysikV00/framework.h"
#include "../PlasmaphysikV00/StateToWindow.h"




// Makros
#define SET(type, name, value)
#define VEC(type, name, size)
#define FUNCTION(returnType, name, args, body) returnType Teilchen::name args body
#define EVOLUTION(name, args, body) decltype(Teilchen::name) Teilchen::evolve_##name args body // FEHLER

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
#define TRACKSET(type, name, value)
#define TRACKFUNC(returnType, name, args, body) returnType Sim_Sys_State::name args body
#define TRACKPARTICLE(type, name, size) 
#define VERTFUNC(name, args, body) decltype(Teilchen::name) vert_##name args body

#include "../PlasmaphysikV00/Configs/System_config.inc"

#undef VERTFUNC
#undef TRACKSET
#undef TRACKFUNC
#undef TRACKPARTICLE
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
        #define SET(type, name, wert) ss << value.name << " ";
        #define VEC(type, name, size) // ss << toString(value.name) << " ";
        #define FUNCTION(returnType, name, args, body)
        #define EVOLUTION(name, args, body)

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