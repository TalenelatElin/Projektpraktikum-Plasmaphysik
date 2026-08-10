#pragma once

#include "framework.h"



//
// Klasse: Systemzustand
//
// Nutzen: Ist der Zustand eines Systems
//
class Sim_Sys_State
{
public:
    unsigned int getID();
    void stateToWindow(HDC hdc);
    std::string stateToString();
    

private:
    unsigned int ID;
};



