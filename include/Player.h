#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <Entity.h>
#include <Effect.h>
#include <Item.h>
#include <Room.h>

/** Player is an extension of the class Entity. It has some extra functions and the addition field carry_capacity
  * carry_capacity is the max amount of weight the player can carry
  * The player is the class that describes the person playing the game
  * It is important to note that player is never technically in a room, more on that later in the room class
  */

class Player : public Entity
{
    public:
        Player(int h, int att, int arm, int acc, int dod, std::vector<Item*> i, bool can_m, std::string nm, std::string b_d, bool a, bool pas, bool aggr, std::vector<EffectType::E> resistances, int c_c)
        : Entity(h, att, arm, acc, dod, i, can_m, nm, b_d, a, pas, aggr, resistances) {
            carry_capacity = c_c;
        };
        virtual ~Player();
        //grab attempts to remove an item *i from a room *r and add it to inventory
        bool grab(Room *r, Item *i);
        //consume attempts to add the effects of item *i of ItemType::CONSUMABLE to effects
        bool consume(Item *i);
        //throw item attempts to add the effects of item *i to entity *target. i must be throwable
        bool throwItem(Item *i, Entity *e);
        //this activate returns a list of items that can be obtained by using item *i in room *r, this function is currently incomplete and unused
        std::vector<Item*> activate(Item *i, Room *r);
        //this activate attemts to unlock room *r with Item usable *i and returns the Room* in direction dir
        Room* activate(Room *r, Item *i, Direction::E dir);
        //returns carry_capacity
        int getCarryCapacity();
        //calculates the total weight carried by the player by looking at the items in inventory
        int getCarryWeight();
        //builds and returns a string describing the player
        std::string getDescription();
    protected:
    private:
        int carry_capacity;
};

#endif // PLAYER_H
