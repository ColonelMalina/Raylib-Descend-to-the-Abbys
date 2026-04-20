#include "Locations.h"
#include "Hero.h"
#include "Enemies.h"

std::vector<GameEvent> eventQueue;

void QueueText(std::string text) { eventQueue.push_back({ EV_TEXT, text, {}, Weapon(), 0 }); }
void QueueBattle(std::vector<Enemies*> enemies) { eventQueue.push_back({ EV_BATTLE, "", enemies, Weapon(), 0 }); }
void QueueWeapon(Weapon w) { eventQueue.push_back({ EV_WEAPON_CHEST, "", {}, w, 0 }); }
void QueueAction(EventType type, double amount) { eventQueue.push_back({ type, "", {}, Weapon(), amount }); }

// ================= FLOOR 1: THE NATURAL CAVERNS =================

void bearDen(Hero& player) {
    QueueText("ENTERING: BEAR DEN\nYou hear heavy steps... A Rabid Bear charges!");
    QueueBattle({ new RabidBear() });
    QueueText("Its mate was hiding in the shadows!");
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueBattle({ new RabidBear() });
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueText("A giant centipede drops from the ceiling to scavenge the remains!");
    QueueBattle({ new GiantCentipede() });
    QueueWeapon(SteelSword());
    QueueAction(EV_UPGRADE_MANA, 10);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_ADD_POTION, 1);
}

void undergroundLake(Hero& player) {
    QueueText("ENTERING: UNDERGROUND LAKE\nA massive swarm of cave creatures attacks from the ceiling!");
    QueueBattle({ new CaveBat(), new Rat() });
    QueueAction(EV_HEAL_BANDAGE, 40);
    QueueText("More creatures emerge from the dark water!");
    QueueBattle({ new CaveBat(), new CaveBat(), new Rat() });
    QueueText("You found a hidden blue herb by the water.");
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_FULL_HEAL);
    QueueAction(EV_REGAIN_MANA);
    QueueWeapon(Dagger());
}

void crystalGrotto(Hero& player) {
    QueueText("ENTERING: CRYSTAL GROTTO\nTwo Giant Centipedes are guarding the glowing crystals!");
    QueueBattle({ new GiantCentipede(), new GiantCentipede() });
    QueueText("A Cave Bat, disturbed by the fight, swoops in!");
    QueueBattle({ new CaveBat() });
    QueueAction(EV_UPGRADE_MANA, 5);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_ADD_POTION, 2);
}

void steepDescent(Hero& player) {
    QueueText("ENTERING: STEEP DESCENT\nA Goblin and his rat companion block the tunnel down!");
    QueueBattle({ new Goblin(), new Rat() });
    QueueText("Another Goblin jumps down from a ledge to avenge them!");
    QueueBattle({ new Goblin() });
    QueueText("You found Dagger dropped by the Goblin.");
    QueueWeapon(Dagger());
}

// ================= FLOOR 2: THE GREAT CHASM =================

void ricketyWoodBridge(Hero& player) {
    QueueText("ENTERING: RICKETY BRIDGE\nHarpies swoop down as you try to cross!");
    QueueBattle({ new ChasmHarpy() });
    QueueText("Two more Harpies dive from the darkness!");
    QueueAction(EV_ADD_POTION, 1);
    QueueBattle({ new ChasmHarpy(), new ChasmHarpy() });
    QueueWeapon(SteelSword());
    QueueAction(EV_ADD_POTION, 1);
}

void stoneOverpass(Hero& player) {
    QueueText("ENTERING: STONE OVERPASS\nA Bridge Troll roars, flanked by two Goblins!");
    QueueBattle({ new BridgeTroll(), new Goblin(), new Goblin() });
    QueueText("More Goblins rush from the other side of the bridge!");
    QueueBattle({ new Goblin(), new ShamGob() });
    QueueText("You absorbed the essence of the Fire Goblin and learned FIREBALL!");
    QueueWeapon(HeavyMace());
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_LEARN_FIREBALL);
}

void hiddenFord(Hero& player) {
    QueueText("ENTERING: HIDDEN FORD\nThe water boils. Three Giant Centipedes emerge!");
    QueueBattle({ new GiantCentipede(), new GiantCentipede(), new GiantCentipede() });
    QueueText("The water boils again... Two more arrive!");
    QueueAction(EV_ADD_POTION, 1);
    QueueBattle({ new GiantCentipede(), new GiantCentipede() });
    QueueText("You found a rusty Orish axe in the water.");
    QueueWeapon(OrcAxe());
    QueueAction(EV_UPGRADE_MANA, 10);
    QueueAction(EV_FULL_HEAL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_ADD_POTION, 1);
}

// ================= FLOOR 3: THE FORGOTTEN RUINS =================

void ruinedAltar(Hero& player) {
    QueueText("ENTERING: RUINED ALTAR\nThree suits of Animated Armor wake from their slumber!");
    QueueBattle({ new AnimatedArmor(), new AnimatedArmor() });
    QueueText("The final, massive armor steps forward!");
    QueueAction(EV_ADD_POTION, 1);
    QueueBattle({ new AnimatedArmor() });
    QueueText("The altar grants you the more knowledge for FIREBALL!");
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_REGAIN_MANA);
}

void illusionCorridor(Hero& player) {
    QueueText("ENTERING: ILLUSION CORRIDOR\nThe shadows twist into four Arcane Wraiths!");
    QueueBattle({ new ArcaneWraith(), new ArcaneWraith() });
    QueueText("Two more Wraiths materialize from the walls!");
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueBattle({ new ArcaneWraith(), new ArcaneWraith() });
    QueueText("You absorbed the essence from wraiths and learned ICESPIKE!");
    QueueWeapon(Edagger());
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_LEARN_ICE);
}

void alchemyLab(Hero& player) {
    QueueText("ENTERING: ALCHEMY LAB\nAn Orc is looting the lab, protected by Wraiths!");
    QueueBattle({ new ArcaneWraith(), new ArcaneWraith() });
    QueueText("You see some bandages on the table.\nThe Orc charges at you with full force!");
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueAction(EV_ADD_POTION, 1);
    QueueBattle({ new Orc() });
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_UPGRADE_MANA, 15);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_FULL_HEAL);
}

void glowingPortal(Hero& player) {
    QueueText("ENTERING: GLOWING PORTAL\nA massive Goblin squad guards the portal!");
    QueueBattle({ new Goblin(), new Goblin(), new Goblin() });
    QueueText("The Orc Commander steps through the portal to face you!");
    QueueAction(EV_ADD_POTION, 1);
    QueueBattle({ new Orc() });
    QueueWeapon(OrcAxe());
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_REGAIN_MANA);
}

// ================= FLOOR 4: THE CORRUPTED DEPTHS =================

void bloodRiver(Hero& player) {
    QueueText("ENTERING: BLOOD RIVER\nA pack of Blood Fiends is drinking from the river!");
    QueueBattle({ new BloodFiend(), new BloodFiend() });
    QueueText("More Fiends crawl out of the bloody water!");
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueBattle({ new BloodFiend(), new BloodFiend() });
    QueueWeapon(Elvensword());
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_REGAIN_MANA);
}

void fleshWallCorridor(Hero& player) {
    QueueText("ENTERING: FLESH WALL\nA Flesh Golem bursts from the wall, releasing a swarm of rats!");
    QueueBattle({ new Rat(), new Rat(), new Rat() });
    QueueText("The Flesh Golem finally breaks free and attacks!");
    QueueAction(EV_ADD_POTION, 1);
    QueueBattle({ new FleshGolem(), new BloodFiend() });
    QueueText("Lots of different spell scrolls are lying around...");
    QueueAction(EV_UPGRADE_ICE);
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_FULL_HEAL);
}

void boneCatacombs(Hero& player) {
    QueueText("ENTERING: BONE CATACOMBS\nA Warlock is leading a patrol!");
    QueueBattle({ new Orc(), new Orc() });
    QueueText("The Orc Warlock unleashes his dark magic!");
    QueueBattle({ new Orc2() });
    QueueText("There's a lot of scrools lying around!");
    QueueAction(EV_UPGRADE_ICE);
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_UPGRADE_MANA, 20);
}

void soulPrison(Hero& player) {
    QueueText("ENTERING: SOUL PRISON\nThe prison is heavily guarded by Fiends and a Shaman!");
    QueueBattle({ new BloodFiend(), new BloodFiend() });
    QueueText("The Shaman summons another Fiend to his side!");
    QueueAction(EV_ADD_POTION, 2);
    QueueBattle({ new Orc2(), new BloodFiend() });
    QueueAction(EV_UPGRADE_ICE);
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_REGAIN_MANA);
}

// ================= FLOOR 5: THE GATES OF HELL =================

void flamingPits(Hero& player) {
    QueueText("ENTERING: FLAMING PITS\nA pack of four Hellhounds surrounds you!");
    QueueBattle({ new Hellhound(), new Hellhound() });
    QueueText("Two more leap from the flames!");
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueBattle({ new Hellhound(), new Hellhound() });
    QueueWeapon(BigAxe());
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_ADD_POTION, 2);
}

void demonicStatues(Hero& player) {
    QueueText("ENTERING: DEMONIC STATUES\nThe gargoyles animate, supported by Animated Armors!");
    QueueBattle({ new AnimatedArmor(), new AnimatedArmor() });
    QueueText("The obsidian Gargoyles spread their wings!");
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueBattle({ new ObsidianGargoyle(), new ObsidianGargoyle() });
    QueueWeapon(Halapartna());
    QueueAction(EV_UPGRADE_ICE);
    QueueAction(EV_REGAIN_MANA);
}

void abyssalGorge(Hero& player) {
    QueueText("ENTERING: ABYSSAL GORGE\nA lone Dread Knight stands on the edge... and summons Blood Fiends!");
    QueueBattle({ new BloodFiend(), new BloodFiend(), new BloodFiend() });
    QueueText("The Dread Knight draws his massive sword!");
    QueueBattle({ new DreadKnight() });
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_UPGRADE_ICE);
    QueueAction(EV_FULL_HEAL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_ADD_POTION, 2);
}

void blackGate(Hero& player) {
    QueueText("ENTERING: THE BLACK GATE\nThe elite guard of the Demon Lord will not let you pass!");
    QueueBattle({ new Hellhound(), new Hellhound() });
    QueueText("Two Dread Knights step forward to finish you!");
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueBattle({ new DreadKnight(), new DreadKnight() });
    QueueText("The gate creaks open... The final battle awaits.");
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_FULL_HEAL);
    QueueAction(EV_ADD_POTION, 1);
}

// ================= FLOOR 6: THE FINAL BOSS =================

void bossRoom(Hero& player) {
    QueueText("ENTERING: THE THRONE OF THE ABYSS\nThe Demon Lord's personal guards attack first!");
    QueueBattle({ new ObsidianGargoyle(), new DreadKnight() });
    QueueText("Azazel rises from his throne. 'Enough play...'");
    QueueBattle({ new DemonLord() });
}