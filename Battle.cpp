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
        return;
    }

    switch (phase) {
    case PLAYER_TURN:
        if (IsKeyPressed(KEY_A)) {
            std::string log = player->Attack(*enemies[0]);
            addMessage(log);
            phase = DISPLAY_MESSAGE;
        }
        if (IsKeyPressed(KEY_H)) {
            bool success = false;
            std::string log = player->Heal(success);
            addMessage(log);
            if (success) phase = DISPLAY_MESSAGE;
        }
        if (IsKeyPressed(KEY_F) && player->hasFireball()) {
            bool success = false;
            std::string log = player->Fireball(*enemies[0], success);
            addMessage(log);
            if (success) phase = DISPLAY_MESSAGE;
        }
        if (IsKeyPressed(KEY_I) && player->hasIce()) {
            bool success = false;
            std::string log = player->Icespike(*enemies[0], success);
            addMessage(log);
            if (success) phase = DISPLAY_MESSAGE;
        }
        break;

    case DISPLAY_MESSAGE:
        if (enemies[0]->getHealth() <= 0) {
            addMessage(enemies[0]->getName() + " was defeated!");
            enemies.erase(enemies.begin());

            if (enemies.empty()) phase = VICTORY_STATE;
            else phase = PLAYER_TURN;
        }
        else {
            phase = ENEMY_TURN;
        }
        break;

    case ENEMY_TURN: {
        std::string enemyLog = "";
        for (auto* enemy : enemies) {
            double dmg = enemy->attackHero();
            std::string dmgLog = player->takeDamage(dmg);
            enemyLog += enemy->getName() + " hits for " + std::to_string((int)dmg) + " dmg!\n";
        }
        addMessage(enemyLog);

        if (player->getHealth() <= 0) phase = DEFEAT_STATE;
        else phase = PLAYER_TURN;
        break;
    }

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

    DrawRectangle(100, 100, 600, 400, DARKGRAY);
    DrawText(("HERO: " + player->getName() + " (HP: " + std::to_string((int)player->getHealth()) + " | MP: " + std::to_string((int)player->getMana()) + ")").c_str(), 120, 120, 20, GREEN);

    int yOffset = 150;
    for (auto* enemy : enemies) {
        DrawText((enemy->getName() + " - HP: " + std::to_string((int)enemy->getHealth())).c_str(), 450, yOffset, 20, RED);
        yOffset += 40;
    }

    DrawRectangle(100, 420, 600, 80, BLACK);
    DrawText(currentMessage.c_str(), 120, 430, 20, RAYWHITE);

    if (phase == PLAYER_TURN) {
        std::string controls = "[A] Attack  [H] Heal";
        if (player->hasFireball()) controls += "  [F] Fireball";
        if (player->hasIce()) controls += "  [I] Icespike";
        DrawText(controls.c_str(), 120, 380, 15, YELLOW);
    }
}

void BattleManager::addMessage(std::string msg) {
    currentMessage = msg;
    messageTimer = 1.8f;
}