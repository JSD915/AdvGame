#ifndef ROOM_H
#define ROOM_H
#include <Entity.h>
#include <Effect.h>
#include <string>
#include <vector>
#include <Item.h>


using namespace Direction;
using namespace EffectType;
using namespace ItemType;

class Item;
class Entity;

class Room
{
    public:
        Room(bool o, std::vector<Entity*> es, std::vector<Item*> is, std::string nm, std::string b_d, std::vector<Item*> us);
        Room(bool o, std::vector<Entity*> es, std::vector<Item*> is, std::string nm, std::string b_d, std::vector<Item*> us, Room *n, Room *s, Room *e, Room *w, Room *u, Room *d);
        virtual ~Room();
        bool isOpen();
        Room* getDirection(Direction::E d);
        void assignDirection(Direction::E d, Room *r);
        std::vector<Item*> getItems();
        std::vector<Item*> getUsables();
        bool hasAgressiveMob();
        std::vector<Entity*> getEntities();
        void lock();
        void unlock();
        void addItem(Item *i);
        void addEntity(Entity *e);
        bool hasItem(Item *i); //TODO
        bool hasEntity(Entity *e); //TODO
        Item* removeItem(Item *i);
        Entity* removeEntity(Entity *e);
        std::string getName();
        std::string getDescription();
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
        std::vector<Direction::E> availableDirections();
};

#endif // ROOM_H
