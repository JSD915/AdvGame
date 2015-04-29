#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <Entity.h>
#include <Effect.h>
#include <Item.h>
#include <Room.h>



class Player : public Entity
{
    public:
        Player(int h, int att, int arm, int acc, int dod, std::vector<Item*> i, bool can_m, std::string nm, std::string b_d, bool a, bool pas, bool aggr, std::vector<EffectType::E> resistances, int c_c)
        : Entity(h, att, arm, acc, dod, i, can_m, nm, b_d, a, pas, aggr, resistances) {
            carry_capacity = c_c;
        };
        virtual ~Player();
        bool grab(Room *r, Item *i);
        bool use(Item *i);
        bool throwItem(Item *i, Entity *e);
        std::vector<Item*> activate(Item *i, Room *r);
        Room* activate(Room *r, Item *i, Direction::E dir);
        Room* go(Room *r, Direction::E d);
        int getCarryCapacity();
        int getCarryWeight();
        std::string getDescription();
    protected:
    private:
        int carry_capacity;
};

#endif // PLAYER_H
