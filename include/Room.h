#ifndef ROOM_H
#define ROOM_H
#include <Entity.h>
#include <Effect.h>
#include <string>
#include <vector>
#include <Item.h>

/** Class Room is the thing that holds entities and items. However Room does not have any information about players. This is because it made it easier for the player to interact with things in the room
  * Room has several fields: open, mobs, items, usables, north, south, east, west, up, down, name, basic_description, instant_death, and available_directions
  * boolean open is whether the room is locked or not. if a room is not open, the player cannot go into it.
  * mobs is a list of the entities in the room
  * items is a list of the items in the room
  * usables is field currently unused by the game. it is list of items that can by used in the room to add items to that room, like a key to a chest
  * north, south, east, west, up, and down, are pointers to other rooms adjacent to this one. the player can move in all of those directions
  * name and basic_description are fields describing the room in plain text
  * boolean instant_death determines whether the player will instantly die upon entering a room
  */

using namespace Direction;
using namespace EffectType;
using namespace ItemType;

class Item;
class Entity;

class Room
{
    public:
        Room(bool o, std::vector<Entity*> es, std::vector<Item*> is, std::string nm, std::string b_d, std::vector<Item*> us);
        Room(bool o, std::vector<Entity*> es, std::vector<Item*> is, std::string nm, std::string b_d, std::vector<Item*> us, bool i_d);
        Room(bool o, std::vector<Entity*> es, std::vector<Item*> is, std::string nm, std::string b_d, std::vector<Item*> us, Room *n, Room *s, Room *e, Room *w, Room *u, Room *d);
        virtual ~Room();
        //getters
        bool isOpen();
        bool death();
        std::string getName();
        std::string getBasicDescrip();
        std::vector<Item*> getItems();
        std::vector<Item*> getUsables();
        std::vector<Entity*> getEntities();
        //get direction returns the Room* in direction d from this room
        Room* getDirection(Direction::E d);
        //assign direction adds sets either N, S, W, E, U, or D to r depending on the direction dir given
        void assignDirection(Direction::E d, Room *r);
        //has aggressive mob returns true or false depending on whether an entity that is aggressive (entity::isAggressive() == true)
        bool hasAgressiveMob();
        //lock sets open to false
        void lock();
        //unlock sets open to true
        void unlock();
        //add item adds item *i to items
        void addItem(Item *i);
        //add entity adds entity *e to mobs
        void addEntity(Entity *e);
        //has item returns true or false depending on whether items contains item *i
        bool hasItem(Item *i);
        //has entity returns true or false depending on whether mobs contains entity *e
        bool hasEntity(Entity *e);
        //attempts to remove item i from items and return it. returns NULL otherwise
        Item* removeItem(Item *i);
        //attempts to remove entity i from mobs and return it. returns NULL otherwise
        Entity* removeEntity(Entity *e);
        //builds and returns a string describing the room
        std::string getDescription();
        //returns 0 if this and room r are the same. returns 1 otherwise
        int Compare(Room *r) const {
            if (basic_description.compare(r->basic_description) != 0) {
                return 1;
            }
            if (north != r->north) {
                return 1;
            }
            if (south != r->south) {
                return 1;
            }
            if (east != r->east) {
                return 1;
            }
            if (west != r->west) {
                return 1;
            }
            if (up != r->up) {
                return 1;
            }
            if (down != r->down) {
                return 1;
            }
            //can add more, should be fine
            return 0;
        }
    protected:
    private:
        bool open;
        std::vector<Entity*> mobs;
        std::vector<Item*> items;
        std::vector<Item*> usables;
        Room *north;
        Room *south;
        Room *east;
        Room *west;
        Room *up;
        Room *down;
        std::string name;
        std::string basic_description;
        bool instantDeath;
        std::vector<Direction::E> availableDirections();
};

#endif // ROOM_H
