#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include <Effect.h>
#include <Room.h>

class Effect;
class Room;

using namespace ItemType;

/** items are different objects that the player can interact with throughout the game.
  * Items have several states associated with them: effects, name, basic_description, throwable, equipped, inheritedRooms, and inheritedItems.
  * effects are a list of effects that given to the user of the item or the target of the item.
  * name and basic_description describe what the item is in plain text
  * throwable says whether the item can be throw at another entity. only certain consumables can be thrown
  * weight is just how much the item weighs, this only matters to the player
  * type is the item's type. there are 6 item type and each have different uses:
  * weapon (used to attack other entities), armor 9used for protection from damage, consumable (gives temporary effects), usable (unlocks doors or chests), treasure (gives points), and decor (for decoration only)
  * equipped is whether an item is equipped. only weapons and armor can be equipped
  * inheritedItems and inheritedRooms are fields used by USABLES, they stor information about what rooms can be opened and what items can be obtained from opening chests by that particular item.
  */

class Item
{
    public:

        Item(std::vector<Effect*> e, std::string name, std::string b_d, int w, ItemType::E t, std::vector<Item*> i_i, std::vector<Room*> i_r);
        Item() { }
        virtual ~Item();
        //getters
        std::vector<Effect*> getEffects();
        std::string getName();
        std::string getDescription();
        int getWeight();
        ItemType::E getType();
        bool isEquipped();
        bool canThrow();
        std::vector<Item*> getInheritedItems();
        std::vector<Room*> getInheritedRooms();
        //equip changed equipped to true
        void equip();
        //unequip changed equipped to false
        void unequip();
        //returns zero if this and Item *i are the same, otherwise returns 1
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
