#pragma once
#include <string>
#include <cstdlib>

class Hero;

class Enemies {
protected:
    std::string Name;
    double Health;
    double minD;
    double maxD;
    double activeDotDamage = 0;
    int activeDotTurns = 0;
    int frozenTurns = 0;

public:
    Enemies(std::string n, double h, double min, double max) : Name(n), Health(h), minD(min), maxD(max) {}

    double getAttackPower() {
        double range = maxD - minD;
        return minD + (rand() % (int)(range + 1));
    }
    std::string takeDamage(double amount) {
        Health -= amount;
        if (Health < 0) Health = 0;
        return Name + " takes " + std::to_string((int)amount) + " damage!";
    }
    double attackHero() {
        return getAttackPower();
    }
    std::string applyDot(double dotDmg, int turns) {
        if (turns > 0) {
            activeDotDamage = dotDmg;
            activeDotTurns = turns;
            return Name + " is on fire/bleeding!";
        }
        return "";
    }
    std::string takeDotDamage() {
        if (activeDotTurns > 0) {
            Health -= activeDotDamage;
            if (Health < 0) Health = 0;
            activeDotTurns--;
            return Name + " suffers " + std::to_string((int)activeDotDamage) + " DoT damage! (" + std::to_string(activeDotTurns) + " turns left)";
        }
        return "";
    }
    std::string freeze(int turns) {
        frozenTurns = turns;
        return Name + " is FROZEN solid!";
    }

    bool isFrozen() {
        if (frozenTurns > 0) {
            frozenTurns--;
            return true;
        }
        return false;
    }
    bool isAlive() { return Health > 0; }
    std::string getName() { return Name; }
    double getHealth() { return Health; }
};

// ================= FLOOR 1: Caves  =================
class CaveBat : public Enemies {
public: CaveBat() : Enemies("Cave Bat", 10, 2, 4) {}
};
class Rat : public Enemies {
public: Rat() : Enemies("Rat", 15, 3, 5) {}
};
class GiantCentipede : public Enemies {
public: GiantCentipede() : Enemies("Giant Centipede", 22, 4, 7) {}
};
class RabidBear : public Enemies {
public: RabidBear() : Enemies("Rabid Bear", 45, 5, 10) {}
};

// ================= FLOOR 2: Chasm  =================
class Goblin : public Enemies {
public: Goblin() : Enemies("Goblin", 30, 8, 12) {}
};
class ShamGob : public Enemies {
public: ShamGob() : Enemies("Fire Goblin", 20, 13, 20) {}
};
class ChasmHarpy : public Enemies {
public: ChasmHarpy() : Enemies("Chasm Harpy", 38, 7, 13) {}
};
class BridgeTroll : public Enemies {
public: BridgeTroll() : Enemies("Bridge Troll", 45, 9, 16) {}
};

// ================= FLOOR 3: Magical Ruins =================
class AnimatedArmor : public Enemies {
public: AnimatedArmor() : Enemies("Animated Armor", 50, 10, 16) {}
};
class ArcaneWraith : public Enemies {
public: ArcaneWraith() : Enemies("Arcane Wraith", 55, 11, 19) {}
};
class Orc : public Enemies {
public: Orc() : Enemies("Orc", 60, 13, 31) {}
};

// ================= FLOOR 4: Demonic Corruption =================
class Orc2 : public Enemies {
public: Orc2() : Enemies("Demonic Warlock", 40, 25, 40) {}
};
class BloodFiend : public Enemies {
public: BloodFiend() : Enemies("Blood Fiend", 70, 18, 28) {}
};
class FleshGolem : public Enemies {
public: FleshGolem() : Enemies("Flesh Golem", 80, 15, 22) {}
};

// ================= FLOOR 5: Gates of the Abyss =================
class Hellhound : public Enemies {
public: Hellhound() : Enemies("Hellhound", 100, 22, 35) {}
};
class DreadKnight : public Enemies {
public: DreadKnight() : Enemies("Dread Knight", 120, 22, 32) {}
};
class ObsidianGargoyle : public Enemies {
public: ObsidianGargoyle() : Enemies("Obsidian Gargoyle", 140, 20, 30) {}
};

// ================= FLOOR 6: FINAL BOSS =================
class DemonLord : public Enemies {
public: DemonLord() : Enemies("Demon Lord Azazel", 300, 28, 35) {}
};
