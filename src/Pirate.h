//
// Created by calli on 2/12/2025.
//

#ifndef PIRATE_H
#define PIRATE_H
#include "Player.h"


class Pirate : public Player {
private:
    unsigned long long bounty; //Pirate bounty number

public:
    Pirate(string n, string aff, const Attack& atk, string ab, unsigned long long b);  // constructor

    //declare Pirate specific methods
    string attack();
    string wantedPoster() const;

    //declare Bounty getter + setter
    unsigned long long getBounty() const;
    void setBounty(unsigned long long b);
};



#endif //PIRATE_H
