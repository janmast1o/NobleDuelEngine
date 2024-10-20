#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include "item.h"

class ItemManager {

private:

    std::vector<Item*> item_set_;

public:

    ItemManager();
    void add_item(Item& item);
    void manage_scheduled_running();
    std::vector<Item*> get_nearby_items(Point& point);

};

#endif