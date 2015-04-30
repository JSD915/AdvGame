#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <Room.h>
#include <Player.h>
#include <Item.h>
#include <Effect.h>
#include <Entity.h>
#include <climits>
#include <stdlib.h>
#include <time.h>


int detail = 1;
Room *currentRoom = NULL;
Room *prevRoom = NULL;
Room *bossRoom = NULL;
Room *exitRoom = NULL;
Player *player = NULL;
Player *savePlayer = NULL;
Room *saveRoom = NULL;
bool actionTaken = false;
bool changedRoom = false;
bool bossDefeated = false;
std::string treasures[][2] = {{"pile of gold coins", ""},{"red gemstone", ""},{"bronze amulet",""},{"metallic ring", ""},{"small gold bar",""},
                             {"jade figurine", ""},{"silver figurine", ""},{"gold figurine", ""}, {"gold rings", ""},{"blue gemstone", ""}}; //TODO add descrips
std::string colors[] = { "red","blue","green","black","white","orange","gray" };
std::string adjs[] = { "cloudy","pulsing","bubbling","viscous","cold","hot","murky","clear" };
std::string p_descrip = ", its properties are unknown to you.";

std::vector<Effect*> getEffects(ItemType::E type, int power) {
    std::vector<Effect*> effects;
    if (type == ItemType::DECOR || type == ItemType::TREASURE || type == ItemType::USABLE){
        return effects;
    }
    switch (type) {
    case ItemType::ARMOR:
        effects.push_back(new Effect(power + 1, -1, EffectType::ARMOR, true));
        break;
    case WEAPON:
        effects.push_back(new Effect(((power + 1) * 3 / 2), -1, EffectType::ATTACK, true));
        break;
    case CONSUMABLE:
        effects.push_back(new Effect((power + 1) * 10, power + 1, static_cast<EffectType::E>(rand() % 14)));
        break;
    }
    for (int i = 0; i < power; i++) {
        if (type == ItemType::ARMOR) {
            effects.push_back(new Effect(power + 1, -1, static_cast<EffectType::E>(rand() % 9)));
        }
        if (type == ItemType::WEAPON) {
            int rnd = rand() % 13;
            if (rnd != 0) rnd += 2;
            effects.push_back(new Effect(power + 1, -1, static_cast<EffectType::E>(rnd)));
        }
        if (type == ItemType::CONSUMABLE) {
            effects.push_back(new Effect((power + 1) * 10, power + 1, static_cast<EffectType::E>(rand() % 15)));
        }
    }
    return effects;
}

void connectRooms(Room *rooms[14][14][4]) {
    for (int r = 0; r < 14; r++) {
        for (int c = 0; c < 14; c++) {
            for (int f = 0; f < 4; f++) {
                if (rooms[r][c][f] != NULL) {
                    if (r != 0) {
                        rooms[r][c][f]->assignDirection(Direction::NORTH, rooms[r - 1][c][f]);
                    }
                    if (r != 13) {
                        rooms[r][c][f]->assignDirection(Direction::SOUTH, rooms[r + 1][c][f]);
                    }
                    if (c != 0) {
                        rooms[r][c][f]->assignDirection(Direction::WEST, rooms[r][c - 1][f]);
                    }
                    if (c != 13) {
                        rooms[r][c][f]->assignDirection(Direction::EAST, rooms[r][c + 1][f]);
                    }
                }
            }
        }
    }
}

void buildWorld() {
    Room *rooms[14][14][4]; //[row][column][floor]
    for (int r = 0; r < 14; r++) {
        for (int c = 0; c < 14; c++) {
            for (int f = 0; f < 4; f++) {
                rooms[r][c][f] = NULL;
            }
        }
    }
    std::vector<Item*> noItems;
    std::vector<Entity*> noEntities;
    std::vector<Effect*> noEffects;
    std::vector<EffectType::E> noRes;
    std::vector<Room*> noRooms;
    ////// FLOOR ! //////
    rooms[13][6][0] = new Room(true, noEntities, noItems, "Cave Entrance",
        "You stand in a small cave entrance. You stand on a crumbling rock floor. The walls are moist and covered in moss. You can see light flowing in from the hole above you", noItems);
    rooms[12][6][0] = new Room(true, noEntities, noItems, "Carved Rock Hallway",
        "You continue through a circular rock tunnel. It was carved perfectly. The walls grow moss and large cobwebs span its diameter.", noItems);

    rooms[11][6][0] = new Room(true, noEntities, noItems, "Ancient Ornate Galley", "You are in a large gallery. The wall in front of you shows a beautiful mosaic.", noItems);
    rooms[11][6][0]->addItem(new Item(noEffects, "mosaic", " depicts a black figure surrounded by a halo of light flying at great speed towards a mountain.", INT_MAX, ItemType::DECOR, noItems, noRooms));
    rooms[11][5][0] = new Room(true, noEntities, noItems, "Small Closet", "You stand in the entranceway to a small dusty room.", noItems);
    rooms[11][5][0]->addItem(new Item(noEffects, "rotten woven basket", " looks very old. It appears to be useless.", 2, ItemType::DECOR, noItems, noRooms));
    rooms[11][5][0]->addItem(new Item(getEffects(ItemType::WEAPON, 0), "crude rusty dagger", " looks very old. It has a leather grip.", 1, ItemType::WEAPON, noItems, noRooms));
    rooms[11][7][0] = new Room(true, noEntities, noItems, "Divided stair", "A long wooden staircase leads down. It splits in two directions.", noItems);
    rooms[11][8][0] = new Room(true, noEntities, noItems, "Living Quarters East",
        "This is a room clear intended to sleep many. There are several dozen piles of rotten grass that were most likely used for beds.", noItems);
    int rnd = rand() % 10;
    rooms[11][8][0]->addItem(new Item(noEffects, treasures[rnd][0], treasures[rnd][1], 0, ItemType::TREASURE, noItems, noRooms));
    std::vector<Entity*> eR12_8_0;
    std::vector<Item*> iR12_8_0;
    iR12_8_0.push_back(new Item(noEffects, "claws", ", how do you have this?", 0, ItemType::WEAPON, noItems, noRooms));
    eR12_8_0.push_back(new Entity(100, 100, 100, 100, 0, iR12_8_0, false, "brown bear", "a very large male brown bear.", true, false, true, noRes));
    eR12_8_0.at(0)->equip(iR12_8_0.at(0));
    rooms[12][8][0] = new Room(true, eR12_8_0, noItems, "Cave exit", "A long natural cave lead to the bright light of day.", noItems);
    rooms[10][7][0] = new Room(true, noEntities, noItems, "Living Quarters North",
        "This is a room clear intended to sleep many. There are several dozen piles of rotten grass that were most likely used for beds.", noItems);
    //Item *finalKey = new Item(noEffects, "old rusted key", ", it's use is a mystery.", 0, ItemType::USABLE, noItems, noRooms); //ad room later
    rooms[10][8][0] = new Room(true, noEntities, noItems, "Grand Hall South",
        "You stand at the end of a large ornate hall. Massive pillars stretch up to the ceiling. Thick granite slabs line the floor. The hall stretches far in front of you.", noItems);
    rooms[9][8][0] = new Room(true, noEntities, noItems, "Grand Hall", "The grand hall continues onward. It is very consistent and symmetrical.", noItems);
    rooms[8][8][0] = new Room(true, noEntities, noItems, "Grand Hall North", "The end of the hall houses a very large, open, wooden door.", noItems);
    std::vector<Item*> iR8_7_0;
    int clr = rand() % 7;
    int adj = rand() % 8;
    iR8_7_0.push_back(new Item(getEffects(ItemType::CONSUMABLE, 0), adjs[adj] + " " + colors[clr] + " potion", p_descrip, 1, ItemType::CONSUMABLE, noItems, noRooms));
    rooms[8][7][0] = new Room(true, noEntities, iR8_7_0, "The Kitchen", "The rooms appears to be a kitchen but there are no signs of any food, just empty bottles.", noItems);
    rooms[8][9][0] = new Room(true, noEntities, noItems, "Hole in the wall", "A large section of wall has broken off exposing a hole in the ground.", noItems);
    Room *deathDrop = new Room(true, noEntities, noItems, "A large drop.", "You fall for several seconds and die in a jagged rock pit.", noItems, true);
    rooms[8][9][0]->assignDirection(Direction::DOWN, deathDrop);
    iR8_7_0.clear();
    iR8_7_0.push_back(new Item(noEffects, "throne", ", it is made of yellow and gray metals. Nothing and no one is in it.", 1000, ItemType::DECOR, noItems, noRooms));
    rooms[7][8][0] = new Room(true, noEntities, iR8_7_0, "Throne Room", "You enter a large throne room. There are human remains scattered around the room and are piled mostly around the large throne at the rooms end. You notice behind the throne there is a trap door.", noItems);

    ///// FLOOR 2 /////

    std::vector<Item*> items;
    std::vector<Entity*> mobs;
    items.push_back(new Item(getEffects(ItemType::CONSUMABLE, 0), "red pile of ooze", ", its questionable as to whether you should eat this.", 1, ItemType::CONSUMABLE, noItems, noRooms));
    Entity *ooze = new Entity(10, 5, 0, 25, 0, items, false, "ooze monster", " a weird ooze monster. It seems to have no formal structure.", true, false, true, noRes);
    items.clear();
    items.push_back(new Item(getEffects(ItemType::ARMOR, 1), "black cloak", ", its looks like it could help to protect you.", 3, ItemType::ARMOR, noItems, noRooms));
    rooms[7][8][1] = new Room(true, noEntities, items, "Ladder up", "You stand on an intricate metallic surface. It slightly compresses under your feet. There are tunnels that run above it.", noItems);
    rooms[7][8][0]->assignDirection(Direction::DOWN, rooms[7][8][1]);
    rooms[7][8][1]->assignDirection(Direction::UP, rooms[7][8][0]);
    items.clear();
    rooms[6][8][1] = new Room(false, noEntities, noItems, "Surface entrance", "This room is connected to one with a ladder downward.", noItems);
    mobs.push_back(ooze);
    rooms[8][8][1] = new Room(true, mobs, noItems, "Hallway", "One of the tunnels through the rock.", noItems);
    mobs.clear();
    items.push_back(new Item(getEffects(ItemType::WEAPON, 10), "knights sword", ", a very deadly weapon", 10, ItemType::WEAPON, noItems, noRooms));
    mobs.push_back(new Entity(1000, 1000, 100, 100, 100, items, false, "black knight", "black knight, a mysterious sight for these areas. It is strangely familiar.", true, false, false, noRes));
    items.clear();
    rooms[9][8][1] = new Room(true, mobs, noItems, "Hallway", "One of the tunnels through the rock.", noItems);
    mobs.clear();
    rooms[10][8][1] = new Room(true, noEntities, noItems, "Crossroads", "This tunnel has ended. It splits in two directions. To the west there is a metallic door.", noItems);
    rooms[10][7][1] = new Room(true, noEntities, noItems, "Death from above", "As you entire the door seals behind you. Spikes slowly descend from the ceiling and impale you.", noItems, true);
    rooms[10][9][1] = new Room(true, noEntities, noItems, "Hallway", "One of the tunnels through the rock.", noItems);
    items.push_back(new Item(getEffects(ItemType::CONSUMABLE, 0), "blue pile of ooze", ", its questionable as to whether you should eat this.", 1, ItemType::CONSUMABLE, noItems, noRooms));
    ooze = new Entity(10, 5, 0, 25, 0, items, false, "ooze monster", " a weird ooze monster. It seems to have no formal structure.", true, false, true, noRes);
    items.clear();
    mobs.push_back(ooze);
    rooms[10][10][1] = new Room(true, mobs, noItems, "Hallway", "One of the tunnels through the rock.", noItems);
    rooms[10][11][1] = new Room(true, noEntities, noItems, "Crossroads", "Two tunnels stop and meet here.", noItems);
    mobs.clear();
    rooms[9][11][1] = new Room(true, noEntities, noItems, "Hallway", "One of the tunnels through the rock. There is a strange room off to the east.", noItems);
    rnd = rand() % 10;
    items.push_back(new Item(noEffects, treasures[rnd][0], treasures[rnd][1], 0, ItemType::TREASURE, noItems, noRooms));
    rnd = rand() % 10;
    items.push_back(new Item(noEffects, treasures[rnd][0], treasures[rnd][1], 0, ItemType::TREASURE, noItems, noRooms));
    rooms[9][12][1] = new Room(true, noEntities, items, "Small treasure room", "This clearly used to hold more treasure. Most of it has been removed.", noItems);
    items.clear();
    clr = rand() % 7;
    adj = rand() % 8;
    items.push_back(new Item(getEffects(ItemType::CONSUMABLE, 3), adjs[adj] + " " + colors[clr] + " potion", p_descrip, 1, ItemType::CONSUMABLE, noItems, noRooms));
    clr = rand() % 7;
    adj = rand() % 8;
    items.push_back(new Item(getEffects(ItemType::CONSUMABLE, 3), adjs[adj] + " " + colors[clr] + " potion", p_descrip, 1, ItemType::CONSUMABLE, noItems, noRooms));
    rooms[8][11][1] = new Room(true, noEntities, items, "Potion room", "Like the kitchen from the area above, this room is filled with glass bottles, most of which are empty.", noItems);
    items.clear();
    std::vector<Effect*> effects;
    effects.push_back(new Effect(10, -1, EffectType::ATTACK));
    items.push_back(new Item(effects, "pickaxe", ", a pickaxe that was used in an attempt to break through the metallic surface.", 5, ItemType::WEAPON, noItems, noRooms));
    rooms[8][10][1] = new Room(true, noEntities, items, "mining room", "one of the last tunnels mined in this area. some broken pickaxes lie around the room. who ever was here was attempting to get through the metallic surface.", noItems);
    items.clear();
    std::vector<Room*> inRoom;
    inRoom.push_back(rooms[6][8][1]);
    items.push_back(new Item(noEffects, "metallic key", ", its use is unknown.", 0, ItemType::USABLE, noItems, inRoom));
    inRoom.clear();
    mobs.push_back(new Entity(30, 6, 2, 40, 20, items, false, "large ooze", "a large ooze. This one is clearly bigger and more powerful.", true, false, true, noRes));
    rooms[7][10][1] = new Room(true, mobs, noItems, "Ooze chamber", "The was are covered in multicolored ooze. Prepare for a fight!", noItems);
    rooms[5][8][1] = new Room(true, noEntities, noItems, "Porthole downwards", "Here is the entrance through the metallic surface.", noItems);
    mobs.clear();

    ///// FLOOR 3 /////

    rooms[5][8][2] = new Room(true, noEntities, noItems, "Facility entrance", "The room you stand in has tons of intricate circuitry. All the surfaces have thin lay of a black grease like substance", noItems);
    rooms[5][8][1]->assignDirection(Direction::DOWN, rooms[5][8][2]);
    rooms[5][8][2]->assignDirection(Direction::UP, rooms[5][8][1]);

    Entity *sentry = new Entity(30, 10, 10, 50, 20, noItems, false, "sentry", "a sentry stands ideal, seemingly uninterested in your presence.", true, false, false, noRes);

    mobs.push_back(sentry);
    rooms[2][6][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[2][9][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[2][10][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[3][6][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[3][7][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[3][8][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[3][9][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[3][10][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[3][11][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[4][6][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[4][7][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[4][8][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[4][11][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[5][6][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[5][7][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[5][9][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[5][10][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[5][11][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[6][7][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[6][8][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[6][9][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[7][7][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
    rooms[7][8][2] = new Room(true, noEntities, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics. There is a sign for the self labeled control facility.", noItems);
    rooms[7][9][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);

    rooms[8][8][2] = new Room(false, noEntities, noItems, "Security Room 1", "2 levels remaining.", noItems);
    rooms[9][8][2] = new Room(false, noEntities, noItems, "Security Room 2", "1 level remaining.", noItems);
    rooms[10][8][2] = new Room(false, noEntities, noItems, "Security Room 3", "Access to control facility granted", noItems);
    inRoom.clear();
    inRoom.push_back(rooms[10][8][2]);
    Item *key3= new Item(noEffects, "key card 3", ", opens an access door.", 0, ItemType::USABLE, noItems, inRoom);
    items.push_back(key3);
    sentry = new Entity(20, 10, 10, 50, 20, items, false, "sentry", "a sentry stands ideal, seemingly uninterested in your presence.", true, false, false, noRes);
    mobs.clear();
    mobs.push_back(sentry);
    switch(rand() % 6) {
        case 0:
            rooms[3][9][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
            break;
        case 1:
            rooms[4][7][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
            break;
        case 2:
            rooms[4][11][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
            break;
        case 3:
            rooms[5][9][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
            break;
        case 4:
            rooms[6][7][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
            break;
        case 5:
            rooms[7][9][2] = new Room(true, mobs, noItems, "Circuit room", "All the walls are lined with ultra-complex electronics", noItems);
            break;
    }
    mobs.clear();

    rooms[2][5][2] = new Room(true, noEntities, noItems, "Facility Exit", "You find a ladder leading up out of the strange facility.", noItems);
    rooms[5][5][2] = new Room(true, noEntities, noItems, "Facility Exit", "You find a ladder leading up out of the strange facility.", noItems);
    rooms[1][10][2] = new Room(true, noEntities, noItems, "Facility Exit", "You find a ladder leading up out of the strange facility.", noItems);
    rooms[6][11][2] = new Room(true, noEntities, noItems, "Facility Exit", "You find a ladder leading up out of the strange facility.", noItems);
    rooms[3][12][2] = new Room(true, noEntities, noItems, "Facility Exit", "You find a ladder leading up out of the strange facility.", noItems);
    Room *roomUp2_5_2 = new Room(true, noEntities, noItems, "Treasure room", "A room that has some treasures. A nice find", noItems);
    Room *roomUp5_5_2 = new Room(true, noEntities, noItems, "Security room", "This room house high tech security equipment.", noItems);
    Room *roomUp1_10_2 = new Room(true, noEntities, noItems, "Security room", "This room house high tech security equipment.", noItems);
    Room *roomUp6_11_2 = new Room(true, noEntities, noItems, "Weapon closet", "You find a room intended from holding various weapons", noItems);
    Room *roomUp3_12_2 = new Room(true, noEntities, noItems, "Sentry hub", "As soon as you enter the room, sentries swarm you.", noItems, true);
    inRoom.clear();
    inRoom.push_back(rooms[8][8][2]);
    Item *key1 = new Item(noEffects, "key card 1", ", opens an access door.", 0, ItemType::USABLE, noItems, inRoom);
    roomUp1_10_2->addItem(key1);
    inRoom.clear();
    inRoom.push_back(rooms[9][8][2]);
    Item *key2 = new Item(noEffects, "key card 2", ", opens an access door.", 0, ItemType::USABLE, noItems, inRoom);
    roomUp5_5_2->addItem(key2);
    inRoom.clear();
    roomUp6_11_2->addItem(new Item(getEffects(ItemType::WEAPON, 5), "electronic sword", ", beneath the clear blade, you can complex circuitry and wiring.", 5, ItemType::WEAPON, noItems, noRooms));
    roomUp6_11_2->addItem(new Item(getEffects(ItemType::ARMOR, 5), "electronic armor", ", this powered armor looks protective.", 10, ItemType::ARMOR, noItems, noRooms));
    for (int i = 0; i < 3; i++) {
        rnd = rand() % 10;
        roomUp2_5_2->addItem(new Item(noEffects, treasures[rnd][0], treasures[rnd][1], 0, ItemType::TREASURE, noItems, noRooms));
    }
    rooms[2][5][2]->assignDirection(Direction::UP, roomUp2_5_2);
    rooms[5][5][2]->assignDirection(Direction::UP, roomUp5_5_2);
    rooms[1][10][2]->assignDirection(Direction::UP, roomUp1_10_2);
    rooms[6][11][2]->assignDirection(Direction::UP, roomUp6_11_2);
    rooms[3][12][2]->assignDirection(Direction::UP, roomUp3_12_2);
    roomUp2_5_2->assignDirection(Direction::DOWN, rooms[2][5][2]);
    roomUp5_5_2->assignDirection(Direction::DOWN, rooms[5][5][2]);
    roomUp1_10_2->assignDirection(Direction::DOWN, rooms[1][10][2]);
    roomUp6_11_2->assignDirection(Direction::DOWN, rooms[6][11][2]);
    roomUp3_12_2->assignDirection(Direction::DOWN, rooms[3][12][2]);

    ///// FLOOR 4 /////

    rooms[10][8][3] = new Room(true, noEntities, noItems, "System control center entrance", "You have reached what you think is the heart of whatever this underground structure is.", noItems);
    rooms[10][8][3]->assignDirection(Direction::UP, rooms[10][8][2]);
    rooms[10][8][2]->assignDirection(Direction::DOWN, rooms[10][8][3]);
    mobs.clear();
    sentry = new Entity(20, 10, 10, 50, 20, items, false, "sentry", "a sentry is on full alert. it seems you may have tripped an alarm.", true, false, true, noRes);
    mobs.push_back(sentry);
    rooms[10][7][3] = new Room(true, mobs, noItems, "Access tunnel", "You are close to the control hub.", noItems);
    rooms[9][7][3] = new Room(true, noEntities, noItems, "Access tunnel", "You are close to the control hub.", noItems);
    items.clear();

    items.push_back(new Item(getEffects(ItemType::CONSUMABLE, 4), adjs[rand() % 8] + " " + colors[rand() % 7] + " potion", p_descrip, 1, ItemType::CONSUMABLE, noItems, noRooms));
    items.push_back(new Item(getEffects(ItemType::CONSUMABLE, 4), adjs[rand() % 8] + " " + colors[rand() % 7] + " potion", p_descrip, 1, ItemType::CONSUMABLE, noItems, noRooms));
    items.push_back(new Item(getEffects(ItemType::CONSUMABLE, 4), adjs[rand() % 8] + " " + colors[rand() % 7] + " potion", p_descrip, 1, ItemType::CONSUMABLE, noItems, noRooms));
    rooms[8][6][2] = new Room(false, noEntities, items, "Supply closet", "You find a small closet with supplies.", noItems);
    mobs.clear();
    inRoom.push_back(rooms[8][6][2]);
    Item *closet_key = new Item(noEffects, "closet key card", ", seems to be for a closet.", 0, ItemType::USABLE, noItems, inRoom);
    items.clear();
    items.push_back(closet_key);
    mobs.push_back(new Entity(20, 10, 10, 50, 20, items, false, "sentry 1", "a sentry is on full alert. it seems you may have tripped an alarm.", true, false, true, noRes));
    mobs.push_back(new Entity(20, 10, 10, 50, 20, noItems, false, "sentry 2", "a sentry is on full alert. it seems you may have tripped an alarm.", true, false, true, noRes));
    rooms[8][7][3] = new Room(true, mobs, noItems, "Access tunnel", "You are close to the control hub.", noItems);
    rooms[7][7][3] = new Room(true, noEntities, noItems, "Access tunnel", "You are close to the control hub.", noItems);
    items.clear();
    rooms[6][7][3] = new Room(false, noEntities, noItems, "Control Room", "You reach the heart of the facility. Monitors and control panels are all over the room.", noItems);
    rooms[5][7][3] = new Room(true, noEntities, noItems, "Exit shoot", "You have made, a way out. A long ladder leads up.", noItems);
    exitRoom = new Room(true, noEntities, noItems, "Surface access", "The ladder leads to the center of a valley. It is dark now, time to head home.", noItems);
    rooms[5][7][3]->assignDirection(Direction::UP, exitRoom);
    bossRoom = rooms[6][7][3];

    connectRooms(rooms);
    currentRoom = rooms[5][8][2];
}

Player* defaultPlayer() {
    std::vector<Item*> inv;
    std::vector<EffectType::E> res;
    return new Player(50, 7, 3, 40, 10, inv, true, "Hiker", "You are a hiker hiking somewhere in the rocky mountains.", true, false, false, res, 50);
}

Player* cheatPlayer() {
    std::vector<Item*> noItems;
    std::vector<Room*> noRooms;
    std::vector<Effect*> noEffects;
    std::vector<Item*> cheatItems;
    std::vector<Effect*> effects;
    std::vector<EffectType::E> res;
    effects.push_back(new Effect(1, -1, EffectType::IMMUNITY));
    cheatItems.push_back(new Item(effects, "armor of the gods", ", if thats how you wan to play.", 0, ItemType::ARMOR, noItems, noRooms));
    effects.clear();
    effects.push_back(new Effect(10000, -1, EffectType::ATTACK));
    cheatItems.push_back(new Item(effects, "sword of the gods", ", if that's how you want to play...", 0, ItemType::WEAPON, noItems, noRooms));
    return new Player(10000, 10000, 10000, 100, 100, cheatItems, true, "Cheater", "Someone who is lazy.", true, false, false, res, 10000);
}

void examine(std::string target) {
    target = target.substr(0, target.size() - 1);
    int possibilities = 0;
    if (target.compare("me") == 0) {
        std::cout << player->getDescription() << std::endl;
        return;
    }
    if (target.compare("room") == 0) {
        std::cout << currentRoom->getDescription() << std::endl;
        return;
    }
    for (int i = 0; i < player->getItems().size(); i++) {
        if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
            possibilities++;
        }
    }
    for (int i = 0; i < currentRoom->getItems().size(); i++) {
        if (currentRoom->getItems().at(i)->getName().find(target) != currentRoom->getItems().at(i)->getName().npos) {
            possibilities++;
        }
    }
    for (int i = 0; i < currentRoom->getEntities().size(); i++) {
        if (currentRoom->getEntities().at(i)->getName().find(target) != currentRoom->getEntities().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << target << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << target << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < player->getItems().size(); i++) {
            if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
                std::cout << player->getItems().at(i)->getDescription() << std::endl;
            }
        }
        for (int i = 0; i < currentRoom->getItems().size(); i++) {
            if (currentRoom->getItems().at(i)->getName().find(target) != currentRoom->getItems().at(i)->getName().npos) {
                std::cout << currentRoom->getItems().at(i)->getDescription() << std::endl;
            }
        }
        for (int i = 0; i < currentRoom->getEntities().size(); i++) {
            if (currentRoom->getEntities().at(i)->getName().find(target) != currentRoom->getEntities().at(i)->getName().npos) {
                std::cout << currentRoom->getEntities().at(i)->getDescription() << std::endl;
            }
        }
    }
}

void attack(std::string target) {
    target = target.substr(0, target.size() - 1);
    int possibilities = 0;
    for (int i = 0; i < currentRoom->getEntities().size(); i++) {
        if (currentRoom->getEntities().at(i)->getName().find(target) != currentRoom->getEntities().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << target << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << target << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < currentRoom->getEntities().size(); i++) {
            if (currentRoom->getEntities().at(i)->getName().find(target) != currentRoom->getEntities().at(i)->getName().npos) {
                if (player->attack(currentRoom, currentRoom->getEntities().at(i)) != -1) {
                    actionTaken = true;
                }
            }
        }
    }

}

void enemyAttacks() {
    for (int i = 0; i < currentRoom->getEntities().size(); i++) {
        if (currentRoom->getEntities().at(i)->isAggressive() && currentRoom->getEntities().at(i)->isAlive() && currentRoom->getEntities().at(i)->getEffect(EffectType::RUSH) == 0) {
            currentRoom->getEntities().at(i)->attack(currentRoom, player);
        }
    }
}

void go(std::string dir) {
    dir.erase(std::remove_if(dir.begin(), dir.end(), isspace), dir.end());
    Room *next = NULL;
    if (dir.compare("north") == 0) {
        next = currentRoom->getDirection(Direction::NORTH);
    }
    else if (dir.compare("south") == 0) {
        next = currentRoom->getDirection(Direction::SOUTH);
    }
    else if (dir.compare("east") == 0) {
        next = currentRoom->getDirection(Direction::EAST);
    }
    else if (dir.compare("west") == 0) {
        next = currentRoom->getDirection(Direction::WEST);
    }
    else if (dir.compare("up") == 0) {
        next = currentRoom->getDirection(Direction::UP);
    }
    else if (dir.compare("down") == 0) {
        next = currentRoom->getDirection(Direction::DOWN);
    }
    else {
        std::cout << "That isn't a direction." << std::endl;
        return;
    }
    if (next != NULL) {
        if (next->isOpen()) {
            std::cout << "You head " << dir << "." << std::endl;
            currentRoom = next;
            actionTaken = true;
            changedRoom = true;

            if (currentRoom->Compare(exitRoom) == 0) {
                std::cout << next->getBasicDescrip() << std::endl;
                std::cout << "Congratulations! You have reached the end of the game. I hope you enjoyed it.\n" << std::endl;
                std::cout << "Restarting..." << std::endl;
                player = defaultPlayer();
                buildWorld();
            }
            else if (currentRoom->Compare(bossRoom) == 0) {
                if (!bossDefeated) {
                    std::cout << next->getBasicDescrip() << std::endl;
                    std::cout << "A figure stands and approaches you. He says in a raspy voice \"Your greed will undo you...\"\nThe doors all lock." << std::endl;
                    currentRoom->lock();
                    int tCount = 0;
                    for (int i = 0; i < player->getItems().size(); i++) {
                        if (player->getItems().at(i)->getType() == ItemType::TREASURE) {
                            tCount++;
                        }
                    }
                    std::vector<Item*> noItems;
                    std::vector<EffectType::E> noRes;
                    Entity *boss = new Entity(20 * tCount, 5 * tCount, 3 * tCount, 15 * tCount, 8 * tCount, noItems, false, "mysterious figure", "a mysterious figure is wearing dark clothing. you cannot make out his face.", true, false, true, noRes);
                    currentRoom->addEntity(boss);
                    if (tCount == 0) {
                        boss->damage(10);
                        std::cout << "The figure collapse to the ground, dead." << std::endl;
                    }
                }
                else {
                    std::cout << next->getDescription() << std::endl;
                }
            }
            else {
                std::cout << next->getDescription() << std::endl;
            }
            if (next->death()) {
                player->damage(10000);
                std::cout << "You are instantly killed." << std::endl;
            }
        }
        else {
            std::cout << "That way is locked." << std::endl;
        }
    }
    else {
        std::cout << "You can't go that way." << std::endl;
    }
}

void activate(std::vector<std::string> phrase) {
    std::string connectors[] = { "with", "on", "in" };
    int cIndex = -1;
    for (int i = 0; i < phrase.size(); i++) {
        for (int j = 0; j < 3; j++) {
            if (phrase.at(i).compare(connectors[j]) == 0) {
                cIndex = i;
            }
        }
    }
    if (cIndex == -1) {
        std::cout << "Your command is unclear." << std::endl;
        return;
    }
    std::string item = "";
    std::string target = "";
    for (int i = 1; i < phrase.size(); i++) {
        if (i < cIndex) {
            item = item + phrase.at(i) + " ";
        }
        else if (i > cIndex) {
            target = target + phrase.at(i) + " ";
        }
    }
    if (target.compare("") == 0) {
        std::cout << "Please give a target." << std::endl;
        return;
    }
    if (item.compare("") == 0) {
        std::cout << "Please specify what item you want to use." << std::endl;
        return;
    }
    target = target.substr(0, target.size() - 1);
    item = item.substr(0, item.size() - 1);
    Item *it = NULL;
    int possibilities = 0;
    for (int i = 0; i < player->getItems().size(); i++) {
        if (player->getItems().at(i)->getName().find(item) != player->getItems().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << item << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << item << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < player->getItems().size(); i++) {
            if (player->getItems().at(i)->getName().find(item) != player->getItems().at(i)->getName().npos) {
                it = player->getItems().at(i);
            }
        }
    }
    if (it->getType() == ItemType::USABLE) {
        if (it->getInheritedItems().size() > 0) {
            //TODO
        }
        else if (it->getInheritedRooms().size() > 0) {
            Direction::E dir;
            if (target.find("north") != target.npos) {
                dir = Direction::NORTH;
            }
            else if (target.find("south") != target.npos) {
                dir = Direction::SOUTH;
            }
            else if (target.find("east") != target.npos) {
                dir = Direction::EAST;
            }
            else if (target.find("west") != target.npos) {
                dir = Direction::WEST;
            }
            else if (target.find("up") != target.npos) {
                dir = Direction::UP;
            }
            else if (target.find("down") != target.npos) {
                dir = Direction::DOWN;
            }
            else if (target.find("door") != target.npos) {
                std::cout << "Please specify the direction." << std::endl;
                return;
            }
            if (currentRoom->getDirection(dir) != NULL) {
                if (currentRoom->getDirection(dir)->isOpen()) {
                    std::cout << "That way is already open." << std::endl;
                    return;
                }
                else {
                    for (int i = 0; i < it->getInheritedRooms().size(); i++) {
                        if (it->getInheritedRooms().at(i)->Compare(currentRoom->getDirection(dir)) == 0) {
                            player->removeItem(it);
                            currentRoom->getDirection(dir)->unlock();
                            actionTaken = true;
                            std::cout << "Done" << std::endl;
                            return;
                        }
                    }
                    std::cout << "That item cannot be used for the specified direction." << std::endl;
                }
            }
            else {
                std::cout << "You can't go in that direction." << std::endl;
            }
        }

    }
    else {
        std::cout << "That item cannot be used in that way." << std::endl;
    }
}

void consume(std::string target) {
    target = target.substr(0, target.size() - 1);
    int possibilities = 0;
    for (int i = 0; i < player->getItems().size(); i++) {
        if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << target << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << target << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < player->getItems().size(); i++) {
            if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
                if (player->use(player->getItems().at(i))) {
                    actionTaken = true;
                }
            }
        }
    }
}

void throwItem(std::vector<std::string> phrase) {
    int atIndex = -1;
    for (int i = 1; i < phrase.size(); i++) {
        if (phrase.at(i).compare("at") == 0) {
            atIndex = i;
        }
    }
    if (atIndex == -1) {
        std::cout << "Please use \"at\" to separate the item from the target." << std::endl;
        return;
    }
    std::string item = "";
    std::string target = "";
    for (int i = 1; i < phrase.size(); i++) {
        if (i < atIndex) {
            item = item + phrase.at(i) + " ";
        }
        else if (i > atIndex) {
            target = target + phrase.at(i) + " ";
        }
    }
    if (target.compare("") == 0) {
        std::cout << "Please give a target." << std::endl;
        return;
    }
    if (item.compare("") == 0) {
        std::cout << "Please specify what you want to throw." << std::endl;
        return;
    }
    target = target.substr(0, target.size() - 1);
    item = item.substr(0, item.size() - 1);
    Item *it = NULL;
    Entity *en = NULL;
    int possibilities = 0;
    for (int i = 0; i < player->getItems().size(); i++) {
        if (player->getItems().at(i)->getName().find(item) != player->getItems().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << item << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << item << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < player->getItems().size(); i++) {
            if (player->getItems().at(i)->getName().find(item) != player->getItems().at(i)->getName().npos) {
                it = player->getItems().at(i);
            }
        }
    }
    possibilities = 0;
    for (int i = 0; i < currentRoom->getEntities().size(); i++) {
        if (currentRoom->getEntities().at(i)->getName().find(target) != currentRoom->getEntities().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << target << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << target << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < currentRoom->getEntities().size(); i++) {
            if (currentRoom->getEntities().at(i)->getName().find(target) != currentRoom->getEntities().at(i)->getName().npos) {
                en = currentRoom->getEntities().at(i);
            }
        }
    }
    if (player->throwItem(it, en)) {
        actionTaken = true;
    }
}

void grab(std::string target) {
    target = target.substr(0, target.size() - 1);
    if (target.compare("all") == 0) {
        while (currentRoom->getItems().size() > 0) {
            Item *i = currentRoom->getItems().at(0);
            if (player->grab(currentRoom, currentRoom->getItems().at(0))) {
                std::cout << i->getName() << " taken." << std::endl;
                actionTaken = true;
            }
        }
        return;
    }
    int possibilities = 0;
    for (int i = 0; i < currentRoom->getItems().size(); i++) {
        if (currentRoom->getItems().at(i)->getName().find(target) != currentRoom->getItems().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << target << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << target << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < currentRoom->getItems().size(); i++) {
            if (currentRoom->getItems().at(i)->getName().find(target) != currentRoom->getItems().at(i)->getName().npos) {
                if (player->grab(currentRoom, currentRoom->getItems().at(i))) {
                    std::cout << "Taken" << std::endl;
                    actionTaken = true;
                }
            }
        }
    }
}

void drop(std::string target) {
    target = target.substr(0, target.size() - 1);
    if (target.compare("all") == 0) {
        while (player->getItems().size() > 0) {
            Item *i = player->getItems().at(0);
            player->drop(currentRoom, i);
        }
        std::cout << "Done" << std::endl;
        return;
    }
    int possibilities = 0;
    for (int i = 0; i < player->getItems().size(); i++) {
        if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << target << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << target << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < player->getItems().size(); i++) {
            if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
                if (player->drop(currentRoom, player->getItems().at(i))) {
                    actionTaken = true;
                    std::cout << "Done" << std::endl;
                }
            }
        }
    }
}

void unequip(std::string target) {
    target = target.substr(0, target.size() - 1);
    int possibilities = 0;
    for (int i = 0; i < player->getItems().size(); i++) {
        if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << target << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << target << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < player->getItems().size(); i++) {
            if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
                if (player->unequip(player->getItems().at(i))) {
                    std::cout << "Done" << std::endl;
                }
            }
        }
    }
}

void equip(std::string target) {
    target = target.substr(0, target.size() - 1);
    int possibilities = 0;
    for (int i = 0; i < player->getItems().size(); i++) {
        if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
            possibilities++;
        }
    }
    if (possibilities == 0) {
        std::cout << target << " could not be recognized." << std::endl;
    }
    else if (possibilities > 1) {
        std::cout << target << " is not specific enough." << std::endl;
    }
    else {
        for (int i = 0; i < player->getItems().size(); i++) {
            if (player->getItems().at(i)->getName().find(target) != player->getItems().at(i)->getName().npos) {
                if (player->equip(player->getItems().at(i))) {
                    std::cout << "Done" << std::endl;
                }
            }
        }
    }
}

int main() {
    bool quit = false;
    std::srand(time(NULL));
    buildWorld();
    player = defaultPlayer();
    std::cout << player->getDescription() << std::endl;
    std::cout << "As you were hiking along a mountain face, you slip and tumble down the steep slope. As you fall you knock you head and go unconscious. When you wake, you find yourself in the bottom of a tall, vertical cave." << std::endl;
    std::cout << currentRoom->getDescription() << std::endl;
    std::string input;
    while (!quit && getline(std::cin, input)) {
        std::transform(input.begin(), input.end(), input.begin(), :: tolower);
        std::istringstream iss(input);
        std::vector<std::string> words;
        std::copy(std::istream_iterator<std::string>(iss),
             std::istream_iterator<std::string>(),
             back_inserter(words));
        if (words.size() == 0) {
            continue;
        }
        std::string action = words.at(0);
        std::string phrase = "";
        actionTaken = false;
        changedRoom = false;
        if (words.size() > 1) {
            if (words.at(0).compare("pick") == 0 && words.at(1).compare("up") == 0) {
                action = "pick up";
            }
        }
        if (action.compare("pick up") == 0) {
            if (words.size() > 2) {
                for (int i = 2; i < words.size(); i++) {
                    phrase = phrase + words.at(i) + " ";
                }
                phrase = phrase.substr(0, phrase.length());
            }
        }
        else {
            if (words.size() > 1) {
                for (int i = 1; i < words.size(); i++) {
                    phrase = phrase + words.at(i) + " ";
                }
                phrase = phrase.substr(0, phrase.length());
            }
        }
        if (currentRoom->hasAgressiveMob()) {
            for (int i = 0; i < currentRoom->getEntities().size(); i++) {
                if (currentRoom->getEntities().at(i)->getEffect(EffectType::RUSH)) {
                    std::cout << "You are rushed by the " << currentRoom->getEntities().at(i)->getName() << "." << std::endl;
                    currentRoom->getEntities().at(i)->attack(currentRoom, player);
                }
            }
        }
        else if (currentRoom->Compare(bossRoom) == 0) {
            bossDefeated = true;
            currentRoom->unlock();
        }
        if (action.compare("l") == 0 || action.compare("look") == 0) {
            std::cout << currentRoom->getDescription() << std::endl;
        }
        else if (action.compare("x") == 0 || action.compare("examine") == 0) {
            if (words.size() == 1) {
                std::cout << "Please specify what you want to examine." <<std::endl;
            }
            else {
                examine(phrase);
            }
        }
        else if (action.compare("attack") == 0) {
            if (words.size() == 1) {
                std::cout << "The command \"attack\" takes a parameter." << std::endl;
            }
            else {
                attack(phrase);
            }
        }
        else if (action.compare("go") == 0 || action.compare("walk") == 0 || action.compare("run") == 0) {
            if (words.size() == 1) {
                std::cout << "The command \"go\" take a parameter." << std::endl;
            }
            else {
                go(phrase);
            }
        }
        else if (action.compare("use") == 0 || action.compare("place") == 0 || action.compare("put") == 0 || action.compare("insert") == 0) {
            if (words.size() < 3) {
                std::cout << "The command \"use\" takes more parameters." << std::endl;
            }
            else {
                activate(words);
            }
        }
        else if (action.compare("throw") == 0) {
            if (words.size() < 3) {
                std::cout << "The command \"drop\" takes more parameters." << std::endl;
            }
            else {
                throwItem(words);
            }
        }
        else if (action.compare("consume") == 0 || action.compare("eat") == 0 || action.compare("eat") == 0) {
            if (words.size() == 1) {
                std::cout << "The command \"drop\" takes a parameter." << std::endl;
            }
            else {
                consume(phrase);
            }
        }
        else if (action.compare("grab") == 0 || action.compare("take") == 0 || action.compare("pick up") == 0) {
            if (words.size() == 1) {
                std::cout << "The command \"grab\" takes a parameter." << std::endl;
            }
            else {
                grab(phrase);
            }
        }
        else if (action.compare("drop") == 0) {
            if (words.size() == 1) {
                std::cout << "The command \"drop\" takes a parameter." << std::endl;
            }
            else {
                drop(phrase);
            }
        }
        else if (action.compare("equip") == 0) {
            if (words.size() == 1) {
                std::cout << "The command \"equip\" takes a parameter." << std::endl;
            }
            else {
                equip(phrase);
            }
        }
        else if (action.compare("unequip") == 0) {
            if (words.size() == 1) {
                std::cout << "The command \"unequip\" takes a parameter." << std::endl;
            }
            else {
                unequip(phrase);
            }
        }
        else if (action.compare("brief") == 0) {
            if (words.size() > 1) {
                std::cout << "The command \"brief\" does not take parameters." << std::endl;
            }
            else {
                detail = 1;
            }
        }
        else if (action.compare("superbrief") == 0) {
            if (words.size() > 1) {
                std::cout << "The command \"superbrief\" does not take parameters." << std::endl;
            }
            else {
                detail = 0;
            }
        }
        else if (action.compare("verbose") == 0) {
            if (words.size() > 1) {
                std::cout << "The command \"verbose\" does not take parameters." << std::endl;
            }
            else {
                detail = 2;
            }
        }
        else if (action.compare("diagnose") == 0) {
            if (words.size() > 1) {
                std::cout << "The command \"diagnose\" does not take parameters." << std::endl;
            }
            else if (player->getHealth() >= 50) {
                std::cout << "You feel very healthy." << std::endl;
            }
            else if (player->getHealth() >= 30) {
                std::cout << "You are feeling weak." << std::endl;
            }
            else if (player->getHealth() >= 10) {
                std::cout << "You are very injured and should find help soon." << std::endl;
            }
            else {
                std::cout << "You feel death approaching" << std::endl;
            }
        }
        else if (action.compare("inventory") == 0 || action.compare("i") == 0) {
            if (words.size() > 1) {
                std::cout << "The command \"inventory\" does not take parameters." << std::endl;
            }
            else if (player->getItems().size() == 0) {
                std::cout << "Your backpack is empty" << std::endl;
            }
            else if (player->getItems().size() == 1) {
                std::cout << "Your only carrying one thing: \na " << player->getItems().at(0)->getName() << std::endl;
            }
            else {
                std::cout << "You are carrying several items:" << std::endl;
                for (int i = 0; i < player->getItems().size(); i++) {
                    std::cout << "A " << player->getItems().at(i)->getName() << std::endl;
                }
            }
        }
        else if (action.compare("save") == 0) {
            if (words.size() > 1) {
                std::cout << "The command \"save\" does not take parameters." << std::endl;
            }
            else {
                std::cout << "Saving..." << std::endl;
                savePlayer = player;
                saveRoom = currentRoom;
            }
        }
        else if (action.compare("restore") == 0) {
            if (words.size() > 1) {
                std::cout << "The command \"restore\" does not take parameters." << std::endl;
            }
            else {
                std::cout << "Loading..." << std::endl;
                player = savePlayer;
                currentRoom = saveRoom;
            }
        }
        else if (action.compare("xyzzy") == 0) {
            std::cout << "A hollow voice says \"fool.\"" << std::endl;
            player = cheatPlayer();
        }
        else if (action.compare("restart") == 0) {
            if (words.size() > 1) {
                std::cout << "The command \"restart\" does not take parameters." << std::endl;
            }
            else {
                player = defaultPlayer();
                buildWorld();
            }
        }
        else if (action.compare("quit") == 0) {
            if (words.size() > 1) {
                std::cout << "The command \"quit\" does not take parameters." << std::endl;
            }
            else {
                quit = true;
                continue;
            }
        }
        else {
            std::cout << action << " could not be interpreted." << std::endl;
        }
        if (actionTaken) {
            if (!changedRoom) {
                enemyAttacks();
            }
            player->refreshEffects();
            for (int i = 0; i < currentRoom->getEntities().size(); i++) {
                currentRoom->getEntities().at(i)->refreshEffects();
            }
        }
        if (!player->isAlive()) {
            std::cout << "\n\nRestarting...\n" << std::endl;
            player = defaultPlayer();
            buildWorld();
            std::cout << player->getDescription() << std::endl;
            std::cout << currentRoom->getDescription() << std::endl;
        }
    }
    return 0;
}
