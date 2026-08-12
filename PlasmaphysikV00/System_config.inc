
// Definiert die Sim_Sys_State (Zustand eines Systems)
// Es können (Und sollten!!!) die Konstante aus Konstanten_config.inc verwendet werden.

//
// Variablen: Falls nur ein Startzustand nötig ist, dann wird dieser von hier extrahiert. Dann ist keine Implementation in Anfangsustände nötig.
//            Die Reihenfolge ist wichtig
//
// SET(int, N, 1000)
//

VEC(std::vector<Teilchen>, particles, N)
VEC(std::vector<double>, probs, 3*N)


//
// Funktionen:
// FUNCTION(return type, name, args, expression)
//
//FUNCTION(double, getDistanceSQ, (), {
//    return x*x+y*y+z*z;
//})

