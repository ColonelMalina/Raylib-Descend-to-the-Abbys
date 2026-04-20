#include "Battle.h"
#include "raylib.h"

BattleManager::BattleManager() : player(nullptr), phase(FINISHED), messageTimer(0), active(false) {}

void BattleManager::Start(Hero* p, std::vector<Enemies*> e) {
    player = p;
    enemies = e;
    phase = PLAYER_TURN;
    currentMessage = "A battle begins! Your turn.";
    messageTimer = 1.0f;
    active = true;
}

void BattleManager::Update() {
    if (phase == FINISHED) return;

    if (messageTimer > 0) {
        messageTimer -= GetFrameTime();
    }

    switch (phase) {
    case PLAYER_TURN:
        if (messageTimer <= 0) {
            bool actionTaken = false;
            std::string log = "";
            //1. ATTACK
            if (IsKeyPressed(KEY_A)) {
                log = player->Attack(*enemies[0]);
                actionTaken = true;
            }
            //2. HEAL
            else if (IsKeyPressed(KEY_H)) {
                bool success = false;
                log = player->Heal(success);
                if (success) actionTaken = true;
            }
            //3. FIREBALL
            else if (IsKeyPressed(KEY_F) && player->hasFireball()) {
                bool success = false;
                log = player->Fireball(*enemies[0], success);
                if (success) actionTaken = true;
            }
            //4. ICESPIKE
            else if (IsKeyPressed(KEY_I) && player->hasIce()) {
                bool success = false;
                log = player->Icespike(*enemies[0], success);
                if (success) actionTaken = true;
            }
            //5. HEAL POTION
            else if (IsKeyPressed(KEY_P)) {
                if (player->gethPotion() > 0) {
                    currentMessage = player->usehPotion();
                }
                else {
                    currentMessage = "No health potions left!";
                }
            }

            if (actionTaken) {
                addMessage(log);
                phase = DISPLAY_MESSAGE;
            }
        }
        break;
    case DISPLAY_MESSAGE:
        if (messageTimer <= 0) {
            if (!enemies.empty() && enemies[0]->getHealth() <= 0) {
                std::string deathLog = enemies[0]->getName() + " was defeated!";
                enemies.erase(enemies.begin());

                if (enemies.empty()) {
                    addMessage(deathLog + "\nVictory! Press ENTER.");
                    phase = VICTORY_STATE;
                }
                else {
                    addMessage(deathLog);
                    phase = PLAYER_TURN;
                }
            }
            else {
                phase = ENEMY_TURN;
            }
        }
        break;
    case ENEMY_TURN:
        if (messageTimer <= 0) {
            std::string enemyLog = "";
            bool anyEnemyAlive = false;

            for (auto* enemy : enemies) {
                if (enemy->getHealth() > 0) {
                    double dmg = enemy->attackHero();
                    player->takeDamage(dmg);
                    enemyLog += enemy->getName() + " hits for " + std::to_string((int)dmg) + " dmg!\n";
                    anyEnemyAlive = true;
                }
            }

            if (anyEnemyAlive) {
                addMessage(enemyLog);
                if (player->getHealth() <= 0) phase = DEFEAT_STATE;
                else phase = PLAYER_TURN;
            }
            else {
                phase = PLAYER_TURN;
            }
        }
        break;
    case VICTORY_STATE:
        addMessage("Victory! Press ENTER to continue.");
        if (IsKeyPressed(KEY_ENTER)) phase = FINISHED;
        break;

    case DEFEAT_STATE:
        addMessage("You died... Press ENTER.");
        if (IsKeyPressed(KEY_ENTER)) phase = FINISHED;
        break;
    }
}

void BattleManager::Draw() {
    if (phase == FINISHED) return;
    DrawRectangle(360, 250, 1200, 500, Fade(BLACK, 0.4f));
    DrawText(("HERO: " + player->getName() + " (HP: " + std::to_string((int)player->getHealth()) + " | MANA: " + std::to_string((int)player->getMana()) + ")").c_str(), 400, 280, 40, GREEN);

    int yOffset = 360;
    for (auto* enemy : enemies) {
        DrawText((enemy->getName() + " - HP: " + std::to_string((int)enemy->getHealth())).c_str(), 1050, yOffset, 40, RED);
        yOffset += 60;
    }
   
    DrawRectangle(380, 550, 1160, 100, Fade(BLACK, 0.6f));
    Color msgColor = RAYWHITE;

    if (currentMessage.find("CRITICAL") != std::string::npos ||
        currentMessage.find("MEGA") != std::string::npos) {
        msgColor = GOLD;
    }
    DrawText(currentMessage.c_str(), 400, 570, 35, msgColor);
    DrawRectangle(380, 760, 1160, 70, Fade(GREEN, 0.6f));
    if (phase == PLAYER_TURN) {
        std::string controls = "[A] Attack  [H] Heal  [P] Potion (" + std::to_string((int)player->gethPotion()) + ")";
        if (player->hasFireball()) controls += "  [F] Fireball";
        if (player->hasIce()) controls += "  [I] Icespike";
        DrawText(controls.c_str(), 400, 770, 30, YELLOW);
    }
}

void BattleManager::addMessage(std::string msg) {
    currentMessage = msg;
    messageTimer = 1.8f;
}