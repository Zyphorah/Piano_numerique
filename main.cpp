#pragma comment(lib, "winmm.lib")
#include <string>
#include <iostream>
#include <cmath>
#include <fstream>

#include "biblioWAV.h"

using namespace std;

//4 DO - 0.75
bool lireFichier(int octave[], string nomNote[], string demiTon[], float tempsNote[], ifstream& fichier, int& tempo,int& taille,const int capacity)
{
    char espace{ ' ' };
    fichier >> tempo;
    if (tempo != 60 && tempo != 120)
    {
        return false;
    }
    while (fichier.peek() != EOF)
    {
        if (taille + 1 > capacity) return false;
        fichier >> octave[taille];
        fichier >> nomNote[taille];
        fichier >> demiTon[taille];
        fichier >> tempsNote[taille];
       
        taille++;
    }

    return true;
}

float calculerDuree(float dureeNote, int tempo) 
{
    float dureeMS;
    if (tempo == 120) 
    {
        dureeMS = (500 * dureeNote);
    }
    else if (tempo == 60) 
    {
      
        dureeMS = (1000 * dureeNote);
    }
    else 
    {
       
        std::cout << "Tempo non pris en charge : " << tempo << " bpm" << std::endl;
        dureeMS = 0; 
    }
    return dureeMS;
}
int calculerDemiTon(string nomNote,string demiTon)
{
    if (nomNote == "DO")
    {
        return 1;
    }
    else if ((nomNote == "DO" && demiTon == "#") || (nomNote == "RE" && demiTon == "b"))
    {
        return 2;
    }
    else if (nomNote == "RE")
    {
        return 3;
    }
    else if ((nomNote == "RE" && demiTon == "#") || (nomNote == "MI" && demiTon == "b"))
    {
        return 4;
    }
    else if ((nomNote == "MI") || (nomNote == "FA"))
    {
        return 5;
    }
    else if ((nomNote == "MI" && demiTon == "#") || (nomNote == "FA"))
    {
        return 6;
    }
    else if ((nomNote == "FA" && demiTon == "#") || (nomNote == "SOL" && demiTon == "b"))
    {
        return 7;
    }
    else if ((nomNote == "SOL"))
    {
        return 8;
    }
    else if ((nomNote == "SOL" && demiTon == "#") || (nomNote == "LA" && demiTon == "b"))
    {
        return 9;
    }
    else if ((nomNote == "LA"))
    {
        return 10;
    }
    else if ((nomNote == "LA" && demiTon == "#") || (nomNote == "SI" && demiTon == "b"))
    {
        return 11;
    }
    else if ((nomNote == "SI"))
    {
        return 12;
    }

    return 0;
}

int calculerFrequence(int octave,string nomNote,string demiTon)
{
    int demiTonValue{ calculerDemiTon(nomNote,demiTon)};

    float frequence = 440.0f * pow(2.0f, (octave - 3) + (demiTonValue - 10.0f) / 12.0f);

    return frequence;
}

void sortieSonor(int octave[],string nomNote[],string demiTon[],float tempsNote[],int tempo, int taille)
{
    const int capacity{1000};
    float frequences[capacity];
    float dureesMillisecondes[capacity];

	for(int i=0;i<taille;i++)
	{
        frequences[i] = calculerFrequence(octave[i], nomNote[i], demiTon[i]);
        dureesMillisecondes[i] = calculerDuree(tempsNote[i], tempo);    
	}

    JouerSons(frequences, dureesMillisecondes, taille);
}

int main(int argc, char* argv[])
{
    
    const int capacity{ 1000 }; 

    //Vitess d'execution sois 500 miliseconde ou 1000 miliseconde, 120 ou 60
    int tempo{ 0 };

    //4 DO - 0.75
    int octave[capacity];
    string nomNote[capacity];
    string demiTon[capacity];
    float tempsNote[capacity];

    int taille{ 0 };
    
    ifstream fichier("mario_bros.musique");

    if (fichier)
    {
        lireFichier(octave,nomNote,demiTon,tempsNote,fichier,tempo,taille,capacity);
        sortieSonor(octave, nomNote, demiTon, tempsNote, tempo, taille);
        
        return 0;
    }
    else
    {
        cout << "Votre fichier n'existe pas" << endl;
    }

    return 0;
}

