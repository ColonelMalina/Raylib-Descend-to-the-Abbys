#include "raylib.h"
#include <iostream>
#include <ctime>
#include <string>
#include <map>
#include <set>
#include <vector>
#include "Hero.h"
#include "Enemies.h"
#include "Weapons.h"
#include "Battle.h"
#include "Locations.h"

enum GameState {
    TITLE_SCREEN,
    ENTER_NAME,
    PLAYING,
    GAME_OVER,
    VICTORY
};

int main() {
    srand(static_cast<unsigned int>(time(0)));

    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Descend to the Abyss - RPG");
    SetTargetFPS(60);

    GameState currentState = TITLE_SCREEN;
    char nameBuffer[256] = "\0";
    int letterCount = 0;

    Hero* player = nullptr;
    int currentFloor = 1;
    std::map<int, std::set<std::string>> clearedLocations;
    BattleManager battleManager;

    while (!WindowShouldClose()) {

        switch (currentState) {
        case TITLE_SCREEN:
            if (IsKeyPressed(KEY_ENTER)) currentState = ENTER_NAME;
            break;

        case ENTER_NAME: {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (letterCount < 255)) {
                    nameBuffer[letterCount] = (char)key;
                    nameBuffer[letterCount + 1] = '\0';
                    letterCount++;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                letterCount--;
                nameBuffer[letterCount] = '\0';
            }
            if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                player = new Hero(std::string(nameBuffer), 100, 30, 0);
                player->EquipW(Weapon());
                currentState = PLAYING;
            }
        } break;

        case PLAYING: {
            if (!player->isAlive()) {
                currentState = GAME_OVER;
                break;
            }

            if (!eventQueue.empty()) {
                GameEvent& ev = eventQueue.front();

                if (ev.type == EV_TEXT) {
                    if (IsKeyPressed(KEY_ENTER)) eventQueue.erase(eventQueue.begin());
                }
                else if (ev.type == EV_BATTLE) {
                    if (!battleManager.isActive()) {
                        battleManager.Start(player, ev.enemies);
                    }
                    battleManager.Update();

                    if (battleManager.isFinished()) {
                        if (battleManager.won()) {
                            battleManager.Reset();
                            eventQueue.erase(eventQueue.begin());
                            if (currentFloor == 6) currentState = VICTORY;
                        }
                        else {
                            eventQueue.clear();
                            currentState = GAME_OVER;
                        }
                    }
                }
                else if (ev.type == EV_WEAPON_CHEST) {
                    if (IsKeyPressed(KEY_ONE)) {
                        player->EquipW(ev.weapon);
                        eventQueue.erase(eventQueue.begin());
                    }
                    if (IsKeyPressed(KEY_TWO)) {
                        eventQueue.erase(eventQueue.begin());
                    }
                }
                else if (ev.type == EV_HEAL_BANDAGE) { player->bandage(ev.amount); eventQueue.erase(eventQueue.begin()); }
                else if (ev.type == EV_FULL_HEAL) { player->fullHeal(); eventQueue.erase(eventQueue.begin()); }
                else if (ev.type == EV_UPGRADE_MANA) { player->upgradeMana(ev.amount); eventQueue.erase(eventQueue.begin()); }
                else if (ev.type == EV_REGAIN_MANA) { player->regainMana(); eventQueue.erase(eventQueue.begin()); }
                else if (ev.type == EV_ADD_POTION) { player->addhPotion(ev.amount); eventQueue.erase(eventQueue.begin()); }
                else if (ev.type == EV_UPGRADE_HEAL) { player->upgradeHeal(); eventQueue.erase(eventQueue.begin()); }
                else if (ev.type == EV_LEARN_FIREBALL) { player->learnFireball(); eventQueue.erase(eventQueue.begin()); }
                else if (ev.type == EV_LEARN_ICE) { player->learnIce(); eventQueue.erase(eventQueue.begin()); }
                else if (ev.type == EV_UPGRADE_FIREBALL) { player->upgradeFireball(); eventQueue.erase(eventQueue.begin()); }
                else if (ev.type == EV_UPGRADE_ICE) { player->upgradeIce(); eventQueue.erase(eventQueue.begin()); }

                break;
            }

            if (currentFloor == 1) {
                if (IsKeyPressed(KEY_ONE) && !clearedLocations[1].count("Bear Den")) { bearDen(*player); clearedLocations[1].insert("Bear Den"); }
                if (IsKeyPressed(KEY_TWO) && !clearedLocations[1].count("Underground Lake")) { undergroundLake(*player); clearedLocations[1].insert("Underground Lake"); }
                if (IsKeyPressed(KEY_THREE) && !clearedLocations[1].count("Crystal Grotto")) { crystalGrotto(*player); clearedLocations[1].insert("Crystal Grotto"); }
                if (IsKeyPressed(KEY_FOUR)) { steepDescent(*player); clearedLocations[1].insert("Steep Descent"); currentFloor = 2; }
            }
            else if (currentFloor == 2) {
                if (IsKeyPressed(KEY_ONE)) { currentFloor = 1; }
                if (IsKeyPressed(KEY_TWO) && !clearedLocations[2].count("Rickety Bridge")) { ricketyWoodBridge(*player); currentFloor = 3; clearedLocations[2].insert("Rickety Bridge"); }
                if (IsKeyPressed(KEY_THREE) && !clearedLocations[2].count("Stone Overpass")) { stoneOverpass(*player); currentFloor = 3; clearedLocations[2].insert("Stone Overpass"); }
                if (IsKeyPressed(KEY_FOUR) && !clearedLocations[2].count("Hidden Ford")) { hiddenFord(*player); currentFloor = 3; clearedLocations[2].insert("Hidden Ford"); }
            }
            else if (currentFloor == 3) {
                if (IsKeyPressed(KEY_ONE)) { currentFloor = 2; }
                if (IsKeyPressed(KEY_TWO) && !clearedLocations[3].count("Ruined Altar")) { ruinedAltar(*player); clearedLocations[3].insert("Ruined Altar"); }
                if (IsKeyPressed(KEY_THREE) && !clearedLocations[3].count("Illusion Corridor")) { illusionCorridor(*player); clearedLocations[3].insert("Illusion Corridor"); }
                if (IsKeyPressed(KEY_FOUR) && !clearedLocations[3].count("Alchemy Lab")) { alchemyLab(*player); clearedLocations[3].insert("Alchemy Lab"); }
                if (IsKeyPressed(KEY_FIVE)) { glowingPortal(*player); currentFloor = 4; clearedLocations[3].insert("Glowing Portal"); }
            }
            else if (currentFloor == 4) {
                if (IsKeyPressed(KEY_ONE)) { currentFloor = 3; }
                if (IsKeyPressed(KEY_TWO) && !clearedLocations[4].count("Blood River")) { bloodRiver(*player); currentFloor = 5; clearedLocations[4].insert("Blood River"); }
                if (IsKeyPressed(KEY_THREE) && !clearedLocations[4].count("Soul Prison")) { soulPrison(*player); currentFloor = 5; clearedLocations[4].insert("Soul Prison"); }
                if (IsKeyPressed(KEY_FOUR) && !clearedLocations[4].count("Flesh Wall")) { fleshWallCorridor(*player); clearedLocations[4].insert("Flesh Wall"); }
                if (IsKeyPressed(KEY_FIVE) && !clearedLocations[4].count("Bone Catacombs")) { boneCatacombs(*player); clearedLocations[4].insert("Bone Catacombs"); }
            }
            else if (currentFloor == 5) {
                if (IsKeyPressed(KEY_ONE)) { currentFloor = 4; }
                if (IsKeyPressed(KEY_TWO) && !clearedLocations[5].count("Flaming Pits")) { flamingPits(*player); clearedLocations[5].insert("Flaming Pits"); }
                if (IsKeyPressed(KEY_THREE) && !clearedLocations[5].count("Demonic Statues")) { demonicStatues(*player); clearedLocations[5].insert("Demonic Statues"); }
                if (IsKeyPressed(KEY_FOUR) && !clearedLocations[5].count("Abyssal Gorge")) { abyssalGorge(*player); clearedLocations[5].insert("Abyssal Gorge"); }
                if (IsKeyPressed(KEY_FIVE)) { blackGate(*player); currentFloor = 6; clearedLocations[5].insert("Black Gate"); }
            }
            else if (currentFloor == 6) {
                bossRoom(*player);
            }
        } break;

        case GAME_OVER:
        case VICTORY:
            if (IsKeyPressed(KEY_ENTER)) CloseWindow();
            break;
        }

        // --- DRAWING ---
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
        case TITLE_SCREEN:
            DrawText("============================================", 150, 200, 20, DARKGRAY);
            DrawText("WELCOME TO THE DESCEND TO THE ABYSS", 160, 240, 20, RAYWHITE);
            DrawText("WILL YOU FIND THE WAY TO THE END?", 180, 280, 20, GRAY);
            DrawText("OR WILL YOU GET LOST AND DIE...", 200, 320, 20, RED);
            DrawText("============================================", 150, 360, 20, DARKGRAY);
            DrawText("Press ENTER to start", 280, 450, 20, YELLOW);
            break;

        case ENTER_NAME:
            DrawText("Enter your hero's name:", 250, 250, 20, RAYWHITE);
            DrawRectangle(250, 290, 300, 40, LIGHTGRAY);
            DrawText(nameBuffer, 260, 300, 20, MAROON);
            DrawText("Press ENTER to confirm", 250, 350, 20, DARKGRAY);
            break;

        case PLAYING: {
            if (!eventQueue.empty()) {
                GameEvent& ev = eventQueue.front();

                if (ev.type == EV_TEXT) {
                    DrawText(ev.text.c_str(), 100, 250, 20, RAYWHITE);
                    DrawText("Press ENTER to continue...", 100, 400, 15, GRAY);
                }
                else if (ev.type == EV_BATTLE) {
                    battleManager.Draw();
                }
                else if (ev.type == EV_WEAPON_CHEST) {
                    DrawText(("[CHEST] Found: " + ev.weapon.Wname + "\nDmg: " + std::to_string((int)ev.weapon.minDamage) + "-" + std::to_string((int)ev.weapon.maxDamage)).c_str(), 100, 250, 20, GREEN);
                    DrawText("Press 1 to EQUIP", 100, 330, 20, RAYWHITE);
                    DrawText("Press 2 to IGNORE", 100, 370, 20, GRAY);
                }
                break;
            }

            std::string status = "FLOOR " + std::to_string(currentFloor) + " - Status: " + player->getName() +
                " (HP: " + std::to_string((int)player->getHealth()) +
                " Mana: " + std::to_string((int)player->getMana()) + ")";
            DrawText(status.c_str(), 50, 30, 20, GREEN);
            DrawLine(50, 60, 750, 60, DARKGRAY);

            if (clearedLocations.count(currentFloor)) {
                std::string clearedTxt = "Cleared: ";
                for (const auto& loc : clearedLocations[currentFloor]) {
                    clearedTxt += "[" + loc + "] ";
                }
                DrawText(clearedTxt.c_str(), 50, 70, 15, DARKGRAY);
            }

            int yPos = 150;
            if (currentFloor == 1) {
                DrawText("--- FLOOR 1: The Natural Caverns ---", 50, yPos, 20, RAYWHITE);
                DrawText("Press 1: [Explore] Bear Den", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 2: [Explore] Underground Lake", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 3: [Explore] Crystal Grotto", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 4: [Proceed] Steep Descent", 50, yPos += 40, 20, YELLOW);
            }
            else if (currentFloor == 2) {
                DrawText("--- FLOOR 2: The Great Chasm ---", 50, yPos, 20, RAYWHITE);
                DrawText("Press 1: [Go Back] Return to Floor 1", 50, yPos += 40, 20, GRAY);
                DrawText("Press 2: [Proceed] Rickety Wood Bridge", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 3: [Proceed] Stone Overpass", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 4: [Proceed] Hidden Ford", 50, yPos += 40, 20, YELLOW);
            }
            else if (currentFloor == 3) {
                DrawText("--- FLOOR 3: The Forgotten Ruins ---", 50, yPos, 20, RAYWHITE);
                DrawText("Press 1: [Go Back] Return to Floor 2", 50, yPos += 40, 20, GRAY);
                DrawText("Press 2: [Explore] Ruined Altar", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 3: [Explore] Illusion Corridor", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 4: [Explore] Alchemy Lab", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 5: [Proceed] Glowing Portal", 50, yPos += 40, 20, YELLOW);
            }
            else if (currentFloor == 4) {
                DrawText("--- FLOOR 4: The Corrupted Depths ---", 50, yPos, 20, RAYWHITE);
                DrawText("Press 1: [Go Back] Return to Floor 3", 50, yPos += 40, 20, GRAY);
                DrawText("Press 2: [Proceed] Follow the River of Blood", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 3: [Proceed] Enter the Soul Prison", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 4: [Explore] Flesh Wall Corridor", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 5: [Explore] Bone Catacombs", 50, yPos += 40, 20, YELLOW);
            }
            else if (currentFloor == 5) {
                DrawText("--- FLOOR 5: The Gates of Hell ---", 50, yPos, 20, RAYWHITE);
                DrawText("Press 1: [Go Back] Return to Floor 4", 50, yPos += 40, 20, GRAY);
                DrawText("Press 2: [Explore] Flaming Pits", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 3: [Explore] Demonic Statues", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 4: [Explore] Abyssal Gorge", 50, yPos += 40, 20, LIGHTGRAY);
                DrawText("Press 5: [BOSS GATE] Black Gate", 50, yPos += 40, 20, RED);
            }

        } break;

        case GAME_OVER:
            DrawText("--- GAME OVER ---", 300, 250, 30, RED);
            DrawText("Press ENTER to Exit", 320, 300, 20, RAYWHITE);
            break;

        case VICTORY:
            DrawText("*** CONGRATULATIONS! ***", 220, 200, 30, GOLD);
            DrawText("YOU HAVE CONQUERED THE ABYSS!", 180, 250, 30, YELLOW);
            DrawText("Press ENTER to Exit", 320, 350, 20, RAYWHITE);
            break;
        }

        EndDrawing();
    }

    if (player != nullptr) delete player;
    CloseWindow();

    return 0;
};