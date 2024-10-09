#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include "item.cpp"

class ItemManager {

private:

    std::vector<Item*> item_set_;

public:

    ItemManager();
    void add_item(Item& item);
    std::vector<Item*> get_nearby_items(Point& point);

};

#endif