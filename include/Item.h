#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include <Effect.h>
#include <Room.h>

class Effect;
class Room;

using namespace ItemType;

class Item
{
    public:

        Item(std::vector<Effect*> e, std::string name, std::string b_d, int w, ItemType::E t, std::vector<Item*> i_i, std::vector<Room*> i_r);
        Item() {

        }
        virtual ~Item();
        std::vector<Effect*> getEffects();
        std::string getName();
        std::string getDescription();
        int getWeight();
        ItemType::E getType();
        bool isEquipped();
        void equip();
        void unequip();
        bool canThrow();
        std::vector<Item*> getInheritedItems();
        std::vector<Room*> getInheritedRooms();
        int Compare(Item *i) const {
            if (type != i->type) {
                return 1;
            }
            if (weight != i->weight) {
                return 1;
            }
            if (basic_description.compare(i->basic_description) != 0) {
                return 1;
            }
            return 0;
        }
    protected:
    private:
        std::vector<Effect*> effects;
        std::string name;
        std::string basic_description;
        bool throwable;
        int weight;
        ItemType::E type;
        bool equipped;
        std::vector<Item*> inheritedItems;
        std::vector<Room*> inheritedRooms;
};

#endif // ITEM_H
