#include "container_inventory.h"
#include <list>

#pragma once

class unsorted_list_inventory : public container_inventory<std::list<record>> {
public:
	bool add(const record& rec);
	bool remove(int number);
};