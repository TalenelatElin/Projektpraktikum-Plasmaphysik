// header.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Selten verwendete Komponenten aus Windows-Headern ausschließen
// Windows-Headerdateien
#include <windows.h>
// C RunTime-Headerdateien
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Weitere Includes
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// Wichtige Konstanten
constexpr UINT WM_SIMULATION_UPDATE = WM_APP + 1;