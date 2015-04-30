#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include <Effect.h>
#include <Item.h>
#include <Room.h>

using namespace EffectType;

class Room;
class Item;

class Entity
{
    public:
        Entity(int h, int att, int arm, int acc, int dod, std::vector<Item*> i, bool can_m, std::string nm, std::string b_d, bool a, bool pas, bool aggr, std::vector<EffectType::E> res);
        Entity() {

        }
        virtual ~Entity();
        std::vector<Effect*> getEffects();
        bool drop(Room *r, Item *i);
        int attack(Room *r, Entity *target);
        std::vector<Item*> getItems();
        bool hasItem(Item *i);
        void refreshEffects();
        bool equip(Item *i);
        bool unequip(Item *item);
        Item* removeItem(Item *i);
        bool canMove();
        bool isAlive();
        bool isPassive();
        bool isAggressive();
        int getBaseAccuracy();
        int getBaseArmor();
        int getBaseAttack();
        int getBaseDodge();
        void damage(int dmg);
        int getHealth();
        void addEffects(Item *i, bool buff); //Only used for consumables!!!
        void anger();
        std::string getName();
        std::string getDescription();
        Effect* getEffect(EffectType::E t);
        int Compare(Entity* e) const {
            if (health != e->health) {
                return 1;
            }
            if (basic_description.compare(e->basic_description) != 0) {
                return 1;
            }
            if (alive != e->alive) {
                return 1;
            }
            return 0;
        }
    protected:
        int health;
        int base_attack; //GETTERS
        int base_armor;
        int base_accuray;
        int base_dodge;
        std::vector<Item*> inventory;
        std::vector<Effect*> effects;
        std::string name;
        std::string basic_description;
        std::vector<EffectType::E> resistances;
    private:
        bool movable;
        bool alive;
        bool passive;
        bool aggressive;
};

#endif // ENTITY_H
