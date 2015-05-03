#include "Effect.h"

//FOR INFORMATION ABOUT THE CLASS AND ITS FUNCTION, PLEASE LOOK AT EFFECT.H

using namespace EffectType;

Effect::Effect(int s, int d, EffectType::E t, bool b) {
    strength = s;
    duration = d;
    type = t;
    buff = b;
    switch (type) {
        case ATTACK:
            name = "Attack";
            break;
        case EffectType::ARMOR:
            name = "Armor";
            break;
        case REGEN:
            name = "Regeneration";
            break;
        case LIFE_STEAL:
            name = "Life steal";
            break;
        case RUSH:
            name = "Rush";
            break;
        case FURY:
            name = "Fury";
            break;
        case IMMUNITY:
            name = "Immunity";
            break;
        case DODGE:
            name = "Dodge";
            break;
        case STEADY:
            name = "Accuracy";
            break;
        case BURN:
            name = "Burn";
            break;
        case FREEZE:
            name = "Freeze";
            break;
        case POISON:
            name = "Poison";
            break;
        case STUN:
            name = "Stun";
            break;
        case FEAR:
            name = "Fear";
            break;
        case BLIND:
            name = "Blind";
            break;
    }
}

Effect::~Effect() {
    delete this;
}

int Effect::getDuration() {
    return duration;
}

int Effect::getStrength() {
    return strength;
}

EffectType::E Effect::getType() {
    return type;
}

bool Effect::isBuff() {
    return buff;
}

bool Effect::isDebuff() {
    return !buff;
}

void Effect::reduce() {
    if (duration != -1) {
        duration--;
    }
}

std::string Effect::getName() {
    return name;
}

std::string Effect::getInfo() {
    switch (type) {
        case ATTACK:
            return "Attack: increase the physical damage done by attacks.";
        case EffectType::ARMOR:
            return "Armor: increases resistance to physical damage.";
        case REGEN:
            return "Regeneration: heals target over time.";
        case LIFE_STEAL:
            return "Life steal: adds an enemies life to your own.";
        case RUSH:
            return "Rush: be the first to attack (mostly).";
        case FURY:
            return "Fury: attack multiple times consecutively.";
        case IMMUNITY:
            return "Immunity: take no damage.";
        case DODGE:
            return "Dodge: there is a chance an incoming attack will miss.";
        case STEADY:
            return "Accuracy: the chance an attack lands is increased.";
        case BURN:
            return "Burn: effected's armor is halved.";
        case FREEZE:
            return "Freeze: cannot preform any actions.";
        case POISON:
            return "Poison: effected is damaged overtime.";
        case STUN:
            return "Stun: target cannot dodge.";
        case FEAR:
            return "Fear: attack damage is decreased.";
        case BLIND:
            return "Blind: accuracy is decreased.";

    }
}
