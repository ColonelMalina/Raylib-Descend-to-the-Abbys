#pragma once
#include <vector>
#include <string>
#include <cstdlib>
#include "Weapons.h"
#include "Enemies.h"

class Hero
{
    std::string Name;
    double health;
    double maxHealth;
    double Mana;
    double maxMana;
    Weapon currentW;
    double hPotion;

public:
    Hero(std::string n, double h, double m, double p) : Name(n), health(h), maxHealth(h), maxMana(m), Mana(m), hPotion(p) {}

    bool isAlive() { return health > 0; }
    std::string getName() { return Name; }
    double getMana() { return Mana; }
    double getHealth() { return health; }
    Weapon getCurrentWeapon() { return currentW; }
    double gethPotion() { return hPotion; }

    void EquipW(Weapon newWeapon) {
        currentW = newWeapon;
    }

    std::string Attack(Enemies& target) {
        double hit = currentW.getPower();
        std::string log = "";

        int roll = rand() % 100;
        if (roll < currentW.critChance) {
            hit *= 1.5;
            log += "*** CRITICAL HIT! ***\n";
        }
        std::vector<std::string> attackMessages = {
            Name + " swings their " + currentW.Wname + " with pure rage!\n",
            Name + " lunges forward, striking with the " + currentW.Wname + ".\n",
            Name + " executes a precise strike using the " + currentW.Wname + ".\n",
            Name + " unleashes a devastating blow with their " + currentW.Wname + "!\n"
        };

        int randomIndex = rand() % attackMessages.size();
        log += attackMessages[randomIndex];
        log += target.takeDamage(hit);

        if (currentW.dotTurns > 0) {
            log += "\n" + target.applyDot(currentW.dotDamage, currentW.dotTurns);
        }
        return log;
    }

    // FIREBALL
    double minF = 10;
    double maxF = 20;
    double fManaCost = 6;
    int fireballLevel = 1;
    int dotChance = 20;

    std::string Fireball(Enemies& target, bool& success) {
        if (Mana < fManaCost) {
            success = false;
            return "Not enough Mana! Need " + std::to_string((int)fManaCost);
        }
        double range = maxF - minF;
        double actualCast = minF + (rand() % (int)(range + 1));

        Mana -= fManaCost;
        success = true;

        std::string log = "You used Fireball Lvl " + std::to_string(fireballLevel) + "!\n";
        log += Name + " casts Fireball towards enemy for " + std::to_string((int)actualCast) + ".\n";
        log += target.takeDamage(actualCast);

        if (rand() % 100 < dotChance) {
            log += "\n" + target.applyDot(5 + (fireballLevel * 2), 3);
        }
        return log;
    }

    void upgradeFireball() {
        fireballLevel++;
        minF += 4;
        maxF += 9;
        fManaCost += 3;
        dotChance += 15;
    }

    // ICESPIKE
    double minI = 7;
    double maxI = 15;
    double iManaCost = 5;
    int icespikeLevel = 1;
    int freezeChance = 10;

    std::string Icespike(Enemies& target, bool& success) {
        if (Mana < iManaCost) {
            success = false;
            return "Not enough Mana! Need " + std::to_string((int)iManaCost);
        }
        double range = maxI - minI;
        double actualCast = minI + (rand() % (int)(range + 1));

        Mana -= iManaCost;
        success = true;

        std::string log = "You used Icespike Lvl " + std::to_string(icespikeLevel) + "!\n";
        log += Name + " casts Icespike towards enemy for " + std::to_string((int)actualCast) + ".\n";
        log += target.takeDamage(actualCast);

        if (rand() % 100 < freezeChance) {
            log += "\n" + target.freeze(1);
        }
        return log;
    }

    void upgradeIce() {
        icespikeLevel++;
        minI += 3;
        maxI += 7;
        iManaCost += 2;
        freezeChance += 10;
    }

    // HEAL
    double minH = 5.0;
    double maxH = 10.0;
    double hManaCost = 5.0;
    int healLevel = 1;
    int critChance = 5;

    std::string Heal(bool& success) {
        if (Mana < hManaCost) {
            success = false;
            return "Not enough Mana! Need " + std::to_string((int)hManaCost);
        }

        double range = maxH - minH;
        double actualHeal = minH + (rand() % (int)(range + 1));
        std::string log = "";

        Mana -= hManaCost;
        if (rand() % 100 < critChance) {
            actualHeal *= 1.8;
            log += "*** CRITICAL HEAL! ***\n";
        }
        health += actualHeal;

        if (health > maxHealth) health = maxHealth;

        success = true;
        log += "You used Heal Lvl " + std::to_string(healLevel) + "!\n";
        log += "Recovered " + std::to_string((int)actualHeal) + " HP.";
        return log;
    }

    void upgradeHeal() {
        healLevel++;
        minH += 5;
        maxH += 8;
        hManaCost += 2;
        critChance += 5;
    }

    void addhPotion(double amount) { hPotion += amount; }

    std::string usehPotion() {
        if (hPotion > 0) {
            hPotion--;
            health += 30;
            if (health > 100) health = maxHealth;
            return "You drank a Health Potion! Recovered 30 HP.\nPotions left: " + std::to_string((int)hPotion);
        }
        return "No potions left!";
    }

    bool unlockedFireball = false;
    void learnFireball() { unlockedFireball = true; }
    bool hasFireball() { return unlockedFireball; }

    bool unlockedIce = false;
    void learnIce() { unlockedIce = true; }
    bool hasIce() { return unlockedIce; }

    void regainMana() { Mana = maxMana; }
    void fullHeal() { health = maxHealth; }

    void bandage(double amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }

    std::string takeDamage(double amount) {
        health -= amount;
        if (health <= 0) health = 0;
        return "You took " + std::to_string((int)amount) + " damage!";
    }

    void upgradeMana(double amount) {
        maxMana += amount;
        Mana = maxMana;
    }
};