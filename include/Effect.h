#ifndef EFFECT_H
#define EFFECT_H
#include <string>

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
        Effect(int s, int d, EffectType::E t);
        Effect(int s, int d, EffectType::E t, bool b);
        int getStrength();
        int getDuration();
        EffectType::E getType();
        bool isBuff();
        bool isDebuff();
        void reduce();
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
