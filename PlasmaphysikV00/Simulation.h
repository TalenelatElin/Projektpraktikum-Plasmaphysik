#pragma once

#include "../PlasmaphysikV00/framework.h"

#include "../PlasmaphysikV00/StateToWindow.h"





int main();
void paintProgress(const HDC hdc, float p);


//
// Struct: LogEntry
//
// Aufgabe: Verfolgt die wichtigen Parameter beim Simulieren
//
struct LogEntry
{
    #define SET(type, name, value)
    #define VEC(type, name, size)
    #define MAT(type, name, size1, size2)
    #define FUNCTION(returnType, name, args, body)

    #define TRACKSET(type, name, value) type name = value;
    #define TRACKFUNC(returnType, name, args, body) returnType var_##name;
    #define TRACKPARTICLE(type, name, size) type name;
    #include "../PlasmaphysikV00/Configs/System_config.inc"
    #undef TRACKPARTICLE
    #undef TRACKFUNC
    #undef TRACKSET


    LogEntry() {}
    LogEntry(Sim_Sys_State& state) {
        #define TRACKSET(type, name, value) name = value;
        #define TRACKFUNC(returnType, name, args, body) var_##name = state.name();
        #define TRACKPARTICLE(type, name, size) name = state.particles;

        #include "../PlasmaphysikV00/Configs/System_config.inc"

        #undef TRACKPARTICLE
        #undef TRACKFUNC
        #undef TRACKSET
    }


    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET
};




//
// Klasse: SimulationLogger
//
// Aufgabe: Verfolgt die wichtigen Parameter beim Simulieren
//
class SimulationLogger
{
public:
    explicit SimulationLogger( const std::string& filename, std::size_t psim_len, std::size_t pmaxQueueSize = 3,HWND phWnd = nullptr, HDC phdc = nullptr)
        : sim_data_name(filename), sim_len(psim_len), maxQueueSize(pmaxQueueSize), ghWnd(phWnd), ghdc(phdc)
    {}

    double simulationWaitMilliseconds() const {
        return std::chrono::duration<double, std::milli>(sim_wait_time ).count();
    }
    double loggerWaitMilliseconds() const {
        return std::chrono::duration<double, std::milli>( log_wait_time).count();
    }

    void pushState(Sim_Sys_State& current);
    void finish();
    void loggerThreadLoop();

    void update(float progress);

    std::chrono::nanoseconds sim_wait_time{ 0 };
    std::chrono::nanoseconds log_wait_time{ 0 };
    std::chrono::nanoseconds total_sim_time{ 0 };

private:
    HWND ghWnd;
    HDC ghdc;

    using Clock = std::chrono::steady_clock;

    std::size_t maxQueueSize;
    std::string sim_data_name;
    std::queue<LogEntry> logQueue;

    std::mutex queueMutex;
    std::condition_variable dataAvailable;
    std::condition_variable spaceAvailable;
        
    bool simulationFinished = false;

    std::size_t sim_len;
    std::string write(LogEntry& entry, float progess);
};




//
// Klasse: Simulation
//
// Aufgabe: Simuliert ein System, das entsprechenden Differentialgleichungen folgt.
//          Der Systemzustand ist ein eigenes Objekt, diese Klasse produziert nur neue Zustaende
//
class Simulation
{
public:
    Simulation();
    void Initialize();
    void Start();
    void Stop();
    void Reset();
    void giveWindow(HWND phWnd, HDC phdc = nullptr);

    Sim_Sys_State* getCurrent();

    float h;

private:
    HWND ghWnd;
    HDC ghdc;

    void translate_SimulationConfig();
    void translate_AnfangsConfig();

    bool stop;
    std::size_t sim_len;                            
    unsigned short depth;                           // depth = anzahl der startzustaende
    std::vector<Sim_Sys_State> entwicklung;

    void evolve_system(const context& c);
    context getContext(unsigned short current_index, std::mt19937& gen, std::uniform_real_distribution<double>& dist);

    void setCurrent();
    void shiftCurrent(int j);

    std::string sim_data_name;
    unsigned short current_id;                     // current_id < depth
    std::size_t nr_updates;
    std::size_t nr_saves;
    std::size_t num_Para;
};