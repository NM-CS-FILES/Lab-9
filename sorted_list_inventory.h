#include "unsorted_list_inventory.h"

#pragma once

class sorted_list_inventory : public unsorted_list_inventory {
public:
	bool add(const record& rec) override;
};
