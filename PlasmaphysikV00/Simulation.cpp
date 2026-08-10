#include "framework.h"
#include "Simulation.h"
#include "StateToWindow.h"
#include <fstream>





//
// Klasse: Simulation
//
// Aufgabe: Simuliert ein System, das entsprechenden Differentialgleichungen folgt.
//          Der Systemzustand ist ein eigenes Objekt, diese Klasse produziert nur neue Zustände
//

Simulation::Simulation()
{
    current = 0;
    current_id = 0;
    depth = 1;
    sim_len = 10;
    ghWnd = nullptr;
}

void Simulation::Initialize()
{   
    translate_AnfangsConfig("Anfangszustaende");
    translate_SimulationConfig("Simulation_config");
    Reset();
    current = startzustaende.back();
    unsigned int current_id = static_cast<unsigned int>(depth - 1);
    update();
    translate_SystemConfig("System_config");
}



void Simulation::Stop()
{
    stop = true;
}
void Simulation::Reset()
{
    std::fill(entwicklung.begin(), entwicklung.end(), nullptr);
    for (int i = 0; i < depth; i++) {
        entwicklung[i] = startzustaende[i];
    }
}

void Simulation::Start()
{
    unsigned int j = static_cast<unsigned int>(depth);
    while (stop == false && j < sim_len)
    {
        std::vector<Sim_Sys_State*> previous(5, nullptr);
        for (int i = 0; i < depth; i++) {
            previous[i] = entwicklung[j-depth+i];
        }
        entwicklung[j] = next(previous);
        current_id = j;
        j += 1;
    }
}

void Simulation::setCurrent()
{
    current = entwicklung[current_id];
}




Sim_Sys_State* Simulation::next(const std::vector<Sim_Sys_State*>& previous) // TODO
{
    return current;
}


void Simulation::translate_SystemConfig(std::string dateiname) // TODO
{

}

void Simulation::translate_AnfangsConfig(std::string dateiname) // TODO
{

}

void Simulation::translate_SimulationConfig(std::string dateiname)  // TODO: füllen aller nötigen Config-Parameter
{
    std::ifstream file(dateiname);

    std::string line;

    while (std::getline(file, line))
    {
        std::size_t pos = line.find('=');

        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            if (key == "sim_length")
            {
                sim_len = std::stoull(value);
            }
            else if (key == "depth")
            {
                depth = std::stoull(value);
            }  
            // Und so weiter und so fort
        }
    }
}


void Simulation::download(std::string dateiname) {
    std::string datei_string = "";
    for (unsigned int i = 0; i < sim_len; i++) {
        datei_string += entwicklung[i]->stateToString();
        datei_string += "\n";
    }
    std::ofstream file(dateiname);

    if (!file.is_open())
    {
        return; // Datei konnte nicht geöffnet werden
    }

    file << datei_string;

    file.close();
}


Sim_Sys_State* Simulation::getCurrent()
{
    return current;
}


void Simulation::giveWindow(HWND phWnd)
{
    ghWnd = phWnd;
}

void Simulation::update()
{
    setCurrent();
    if (ghWnd != nullptr)
    {
        PostMessage(
            ghWnd,
            WM_SIMULATION_UPDATE,
            0,
            0
        );
    }
}