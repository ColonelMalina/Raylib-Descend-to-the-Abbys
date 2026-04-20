
#pragma once
#include "Hero.h"
#include "Enemies.h"
#include <vector>
#include <string>

enum BattlePhase {
    STARTING,
    PLAYER_TURN,
    ENEMY_TURN,
    DISPLAY_MESSAGE,
    VICTORY_STATE,
    DEFEAT_STATE,
    FINISHED
};

class BattleManager {
public:
    BattleManager();
    
    void Start(Hero* p, std::vector<Enemies*> e);
    void Update();
    void Draw();
    void Reset() { phase = FINISHED; active = false; }

    bool isFinished() { return phase == FINISHED; }
    bool won() { return phase == VICTORY_STATE || phase == FINISHED; }
    bool isActive() { return active; }

private:
    Hero* player;
    std::vector<Enemies*> enemies;
    BattlePhase phase;
    std::string currentMessage;
    float messageTimer;
    bool active;

    void addMessage(std::string msg);
};