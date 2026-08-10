
// Definiert die Sim_Sys_State (Zustand eines Systems)
// Es können (Und sollten!!!) die Konstante aus Konstanten_config.inc verwendet werden.

//
// Variablen: Falls nur ein Startzustand nötig ist, dann wird dieser von hier extrahiert. Dann ist keine Implementation in Anfangsustände nötig.
//            Die Reihenfolge ist wichtig
//
SET(int, ps, 510)
SET(double, gewicht, 1780.0)



//
// Funktionen:
// FUNCTION(return type, name, args, expression)
//
FUNCTION(void, hupen, (), {
    std::cout << "Hup!\n";
})

FUNCTION(double, leistungProKg, (), {
    return ps / gewicht;
})


//
// Gleichngen:
// EQUATION(return type, name, args, expression)
//
EQUATION(double, ps_new, (), {
    return ps / gewicht;
})