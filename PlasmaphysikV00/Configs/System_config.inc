
// Definiert die Sim_Sys_State (Zustand eines Systems)
// Es können (Und sollten!!!) die Konstante aus Konstanten_config.inc verwendet werden.

//
// Variablen: Falls nur ein Startzustand nötig ist, dann wird dieser von hier extrahiert. Dann ist keine Implementation in Anfangsustände nötig.
//            Die Reihenfolge ist wichtig
//
// SET(int, MSD, 0)
//
SET(float, MSD, 0)
MAT(float, distanzen, N, N)

//
// Funktionen: Definieren Größen die mehrerere Teilchen zum errechnen brauchen.
// FUNCTION(return type, name, args, expression)
//
FUNCTION(float, squadd, (float a, float b, float c), {
    return std::sqrt(a*a+b*b+c*c);
})

FUNCTION(void, setDistanzen, (), {
    for (std::size_t i = 0; i < N; i++)
    {
        for (std::size_t j = i+1; j < N; j++)
        {
            float dist = squadd(particles[i].x-particles[j].x, particles[i].y-particles[j].y, particles[i].z-particles[j].z);
            distanzen[i][j] = dist;
            distanzen[j][i] = dist;
        }
    }
})  

FUNCTION(float, getMeanSquare, (), {
    float mean_square = 0;
    for (int i = 0; i < N; i++){
        float r = particles[i].getDistance(); 
        mean_square += r*r;
    }
    return mean_square/N;
})

TRACK(getMeanSquare)