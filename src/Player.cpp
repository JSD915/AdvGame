#include "Player.h"
#include <Effect.h>
#include <Entity.h>
#include <iostream>

Player::~Player() {
    delete this;
}

bool Player::grab(Room *r, Item *i) {
    if (r->hasItem(i)) {
        if (carry_capacity >= i->getWeight() + getCarryWeight()) {
            inventory.push_back(r->removeItem(i));
            return true;
        }
        std::cout << "You cant carry that much." << std::endl;
        return false;
    }
    std::cout << "That item is not in the room." << std::endl;
    return false;
}

bool Player::use(Item *i) {
    if (hasItem(i)) {
        if (i->getType() == ItemType::CONSUMABLE) {
            addEffects(i, true);
            addEffects(i, false);
            removeItem(i);
            return true;
        }
        std::cout << "That item is not consumable." << std::endl;
        return false;
    }
    std::cout << "You are not holding that item." << std::endl;
    return false;
}

bool Player::throwItem(Item *i, Entity *e) {
    if (hasItem(i)) {
        if (i->canThrow()) {
            e->addEffects(i, true);
            e->addEffects(i, true);
            removeItem(i);
            return true;
        }
        std::cout << "You cant throw that item." << std::endl;
        return false;
    }
    std::cout << "You are not holding that item." << std::endl;
    return false;
}

std::vector<Item*> Player::activate(Item *item, Room *r) {
    if (item->getInheritedItems().size() > 0 && !r->hasAgressiveMob()) {
        bool usable = false;
        for (int i = 0; i < r->getUsables().size(); i++) {
            if (item->Compare(r->getUsables().at(i)) == 0) {
                usable = true;
            }
        }
        if (usable) {
            for(int i = 0; i < item->getInheritedItems().size(); i++) {
                r->addItem(item->getInheritedItems().at(i));
            }
            removeItem(item);
            return item->getInheritedItems();
        }
    }
    std::vector<Item*> null;
    return null;
}

Room* Player::activate(Room *r, Item *item, Direction::E dir) {
    if (item->getInheritedRooms().size() > 0 && !r->hasAgressiveMob()) {
        bool usable = false;
        for (int i = 0; i < r->getUsables().size(); i++) {
            if (item->Compare(r->getUsables().at(i)) == 0) {
                usable = true;
            }
        }
        if (usable) {
            for (int i = 0; i < item->getInheritedRooms().size(); i++) {
                if (r->getDirection(dir)->Compare(item->getInheritedRooms().at(i)) == 0) {
                    r->getDirection(dir)->unlock();
                    removeItem(item);
                    return r->getDirection(dir);
                }
            }
        }
    }
    return NULL;
}

Room* Player::go(Room *r, Direction::E d) {

}

int Player::getCarryCapacity() {
    return carry_capacity;
}

int Player::getCarryWeight() {
    int tot = 0;
    for (int i = 0; i < inventory.size(); i++) {
        tot = tot + inventory.at(i)->getWeight();
    }
    return tot;
}

std::string Player::getDescription() {
    return basic_description;
}
