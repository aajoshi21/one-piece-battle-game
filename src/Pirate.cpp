//
// Created by calli on 2/12/2025.
//

#include "Pirate.h"
#include "Player.h"

//constructor
Pirate::Pirate(string n, string aff, const Attack& atk, string ab, unsigned long long b)
    : Player(n, aff, atk, ab), bounty(b) {}

//pirate launches an attack
string Pirate::attack() {
        return "Pirate" + getName() + " attacks with " + getAttackType() + ": " + getAbility();
    }

// Report Pirates details
string Pirate::wantedPoster() const {
        return "WANTED: " + getName() + "\nBounty: " + to_string(bounty) + " berries \n" "DEAD or ALIVE";
    }

// Setter for bounty number
void Pirate::setBounty(unsigned long long b) {
    bounty = b;
}

// Getter for pirate bounty number
unsigned long long Pirate::getBounty() const {
        return bounty;
    }


