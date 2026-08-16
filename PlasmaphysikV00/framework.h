
#pragma once


#include "../PlasmaphysikV00/targetver.h" // Windowsspezifisch
#define WIN32_LEAN_AND_MEAN             // Selten verwendete Komponenten aus Windows-Headern ausschließen
// Windows-Headerdateien
#include <windows.h> // Windowsspezifisch
// C RunTime-Headerdateien
#include <cstdlib>
//#include <malloc.h>
#include <cstring>
#include <tchar.h> // Windowsspezifisch

// Nützliche Bibliotheken
#include <iostream>
#include <thread>
#include <mutex>

#include <random>
#include <vector>
#include <queue>

#include <string>
#include <fstream>
#include <filesystem>


// Programmspezifische Includes, Konstanten, Makros etc
constexpr UINT WM_SIMULATION_UPDATE = WM_APP + 1; // nur für die .exe wichtig
#include "../PlasmaphysikV00/Configs/Konstanten_config.inc"
