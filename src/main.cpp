
#include "engine.h"
#include "Pirate.h"
#include "Marine.h"
#include "Attack.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <vector>
#include <stdexcept>
using namespace std;


bool getDataFromFilePirates(vector<Pirate>& pirates, string fileNamePirates);
bool getDataFromFileMarines(vector<Marine>& marines, string fileNameMarines);

int main(int argc, char *argv[]) {
    Engine engine;

    vector<Pirate>pirates;
    vector<Marine>marines;
    //load CSV data into vectors of marines and pirates
    getDataFromFileMarines(marines, "marines.csv");
    getDataFromFilePirates(pirates, "pirates.csv");

    engine.passInVectors(pirates, marines);


    while (!engine.shouldClose()) {
        engine.processInput();
        engine.update();
        engine.render();
    }

    glfwTerminate();
    return 0;
}
bool getDataFromFilePirates(vector<Pirate>& pirates, string fileNamePirates)
{
    ifstream fileIn;
    fileIn.open("../src/" + fileNamePirates);

    if (fileIn)
    {
        //cout<<"File opened"<<endl;
        string header;//reading the header row
        getline(fileIn,header);

        //read in the data
        string name, affiliation, devilFruit, weapon, haki, ability, newline;
        unsigned long long bounty = 3333333;
        string comma;
        while(fileIn)
        {
            getline(fileIn,name,',');//read in name
            getline(fileIn,affiliation,',');//read in affiliation
            fileIn>> bounty;//read in bounty
            getline(fileIn, comma, ',');//get rid of comma after integer
            getline(fileIn,devilFruit,',');//read in devil fruit
            getline(fileIn,weapon,',');//read in weapon
            getline(fileIn,haki,',');//read in haki
            getline(fileIn,ability);//read in ability

            //Store the attack relevant data in an Attack class that gets passed in as a Pirate argument
            Attack attack(weapon, devilFruit, haki, bounty);
            //Store the rest of the data from the row into a Pirate object, in a vector.
            pirates.push_back(Pirate(name, affiliation, attack, ability, bounty));
        }
        fileIn.close();
        //cout<<"File closed"<<endl;
    }
    else
    {
        cout<<"Error opening file"<<endl;
        return false;
    }
    return true;
}
bool getDataFromFileMarines(vector<Marine>& marines, string fileNameMarines)
{

    ifstream fileIn2;
    fileIn2.open("../src/" + fileNameMarines);
    if (fileIn2)
    {
        //cout<<"File opened"<<endl;
        string header;//reading the header row
        getline(fileIn2,header);

        //read in the data
        string name, affiliation, rank, devilFruit, weapon, haki, ability, newline;
        while(fileIn2 && fileIn2.peek() != EOF)
        {

            getline(fileIn2,name,',');//read in name
            getline(fileIn2,affiliation,',');//read in affiliation
            getline(fileIn2, rank, ',');//read in rank
            getline(fileIn2,devilFruit,',');//read in devil fruit
            getline(fileIn2,weapon,',');//read in weapon
            getline(fileIn2,haki,',');//read in haki
            getline(fileIn2,ability);//read in ability

            //Store the attack relevant data in an Attack class that gets passed in as a marine argument
            Attack attack(weapon, devilFruit, haki, rank);
            //Store the rest of the data from the row into a Marine object, in a vector.
            marines.push_back(Marine(name, affiliation, attack, ability, rank));

        }
        fileIn2.close();
        //cout<<"File closed"<<endl;
    }
    else
    {
        cout<<"Error opening file"<<endl;
        return false;
    }
    return true;

}