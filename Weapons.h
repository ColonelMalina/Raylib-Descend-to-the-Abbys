#pragma once
#include <string>
#include <cstdlib> 

class Weapon
{
public:
    std::string Wname;
    int minDamage;
    int maxDamage;
    int dotDamage;
    int dotTurns;
    int critChance;

    Weapon(std::string w = "Pointy stick", int minD = 6, int maxD = 8, int dotD = 2, int dotT = 2, int critC = 2)
        : Wname(w), minDamage(minD), maxDamage(maxD), dotDamage(dotD), dotTurns(dotT), critChance(critC) {}

    int getPower() {
        int range = maxDamage - minDamage;
        return minDamage + (rand() % (int)(range + 1));
    }
    double getWeaponScore() const {
        double avgDmg = (minDamage + maxDamage) / 2.0;
        double critBonus = avgDmg * (critChance / 100.0) * 0.5;
        double dotTotal = dotDamage * dotTurns;
        return avgDmg + critBonus + dotTotal;
    }

    bool operator>(const Weapon& other) const {
        return this->getWeaponScore() > other.getWeaponScore();
    }
};

Weapon Dagger();
Weapon SteelSword();
Weapon OrcAxe();
Weapon Halapartna();
Weapon BigAxe();
Weapon Elvensword();
Weapon Edagger();
Weapon HeavyMace();
Weapon Scikle();
Weapon EAxe();
Weapon Hammer();