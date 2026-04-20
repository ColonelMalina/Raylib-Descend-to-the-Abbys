#pragma once
#include <string>
#include <cstdlib> // pro rand()

class Weapon
{
public:
    std::string Wname;
    double minDamage;
    double maxDamage;
    double dotDamage;
    int dotTurns;
    int critChance;

    Weapon(std::string w = "Bare hands", double minD = 5, double maxD = 7, double dotD = 0, int dotT = 0, int critC = 2)
        : Wname(w), minDamage(minD), maxDamage(maxD), dotDamage(dotD), dotTurns(dotT), critChance(critC) {}

    double getPower() {
        double range = maxDamage - minDamage;
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