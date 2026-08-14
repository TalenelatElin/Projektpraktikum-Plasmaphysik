#pragma once

#include "../PlasmaphysikV00/framework.h"

#include "../PlasmaphysikV00/StateToWindow.h"

int main();
void paintProgress(const HDC hdc, float p);

//
// Klasse: Simulation
//
// Aufgabe: Simuliert ein System, das entsprechenden Differentialgleichungen folgt.
//          Der Systemzustand ist ein eigenes Objekt, diese Klasse produziert nur neue Zustände
//
class Simulation
{
public:
    Simulation();
    void Initialize();
    void Start();
    void Stop();
    void Reset();
    void giveWindow(HWND phWnd);
    void giveWindow(HWND phWnd, HDC phdc);

    Sim_Sys_State* getCurrent();
    void update();
    void update(int j);
    void make_save(Sim_Sys_State& current);
    void make_save(Sim_Sys_State& current, std::string dateipfad);
    void download();
    float h;

private:
    HWND ghWnd;
    HDC ghdc;
    void translate_SimulationConfig();
    void translate_AnfangsConfig();

    bool stop;
    std::size_t sim_len;                            
    unsigned short depth;                           // depth = startzustaende.size()
    std::vector<Sim_Sys_State> startzustaende;
    std::vector<Sim_Sys_State> entwicklung;

    void evolve_system(const context& c);
    context getContext(std::size_t current_index, std::mt19937& gen, std::uniform_real_distribution<double>& dist);

    void setCurrent();
    void shiftCurrent(int j);

    std::string sim_data_name;
    unsigned short current_id;                     // current_id < depth
    std::size_t nr_updates;
    std::size_t nr_saves;
    std::size_t num_Para;
};





