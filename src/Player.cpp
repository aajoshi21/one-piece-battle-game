//
// Created by calli on 2/20/2025.
//

#include "Player.h"

Player::Player(string n, string aff, const Attack& atk, string ab) {  // Constructor Definition
        name = n;
        affiliation = aff;
        attack = Attack(atk.getWeapon(), atk.getHaki(), atk.getDevilFruit(), atk.getAttackPower());
        ability = ab;
}

//setters
void Player::setName(string n)
{
    name = n;
}
void Player::setAffiliation(string aff)
{
    affiliation = aff;
}
void Player::setAbility(string ab)
{
    ability = ab;
}
void Player::setAttack(Attack atk)
{
    attack = atk;
}

//getters
string Player::getName() const
{
    return name;
}
string Player::getAffiliation() const
{
    return affiliation;
}
string Player::getAttackType() const
{
    if (attack.getWeapon() != "none")
        return attack.getWeapon();
    if (attack.getDevilFruit() != "none")
        return attack.getDevilFruit();
    if (attack.getHaki() != "none")
        return attack.getHaki();
    return "Weapon";
}
string Player::getAbility() const
{
    return ability;
}
Attack Player::getAttack() const
{
    return attack;
}
