//
// Created by calli on 2/20/2025.
//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <random>
#include "Pirate.h"
#include "Marine.h"
#include "src/Attack.h"
using namespace std;

bool validateLetter(const string& input);
bool validateLetter2(const string& input);
bool validateName(const string& input, vector<Pirate>& pirates);
bool validateBounty(const string& input);
bool battle(Pirate player, Pirate enemy);
bool battle(Pirate player, Marine enemy);
bool getDataFromFilePirates(vector<Pirate>& pirates, string fileNamePirates);
bool getDataFromFileMarines(vector<Marine>& marines, string fileNameMarines);
// comment
int main()
{
    const int GAMELENGTH = 10;
    vector<Pirate>pirates;
    vector<Marine>marines;
    //load CSV data into vectors of marines and pirates
    getDataFromFileMarines(marines, "marines.csv");
    getDataFromFilePirates(pirates, "pirates.csv");

    // shuffle the vectors to make game more unique
    time_t current_time = time(NULL);
    shuffle(std::begin(marines), std::end(marines), std::mt19937(std::random_device()()));
    shuffle(std::begin(pirates), std::end(pirates), std::mt19937(std::random_device()()));

    int time = 0;
    bool playerWins = true;
    int lastFought = 0;

    string input;
    unsigned long long playerBounty = 0;
    Attack playerAttack = Attack("w","h","df",playerBounty);
    Pirate playerCharacter = Pirate("","Pirate",playerAttack,"",playerBounty);

    cout << "Welcome to One Piece Battle Simulator!" << endl;
    cout << "======================================" << endl << endl;
    cout << "Would you like to pick an existing pirate or make your own?" << endl;
    cout << "To pick an existing pirate type: P" <<endl;
    cout << "To make your own type: M \n" <<endl;
    getline(cin, input);
    while(!validateLetter(input))
    {
        getline(cin, input);
    }
    if (input == "P" || input == "p")// pick an existing character
    {
        cout << "Please type the name of your character:  ";
        getline(cin, input);
        while (!validateName(input, pirates))// checks that the name they choose is valid
        {
            getline(cin, input);
        }
        for (int i = 0; i < pirates.size(); i++)// finds the name of the pirate they chose
        {
            if(pirates[i].getName() == input)
            {
                // fill in player values to match the pirate they chose
                playerAttack.setWeapon(pirates[i].getAttack().getWeapon());
                playerAttack.setHaki(pirates[i].getAttack().getHaki());
                playerAttack.setDevilFruit(pirates[i].getAttack().getDevilFruit());
                playerCharacter.setName(pirates[i].getName());
                playerCharacter.setAffiliation(pirates[i].getAffiliation());
                playerCharacter.setAttack(playerAttack);
                playerCharacter.setAbility(pirates[i].getAbility());
                playerCharacter.setBounty(pirates[i].getBounty());

                //remove the pirate they choose from the vector so they cannot fight themselves
                pirates.erase(pirates.begin()+i);
            }
        }

    }
    else if (input == "M" || input == "m") // make own character
    {
        //set the name to user's preference
        cout << "Please type the name of your character:" << endl;
        getline(cin, input);
        playerCharacter.setName(input);

        //players are automatically pirates, set affiliation to pirate
        playerCharacter.setAffiliation(playerCharacter.getAffiliation());

        //set the bounty to user's preference
        cout << "Please type the bounty of your character (no commas or letters):" << endl;
        getline(cin, input);
        while (!validateBounty(input))
            getline(cin, input);
        playerBounty = stoull(input);
        playerCharacter.setBounty(playerBounty);

        // set weapon as user specifies
        cout << "Please type the name of your weapon or 'none' if you do not want one" << endl;
        getline(cin, input);
        playerAttack.setWeapon(input);
        //haki
        cout << "Please type the level of haki you would like to have or 'none' if you do not want any" << endl;
        getline(cin, input);
        playerAttack.setHaki(input);
        //set devil fruit as user specifies
        cout << "Please type the name of your devil fruit or 'none' if you do not want one" << endl;
        getline(cin, input);
        playerAttack.setDevilFruit(input);
        playerCharacter.setAttack(playerAttack);
    }

    cout << endl << endl << endl << "~=-=| Your voyage begins! |=-=~" << endl;
    cout << "As you set sail you check the newspapers. On the final page you find your wanted poster" << endl;
    cout << endl << playerCharacter.wantedPoster() << endl << endl;

    for (int i = 0; i < GAMELENGTH; i++)
    {
        //randomly decide if they fight pirate or marine
        time = static_cast<int>(current_time);
        int pAtk = 0;
        int eAtk = 0;
        if (time % 2 == 0)// pirate
        {
            pAtk = playerCharacter.getAttack().getAttackPower();
            eAtk = pirates[i].getAttack().getAttackPower();
            if(battle(playerCharacter, pirates[i])) // initiate battle and prompt user to fight
            {
                if (pAtk >= eAtk)
                {
                    cout << "YOU WIN" << endl;
                    cout << "You used " << playerCharacter.getAbility() << endl;
                    cout << pirates[i].getName() << ": "<< pirates[i].getAttack().getAttackPower() << endl;
                    cout << "You: " << playerCharacter.getAttack().getAttackPower() << endl;
                }
                else
                {
                    cout << "YOU LOSE!" << endl;
                    cout << pirates[i].getName() << " used " << pirates[i].getAbility() << endl;
                    cout << pirates[i].getName() << ": "<< pirates[i].getAttack().getAttackPower() << endl;
                    cout << "You: " << playerCharacter.getAttack().getAttackPower() << endl;
                    playerWins = false;
                    break;
                }
            }
            else
            {
                cout << endl;
            }
        }
        else// marine
        {
            if(battle(playerCharacter, marines[i]))
            {
                pAtk = playerCharacter.getAttack().getAttackPower();
                eAtk = marines[i].getAttack().getAttackPower();
                if (pAtk >= eAtk)
                {
                    cout << "YOU WIN" << endl;
                    cout << "You used " << playerCharacter.getAbility() << endl;
                    cout << marines[i].getName() << ": "<< marines[i].getAttack().getAttackPower() << endl;
                    cout << "You: " << playerCharacter.getAttack().getAttackPower() << endl;
                }
                else
                {
                    cout << "YOU LOSE!" << endl;
                    cout << marines[i].getName() << " used " << marines[i].getAbility() << endl;
                    cout << marines[i].getName() << ": "<< marines[i].getAttack().getAttackPower() << endl;
                    cout << "You: " << playerCharacter.getAttack().getAttackPower() << endl;
                    playerWins = false;
                    break;
                }
            }
            else
            {
                cout << endl;
            }
        }
    }
    if (playerWins == true)
    {
        cout << "Congratulations!" << endl;
        cout << "You reached the end of the grand line and found the One Piece!" << endl;
    }
    else
    {
        cout << "You have been defeated and sent to impel down for the rest of your days." << endl;
    }

}
bool battle(Pirate player, Pirate enemy)// returns true if the player gets into a fight, false otherwise
{
    time_t current_time = time(NULL);
    string input;
    cout << "An enemy ship approaches!" << endl;
    cout << "They are flying a jolly roger!" << endl << "You look up their wanted poster." << endl;
    cout << enemy.wantedPoster() << endl << endl;
    cout << "Would you like to fight?" << endl << "Y or N" << endl;
    getline(cin, input);
    while(!validateLetter2(input))// makes input y or n
    {
        getline(cin, input);
    }
    if (input == "N" || input == "n") // they try to run
    {
        if (static_cast<int>(current_time) % 2 == 0)// 50 50 chance they run successfully
        {
            cout << "You failed to get away and a fight ensued!" << endl;
            return true;
        }
        else
        {
            cout << "You got away safely!" << endl;
            return false;
        }
    }
    //else if (input == "Y" || input == "y")// they want to fight
    return true;
}
bool battle(Pirate player, Marine enemy)
{
    time_t current_time = time(NULL);
    string input;
    cout << "An enemy ship approaches!" << endl;
    cout << "It's a marine battle ship with one " << enemy.getRank() << " aboard!" << endl;
    cout << "Would you like to fight?" << endl << "Y or N" << endl;
    getline(cin, input);
    while(!validateLetter2(input))// makes input y or n
    {
        getline(cin, input);
    }
    if (input == "N" || input == "n") // Player tries to run away
    {
        if (static_cast<int>(current_time) % 2 == 0)// 50/50 chance they run successfully
        {
            cout << "You failed to get away and a fight ensued" << endl;
            return true;
        }
        else
        {
            cout << "You got away safely!" << endl;
            return false;
        }
    }
    //else if (input == "Y" || input == "y")// they want to fight
    return true;
}
bool validateBounty(const string& input)
{
    unsigned long long ullInput;
    try
    {
        ullInput = stoull(input);
    }
    catch(std::invalid_argument& e)
    {
        cout << "Please only type numbers";
        return false;
    }
    if (ullInput < 1)
    {
        cout << "Please enter a positive number" << endl;
        return false;
    }
    if (ullInput > 100000000000) // 100 billion
    {
        cout << "Please enter a lower number" << endl;
        return false;
    }
    return true;
}
bool validateName(const string& input, vector<Pirate>& pirates)
{
    int i = 0;
    bool pirateFound = false;
    while(i < pirates.size() && pirateFound == false)
    {
        if(pirates[i].getName() == input)
        {
            pirateFound = true;
        }
        i++;
    }
    if (pirateFound == false)
        cout << "Name not recognized. Try again." << endl;
    return pirateFound;
}
bool validateLetter(const string& input)
{
    if (input.length() != 1) // make sure input is only 1 character
    {
        cout << "Invalid input. Please only type P or M" << endl;
        return false;
    }
    if (input != "P" && input != "M" && input != "p" && input != "m") // make sure character is M or P
    {
        cout << "Invalid input. Please only type P or M" << endl;
        return false;
    }
    return true;
}
bool validateLetter2(const string& input)
{
    if (input.length() != 1) // make sure input is only 1 character
    {
        cout << "Invalid input. Please only type Y or N" << endl;
        return false;
    }
    if (input != "Y" && input != "y" && input != "N" && input != "n") // make sure character is M or P
    {
        cout << "Invalid input. Please only type Y or N" << endl;
        return false;
    }
    return true;
}
bool getDataFromFilePirates(vector<Pirate>& pirates, string fileNamePirates)
{
    ifstream fileIn;
    fileIn.open("../" + fileNamePirates);

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
    fileIn2.open("../" + fileNameMarines);
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