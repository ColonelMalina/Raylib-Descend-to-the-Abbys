#include "Battle.h"
#include "raylib.h"
#include <map>

// Pomocné proměnné pro sledování stavů (aby se nemusely měnit .h soubory)
static int playerBurn = 0;
static int playerFreeze = 0;
// Mapa pro sledování stavů více nepřátel najednou
static std::map<Enemies*, int> enemyBurn;
static std::map<Enemies*, int> enemyFreeze;

BattleManager::BattleManager() : player(nullptr), phase(FINISHED), messageTimer(0), active(false) {}

void BattleManager::Start(Hero* p, std::vector<Enemies*> e) {
    player = p;
    enemies = e;
    phase = PLAYER_TURN;
    currentMessage = "A battle begins! Your turn.";
    messageTimer = 1.0f;
    active = true;

    // Reset stavů na začátku bitvy
    playerBurn = 0;
    playerFreeze = 0;
    enemyBurn.clear();
    enemyFreeze.clear();
}

void BattleManager::Update() {
    if (phase == FINISHED) return;

    if (messageTimer > 0) {
        messageTimer -= GetFrameTime();
    }

    switch (phase) {
    case PLAYER_TURN:
        if (messageTimer <= 0) {
            if (playerBurn > 0) {
                player->takeDamage(5);
                playerBurn--;
                addMessage("Burning hurts! -5 HP");
                if (player->getHealth() <= 0) { phase = DEFEAT_STATE; return; }
            }

            if (playerFreeze > 0) {
                playerFreeze--;
                addMessage("You are frozen and skip a turn!");
                phase = DISPLAY_MESSAGE;
                return;
            }
            if (IsKeyPressed(KEY_A)) { pendingAction = 1; phase = SELECT_TARGET; }
            else if (IsKeyPressed(KEY_H)) {
                bool success = false;
                std::string log = player->Heal(success);
                if (success) { addMessage(log); phase = DISPLAY_MESSAGE; }
            }
            else if (IsKeyPressed(KEY_F) && player->hasFireball()) { pendingAction = 2; phase = SELECT_TARGET; }
            else if (IsKeyPressed(KEY_I) && player->hasIce()) { pendingAction = 3; phase = SELECT_TARGET; }
            else if (IsKeyPressed(KEY_P)) {
                if (player->gethPotion() > 0) currentMessage = player->usehPotion();
                else currentMessage = "No health potions left!";
            }
        }
        break;

    case SELECT_TARGET:
        for (int i = 0; i < enemies.size(); i++) {
            if (IsKeyPressed(KEY_ONE + i)) {
                std::string log = "";
                bool success = false;
                if (pendingAction == 1) log = player->Attack(*enemies[i]);
                else if (pendingAction == 2) {
                    log = player->Fireball(*enemies[i], success);
                    if (success) enemyBurn[enemies[i]] = 3; 
                }
                else if (pendingAction == 3) {
                    log = player->Icespike(*enemies[i], success);
                    if (success) enemyFreeze[enemies[i]] = 2; 
                }

                addMessage(log);
                phase = DISPLAY_MESSAGE;
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)) phase = PLAYER_TURN;
        break;

    case DISPLAY_MESSAGE:
        if (messageTimer <= 0) {
            for (int i = 0; i < enemies.size(); ) {
                if (enemies[i]->getHealth() <= 0) {
                    std::string dLog = enemies[i]->getName() + " was defeated!";
                    enemies.erase(enemies.begin() + i);
                    addMessage(dLog);
                }
                else i++;
            }

            if (enemies.empty()) phase = VICTORY_STATE;
            else phase = ENEMY_TURN;
        }
        break;

    case ENEMY_TURN:
        if (messageTimer <= 0) {
            std::string enemyLog = "";
            bool anyEnemyActed = false;

            for (auto* e : enemies) {
                if (e->getHealth() > 0) {
                    if (enemyBurn[e] > 0) {
                        e->takeDamage(7);
                        enemyBurn[e]--;
                        enemyLog += e->getName() + " burns!\n";
                    }

                    if (enemyFreeze[e] > 0) {
                        enemyFreeze[e]--;
                        enemyLog += e->getName() + " is frozen!\n";
                        continue;
                    }

                    if (e->getHealth() > 0) {
                        double dmg = e->attackHero();
                        player->takeDamage(dmg);
                        enemyLog += e->getName() + " hits for " + std::to_string((int)dmg) + " dmg!\n";
                        anyEnemyActed = true;
                    }
                }
            }

            addMessage(enemyLog);
            if (player->getHealth() <= 0) phase = DEFEAT_STATE;
            else phase = PLAYER_TURN;
        }
        break;

    case VICTORY_STATE:
        addMessage("Victory! Press ENTER to continue.");
        if (IsKeyPressed(KEY_ENTER)) phase = FINISHED;
        break;
    case DEFEAT_STATE:
        if (IsKeyPressed(KEY_ENTER)) phase = FINISHED;
        break;
    }
}

void BattleManager::Draw() {
    if (phase == FINISHED) return;
    DrawRectangle(360, 250, 1200, 500, Fade(BLACK, 0.4f));
    DrawText(("HERO: " + player->getName() + " (HP: " + std::to_string((int)player->getHealth()) + " | MANA: " + std::to_string((int)player->getMana()) + ")").c_str(), 400, 280, 40, GREEN);

    int yOffset = 360;
    for (auto* e : enemies) {
        Color eCol = RED;
        std::string status = "";
        if (enemyBurn[e] > 0) { eCol = ORANGE; status = " [BURN]"; }
        if (enemyFreeze[e] > 0) { eCol = BLUE; status = " [FROZEN]"; }

        DrawText((e->getName() + " - HP: " + std::to_string((int)e->getHealth()) + status).c_str(), 1050, yOffset, 40, eCol);
        yOffset += 60;
    }

    DrawRectangle(380, 550, 1160, 100, Fade(BLACK, 0.6f));
    DrawText(currentMessage.c_str(), 400, 570, 35, RAYWHITE);

    if (phase == PLAYER_TURN) {
        std::string controls = "[A] Attack  [H] Heal  [P] Potion (" + std::to_string((int)player->gethPotion()) + ")";
        if (player->hasFireball()) controls += "  [F] Fireball";
        if (player->hasIce()) controls += "  [I] Icespike";
        DrawText(controls.c_str(), 400, 770, 30, YELLOW);
    }

    if (phase == SELECT_TARGET) {
        DrawText("SELECT TARGET (Press 1, 2... or ESC to back)", 400, 770, 30, ORANGE);
    }
}

void BattleManager::addMessage(std::string msg) {
    currentMessage = msg;
    messageTimer = 1.8f;
}