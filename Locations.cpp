#include "Locations.h"
#include "Hero.h"
#include "Enemies.h"

std::vector<GameEvent> eventQueue;

void QueueText(std::string text) { eventQueue.push_back({ EV_TEXT, text, {}, Weapon(), 0 }); }
void QueueBattle(std::vector<Enemies*> enemies) { eventQueue.push_back({ EV_BATTLE, "", enemies, Weapon(), 0 }); }
void QueueWeapon(Weapon w) { eventQueue.push_back({ EV_WEAPON_CHEST, "", {}, w, 0 }); }
void QueueAction(EventType type, double amount) { eventQueue.push_back({ type, "", {}, Weapon(), amount }); }

void bearDen(Hero& player) {
    QueueText("ENTERING: BEAR DEN\n");
    QueueText("A giant centipede drops from the ceiling to feast on you!");
    QueueBattle({ new GiantCentipede() });
    QueueAction(EV_HEAL_BANDAGE, 15);
    QueueText("You hear heavy steps... A Bear charges!");
    QueueBattle({ new RabidBear() });
    QueueWeapon(Dagger());
    QueueAction(EV_UPGRADE_MANA, 10);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_HEAL_BANDAGE, 15);
    QueueAction(EV_ADD_POTION, 1);
}

void undergroundLake(Hero& player) {
    QueueText("ENTERING: UNDERGROUND LAKE\nA massive swarm of cave creatures attacks from the crevices!");
    QueueBattle({ new CaveBat(), new Rat() });
    QueueAction(EV_HEAL_BANDAGE, 20);
    QueueText("More creatures emerge from the dark water!");
    QueueBattle({ new CaveBat(), new CaveBat(), new Rat() });
    QueueText("You found a hidden blue herb by the water.");
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_HEAL_BANDAGE, 20);
    QueueAction(EV_REGAIN_MANA);
    QueueWeapon(Dagger());
}

void crystalGrotto(Hero& player) {
    QueueText("ENTERING: CRYSTAL GROTTO\nTwo Giant Centipedes are guarding the glowing crystals!");
    QueueBattle({ new GiantCentipede(), new GiantCentipede() });
    QueueText("A Cave Bat, disturbed by the fight, swoops in!");
    QueueBattle({ new CaveBat() });
    QueueAction(EV_UPGRADE_MANA, 10);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_ADD_POTION, 3);
}

void steepDescent(Hero& player) {
    QueueText("ENTERING: STEEP DESCENT\nA Goblin and his rat companions block the tunnel down!");
    QueueBattle({ new Goblin(), new Rat(), new Rat()});
    QueueAction(EV_HEAL_BANDAGE, 25);
    QueueText("Another Goblins jumps down from a ledge to avenge them!");
    QueueBattle({ new Goblin(), new Goblin() });
    QueueText("You found Dagger dropped by the Goblin.");
    QueueWeapon(Scikle());
}

void ricketyWoodBridge(Hero& player) {
    QueueText("ENTERING: RICKETY BRIDGE\nHarpies swoop down as you try to cross!");
    QueueBattle({ new ChasmHarpy() });
    QueueText("Two more Harpies dive from the darkness!");
    QueueAction(EV_ADD_POTION, 1);
    QueueBattle({ new ChasmHarpy(), new ChasmHarpy() });
    QueueWeapon(SteelSword());
    QueueAction(EV_ADD_POTION, 2);
}

void stoneOverpass(Hero& player) {
    QueueText("ENTERING: STONE OVERPASS\nA Bridge Troll roars, flanked by two Goblins!");
    QueueBattle({ new BridgeTroll(), new Goblin(), new Goblin() });
    QueueText("More Goblins rush from the other side of the bridge!");
    QueueBattle({ new Goblin(), new ShamGob() });
    QueueText("You absorbed the essence of the Fire Goblin!");
    QueueAction(EV_LEARN_FIREBALL);
    QueueWeapon(HeavyMace());
    QueueAction(EV_HEAL_BANDAGE, 20);
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_REGAIN_MANA);
    
}

void hiddenFord(Hero& player) {
    QueueText("ENTERING: HIDDEN FORD\nThe water boils. Two Giant Centipedes emerge!");
    QueueBattle({ new GiantCentipede(), new GiantCentipede()});
    QueueAction(EV_HEAL_BANDAGE, 15);
    QueueAction(EV_ADD_POTION, 1);
    QueueText("The water boils again... Three more arrive!");
    QueueBattle({ new GiantCentipede(), new GiantCentipede(),new GiantCentipede() });
    QueueText("You found a rusty Orish axe in the water.");
    QueueWeapon(OrcAxe());
    QueueAction(EV_UPGRADE_MANA, 10);
    QueueAction(EV_FULL_HEAL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_ADD_POTION, 2);
}

void ruinedAltar(Hero& player) {
    QueueText("ENTERING: RUINED ALTAR\nThree suits of Animated Armor wake from their slumber!");
    QueueBattle({ new AnimatedArmor(), new AnimatedArmor() });
    QueueText("The final, massive armor steps forward!");
    QueueAction(EV_ADD_POTION, 1);
    QueueBattle({ new PAnimatedArmor() });
    QueueText("The altar grants you the more knowledge for FIREBALL!");
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_REGAIN_MANA);
}

void illusionCorridor(Hero& player) {
    QueueText("ENTERING: ILLUSION CORRIDOR\nThe shadows twist into four Arcane Wraiths!");
    QueueBattle({ new ArcaneWraith(), new ArcaneWraith() });
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueText("Two more Wraiths materialize from the walls!");
    QueueBattle({ new ArcaneWraith(), new ArcaneWraith() });
    QueueText("You absorbed the essence from wraiths!");
    QueueWeapon(Edagger());
    QueueAction(EV_ADD_POTION, 1);
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_LEARN_ICE);
}

void alchemyLab(Hero& player) {
    QueueText("ENTERING: ALCHEMY LAB\nAn Orc is looting the lab, protected by Wraiths!");
    QueueBattle({ new ArcaneWraith(), new ArcaneWraith() });
    QueueText("You see some bandages on the table.");
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueAction(EV_ADD_POTION, 1);
    QueueText("The Orc charges at you with full force!");
    QueueBattle({ new Orc() });
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_UPGRADE_MANA, 15);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_FULL_HEAL);
}

void glowingPortal(Hero& player) {
    QueueText("ENTERING: GLOWING PORTAL\nA massive Goblin squad guards the portal!");
    QueueBattle({ new Goblin(), new Goblin(), new Goblin() });
    QueueAction(EV_ADD_POTION, 2);
    QueueText("The Orc Commander steps through the portal to face you!");
    QueueBattle({ new COrc() });
    QueueWeapon(OrcAxe());
    QueueAction(EV_UPGRADE_FIREBALL);
    QueueAction(EV_REGAIN_MANA);
}

void bloodRiver(Hero& player) {
    QueueText("ENTERING: BLOOD RIVER\nA pack of Blood Fiends is drinking from the river!");
    QueueBattle({ new BloodFiend(), new BloodFiend() });
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueText("More Fiends crawl out of the bloody water!");
    QueueBattle({ new BloodFiend(), new BloodFiend() });
    QueueWeapon(Elvensword());
    QueueAction(EV_ADD_POTION, 2);
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_REGAIN_MANA);
}

void fleshWallCorridor(Hero& player) {
    QueueText("ENTERING: FLESH WALL\nA Flesh Golem bursts from the wall, releasing a swarm of rats!");
    QueueBattle({ new Rat(), new Rat(), new Rat(), new Rat(), new Rat()});
    QueueAction(EV_ADD_POTION, 2);
    QueueText("There's heavy looking axe on the floor");
    QueueWeapon(EAxe());
    QueueText("The Flesh Golem finally breaks free and attacks!");
    QueueBattle({ new FleshGolem(), new Rat(), new Rat() });
    QueueText("Lots of different spell scrolls are lying around...");
    QueueAction(EV_UPGRADE_ICE);
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_REGAIN_MANA);
    QueueAction(EV_FULL_HEAL);
}

void boneCatacombs(Hero& player) {
    QueueText("ENTERING: BONE CATACOMBS\nA Warlock is leading a patrol!");
    QueueBattle({ new Orc(), new Orc() });
    QueueAction(EV_ADD_POTION, 1);
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
    QueueWeapon(BigAxe());
    QueueAction(EV_UPGRADE_ICE);
    QueueAction(EV_UPGRADE_HEAL);
    QueueAction(EV_REGAIN_MANA);
}

void flamingPits(Hero& player) {
    QueueText("ENTERING: FLAMING PITS\nA pack of four Hellhounds surrounds you!");
    QueueBattle({ new Hellhound(), new Hellhound() });
    QueueText("Two more leap from the flames!");
    QueueAction(EV_HEAL_BANDAGE, 30);
    QueueBattle({ new Hellhound(), new Hellhound() });
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
    QueueWeapon(Hammer());
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
    QueueAction(EV_ADD_POTION, 2);
}

void bossRoom(Hero& player) {
    QueueText("ENTERING: THE THRONE OF THE ABYSS\nThe Demon Lord's personal guards attack first!");
    QueueBattle({ new ObsidianGargoyle(), new DreadKnight() });
    QueueAction(EV_ADD_POTION, 1);
    QueueText("Azazel rises from his throne. 'Enough play...'");
    QueueBattle({ new DemonLord() });
}