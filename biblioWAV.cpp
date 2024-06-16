// Stephane Duguay, 2024
#include "biblioWAV.h"

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

template <typename TYPE_VALEUR>
void EcrireValeur(std::ostream& sortie, TYPE_VALEUR valeur, size_t taille = sizeof(TYPE_VALEUR))
{
    for (; taille > 0; taille--)
    {
        sortie.put(static_cast <char> (valeur & 0xFF));
        valeur >>= 8;
    }
}

float fadeInOut(float valeur, float tNormalise, float rampeNormalisee)
{
    if (tNormalise < rampeNormalisee)
    {
        return valeur * tNormalise / rampeNormalisee;
    }
    else if (tNormalise > 1 - rampeNormalisee)
    {
        return valeur * (1 - tNormalise) / rampeNormalisee;
    }
    else
    {
        return valeur;
    }
}

void GenererWAV(ostream& memoire, float frequences[], float dureesMillisecondes[], int n)
{
    const int hz{ 44100 };    // nombre d'échantillons par seconde

    // Écrire l'entête du format WAV
    memoire << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
    EcrireValeur(memoire, 16, 4);  // no extension data
    EcrireValeur(memoire, 1, 2);  // PCM - integer samples
    EcrireValeur(memoire, 2, 2);  // two channels (stereo file)
    EcrireValeur(memoire, hz, 4);  // samples per second (Hz)
    EcrireValeur(memoire, (hz * 16 * 2) / 8, 4);  // (Sample Rate * BitsPerSample * Channels) / 8
    EcrireValeur(memoire, 4, 2);  // data block size (size of two integer samples, one for each channel, in bytes)
    EcrireValeur(memoire, 16, 2);  // number of bits per sample (use a multiple of 8)
  
    // Écrire l'entête du bloc de données
    size_t data_chunk_pos = memoire.tellp();
    memoire << "data----";  // (chunk size to be filled in later)
    
    // Écrire les échantillons audio
    constexpr float PI = 3.14159265f;
    constexpr double WAV_MAX_AMPLITUDE = 32760; // "volume"
    
    for (int i{ 0 }; i < n; ++i) // Pour chaque note
    {
        int nbEchantillonsNote = static_cast<int>(hz * dureesMillisecondes[i] / 1000.f);
        for (int j{ 0 }; j < nbEchantillonsNote; j++)
        {
            float value = fadeInOut(sin((2.f * PI * j * frequences[i]) / hz), static_cast<float>(j) / nbEchantillonsNote, 0.05f);
            EcrireValeur(memoire, static_cast<std::int16_t>(WAV_MAX_AMPLITUDE * value));
            EcrireValeur(memoire, static_cast<std::int16_t>(WAV_MAX_AMPLITUDE * value));
        }
    }
    
    // (We'll need the final file size to fix the chunk sizes above)
    size_t file_length = memoire.tellp();
    
    // Fix the data chunk header to contain the data size
    memoire.seekp(data_chunk_pos + 4);
    EcrireValeur(memoire, file_length - data_chunk_pos + 8);
    
    // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    memoire.seekp(0 + 4);
    EcrireValeur(memoire, file_length - 8, 4);
}

#ifdef _WIN32
#include <Windows.h>
void JouerSons(float frequences[], float dureesMillisecondes[], int n)
{
#if 0
    for (int i = 0; i < n; ++i)
    {
        Beep(frequences[i], dureesMillisecondes[i]);
    }
#else
    stringstream memoire{};
    GenererWAV(memoire, frequences, dureesMillisecondes, n);
    PlaySoundA(static_cast<LPCSTR>(memoire.str().data()), NULL, SND_MEMORY | SND_SYNC);
#endif
}
#endif

bool EcrireSons(float frequences[], float dureesMillisecondes[], int n, const string& nomFichierSortie)
{
    ofstream fichierSortie{ nomFichierSortie, ios::binary };
    if (!fichierSortie || !fichierSortie.is_open())
    {
        return false;
    }

    GenererWAV(fichierSortie, frequences, dureesMillisecondes, n);
    return true;
}
