
// Definiert ein Teichen
// Es können (Und sollten!!!) die Konstante aus Konstanten_config.inc verwendet werden.

//
// Variablen: Falls nur ein Startzustand nötig ist, dann wird dieser von hier extrahiert. Dann ist keine Implementation in Anfangsustände nötig.
//            Die Reihenfolge ist wichtig
//            Aus Speichergründen stehen hier nur variablen die sich unter der Systementwicklung ändern. Konstanten gehören in Konstanten_config.inc
//
SET(float, x, 0)
SET(float, y, 0)
SET(float, z, 0)


//
// Funktionen: Definieren errechenbare Größen aus den Variablen.
// FUNCTION(return type, name, args, expression)
//
FUNCTION(float, getDistance, (), {
    return std::sqrt(x*x+y*y+z*z);
})




//
// Funktionen: Evolutionen der einzelnen Größen. Dies entspricht also den Bewegungsgleichungen.
//             Es ist wichtig, dass hier die Namen (und selbe Rückgabetypen) der Teilchenzustandsparameter stehen
//
//        Bsp: oben steht:       SET( float, x,  0)
//              dann muss: EVOLUTION( float, x,  const context& c, expression)
//                                      ^     ^
//      
EVOLUTION(float, x, (const context& c), {
    return x + c.px*dx;
})
EVOLUTION(float, y, (const context& c), {
    return y + c.py*dy;

})
EVOLUTION(float, z, (const context& c), {
    return z + c.pz*dz;    
})