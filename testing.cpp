//
// Created by calli on 2/20/2025.
//

#include "Pirate.h"
#include "Marine.h"
#include "src/Attack.h"

bool test_Pirate();
bool test_Marine();
bool test_Attack();

int main() {
    srand(time(nullptr));
    if (test_Pirate()) {
        cout << "PASSED all Pirate test cases" << endl;
    }
    if (test_Marine()) {
        cout << "PASSED all Marine test cases" << endl;
    }
    if (test_Attack()){
        cout << "PASSED all Attack test cases" << endl;
    }
    return 0;
}

bool test_Pirate()
{
    bool passed = true;
    Attack P1_attack("Sword", "haki", "Devil Fruit", 10000000);
    Pirate p1("Luffy", "Pirate", P1_attack, "Gomu Gomu no Pistol", 1000);
    if (p1.getAffiliation() != "Pirate") {
        passed = false;
        cout << "FAILED Pirate Affiliation test case" << endl;
    }
    if (p1.getAbility() != "Gomu Gomu no Pistol")
    {
        passed = false;
        cout << "FAILED Pirate Ability test case" << endl;
    }

    if (p1.wantedPoster() != ("WANTED: " + p1.getName() + "\nBounty: " + to_string(p1.getBounty()) + " berries \n" "DEAD or ALIVE"))
    {
        passed = false;
        cout << "FAILED wanted poster test case" << endl;
    }
    else
        return passed;
}

bool test_Marine()
{
    bool passed = true;
    Attack M1_attack("Sword", "haki", "Moku Moku no Mi", "Captain");
    Marine m1("Smoker","Marine", M1_attack, "Seastone Sword", "Captain");
    if (m1.getAffiliation() != "Marine") {
        passed = false;
        cout << "FAILED Marine Affiliation test case" << endl;
    }
    if (m1.getAbility() != "Seastone Sword")
    {
        passed = false;
        cout << "FAILED Marine Ability Type test case" << endl;
    }
    if (m1.getAbility() != "Seastone Sword")
    {
        passed = false;
        cout << "FAILED Marine Ability test case" << endl;
    }
    if (m1.ReportForDuty() != ("Marine: " + m1.getName() + " | Rank: " + m1.getRank()))
    {
        passed = false;
        cout << "FAILED Report for Duty test case" << endl;
    }
    else
    return passed;
}

bool test_Attack()
{
    bool passed = true;
    Attack a1M("Sword", "haki", "Gomu Gomu no Mi", "Admiral"); // marines
    Attack a2P("Sword", "haki", "Gomu Gomu no Mi", 10000000); // pirates
    Attack a1("none", "none", "none", 0);
    Attack a2("Sword", "haki", "Gomu Gomu no Mi", 10);

    srand(33); //seeded for consistency
    cout << "--Zero power test case = " << a1.getAttackPower() << endl; //
    cout << "--Power level test case = " << a2.getAttackPower() << endl;
    if (a1M.getWeapon() != "Sword") {
        passed = false;
        cout << "FAILED Weapon test case" << endl;
    }
    if (a2P.getHaki() != "haki")
    {
        passed = false;
        cout << "FAILED Haki test case" << endl;
    }
    if (a2P.getDevilFruit() != "Gomu Gomu no Mi")
    {
        passed = false;
        cout << "FAILED Devil Fruit test case" << endl;
    }
    else
        return passed;

}




