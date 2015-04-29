#include "Room.h"
#include <Entity.h>
#include <Item.h>
#include <iostream>

using namespace Direction;

Room::Room(bool o, std::vector<Entity*> es, std::vector<Item*> is, std::string nm, std::string b_d, std::vector<Item*> us) {
    open = o;
    mobs = es;
    items = is;
    usables = us;
    name = nm;
    basic_description = b_d;
    north = NULL, south = NULL, east = NULL, west = NULL, up = NULL, down = NULL;
}

Room::Room(bool o, std::vector<Entity*> es, std::vector<Item*> is, std::string nm, std::string b_d, std::vector<Item*> us, Room *n, Room *s, Room *e, Room *w, Room *u, Room *d) {
    open = o;
    mobs = es;
    items = is;
    usables = us;
    name = nm;
    basic_description = b_d;
    north = n;
    south = s;
    east = e;
    west = w;
    up = u;
    down = d;
}

Room::~Room() {
    delete this;
}

bool Room::isOpen() {
    return open;
}

Room* Room::getDirection(Direction::E d) {
    switch (d) {
        case NORTH:
            return north;
        case SOUTH:
            return south;
        case EAST:
            return east;
        case WEST:
            return west;
        case UP:
            return up;
        case DOWN:
            return down;
    }
    return NULL;
}

void Room::assignDirection(Direction::E d, Room *r) {
    switch (d) {
        case NORTH:
            north = r;
            break;
        case SOUTH:
            south = r;
            break;
        case EAST:
            east = r;
            break;
        case WEST:
            west = r;
            break;
        case UP:
            up = r;
            break;
        case DOWN:
            down = r;
            break;
    }
}

std::vector<Item*> Room::getItems() {
    return items;
}

std::vector<Item*> Room::getUsables() {
    return usables;
}
bool Room::hasAgressiveMob() {
    for (int i = 0; i < mobs.size(); i++) {
        if (mobs.at(i)->isAggressive() && mobs.at(i)->isAlive()) {
            return true;
        }
    }
    return false;
}

std::vector<Entity*> Room::getEntities() {
    return mobs;
}

void Room::lock() {
    open = false;
}

void Room::unlock() {
    open = true;
}

void Room::addItem(Item *i) {
    items.push_back(i);
}

void Room::addEntity(Entity *e) {
    mobs.push_back(e);
}

Item* Room::removeItem(Item *item) {
    for (int i = 0; i < items.size(); i++) {
        if (items.at(i)->Compare(item) == 0) {
            items.erase(items.begin() + i);
            return item;
        }
    }
    return NULL;
}

Entity* Room::removeEntity(Entity *e) {
    for (int i = 0; i < items.size(); i++) {
        if (mobs.at(i)->Compare(e)) {
            mobs.erase(mobs.begin() + i);
            return e;
        }
    }
    return NULL;
}
std::vector<Direction::E> Room::availableDirections() {
    std::vector<Direction::E> dirs;
    if (north != NULL) {
        dirs.push_back(Direction::NORTH);
    }
    if (south != NULL) {
        dirs.push_back(Direction::SOUTH);
    }
    if (east != NULL) {
        dirs.push_back(Direction::EAST);
    }
    if (west != NULL) {
        dirs.push_back(Direction::WEST);
    }
    if (up != NULL) {
        dirs.push_back(Direction::UP);
    }
    if (down != NULL) {
        dirs.push_back(Direction::DOWN);
    }
    return dirs;
}

bool Room::hasEntity(Entity *e) {
    return true; //TODO
}

bool Room::hasItem(Item *i) {
    return true; //TODO
}
std::string Room::getName() {
    return name;
}

std::string Room::getDescription() {
    std::string s = name + ":\n" + basic_description + "\n";
    std::vector<Direction::E> dirs = availableDirections();
    if (dirs.size() > 1) {
        s = s + "There are passageways leading ";
        for (int i = 0; i < dirs.size(); i++) {
            if (i == dirs.size() - 1) {
                s = s.substr(0, s.size() - 2);
                s = s + " and ";
            }
            switch (dirs.at(i)) {
                case NORTH:
                    s = s + "north, ";
                    break;
                case SOUTH:
                    s = s + "south, ";
                    break;
                case EAST:
                    s = s + "east, ";
                    break;
                case WEST:
                    s = s + "west, ";
                    break;
                case UP:
                    s = s + "up, ";
                    break;
                case DOWN:
                    s = s + "down, ";
                    break;
            }
        }
        s = s.substr(0, s.size() - 2);
        s = s + ".\n";
    }
    else if (dirs.size() == 1) {
        s = s + "There is a passageway leading ";
        switch (dirs.at(0)) {
            case NORTH:
                s = s + "north.\n";
                break;
            case SOUTH:
                s = s + "south.\n";
                break;
            case EAST:
                s = s + "east.\n";
                break;
            case WEST:
                s = s + "west.\n";
                break;
            case UP:
                s = s + "up.\n";
                break;
            case DOWN:
                s = s + "down.\n";
                break;
        }
    }
    else if (dirs.size() == 0) {
        s = s + "There are no clear exits.\n";
    }
    if (mobs.size() > 1) {
        s = s + "There are several figures in the room:";
        for (int i = 0; i < mobs.size(); i++) {
            s = s + mobs.at(i)->getDescription() + "\n";
        }
    }
    if (mobs.size() == 1) {
        s = s + "You see " + mobs.at(0)->getDescription() + "\n";
    }
    if (mobs.size() == 0) {
        s = s + "You don't see anyone else.\n";
    }

    if (items.size() > 0) {
        s = s + "You notice there is...\n";
        /*for (int i = 0; i < items.size(); i++) {
            if (i == items.size() - 1) {
                if (i == 0) {
                    s = s + items.at(i)->getDescription();
                }
                else {
                    s = s.substr(0, s.size() - 2);
                    s = s + " and " + items.at(i)->getDescription();
                }
            }
            else {
                s = s + items.at(i)->getDescription() + ", ";
            }
        }*/
        for (int i = 0; i < items.size(); i++) {
            s = s + "a " + items.at(i)->getName() + "\n";
        }
    }

    return s;
}
