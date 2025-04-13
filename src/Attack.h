//
// Created by caden on 2/22/2025.
//

#ifndef ATTACK_H
#define ATTACK_H
#include <string>
using namespace std;


class Attack {
protected:
    string weapon;
    string haki;
    string devilFruit;
    int reputation;
public:
    Attack() = default;
    // constructor for marines
    Attack(string wep, string hak, string dFruit, string rank);
    // constructor for pirates
    Attack(const string& wep, const string& hak, const string& dFruit, unsigned long long bounty);
    // constructor for declaring attack from another attack
    Attack(const string& wep, const string& hak, const string& dFruit, int attackPower);

    //declare attack class setters
    void setWeapon(const string& w);
    void setHaki(const string& h);
    void setDevilFruit(const string& d);
    void setReputation(int r);

    //declare attack class getters
    string getWeapon() const;
    string getHaki() const;
    string getDevilFruit() const;
    int getAttackPower() const;
};


#endif //ATTACK_H
