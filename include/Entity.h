#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include <Effect.h>
#include <Item.h>
#include <Room.h>

/** The Entity class describes the different monsters and people that can be interacted with throughout the game
  * It has many fields describing it: health, base_attack, base_armor, base_accuracy, base_dodge, inventory, effects, name, basic_description, resistances, movable, alive, passive, aggressive
  * health is the amount of hit points the entity has remaining
  * base attack, armor, accuracy, and dodge are the basic stats of the entity (items stats are added onto these).
  * attack is the amount of damage in combat, armor is an entity's resistance to damage, accuracy is how likely an attack will land, and dodge is how likely you will dodge an attack.
  * inventory is what items are being carried by the entity.
  * effects are the effects currently affect the entity
  * name and basic_description are fields that describe the entity in plain text
  * resistances is a list of what effects cannot affect the entity
  * movable is whether the entity can change rooms
  * passive is whether the entity will attack back when attacked
  * alive is whether the entity is alive. if an entity is not alive, it cannot preform actions.
  * aggressive is whether an entity wil attack you on sight
  */

using namespace EffectType;

class Room;
class Item;

class Entity
{
    public:
        Entity(int h, int att, int arm, int acc, int dod, std::vector<Item*> i, bool can_m, std::string nm, std::string b_d, bool a, bool pas, bool aggr, std::vector<EffectType::E> res);
        Entity() { }
        virtual ~Entity();
        //getters
        std::vector<Effect*> getEffects();
        std::vector<Item*> getItems();
        bool canMove();
        bool isAlive();
        bool isPassive();
        bool isAggressive();
        int getBaseAccuracy();
        int getBaseArmor();
        int getBaseAttack();
        int getBaseDodge();
        int getHealth();
        std::string getName();
        //build and returns a string describing the entity
        std::string getDescription();
        //drop attempts to remove an item from the entities inventory and put it in the room
        bool drop(Room *r, Item *i);
        //attack attempts to damage and debuff an entity
        int attack(Room *r, Entity *target);
        //hasItem returns true if inventory contains Item *i
        bool hasItem(Item *i);
        //refresh effects go through and checks if any effects should be removed and reduces all temporary effects
        void refreshEffects();
        //equip attempts to set equipped of Item *i to true
        bool equip(Item *i);
        //unequip attempts to set equipped of Item *i to false
        bool unequip(Item *item);
        //remove item attempts to remove an item from inventory
        Item* removeItem(Item *i);
        //damage decreases health by dmg. it also trues alive to false if health <= 0 after dmg is done
        void damage(int dmg);
        //add effect adds the effects of an item to an entity
        void addEffects(Item *i);
        //anger makes an entity aggressive if passive is false
        void anger();
        //get effect returns the effect of type t that has isBuff() == buff. returns NULL if no effect of that type is found
        Effect* getEffect(EffectType::E t, bool buff);
        //adds a single effect to the entity
        void addEffect(Effect *e);
        //returns 0 if this is the same entity as Entity *e. return 1 otherwise.
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
