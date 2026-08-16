
#include "../PlasmaphysikV00/framework.h"
#include "../PlasmaphysikV00/Simulation.h"

#include "../PlasmaphysikV00/StateToWindow.h"





// Alternativer Einstieg, falls Ausführung aus dem Terminal
int main()
{
    Simulation s;
    s.Initialize();
    s.Start();
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
        

    std::size_t counter = 0;
    unsigned short entw_pos = 0;

    std::size_t mr = 1;
    if (nr_saves != 0) {
        mr = max(1, sim_len / nr_saves);
    }

    // Zeitmessung
    using Clock = std::chrono::steady_clock;

    // Schreiben und Simulieren in verschiedenen Threads
    SimulationLogger sLog(sim_data_name, sim_len,static_cast<int>(1000/N) ,ghWnd, ghdc);

    std::thread loggerThread(&SimulationLogger::loggerThreadLoop, &sLog);
    const auto totalStart = Clock::now();

    // Simulationsalgorithmus
    while (!stop && counter < sim_len)
    {
        // Treat entwicklung like a Queue with length depth (fixed)
        evolve_system(getContext(entw_pos, gen, dist));

        // Track stuff
        if (counter % mr == 0) {
            sLog.pushState(entwicklung[entw_pos]);
        }

        // Increment
        counter++;
        entw_pos++;
        entw_pos = entw_pos % depth;
    }

    sLog.finish();
    loggerThread.join();

    stop = false; 

    sLog.total_sim_time = Clock::now() - totalStart;
    //std::chrono::duration<double, std::milli>( ... ).count()
}


void Simulation::evolve_system(const context& c) // TODO: Randbedingungen berücksichtigen
{
    for (int l = 0; l < N; l++) {
        // Makros
        #define SET(type, name, value)
        #define VEC(type, name, size)
        #define FUNCTION(returnType, name, args, body)
        #define EVOLUTION(name, args, body) \
            decltype(Teilchen::name) evolved_##name = c.prevs[0]->particles[l].evolve_##name(c);

        #include "../PlasmaphysikV00/Configs/Teilchen_config.inc"

        #undef EVOLUTION
        #define EVOLUTION(name, args, body) c.prevs[0]->particles[l].name = evolved_##name;

        #include "../PlasmaphysikV00/Configs/Teilchen_config.inc"

        #undef EVOLUTION
        #undef FUNCTION
        #undef VEC
        #undef SET
    }
}

// TODO: dist(gen) durch die gewünschte Wahrscheinlichkeitsverteilung ersetzen+Randbedingungen?
context Simulation::getContext(unsigned short current_index, std::mt19937& gen, std::uniform_real_distribution<double>& dist)
{
    // Generate context
    context c(h, depth, N, num_Para);

    // Set Previous
    for (int i = 0; i < depth; i++) {
        c.prevs[i] = &entwicklung[(current_index + i) % depth];
    }

    // Set randomness
    for (std::size_t l = 0; l < N; l++) {
        for (std::size_t m = 0; m < num_Para; m++) {
            if (dist(gen) > 0.5) {
                c.probs[l][m] = 1;
            }
            else {
                c.probs[l][m] = -1;
            }
        }
    }

    // Return
    return c;
}






void SimulationLogger::pushState(Sim_Sys_State& current)
{
    std::unique_lock<std::mutex> lock(queueMutex);

    // Warten, falls der Logger nicht hinterherkommt
    const auto waitStart = Clock::now();
    spaceAvailable.wait(lock, [this] { return logQueue.size() < maxQueueSize; });
    sim_wait_time += Clock::now() - waitStart;

    logQueue.emplace(current);
    lock.unlock();
    dataAvailable.notify_one();
}
void SimulationLogger::loggerThreadLoop()
{
    std::filesystem::path pfad = std::filesystem::path{ ".." } / "Diffusionstest" / "Data" / sim_data_name;
    std::ofstream file(pfad);

    if (!file) {
        std::string debugText =
            "Aktuelles Arbeitsverzeichnis konnte nicht geöffnet werden: " +
            std::filesystem::current_path().string() +
            "\n";

        std::cerr << "Logdatei konnte nicht geöffnet werden\n";
        return;
    }

    float counter = 0;
    while (true) {
        LogEntry entry;
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            // Warten, bis Daten vorhanden oder die Simulation fertig ist
            const auto waitStart = Clock::now();
            dataAvailable.wait(lock, [this] {  return !logQueue.empty() || simulationFinished; });
            log_wait_time += Clock::now() - waitStart;

            // Erst beenden, wenn alle Einträge geschrieben wurden
            if (logQueue.empty() && simulationFinished) {
                break;
            }

            counter++;
            entry = logQueue.front();
            logQueue.pop();
        }

        // Simulation darüber informieren, dass Platz frei ist
        spaceAvailable.notify_one();

        // Schreiben außerhalb des gesperrten Bereichs
        file << write(entry, counter/sim_len);
    }

    file.flush();
}
void SimulationLogger::finish()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        simulationFinished = true;
    }

    dataAvailable.notify_one();
}
std::string SimulationLogger::write(LogEntry& entry, float progess) // TODO
 {
    std::ostringstream ss;

    #define SET(type, name, value)
    #define VEC(type, name, size)
    #define MAT(type, name, size1, size2)
    #define FUNCTION(returnType, name, args, body)


    #define TRACKFUNC(returnType, name, args, body) ss << entry.var_##name << " ";
    #define TRACKSET(type, name, value) ss << name << " ";
    #define TRACKPARTICLE(type, name, size) ss << toString(name) << " ";

    #include "../PlasmaphysikV00/Configs/System_config.inc"

    #undef TRACKPARTICLE
    #undef TRACKFUNC
    #undef TRACKSET


    #undef FUNCTION
    #undef MAT
    #undef VEC
    #undef SET

    ss << "\n";
    update(progess);
    return ss.str();
}





























void Simulation::translate_AnfangsConfig() // TODO: Wie behandle ich einzelne Teilchenverteilungen, Vektoren und co
{
    // Random generator:
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_int_distribution<int> dist(0, 1);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Set randomness
    std::vector<std::vector<double>> probs(N, std::vector<double>(num_Para));
    for (std::size_t l = 0; l < N; l++) {
        for (std::size_t m = 0; m < num_Para; m++) {
            probs[l][m] = dist(gen);
        }
    }


#define MAT(type, name, size1, size2)
#define VEC(type, name, size)

    entwicklung.reserve(depth);
    for (unsigned short i = 0; i < depth; i++) {
        entwicklung.emplace_back(N, i);

        #define SET(type, name, ...) \
            entwicklung[i].name = std::vector<type>{__VA_ARGS__}[i];
        #define PARTICLES(id, ...) \
            entwicklung[id].particles = std::vector<Teilchen>{__VA_ARGS__};
  //      #define VERTCUNC(name, ) \

     //       entwicklung[id].particles.name = std::vector<Teilchen>{__VA_ARGS__};

        #include "../PlasmaphysikV00/Configs/Anfangszustaende.inc"
    }

#undef SET
#undef VEC
#undef MAT
#undef PARTICLES
}

void Simulation::translate_SimulationConfig()  // LTODO: füllen aller nötigen Config-Parameter
{
    std::filesystem::path pfad = std::filesystem::path{ ".." } / "PlasmaphysikV00" / "Configs" / "Simulation_config.txt";
    std::ifstream file(pfad);
    
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
            else if (key == "nr_saves")
            {
                nr_saves = std::stoull(value);
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




void SimulationLogger::update(float progress)
{
    if (ghWnd != nullptr){
        PostMessage(ghWnd,WM_SIMULATION_UPDATE,0,0);
            paintProgress(ghdc, progress);        
    }
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
    #define EVOLUTION(name, args, body)

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
void Simulation::giveWindow(HWND phWnd, HDC phdc)
{
    ghWnd = phWnd;
    if (phdc == nullptr) {
        HDC hdc = GetDC(ghWnd);
        ghdc = hdc;
    }
    else {
        ghdc = phdc;
    }

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