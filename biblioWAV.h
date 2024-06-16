// Stephane Duguay, 2024
#pragma once

#include <string>

// Permet de jouer des sons (Windows seulement)
#ifdef _WIN32
void JouerSons(float frequences[], float dureesMillisecondes[], int n);
#endif

// Permet d'écrire des sons dans un fichier WAV
bool EcrireSons(float frequences[], float dureesMillisecondes[], int n, const std::string& nomFichierSortie);
