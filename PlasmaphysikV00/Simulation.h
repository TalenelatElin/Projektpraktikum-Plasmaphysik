#pragma once

#include "framework.h"
#include "StateToWindow.h"


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
    void download();

private:
    HWND ghWnd;
    HDC ghdc;
    std::vector<Sim_Sys_State> startzust;
    std::vector<Sim_Sys_State*> startzustaende;
    bool stop = false;
    void translate_SimulationConfig(std::string dateiname);
    void translate_AnfangsConfig(std::string dateiname);

    std::size_t sim_len;                                                // sim_len > depth !!!
    std::vector<Sim_Sys_State> entwicklung;

    std::size_t depth;                                                  // depth = startzustaende.size()
    Sim_Sys_State next(std::vector<Sim_Sys_State*>& previous, std::mt19937& gen, std::uniform_real_distribution<double>& dist);

    void setCurrent();
    void setCurrent(int j);

    Sim_Sys_State* current;
    std::string dateiname;
    unsigned int current_id;
    unsigned int nr_updates;
    double h;
};