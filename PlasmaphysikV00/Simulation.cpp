#include "framework.h"
#include "Simulation.h"
#include "StateToWindow.h"
#include <fstream>
#include <filesystem>
#include "Konstanten_config.inc"




// Alternativer Einstieg, falls Ausführung aus dem Terminal
int main()
{
    Simulation s;
    s.Initialize();
    s.Start();
    s.download();
    return 0;
}




//
// Klasse: Simulation
//
// Aufgabe: Simuliert ein System, das entsprechenden Differentialgleichungen folgt.
//          Der Systemzustand ist ein eigenes Objekt, diese Klasse produziert nur neue Zustände
//


void Simulation::Start() // TODO
{
    // Random generator:
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_int_distribution<int> dist(0, 1);
    std::uniform_real_distribution<double> dist(0.0, 1.0);


    unsigned int j = static_cast<unsigned int>(depth);
    while (stop == false && j < sim_len)
    {
        context c(depth, h);

        for (int i = 0; i < depth; i++) {
            c.prevs[i] = &entwicklung[j - depth + i];
        }
        entwicklung[j] = next(c, gen, dist);


        unsigned int r = max(1, sim_len / nr_updates);
        if (j % r == 0) {
            update(j);
        }
        j += 1;
    }
    stop = false;
}


Sim_Sys_State Simulation::next(context& c, std::mt19937& gen, std::uniform_real_distribution<double>& dist) // TODO
{
    std::vector<double> p; //Ist das überhaupt nötig?
    p.resize(3 * N);
    for (int l = 0; l < 3*N; l++) {
        p[l] = 2*dist(gen) - 1;
    }

    Sim_Sys_State z_new;

    for (int l = 0; l < N; l++) {
        z_new.particles[l].id = l;

        c.px = p[3 * l];
        c.py = p[3 * l + 1];
        c.pz = p[3 * l + 2];

        

        #define SET(type, name, value)
        #define VEC(type, name, size)
        #define FUNCTION(returnType, name, args, body)
        #define EVOLUTION(returnType, name, args, body) \
            z_new.particles[l].name = c.prevs.back()->particles[l].evolve_##name(c);

        #include "Teilchen_config.inc"

        #undef EVOLUTION
        #undef FUNCTION
        #undef VEC
        #undef SET
    }
    //z_new.setDistanzen();
    z_new.MSD = c.prevs.back()->getMeanSquare();

    return z_new;
}




void Simulation::translate_AnfangsConfig(std::string dateiname) // TODO: Wie behandle ich vektoren die zu einem teilchen gehören?
{
    startzust.resize(depth);
    startzustaende.resize(depth);
    if (dateiname != "Anfangszustaende.inc") { // Ansonsten Codeänderung nötig.
        return;
    }
    if (depth > 2) {
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

void Simulation::translate_SimulationConfig(std::string pdateiname)  // TODO: füllen aller nötigen Config-Parameter
{
    std::ifstream file(pdateiname);

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
            else if (key == "nr_updates")
            {
                nr_updates = std::stoull(value);
            }
            else if (key == "dateiname")
            {
                dateiname = value;
            }
            else if (key == "h")
            {
                h = std::stoull(value);
            }
            // Und so weiter und so fort
        }
    }
}


void Simulation::update()
{
    paintProgress(ghdc, static_cast<float>(current_id) / sim_len);

    if (ghWnd != nullptr){
        PostMessage(ghWnd,WM_SIMULATION_UPDATE,0,0);
    }
}
void Simulation::update(int j)
{
    setCurrent(j);

    update();
}



void Simulation::download()
{
    namespace fs = std::filesystem;
    const fs::path dateipfad = fs::path("..") / "Diffusionstest" / "Data" / dateiname;
    std::ofstream file(dateipfad);

    if (!file)
        return;

    for (int j = 0; j < sim_len && !stop; ++j)
    {
        file << entwicklung[j].stateToString() << '\n';

        paintProgress(ghdc, static_cast<float>(j + 1) / sim_len);
    }

    stop = false;
}


Simulation::Simulation()
{
    current = 0;
    current_id = 0;
    depth = 1;
    sim_len = 10;
    ghWnd = nullptr;
    ghdc = nullptr;
    nr_updates = 100;
    h = 0.1;
    dateiname = "Simul.txt";
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
void Simulation::giveWindow(HWND phWnd, HDC phdc)
{
    ghWnd = phWnd;
    ghdc = phdc;
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

    if (ghWnd != nullptr && ghdc != nullptr )
    {
        RECT rect;
        GetClientRect(ghWnd, &rect);

        FillRect(ghdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        ReleaseDC(ghWnd, ghdc);
    }
}




void paintProgress(const HDC phdc, float p) {
    if (phdc != nullptr) {
        std::wstring text = std::to_wstring(100 * p) + L" %";

        TextOutW(phdc, 20, 20, text.c_str(), static_cast<int>(text.length()));
    }
}