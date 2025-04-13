//
// Created by calli on 2/20/2025.
//

#ifndef MARINE_H
#define MARINE_H
#include "Player.h"

class Marine : public Player {
private:
    string rank;  // Marine rank
public:
    Marine(string n, string aff, Attack atk, string ab, string r);  // Marine Constructor

    //declare marine specific methods
    string attack() const;
    string ReportForDuty() const;

    //Rank getter + setter
    void setRank(string r);
    string getRank() const;
};

#endif

