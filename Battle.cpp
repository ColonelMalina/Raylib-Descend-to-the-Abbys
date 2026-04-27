#include "Battle.h"
#include "raylib.h"
#include <map>
#include <algorithm> 

int playerBurn = 0; static int playerFreeze = 0;

BattleManager::BattleManager() : player(nullptr), phase(FINISHED), messageTimer(0), active(false) {}void BattleManager::Start(Hero* p, std::vector<Enemies*> e) {
    player = p;
    enemies = e;
    phase = PLAYER_TURN;
    currentMessage = "A battle begins! Your turn.";
    messageTimer = 1.0f;
    active = true;


    //playerBurn = 0;
    //playerFreeze = 0;
}void BattleManager::Update() {
    if (phase == FINISHED) return;

    if (messageTimer > 0) {
        messageTimer -= GetFrameTime();
    }

    switch (phase) {
    case PLAYER_TURN:
        if (messageTimer <= 0) {
            //if (playerBurn > 0) {
            //    player->takeDamage(5);
            //    playerBurn--;
            //    addMessage("Burning hurts! -5 HP");
            //    if (player->getHealth() <= 0) { phase = DEFEAT_STATE; return; }
            //}

            //if (playerFreeze > 0) {
            //    playerFreeze--;
            //    addMessage("You are frozen and skip a turn!");
            //    phase = DISPLAY_MESSAGE;
            //    return;
            //}
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
        for (int i = 0; i < (int)enemies.size(); i++) {
            if (IsKeyPressed(KEY_ONE + i)) {
                std::string log = "";
                bool success = true; 

                if (pendingAction == 1) {
                    log = player->Attack(*enemies[i]);
                }
                else if (pendingAction == 2) {
                    log = player->Fireball(*enemies[i], success);
                }
                else if (pendingAction == 3) {
                    log = player->Icespike(*enemies[i], success);
                }

                if (success) {
                    addMessage(log);
                    phase = DISPLAY_MESSAGE;
                } else {
                    phase = PLAYER_TURN;
                    pendingAction = 0;
                }
            }
        }
        if (IsKeyPressed(KEY_FOUR)) phase = PLAYER_TURN;
        break;

    case DISPLAY_MESSAGE:
        if (messageTimer <= 0) {
            for (int i = 0; i < (int)enemies.size(); ) {
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

            for (auto* e : enemies) {
                if (e->getHealth() > 0) {
                    std::string dotMsg = e->takeDotDamage();
                    if (!dotMsg.empty()) {
                        enemyLog += dotMsg + "\n";
                    }
                }
            }

            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [](Enemies* e) { return e->getHealth() <= 0; }), enemies.end());

            for (auto* e : enemies) {
                if (e->isFrozen()) {
                    enemyLog += e->getName() + " is frozen and skips turn!\n";
                    continue;
                }

                double dmg = e->attackHero();
                player->takeDamage(dmg);
                enemyLog += e->getName() + " hits for " + std::to_string((int)dmg) + " dmg!\n";
            }

            addMessage(enemyLog);
            if (player->getHealth() <= 0) phase = DEFEAT_STATE;
            else phase = PLAYER_TURN;
        }
        break;

    case VICTORY_STATE:
        if (IsKeyPressed(KEY_ENTER)) phase = FINISHED;
        break;
    case DEFEAT_STATE:
        if (IsKeyPressed(KEY_ENTER)) phase = FINISHED;
        break;
    }
}

void BattleManager::Draw() {
    if (phase == FINISHED) return;
    DrawRectangle(360, 250, 1200, 615, Fade(BLACK, 0.4f));
    DrawText(("HERO: " + player->getName() + " (HP: " + std::to_string((int)player->getHealth()) + " | MANA: " + std::to_string((int)player->getMana()) + ")").c_str(), 400, 280, 40, GREEN);

    // Spell Info
    int spellInfoY = 320;
    int spellInfoX = 400;
    int spellInfoSpacing = 230;
    int spellInfoIdx = 0;

    // Heal Info
    Hero::SpellInfo healInfo = player->getHealInfo();
    std::string healBattle = "[H] " + healInfo.name + ": " + std::to_string(healInfo.manaCost) + " Mana";
    DrawText(healBattle.c_str(), spellInfoX + spellInfoIdx * spellInfoSpacing, spellInfoY, 22, LIME);
    spellInfoIdx++;

    // Fireball Info
    if (player->hasFireball()) {
        Hero::SpellInfo fireInfo = player->getFireballInfo();
        std::string fireBattle = "[F] " + fireInfo.name + ": " + std::to_string(fireInfo.manaCost) + " Mana";
        DrawText(fireBattle.c_str(), spellInfoX + spellInfoIdx * spellInfoSpacing, spellInfoY, 22, ORANGE);
        spellInfoIdx++;
    }

    // Icespike Info
    if (player->hasIce()) {
        Hero::SpellInfo iceInfo = player->getIcespikeInfo();
        std::string iceBattle = "[I] " + iceInfo.name + ": " + std::to_string(iceInfo.manaCost) + " Mana";
        DrawText(iceBattle.c_str(), spellInfoX + spellInfoIdx * spellInfoSpacing, spellInfoY, 22, SKYBLUE);
    }

    int yOffset = 380;
    for (auto* e : enemies) {
        Color eCol = RED;
        std::string status = "";
        DrawText((e->getName() + " - HP: " + std::to_string((int)e->getHealth())).c_str(), 900, yOffset, 40, eCol);
        yOffset += 60;
    }

    DrawRectangle(380, 550, 1160, 290, Fade(BLACK, 0.6f));
    DrawText(currentMessage.c_str(), 400, 570, 35, RAYWHITE);

    if (phase == PLAYER_TURN) {
        std::string controls = "[A] Attack  [H] Heal  [P] Potion (" + std::to_string((int)player->gethPotion()) + ")";
        if (player->hasFireball()) controls += "  [F] Fireball";
        if (player->hasIce()) controls += "  [I] Icespike";
        DrawText(controls.c_str(), 400, 870, 30, YELLOW);
    }

    if (phase == SELECT_TARGET) {
        DrawText("SELECT TARGET (Press 1, 2, 3 or 4 to go back)", 400, 870, 30, ORANGE);
    }

    if (phase == VICTORY_STATE) {
        DrawText("VICTORY! Press ENTER to continue.", 400, 870, 40, GOLD);
    }
    if (phase == DEFEAT_STATE) {
        DrawText("YOU DIED! Press ENTER to exit.", 400, 870, 40, RED);
    }
}

void BattleManager::addMessage(std::string msg) {
    currentMessage = msg;
    messageTimer = 1.8f;
}