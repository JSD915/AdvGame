#ifndef EFFECT_H
#define EFFECT_H
#include <string>

/** The following enums are different types classes (Effect and Item) can have.
  * Depending on the type, the object affects the game differently. The Effect class is used to store information about combat buffs
  * EffectType outlines the different functions it can serve.
  * Direction enum describes the different directions the player can move.
  * Item type describes what kinds of items are in the game. items of different types can be used in different ways.
  */

namespace EffectType {
    enum E { ATTACK, ARMOR, REGEN, LIFE_STEAL, RUSH, FURY, IMMUNITY, DODGE, STEADY, BURN, FREEZE, POISON, STUN, FEAR, BLIND };
}
namespace Direction {
     enum E { NORTH, SOUTH, EAST, WEST, UP, DOWN };
}
namespace ItemType {
    enum E { ARMOR, CONSUMABLE, USABLE, WEAPON, TREASURE, DECOR};
}

using namespace EffectType;

class Effect
{
    public:
        //s is the strength of the Effect, d is for how many in game ticks the effect will last. t is the type, and b is whether it is a buff or a debuff
        //when applied, a buff is beneficial and a debuff is harmful. For example, if a sword had a buff freeze, then the attacked would be frozen, but
        //if the player has the debuff freeze they would be frozen.
        Effect(int s, int d, EffectType::E t, bool b);
        //getters
        int getStrength();
        int getDuration();
        EffectType::E getType();
        bool isBuff();
        bool isDebuff();
        //decreases duration by one, used to keep track of how long the effect should stay active.
        void reduce();
        //these two function return strings describing the effect
        std::string getInfo();
        std::string getName();
        virtual ~Effect();
    protected:
    private:
        bool buff;
        int strength;
        int duration;
        std::string name;
        EffectType::E type;
};

#endif // EFFECT_H
