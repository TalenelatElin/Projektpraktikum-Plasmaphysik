#include "framework.h"
#include "Simulation.h"
#include "StateToWindow.h"
#include <fstream>
#include "Konstanten_config.inc"
#include <random>





//
// Klasse: Simulation
//
// Aufgabe: Simuliert ein System, das entsprechenden Differentialgleichungen folgt.
//          Der Systemzustand ist ein eigenes Objekt, diese Klasse produziert nur neue Zustände
//


void Simulation::Start() // TODO: Random generator 
{
    unsigned int j = static_cast<unsigned int>(depth);
    while (stop == false && j < sim_len)
    {
        std::vector<Sim_Sys_State*> previous(5, nullptr);
        for (int i = 0; i < depth; i++) {
            previous[i] = &entwicklung[j - depth + i];
        }
        entwicklung[j] = next(previous);

        current_id = j;
        update();
        j += 1;
    }
    stop = false;
}


Sim_Sys_State Simulation::next(std::vector<Sim_Sys_State*>& previous) // TODO
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    std::vector<double> p;
    p.resize(3 * N);
    for (int l = 0; l < 3*N; l++) {
        p[l] = dist(gen) - 0.5;
    }

    Sim_Sys_State z = *(previous[0]);

    Sim_Sys_State z_new;
    z_new.probs = p;

    for (int l = 0; l < N; l++) {
        //z_new.particles[l].x = z.particles[l].x_new(p[l]);
        //z_new.particles[l].y = z.particles[l].y_new(p[l+1]);
        //z_new.particles[l].z = z.particles[l].z_new(p[l+2]);
    }

    return z_new;
}




void Simulation::translate_AnfangsConfig(std::string dateiname) // TODO: Wie behandle ich vektoren die zu einem teilchen gehören?
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
        entwicklung.resize(sim_len);
        startzust[0] = entwicklung[0];
        startzustaende[0] = &startzust[0];
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

            if (key == "sim_len")
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




void Simulation::update()
{
    setCurrent();

    std::wstring text = std::to_wstring(100*current_id/sim_len) + L" %";

    TextOutW(ghdc, 20, 20, text.c_str(), static_cast<int>(text.length()));

    if (ghWnd != nullptr){
        PostMessage(ghWnd,WM_SIMULATION_UPDATE,0,0);
    }
}
void Simulation::update(int j)
{
    setCurrent(j);

    std::wstring text = std::to_wstring(100 * current_id / sim_len) + L" %";

    TextOutW(ghdc, 20, 20, text.c_str(), static_cast<int>(text.length()));

    if (ghWnd != nullptr){
        PostMessage(ghWnd,WM_SIMULATION_UPDATE,0,0);
    }
}
void Simulation::download(std::string dateiname) {
    std::string datei_string = "";
    for (int j = 0; j < sim_len; j++) {
        //if (entwicklung[j] != 0) {
        datei_string += entwicklung[j].stateToString();
        datei_string += "\n";
        //}
    }
    std::ofstream file(dateiname);

    if (!file.is_open())
    {
        return; // Datei konnte nicht geöffnet werden
    }

    file << datei_string;

    file.close();
}


Simulation::Simulation()
{
    current = 0;
    current_id = 0;
    depth = 1;
    sim_len = 10;
    ghWnd = nullptr;
    ghdc = nullptr;
    Initialize();
}
void Simulation::Initialize()
{
    translate_SimulationConfig("Simulation_config.txt");
    translate_AnfangsConfig("Anfangszustaende.inc");

    Reset();

    setCurrent();
}
void Simulation::Stop()
{
    stop = true;
}
void Simulation::giveWindow(HWND phWnd)
{
    ghWnd = phWnd;
    HDC hdc = GetDC(ghWnd);
    ghdc = hdc;
}
Sim_Sys_State* Simulation::getCurrent()
{
    return current;
}
void Simulation::setCurrent()
{
    current = &entwicklung[current_id];
}
void Simulation::setCurrent(int j)
{
    current_id = j;
    current = &entwicklung[j];
}
void Simulation::Reset()
{
    entwicklung.resize(sim_len);
    for (int i = 0; i < depth; i++) {
        entwicklung[i] = startzust[i];
    }
    current = startzustaende.back();
    unsigned int current_id = static_cast<unsigned int>(depth - 1);

    RECT rect;
    GetClientRect(ghWnd, &rect);

    FillRect(ghdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    ReleaseDC(ghWnd, ghdc);
}