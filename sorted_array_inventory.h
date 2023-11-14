#include "container_inventory.h"
#include <array>

#pragma once

class sorted_array_inventory : public container_inventory<std::array<record, 100>> {
public:
	sorted_array_inventory();

	bool add(const record & rec);
	bool remove(int number);
};