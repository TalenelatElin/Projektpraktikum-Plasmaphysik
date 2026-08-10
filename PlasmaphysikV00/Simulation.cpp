#include "framework.h"
#include "Simulation.h"
#include "StateToWindow.h"
#include <fstream>
#include "Konstanten_config.inc"





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

    translate_SimulationConfig("Simulation_config.txt");
    translate_AnfangsConfig("Anfangszustaende.inc");

    Reset();

    update();

}



void Simulation::Stop()
{
    stop = true;
}
void Simulation::Reset()
{
    entwicklung.resize(sim_len);
    for (int i = 0; i < depth; i++) {
        entwicklung[i] = startzustaende[i];
    }
    current = startzustaende.back();
    unsigned int current_id = static_cast<unsigned int>(depth - 1);
}

void Simulation::Start()
{
    unsigned int j = static_cast<unsigned int>(depth);
    while (stop == false && j < sim_len)
    {
        std::vector<Sim_Sys_State*> previous(5, nullptr);
        for (int i = 0; i < depth; i++) {
            previous[i] = entwicklung[j - depth + i];
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


void Simulation::translate_AnfangsConfig(std::string dateiname)
{
    startzust.resize(depth);
    startzustaende.resize(depth);
    if (dateiname != "Anfangszustaende.inc") { // Ansonsten Codeänderung nötig.
        return;
    }
    if (depth > 1) {
        for (unsigned int i = 0; i < depth; i++) {
            // Makros zum Übersetzen der Anfangszustände
            #define SET(name, ...) \
                startzust[i].name = std::vector<int>{__VA_ARGS__}[i];

            #include "Anfangszustaende.inc"
            #undef SET

            startzustaende[i] = &startzust[i];
        }
    }
    else {  // Dann ist die Initialisierung des einen Startzustandes schon in System_config passiert.
        return;
    }
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
    for (int j = 0; j < sim_len; j++) {
        if (entwicklung[j] != nullptr) {
            datei_string += entwicklung[j]->stateToString();
            datei_string += "\n";
        }
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