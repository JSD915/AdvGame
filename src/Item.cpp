#include "Item.h"
#include <Effect.h>

using namespace ItemType;

Item::Item(std::vector<Effect*> e, std::string nm, std::string b_d, int w, ItemType::E t, std::vector<Item*> i_i, std::vector<Room*> i_r) {
    effects = e;
    name = nm;
    basic_description = b_d;
    weight = w;
    type = t;
    inheritedItems = i_i;
    inheritedRooms = i_r;
    equipped = false;
    if (t == ItemType::CONSUMABLE && name.find_first_of("potion") != name.npos) {
        throwable = true;
    }
    else {
        throwable = false;
    }
}

Item::~Item() {
    delete this;
}

std::vector<Effect*> Item::getEffects() {
    return effects;
}

std::string Item::getName() {
    return name;
}

std::string Item::getDescription() {
    std::string s = "The " + name + basic_description;/*+ "with the following effects:\n";
    bool b = false;
    bool db = false;
    for (int i = 0; i < effects.size(); i++) {
        if (effects.at(i)->isBuff()) {
            b = true;
        }
        else if (!effects.at(i)->isBuff()) {
            db = true;
        }
        if (b && db) {
            break;
        }
    }
    if (b) {
        s = s + "\tBuffs: ";
        for (int i = 0; i < effects.size(); i++) {
            if (effects.at(i)->isBuff()) {
                s = s + effects.at(i)->getName() + ", ";
            }
        }
    }
    s = s.substr(0, s.size() - 1);
    s = s + "\n";
    if (db) {
        s = s + "\tDebuffs: ";
        for (int i = 0; i < effects.size(); i++) {
            if (!effects.at(i)->isBuff()) {
                s = s + effects.at(i)->getName() + ", ";
            }
        }
    }
    s = s.substr(0, s.size() - 1);*/
    return s;
}

int Item::getWeight() {
    return weight;
}

ItemType::E Item::getType() {
    return type;
}

bool Item::isEquipped() {
    return equipped;
}

void Item::equip() {
    equipped = true;
}

void Item::unequip() {
    equipped = false;
}

bool Item::canThrow() {
    return throwable;
}

std::vector<Item*> Item::getInheritedItems() {
    return inheritedItems;
}

std::vector<Room*> Item::getInheritedRooms() {
    return inheritedRooms;
}
