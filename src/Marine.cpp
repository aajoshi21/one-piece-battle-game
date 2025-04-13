//
// Created by calli on 2/20/2025.
//

#include "Marine.h"
#include "Player.h"

//constructor
    Marine::Marine(string n, string aff, Attack atk, string ab, string r)
        : Player(n, aff, atk, ab), rank(r) {}

    //marine launches an attack
    string Marine::attack() const
    {
            return getName() + " attacks with " + getAttackType() + ": " + getAbility();
        }

    // Report marine's details
    string Marine::ReportForDuty() const {
            return "Marine: " + getName() + " | Rank: " + rank;
        }

    // Getter for marine rank
    string Marine::getRank() const {
            return rank;
        }

    // Setter for marine rank
    void Marine::setRank(string r) {
            rank = r;
        }

