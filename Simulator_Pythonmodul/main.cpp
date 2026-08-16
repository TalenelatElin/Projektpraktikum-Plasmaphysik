#include <pybind11/pybind11.h>
#include "../PlasmaphysikV00/framework.h"
#include "../PlasmaphysikV00/Simulation.h"
#include "../PlasmaphysikV00/StateToWindow.h"

namespace py = pybind11;

void runSimulation()
{
    py::print("C++: Simulationsfunktion betreten");

    Simulation simulation;
    py::print("C++: Objekt erstellt");

    simulation.Initialize();
    py::print("C++: Initialisierung abgeschlossen, Simulation beginnt");
    
    simulation.Start();
    py::print("C++: Simulation abgeschlossen.");
}


// Einstieg für Python:
PYBIND11_MODULE(Simulator_Pythonmodul, m)
{
    m.def("main", &runSimulation);
}