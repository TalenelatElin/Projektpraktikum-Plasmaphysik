
// Definiert ein Teichen
// Es können (Und sollten!!!) die Konstante aus Konstanten_config.inc verwendet werden.

//
// Variablen: Falls nur ein Startzustand nötig ist, dann wird dieser von hier extrahiert. Dann ist keine Implementation in Anfangsustände nötig.
//            Die Reihenfolge ist wichtig
//
SET(double, x, 0)
SET(double, y, 0)
SET(double, z, 0)


//
// Funktionen:
// FUNCTION(return type, name, args, expression)
//
FUNCTION(double, getDistanceSQ, (), {
    return x*x+y*y+z*z;
})
FUNCTION(double, x_new, (double p), {
    return x+p*dx;
})
FUNCTION(double, y_new, (double p), {
    return y+p*dy;
})
FUNCTION(double, z_new, (double p), {
    return z+p*dz;
})