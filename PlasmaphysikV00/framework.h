
#pragma once


#include "../PlasmaphysikV00/targetver.h"
#define WIN32_LEAN_AND_MEAN             // Selten verwendete Komponenten aus Windows-Headern ausschließen
// Windows-Headerdateien
#include <windows.h>
// C RunTime-Headerdateien
#include <stdlib.h>
#include <malloc.h>1
#include <memory.h>
#include <tchar.h>

// Nützliche Bibliotheken
#include <iostream>
#include <thread>
#include <random>
#include <vector>

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>


// Programmspezifische Includes, Konstanten, Makros etc
constexpr UINT WM_SIMULATION_UPDATE = WM_APP + 1; // nur für die .exe wichtig
#include "../PlasmaphysikV00/Configs/Konstanten_config.inc"
