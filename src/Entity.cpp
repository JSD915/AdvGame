#include "Entity.h"
#include <cstdlib>
#include <iostream>
#include <Player.h>
#include <Player.h>

using namespace EffectType;
using namespace ItemType;

Entity::Entity(int h, int att, int arm, int acc, int dod, std::vector<Item*> i, bool can_m, std::string nm, std::string b_d, bool a, bool pas, bool aggr, std::vector<EffectType::E> res) {
    health = h;
    base_attack = att;
    base_armor = arm;
    base_accuray = acc;
    base_dodge = dod;
    inventory = i;
    movable = can_m;
    name = nm;
    basic_description = b_d;
    alive = a;
    passive = pas;
    aggressive = aggr;
    resistances = res;
}

Entity::~Entity() {
    delete this;
}

std::vector<Effect*> Entity::getEffects() {
    return effects;
}

bool Entity::hasItem(Item *item) { //REDO
    for (int i = 0; i < inventory.size(); i++) {
        if (inventory.at(i)->Compare(item) == 0) {
            return true;
        }
    }
    return false;
}

bool Entity::drop(Room *r, Item *i) {
    if (hasItem(i)) {
        r->addItem(removeItem(i));
        return true;
    }
    std::cout << "You are not holding that item." << std::endl;
    return false;
}

/*
bool Entity::use(Room *r, Entity *target, Item *i) {
    EffectType::E type = EffectType::FREEZE;
    if (getEffect(type) == NULL && hasItem(i) && i->getType() == ItemType::CONSUMABLE ) {
        if (r->hasEntity(target) && r->hasEntity(this)) {
            if (this->Compare(target) == 0) {
                int acc = getBaseAccuracy();
                if (getEffect(EffectType::STEADY) != NULL) {
                    acc = acc + getEffect(EffectType::STEADY)->getStrength();
                }
                if (getEffect(EffectType::BLIND) != NULL) {
                    acc = acc - getEffect(EffectType::BLIND)->getStrength();
                }
                int dod = target->getBaseDodge();
                if (target->getEffect(EffectType::DODGE) != NULL) {
                    dod = dod + target->getEffect(EffectType::DODGE)->getStrength();
                }
                if (target->getEffect(EffectType::STUN) != NULL) {
                    dod = dod - target->getEffect(EffectType::STUN)->getStrength();
                }
                if (!(rand() % 100 < acc && rand() % 100 < dod)) {
                    return false;
                    removeItem(i);
                }
            }
            target->addEffects(i, true);
            target->addEffects(i, false);
            removeItem(i);
            return true;
        }
    }
    return false;
}*/

int Entity::attack(Room *r, Entity *target) {
    if (r->hasEntity(target) && r->hasEntity(this) || dynamic_cast<Player*>(target) != NULL) {
        bool has_weap = false;
        Item *weapon = NULL;
        for (int i = 0; i < inventory.size(); i++) {
            if (inventory.at(i)->getType() == ItemType::WEAPON && inventory.at(i)->isEquipped()) {
                weapon = inventory.at(i);
                has_weap = true;
            }
        }
        int acc = 0;
        if (has_weap || (dynamic_cast<Player*>(this) == NULL)) {
            //for (int i = 0; i < getEffects().size(); i++) {
            //    std::cout << getEffects().at(i)->getInfo() << std::endl;
            //}
            if (getEffect(EffectType::FREEZE) == 0) {
                if (!target->isPassive()) {
                    target->anger();
                }
                int attacks = 1;
                if (getEffect(EffectType::FURY) != 0) {
                    attacks = getEffect(EffectType::FURY)->getStrength();
                }
                for (int i = 0; i < attacks; i++)
                acc = getBaseAccuracy();
                int dod = target->getBaseDodge();
                if (getEffect(EffectType::STEADY) != 0) {
                    acc = acc + getEffect(EffectType::STEADY)->getStrength();
                }
                if (getEffect(EffectType::BLIND) != 0) {
                    acc = acc - getEffect(EffectType::BLIND)->getStrength();
                }
                if (target->getEffect(EffectType::DODGE) != 0) {
                    dod = dod + target->getEffect(EffectType::DODGE)->getStrength();
                }
                if (target->getEffect(EffectType::STUN) != 0) {
                    dod = dod - target->getEffect(EffectType::STUN)->getStrength();
                }
                int dmg = getBaseAttack();
                if (getEffect(EffectType::ATTACK) != 0) {
                    dmg = dmg + getEffect(EffectType::ATTACK)->getStrength();
                }
                if (getEffect(EffectType::FEAR) != 0) {
                    dmg = dmg - getEffect(EffectType::FEAR)->getStrength();
                }
                dmg = dmg - target->getBaseArmor();
                if (target->getEffect(EffectType::ARMOR) != 0) {
                    dmg = dmg - target->getEffect(EffectType::ARMOR)->getStrength();
                }
                if (target->getEffect(EffectType::BURN) != 0 ) {
                    dmg = dmg + target->getEffect(EffectType::BURN)->getStrength();
                }
                if (target->getEffect(EffectType::IMMUNITY) != 0) {
                    if (dynamic_cast<Player*>(target) == NULL) {
                        std::cout << "The target is immune." << std::endl;
                    }
                    else {
                        std::cout << "You are immune to damage." << std::endl;
                    }
                    return 0;
                }
                for (int i = 0; i < attacks; i++) {
                    if (dynamic_cast<Player*>(target) == NULL) {
                        std::cout << "You swing at the " << target->getName() << "." << std::endl;
                    }
                    else {
                        std::cout << "The " << this->getName() << " attacks you." << std::endl;
                    }
                    if (rand() % 100 > dod) {
                        if (rand() % 100 < acc) {
                            if (dynamic_cast<Player*>(target) == NULL) {
                                std::cout << "Your attack lands." << std::endl;
                            }
                            else {
                                std::cout << "You are hit." << std::endl;
                            }
                            if (getEffect(EffectType::LIFE_STEAL) != 0) {
                                if (dmg > target->getHealth()) {
                                    health = health + int(target->getHealth() * (getEffect(EffectType::LIFE_STEAL)->getStrength() / 100));
                                    if (dynamic_cast<Player*>(target) == NULL) {
                                        std::cout << "You drain some your enemies life force." << std::endl;
                                    }
                                    else {
                                        std::cout << "Feel your life force drained" << std::endl;
                                    }
                                }
                                else {
                                    health = health + int(dmg * (getEffect(EffectType::LIFE_STEAL)->getStrength() / 100));
                                }
                            }
                            if (dmg < 2) {
                                std::cout << base_attack << ", " << target->getBaseArmor() << ", " << dmg << std::endl;
                                std::cout << "The attack damage is negligible." << std::endl;
                            }
                            else if (dmg < 10) {
                                std::cout << "The attack does some damage." << std::endl;
                            }
                            else if (dmg < 20) {
                                std::cout << "The attack does a fair bit of damage" << std::endl;
                            }
                            else {
                                std::cout << "The attack deals massive damage." << std::endl;
                            }
                            target->damage(dmg);
                            if (weapon != NULL) {
                                target->addEffects(weapon, false);
                            }
                            if (!target->isAlive()) {
                                if (dynamic_cast<Player*>(target) == NULL) {
                                    std::cout << "The " << target->getName() << " dies." << std::endl;
                                }
                                else {
                                    std::cout << "You are brutally murdered." << std::endl;
                                }
                                for (int i = 0; i < target->getItems().size(); i++) {
                                    if (dynamic_cast<Player*>(target) == NULL) {
                                        target->drop(r, target->getItems().at(i));
                                    }
                                    else {
                                        drop(r, target->getItems().at(i));
                                    }
                                }
                                break;
                            }
                        }
                        else if (dynamic_cast<Player*>(target) == NULL) {
                            std::cout << "You miss your attack." << std::endl;
                        }
                        else {
                            std::cout << "The " << this->getName() << " misses its attack." << std::endl;
                        }
                    }
                    else if (dynamic_cast<Player*>(target) == NULL) {
                        std::cout << "Your attack is dodged." << std::endl;
                    }
                    else {
                        std::cout << "You dodge the incoming blow." << std::endl;
                    }
                }
                return dmg;
            }
            else if (dynamic_cast<Player*>(target) == NULL) {
                std::cout << "You are froze and can't attack" << std::endl;
            }
            else {
                std::cout << "The " << this->getName() << " is frozen and can't attack." << std::endl;
            }
        }
        std::cout << "No weapon equipped." << std::endl;
        return -1;
    }
    std::cout << "Target not found." << std::endl;
    return -1;
}

std::vector<Item*> Entity::getItems() {
    return inventory;
}

Item* Entity::removeItem(Item *item) {
    for (int i = 0; i < inventory.size(); i++) {
        if (item->Compare(inventory.at(i)) == 0) {
            item->unequip();
            inventory.erase(inventory.begin() + i);
            return item;
        }
    }
    return NULL;
}

void Entity::refreshEffects() {
    for (int i = 0; i < effects.size(); i++) {
        effects.at(i)->reduce();
        switch (effects.at(i)->getType()) {
            case EffectType::POISON:
                health = health - effects.at(i)->getStrength();
                break;
            case EffectType::REGEN:
                health = health + effects.at(i)->getStrength();
                break;
        }
        if (effects.at(i)->getDuration() == 0) {
            effects.erase(effects.begin() + i);
        }
    }
}

bool Entity::equip(Item *item) {
    if (hasItem(item)) {
        if (item->getType() == ItemType::ARMOR || item->getType() == ItemType::WEAPON) {
            for (int i = 0; i < inventory.size(); i++) {
                if (inventory.at(i)->getType() == item->getType()) {
                    if (inventory.at(i)->isEquipped()) {
                        std::cout << "An item of that type is already equipped." << std::endl;
                        return false;

                    }
                }
            }
        }
        else {
            std::cout << "You can't equip that item type." << std::endl;
            return false;
        }
        item->equip();
        return true;
    }
    std::cout << "You are not holding that item." << std::endl;
    return false;
}

bool Entity::unequip(Item *item) {
    if (hasItem(item)) {
        if (item->getType() == ItemType::ARMOR || item->getType() == ItemType::WEAPON) {
            if (item->isEquipped()) {
                item->unequip();
                return true;
            }
            else {
                std::cout << "That item isn't equipped." << std::endl;
            }
        }
        else {
            std::cout << "You can't upequip that item type." << std::endl;
            return false;
        }
    }
    std::cout << "You are not holding that item." << std::endl;
    return false;
}

bool Entity::canMove() {
    return movable;
}

bool Entity::isAlive() {
    return alive;
}

bool Entity::isPassive() {
    return passive;
}

bool Entity::isAggressive() {
    return aggressive;
}

int Entity::getBaseAccuracy() {
    return base_accuray;
}

int Entity::getBaseArmor() {
    return base_armor;
}

int Entity::getBaseAttack() {
    return base_attack;
}

int Entity::getBaseDodge() {
    return base_dodge;
}

void Entity::damage(int dmg) {
    health = health - dmg;
    if (health < 0) {
        health = 0;
        alive = false;
    }
}

int Entity::getHealth() {
    return health;
}

void Entity::addEffects(Item *item, bool buff) {
    for (int i = 0; i < item->getEffects().size(); i++) {
        if (item->getEffects().at(i)->isBuff() == buff) {
            effects.push_back(item->getEffects().at(i));
        }
    }
}

std::string Entity::getScript(bool victory, Entity other) {
    return "TODO";
}

std::string Entity::getName() {
    return name;
}

std::string Entity::getDescription() {

    std::string s = basic_description + "\n";
    if (aggressive && alive) {
        s = s + "When you enter the room, it charges at you!\n";
    }
    else if (!alive) {
        s = s + "It appears to be dead.\n";
    }
    return s;
}

Effect* Entity::getEffect(EffectType::E t) {
    for (int i = 0; i < getEffects().size(); i++) {
        if (getEffects().at(i)->getType() == t) {
            return getEffects().at(i);
        }
    }
    return NULL;
}

void Entity::anger() {
    aggressive = true;
}
