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
    CONTROLS_SCREEN,
    ENTER_NAME,
    PLAYING,
    GAME_OVER,
    VICTORY
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    const int gameScreenWidth = 1920;
    const int gameScreenHeight = 1080;

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_UNDECORATED | FLAG_FULLSCREEN_MODE);

    InitWindow(0, 0, "Descend to the Abyss");

    int monitor = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));

    InitAudioDevice();

    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    // BACKGROUNDS
    std::map<int, Texture2D> floorBackgrounds;
    floorBackgrounds[0] = LoadTexture("resources/menu_bg.png");
    floorBackgrounds[1] = LoadTexture("resources/floor1.png");
    floorBackgrounds[2] = LoadTexture("resources/floor2.png");
    floorBackgrounds[3] = LoadTexture("resources/floor3.png");
    floorBackgrounds[4] = LoadTexture("resources/floor4.png");
    floorBackgrounds[5] = LoadTexture("resources/floor5.png");
    floorBackgrounds[6] = LoadTexture("resources/floor6.png");

    // MUSIC 
    std::map<int, Music> floorMusic;
    floorMusic[0] = LoadMusicStream("resources/menu_music.mp3");
    floorMusic[1] = LoadMusicStream("resources/music1.mp3");
    floorMusic[2] = LoadMusicStream("resources/music2.mp3");
    floorMusic[3] = LoadMusicStream("resources/music3.mp3");
    floorMusic[4] = LoadMusicStream("resources/music4.mp3");
    floorMusic[5] = LoadMusicStream("resources/music5.mp3");
    floorMusic[6] = LoadMusicStream("resources/music6.mp3");

    int lastMusicFloor = -1;
    SetTargetFPS(60);

    GameState currentState = TITLE_SCREEN;
    char nameBuffer[256] = "\0";
    int letterCount = 0;

    Hero* player = nullptr;
    int currentFloor = 1;
    std::map<int, std::set<std::string>> clearedLocations;
    BattleManager battleManager;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        float scale = fminf((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);

        // MUSIC LOGIC
        int activeMusicIndex = (currentState == PLAYING) ? currentFloor : 0;

        if (activeMusicIndex != lastMusicFloor) {
            if (lastMusicFloor != -1) StopMusicStream(floorMusic[lastMusicFloor]);
            PlayMusicStream(floorMusic[activeMusicIndex]);
            lastMusicFloor = activeMusicIndex;
        }
        UpdateMusicStream(floorMusic[activeMusicIndex]);

        //  GAME LOGIC 
        switch (currentState) {
        case TITLE_SCREEN:
            if (IsKeyPressed(KEY_ENTER)) currentState = CONTROLS_SCREEN;
            break;

        case CONTROLS_SCREEN:
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
                if (player != nullptr) { delete player; player = nullptr; }
                player = new Hero(std::string(nameBuffer), 100, 30, 0);
                player->EquipW(Weapon());
                currentFloor = 1;
                clearedLocations.clear();
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
                    else if (IsKeyPressed(KEY_TWO)) {
                        eventQueue.erase(eventQueue.begin());
                    }
                }
                else if (IsKeyPressed(KEY_ENTER)) {
                    if (ev.type == EV_HEAL_BANDAGE) { player->bandage(ev.amount); }
                    else if (ev.type == EV_FULL_HEAL) { player->fullHeal(); }
                    else if (ev.type == EV_UPGRADE_MANA) { player->upgradeMana(ev.amount); }
                    else if (ev.type == EV_REGAIN_MANA) { player->regainMana(); }
                    else if (ev.type == EV_ADD_POTION) { player->addhPotion(ev.amount); }
                    else if (ev.type == EV_UPGRADE_HEAL) { player->upgradeHeal(); }
                    else if (ev.type == EV_LEARN_FIREBALL) { player->learnFireball(); }
                    else if (ev.type == EV_LEARN_ICE) { player->learnIce(); }
                    else if (ev.type == EV_UPGRADE_FIREBALL) { player->upgradeFireball(); }
                    else if (ev.type == EV_UPGRADE_ICE) { player->upgradeIce(); }

                    eventQueue.erase(eventQueue.begin());
                }
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
                if (IsKeyPressed(KEY_TWO) && !clearedLocations[2].count("Rickety Bridge")) { ricketyWoodBridge(*player); clearedLocations[2].insert("Rickety Bridge"); }
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
                if (IsKeyPressed(KEY_FOUR) && !clearedLocations[4].count("Blood River")) { bloodRiver(*player); currentFloor = 5; clearedLocations[4].insert("Blood River"); }
                if (IsKeyPressed(KEY_FIVE) && !clearedLocations[4].count("Soul Prison")) { soulPrison(*player); currentFloor = 5; clearedLocations[4].insert("Soul Prison"); }
                if (IsKeyPressed(KEY_TWO) && !clearedLocations[4].count("Flesh Wall")) { fleshWallCorridor(*player); clearedLocations[4].insert("Flesh Wall"); }
                if (IsKeyPressed(KEY_THREE) && !clearedLocations[4].count("Bone Catacombs")) { boneCatacombs(*player); clearedLocations[4].insert("Bone Catacombs"); }
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
            if (IsKeyPressed(KEY_ENTER)) {
                // RESET HRY
                letterCount = 0;
                nameBuffer[0] = '\0';
                currentState = ENTER_NAME;
            }
            if (IsKeyPressed(KEY_ESCAPE)) CloseWindow();
            break;

        case VICTORY:
            if (IsKeyPressed(KEY_ENTER)) CloseWindow();
            break;
        }

        // --- DRAWING ---
        BeginDrawing();
        ClearBackground(BLACK);

        BeginTextureMode(target);
        ClearBackground(BLACK);

        switch (currentState) {
        case TITLE_SCREEN:
            DrawTexturePro(floorBackgrounds[0], { 0, 0, (float)floorBackgrounds[0].width, (float)floorBackgrounds[0].height }, { 0, 0, (float)gameScreenWidth, (float)gameScreenHeight }, { 0,0 }, 0, WHITE);
            DrawText("==========================================================================", 230, 300, 40, DARKGRAY);
            DrawText("WELCOME TO THE DESCEND TO THE ABYSS", 490, 400, 40, RAYWHITE);
            DrawText("WILL YOU FIND THE WAY TO THE END?", 580, 500, 35, RAYWHITE);
            DrawText("OR WILL YOU GET LOST AND DIE...", 660, 600, 30, RED);
            DrawText("==========================================================================", 230, 700, 40, DARKGRAY);
            DrawRectangle(680,845,420,60,BLACK);
            DrawText("Press ENTER to start", 690, 850, 35, YELLOW);
            break;

        case CONTROLS_SCREEN:
            DrawTexturePro(floorBackgrounds[0], { 0, 0, (float)floorBackgrounds[0].width, (float)floorBackgrounds[0].height }, { 0, 0, (float)gameScreenWidth, (float)gameScreenHeight }, { 0,0 }, 0, WHITE);
            DrawRectangle(460, 250, 1000, 550, Fade(BLACK, 0.8f));

            DrawText("--- HOW TO PLAY ---", 780, 300, 45, GOLD);
            DrawText("Exploration:", 500, 400, 35, GREEN);
            DrawText("Use keys [1] - [5] to select locations or proceed.", 500, 450, 30, RAYWHITE);

            DrawText("Combat:", 500, 530, 35, RED);
            DrawText("[A] Attack | [H] Heal | [P] Use Potion", 500, 580, 30, RAYWHITE);
            DrawText("(You can attack after using potion)", 500, 630, 30, GRAY);
            DrawText("[F] Fireball | [I] Icespike (If learned)", 500, 680, 30, RAYWHITE);
            DrawText("In battle, select target using keys [1], [2]...", 500, 730, 30, LIGHTGRAY);

            DrawText("Press ENTER to start your journey", 680, 850, 35, YELLOW);
            break;

        case ENTER_NAME:
            DrawTexturePro(floorBackgrounds[0], { 0, 0, (float)floorBackgrounds[0].width, (float)floorBackgrounds[0].height }, { 0, 0, (float)gameScreenWidth, (float)gameScreenHeight }, { 0,0 }, 0, WHITE);
            DrawRectangle(700, 440, 520, 205, BLACK);
            DrawText("Enter your hero's name:", 710, 450, 40, RAYWHITE);
            DrawRectangle(710, 510, 500, 80, LIGHTGRAY);
            DrawText(nameBuffer, 730, 530, 40, MAROON);
            DrawText("Press ENTER to confirm", 710, 600, 30, DARKGRAY);
            break;

        case PLAYING: {
            DrawTexturePro(floorBackgrounds[currentFloor], { 0, 0, (float)floorBackgrounds[currentFloor].width, (float)floorBackgrounds[currentFloor].height }, { 0, 0, (float)gameScreenWidth, (float)gameScreenHeight }, { 0,0 }, 0, WHITE);
            DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, Fade(BLACK, 0.4f));

            if (!eventQueue.empty()) {
                GameEvent& ev = eventQueue.front();
                if (ev.type == EV_TEXT) {
                    DrawText(ev.text.c_str(), 200, 450, 40, RAYWHITE);
                    DrawText("Press ENTER to continue...", 200, 700, 30, GRAY);
                }
                else if (ev.type == EV_BATTLE) {
                    battleManager.Draw();
                }
                else if (ev.type == EV_WEAPON_CHEST) {
                    int currentMin = (int)player->getCurrentWeapon().minDamage;
                    int currentMax = (int)player->getCurrentWeapon().maxDamage;
                    int diffMin = (int)ev.weapon.minDamage - currentMin;
                    int diffMax = (int)ev.weapon.maxDamage - currentMax;

                    DrawRectangle(150, 300, 1620, 500, Fade(DARKGRAY, 0.95f));
                    DrawText(("[LOOT]: " + ev.weapon.Wname).c_str(), 200, 350, 50, GOLD);
                    std::string statsTxt = "New Dmg: " + std::to_string((int)ev.weapon.minDamage) + "-" + std::to_string((int)ev.weapon.maxDamage);
                    std::string compareTxt = " (Change: " + std::string(diffMin >= 0 ? "+" : "") + std::to_string(diffMin) + "/" + std::string(diffMax >= 0 ? "+" : "") + std::to_string(diffMax) + ")";
                    DrawText((statsTxt + compareTxt).c_str(), 200, 430, 40, (diffMin + diffMax >= 0 ? GREEN : RED));
                    DrawText(("Current Weapon: " + player->getCurrentWeapon().Wname + " (" + std::to_string(currentMin) + "-" + std::to_string(currentMax) + ")").c_str(), 200, 500, 30, GRAY);
                    DrawText("Press 1 to EQUIP", 200, 660, 40, RAYWHITE);
                    DrawText("Press 2 to IGNORE", 200, 740, 40, GRAY);
                }
                else {
                    std::string infoText = "";
                    if (ev.type == EV_HEAL_BANDAGE) infoText = "You found a bandage and recovered " + std::to_string((int)ev.amount) + " HP!";
                    else if (ev.type == EV_FULL_HEAL) infoText = "You feel a warm light... HP fully restored!";
                    else if (ev.type == EV_UPGRADE_MANA) infoText = "You found an ancient scroll! Max Mana increased by " + std::to_string((int)ev.amount) + ".";
                    else if (ev.type == EV_REGAIN_MANA) infoText = "You rested by the light. Mana restored.";
                    else if (ev.type == EV_ADD_POTION) infoText = "You found " + std::to_string((int)ev.amount) + " Health Potion(s)!";
                    else if (ev.type == EV_LEARN_FIREBALL) infoText = "The flames speak to you... You learned FIREBALL!";
                    else if (ev.type == EV_LEARN_ICE) infoText = "A cold breeze surrounds you... You learned ICESPIKE!";
                    else if (ev.type == EV_UPGRADE_HEAL) infoText = "Your healing power rise!";
                    else if (ev.type == EV_UPGRADE_FIREBALL) infoText = "Your fire power rise!";
                    else if (ev.type == EV_UPGRADE_ICE) infoText = "Your freezing power rise!";
                    if (!ev.text.empty()) infoText = ev.text;

                    DrawText(infoText.c_str(), 300, 480, 40, RAYWHITE);
                    DrawText("Press ENTER to confirm", 300, 620, 30, YELLOW);
                }
                break;
            }

            std::string status = "FLOOR " + std::to_string(currentFloor) + " - Status: " + player->getName() + " (HP: " + std::to_string((int)player->getHealth()) + " Mana: " + std::to_string((int)player->getMana()) + ")";
            DrawText(status.c_str(), 100, 50, 40, GREEN);
            DrawLine(100, 110, 1820, 110, DARKGRAY);

            if (clearedLocations.count(currentFloor)) {
                std::string clearedTxt = "Cleared: ";
                for (const auto& loc : clearedLocations[currentFloor]) {
                    clearedTxt += "[" + loc + "] ";
                }
                DrawText(clearedTxt.c_str(), 100, 130, 30, DARKGRAY);
            }

            int yPos = 300;
            int centerX = gameScreenWidth / 2;

            if (currentFloor == 1) {
                const char* title = "--- FLOOR 1: The Natural Caverns ---";
                DrawText(title, centerX - MeasureText(title, 45) / 2, yPos, 45, RAYWHITE);
                const char* t1 = "Press 1: [Explore] Bear Den";
                DrawText(t1, centerX - MeasureText(t1, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t2 = "Press 2: [Explore] Underground Lake";
                DrawText(t2, centerX - MeasureText(t2, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t3 = "Press 3: [Explore] Crystal Grotto";
                DrawText(t3, centerX - MeasureText(t3, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t4 = "Press 4: [Proceed] Steep Descent";
                DrawText(t4, centerX - MeasureText(t4, 40) / 2, yPos += 80, 40, YELLOW);
            }
            else if (currentFloor == 2) {
                const char* title = "--- FLOOR 2: The Great Chasm ---";
                DrawText(title, centerX - MeasureText(title, 45) / 2, yPos, 45, RAYWHITE);
                const char* t1 = "Press 1: [Go Back] Return to Floor 1";
                DrawText(t1, centerX - MeasureText(t1, 40) / 2, yPos += 80, 40, GRAY);
                const char* t2 = "Press 2: [Explore] Rickety Wood Bridge";
                DrawText(t2, centerX - MeasureText(t2, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t3 = "Press 3: [Proceed] Stone Overpass";
                DrawText(t3, centerX - MeasureText(t3, 40) / 2, yPos += 80, 40, YELLOW);
                const char* t4 = "Press 4: [Proceed] Hidden Ford";
                DrawText(t4, centerX - MeasureText(t4, 40) / 2, yPos += 80, 40, YELLOW);
            }
            else if (currentFloor == 3) {
                const char* title = "--- FLOOR 3: The Forgotten Ruins ---";
                DrawText(title, centerX - MeasureText(title, 45) / 2, yPos, 45, RAYWHITE);
                const char* t1 = "Press 1: [Go Back] Return to Floor 2";
                DrawText(t1, centerX - MeasureText(t1, 40) / 2, yPos += 80, 40, GRAY);
                const char* t2 = "Press 2: [Explore] Ruined Altar";
                DrawText(t2, centerX - MeasureText(t2, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t3 = "Press 3: [Explore] Illusion Corridor";
                DrawText(t3, centerX - MeasureText(t3, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t4 = "Press 4: [Explore] Alchemy Lab";
                DrawText(t4, centerX - MeasureText(t4, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t5 = "Press 5: [Proceed] Glowing Portal";
                DrawText(t5, centerX - MeasureText(t5, 40) / 2, yPos += 80, 40, YELLOW);
            }          
            else if (currentFloor == 4) {
                const char* title = "--- FLOOR 4: The Corrupted Depths ---";
                DrawText(title, centerX - MeasureText(title, 45) / 2, yPos, 45, RAYWHITE);
                const char* t1 = "Press 1: [Go Back] Return to Floor 3";
                DrawText(t1, centerX - MeasureText(t1, 40) / 2, yPos += 80, 40, GRAY);                
                const char* t2 = "Press 2: [Explore] Flesh Wall Corridor";
                DrawText(t2, centerX - MeasureText(t2, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t3 = "Press 3: [Explore] Bone Catacombs";
                DrawText(t3, centerX - MeasureText(t3, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t4 = "Press 4: [Proceed] Follow the River of Blood";
                DrawText(t4, centerX - MeasureText(t4, 40) / 2, yPos += 80, 40, YELLOW);
                const char* t5 = "Press 5: [Proceed] Enter the Soul Prison";
                DrawText(t5, centerX - MeasureText(t5, 40) / 2, yPos += 80, 40, YELLOW);
            }
            else if (currentFloor == 5) {
                const char* title = "--- FLOOR 5: The Gates of Hell ---";
                DrawText(title, centerX - MeasureText(title, 45) / 2, yPos, 45, RAYWHITE);
                const char* t1 = "Press 1: [Go Back] Return to Floor 4";
                DrawText(t1, centerX - MeasureText(t1, 40) / 2, yPos += 80, 40, GRAY);
                const char* t2 = "Press 2: [Explore] Flaming Pits";
                DrawText(t2, centerX - MeasureText(t2, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t3 = "Press 3: [Explore] Demonic Statues";
                DrawText(t3, centerX - MeasureText(t3, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t4 = "Press 4: [Explore] Abyssal Gorge";
                DrawText(t4, centerX - MeasureText(t4, 40) / 2, yPos += 80, 40, LIGHTGRAY);
                const char* t5 = "Press 5: [BOSS GATE] Black Gate";
                DrawText(t5, centerX - MeasureText(t5, 40) / 2, yPos += 80, 40, RED);
            }
        } break;

        case GAME_OVER:
            DrawTexturePro(floorBackgrounds[0], { 0, 0, (float)floorBackgrounds[0].width, (float)floorBackgrounds[0].height }, { 0, 0, (float)gameScreenWidth, (float)gameScreenHeight }, { 0,0 }, 0, WHITE);
            DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, Fade(BLACK, 0.6f));
            DrawText("--- GAME OVER ---", 615, 450, 60, RED);
            DrawText("Press ENTER to Try again", 630, 550, 40, RAYWHITE);
            DrawText("or press ESC to Exit", 710, 600, 35, RAYWHITE);
            break;

        case VICTORY:
            DrawText("*** CONGRATULATIONS! ***", 620, 400, 60, GOLD);
            DrawText("YOU HAVE CONQUERED THE ABYSS!", 580, 500, 50, YELLOW);
            DrawText("Press ENTER to Exit", 820, 650, 40, RAYWHITE);
            break;
        }
        EndTextureMode();

        DrawTexturePro(target.texture, { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height }, { (GetScreenWidth() - ((float)gameScreenWidth * scale)) * 0.5f, (GetScreenHeight() - ((float)gameScreenHeight * scale)) * 0.5f, (float)gameScreenWidth * scale, (float)gameScreenHeight * scale }, { 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }
    // UNLOADING
    for (int i = 0; i <= 6; i++) {
        UnloadTexture(floorBackgrounds[i]);
        UnloadMusicStream(floorMusic[i]);
    }
    UnloadRenderTexture(target);
    CloseAudioDevice();
    if (player != nullptr) delete player;
    CloseWindow();
    return 0;
}