//
// Created by caden on 2/22/2025.
//

#include "Attack.h"
//marine attack constructor
Attack::Attack(string wep, string hak, string dFruit, string rank) {
    weapon = wep;
    haki = hak;
    devilFruit = dFruit;

    //assign reputation based on marine hierarchy
    if (rank == "Lieutenant")
        reputation = 10;
    else if (rank == "Captain")
        reputation = 50;
    else if (rank == "Science Division")
        reputation = 75;
    else if (rank == "SWORD member")
        reputation = 125;
    else if (rank == "Rear Admiral")
        reputation = 140;
    else if (rank == "Vice Admiral")
        reputation = 150;
    else if (rank == "Admiral")
        reputation = 200;
    else if (rank == "Fleet Admiral")
        reputation = 225;
    else
        reputation = 100;
}
//pirate attack constructor
Attack::Attack(const string& wep, const string& hak, const string& dFruit, unsigned long long bounty) {
    weapon = wep;
    haki = hak;
    devilFruit = dFruit;

    //assign reputation based on pirate bounty
    if (bounty > 10000000000)
        reputation = 300;
    else if (bounty >= 1500000000)
        reputation = 250;
    else if (bounty >= 750000000)
        reputation = 200;
    else if (bounty >= 320000000)
        reputation = 175;
    else if (bounty >= 73000000)
        reputation = 200;
    else if (bounty < 60000000)
        reputation = 200;
}
//attack constructor
Attack::Attack(const string& wep, const string& hak, const string& dFruit, const int attackPower){
    weapon = wep;
    haki = hak;
    devilFruit = dFruit;
    reputation = attackPower;
}
//setters
void Attack::setWeapon(const string& w)
{
    weapon = w;
}
void Attack::setHaki(const string& h)
{
    haki = h;
}
void Attack::setDevilFruit(const string& d)
{
    devilFruit = d;
}
void Attack::setReputation(const int r)
{
    reputation = r;
}
//getters
string Attack::getWeapon() const
{
    return weapon;
}

string Attack::getHaki() const
{
    return haki;
}

string Attack::getDevilFruit() const
{
    return devilFruit;
}

int Attack::getAttackPower() const
{
    int attackPower = 0;
    if(weapon != "none")// if they have a weapon their attack power increases
    {
        attackPower ++;
    }
    if(haki != "none")// if they have haki their attack power increases by 2 (haki is important)
    {
        attackPower += 2;
    }
    if(devilFruit != "none")// if they have a devil fruit their attack power increases
    {
        attackPower ++;
    }
    // multiply attack power by random number to make game have some mystery/variation
    attackPower = attackPower * (1 + rand() % 75) + reputation;
    return attackPower;
}
