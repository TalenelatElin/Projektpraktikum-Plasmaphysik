
// Falls es mehr als einen Anfangszustand gibt, so müssen diese hier definiert werden. (Alle, auch der erste!!!)
// Der Index in den Arrays entspricht dem Index des Startzustandes. 


//
// Anfangszustände
// SET(type, name, wert_0, wert_1, wert_2, ...)
// VEC(double, x, 0) ?????????

//{
//using t = Teilchen;
//PARTICLES(0, t{}, t{}, t{}, t{}, t{}, usw.)
//}

//VERTFUNC(x, (const std::vector<std::vector<double>>& probs, ), {
//	std::vector<std::vector<double>> converged; // Jede Variable folgt einer Verteilungsfunktion. Z.b. gauß auf converged[][0]
//	converged.resize(N, num_Para); // Deltapeak
//})