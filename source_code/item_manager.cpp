#ifndef ITEM_MANAGER_CPP
#define ITEM_MANAGER_CPP

#include "item_manager.h"

ItemManager::ItemManager() {;}


void ItemManager::add_item(Item& item) {
    item_set_.push_back(&item);
}


std::vector<Item*> ItemManager::get_nearby_items(Point& point) {
    return item_set_;
} 

#endif