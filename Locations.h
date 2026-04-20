#pragma once
#include <string>
#include <vector>
#include "Weapons.h"

class Hero;
class Enemies;

enum EventType {
    EV_TEXT,
    EV_BATTLE,
    EV_WEAPON_CHEST,
    EV_HEAL_BANDAGE,
    EV_UPGRADE_MANA,
    EV_REGAIN_MANA,
    EV_ADD_POTION,
    EV_UPGRADE_HEAL,
    EV_LEARN_FIREBALL,
    EV_LEARN_ICE,
    EV_UPGRADE_FIREBALL,
    EV_UPGRADE_ICE,
    EV_FULL_HEAL
};

struct GameEvent {
    EventType type;
    std::string text;
    std::vector<Enemies*> enemies;
    Weapon weapon;
    double amount;
};

extern std::vector<GameEvent> eventQueue;

void QueueText(std::string text);
void QueueBattle(std::vector<Enemies*> enemies);
void QueueWeapon(Weapon w);
void QueueAction(EventType type, double amount = 0);

// 1 floor
void bearDen(Hero& player);
void undergroundLake(Hero& player);
void crystalGrotto(Hero& player);
void steepDescent(Hero& player);

// 2 floor
void ricketyWoodBridge(Hero& player);
void stoneOverpass(Hero& player);
void hiddenFord(Hero& player);

// 3 floor 
void ruinedAltar(Hero& player);
void illusionCorridor(Hero& player);
void alchemyLab(Hero& player);
void glowingPortal(Hero& player);

// 4 floor
void bloodRiver(Hero& player);
void fleshWallCorridor(Hero& player);
void boneCatacombs(Hero& player);
void soulPrison(Hero& player);

// 5 floor
void flamingPits(Hero& player);
void demonicStatues(Hero& player);
void abyssalGorge(Hero& player);
void blackGate(Hero& player);

// boss room
void bossRoom(Hero& player);