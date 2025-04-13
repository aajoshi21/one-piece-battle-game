//
// Created by calli on 2/20/2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Attack.h"
using namespace std;

class Player {
protected:
    string name;
    string affiliation;
    Attack attack;
    string ability;
public:
    Player(string name, string aff, const Attack& atk, string ab);  // Player Constructor

    //declare setters
    void setName(string n);
    void setAffiliation(string aff);
    void setAbility(string ab);
    void setAttack(Attack atk);

    //declare getters
    string getName() const;
    string getAffiliation() const;
    string getAttackType() const;
    string getAbility() const;
    Attack getAttack() const;
};







#endif //PLAYER_H
