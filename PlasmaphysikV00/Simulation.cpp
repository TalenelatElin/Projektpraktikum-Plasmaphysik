
#include "../PlasmaphysikV00/framework.h"
#include "../PlasmaphysikV00/Simulation.h"

#include "../PlasmaphysikV00/StateToWindow.h"





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

//
// The Simulation algorithm (works on depth long vector<Sim_Sys_State> entwicklung)
// Nötiger Arbeitsspeicher ~= 2*depth*speicher(Sim_Sys_State.o), wird halbiert wenn anfangszustände auf Festplatte
// 
void Simulation::Start() // TODO: Random generator
{
    // Random generator:
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_int_distribution<int> dist(0, 1);
    std::uniform_real_distribution<double> dist(0.0, 1.0);


    std::size_t counter = depth-1;
    unsigned short entw_pos = 0;
    while (stop == false && counter < sim_len)
    {
        // Treat entwicklung like a Queue with length depth (fixed)
        evolve_system(getContext(entw_pos, gen, dist));

        // Track stuff
        std::size_t r = max(1, sim_len / nr_saves);
        if (counter % r == 0) {
            make_save(entwicklung[entw_pos]);
        }

        // Update (only for .exe)
        if (nr_updates != 0) {
            unsigned short r = max(1, sim_len / nr_updates);
            if (counter % r == 0) {
                update();
            }
        }

        counter++;
        entw_pos++;
        entw_pos = entw_pos % depth;
    }
    stop = false;
}


void Simulation::evolve_system(const context& c) // TODO
{
    for (int l = 0; l < N; l++) {
        // Makros
        #define SET(type, name, value)
        #define VEC(type, name, size)
        #define FUNCTION(returnType, name, args, body)
        #define EVOLUTION(returnType, name, args, body) \
            returnType evolved_##name = c.prevs[0]->particles[l].evolve_##name(c);

        #include "../PlasmaphysikV00/Configs/Teilchen_config.inc"

        #undef EVOLUTION
        #define EVOLUTION(returnType, name, args, body) c.prevs[0].name = evolved_##name

        #include "../PlasmaphysikV00/Configs/Teilchen_config.inc"

        #undef EVOLUTION
        #undef FUNCTION
        #undef VEC
        #undef SET
    }
}

// TODO: dist(gen) durch die gewünschte Wahrscheinlichkeitsverteilung ersetzen
context Simulation::getContext(std::size_t current_index, std::mt19937& gen, std::uniform_real_distribution<double>& dist)
{
    // Generate context
    context c(h, depth, N, num_Para);

    // Set Previous
    for (int i = 0; i < depth; i++) {
        c.prevs[i] = &entwicklung[(current_index + i) % depth];
    }

    // Set randomness
    for (std::size_t l = 0; l < 3 * N; l++) {
        for (std::size_t m = 0; m < 3 * N; m++) {
            c.probs[l][m] = dist(gen);
        }
    }

    // Return
    return c;
}




void Simulation::translate_AnfangsConfig() // TODO: Wie behandle ich Vektoren die zu einem Teilchen gehören?
{
    entwicklung.resize(depth);
    startzustaende.resize(depth);
    for (unsigned int i = 0; i < depth; i++) {
        // Makros
        #define SET(name, ...) \
            startzustaende[i].name = std::vector<int>{__VA_ARGS__}[i];

        #include "../PlasmaphysikV00/Configs/Anfangszustaende.inc"
        #undef SET
    }
}

void Simulation::translate_SimulationConfig()  // LTODO: füllen aller nötigen Config-Parameter
{
    std::ifstream file("../PlasmaphysikV00/Configs/Simulation_config.inc");

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
                sim_data_name = value;
            }
            else if (key == "h")
            {
                h = std::stof(value);
            }
            // Und so weiter und so fort
        }
    }
}




void Simulation::update()
{
    if (ghWnd != nullptr){
        PostMessage(ghWnd,WM_SIMULATION_UPDATE,0,0);
        if (ghdc != nullptr) {
            paintProgress(ghdc, static_cast<float>(current_id) / sim_len);
        }
        else {
            giveWindow(ghWnd);
            paintProgress(ghdc, static_cast<float>(current_id) / sim_len);
        }
    }
}
void Simulation::update(int j)
{
    shiftCurrent(j);
    update();
}


void Simulation::download()
{
    namespace fs = std::filesystem;
    const fs::path dateipfad = fs::path("..") / "Diffusionstest" / "Data" / sim_data_name;
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


void Simulation::make_save(Sim_Sys_State& current) // TODO
{
    // Makros
    #define SET(type, name, value)
    #define VEC(type, name, size)
    #define MAT(type, name, size1, size2)
    #define FUNCTION(returnType, name, args, body) 
    #define TRACK(returnType, name, args, body)

    #include "../PlasmaphysikV00/Configs/System_config.inc"

    #undef TRACK
    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET


    //current.setDistanzen();
    //current.MSD = c.prevs.back()->getMeanSquare();
}
void make_save(Sim_Sys_State& current, std::string dateipfad)
{


}




Simulation::Simulation()
{
    ghWnd = nullptr;
    ghdc = nullptr;
    h = 0.1;
    stop = false;
    sim_len = 10;
    depth = 1;
    sim_data_name = "Simul_default_name.txt";
    current_id = 0;
    nr_updates = 10;
    nr_saves = 10;
    num_Para = 3;
}
void Simulation::Initialize()
{
    num_Para = 0
    // Makros
    #define SET(type, name, value) +1 // Zählen der Parameter
    #define VEC(type, name, size)
    #define FUNCTION(returnType, name, args, body)
    #define EVOLUTION(returnType, name, args, body)

    #include "../PlasmaphysikV00/Configs/Teilchen_config.inc"
            ;

    #undef SET
    #undef Vec
    #undef FUNCTION
    #undef EVOLUTION

    translate_SimulationConfig();
    translate_AnfangsConfig();
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
    return &entwicklung[current_id];
}
void Simulation::shiftCurrent(int j)
{
    int limit = current_id + j;
    if (limit >= 0 && limit < depth){
        current_id = limit;
    }
}
void Simulation::Reset()
{
    entwicklung.resize(depth);
    current_id = 0;
    stop = false;

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