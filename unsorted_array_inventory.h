#include "container_inventory.h"
#include <array>

#pragma once


class unsorted_array_inventory : public container_inventory<std::array<record, 100>> {
protected:
	size_t m_record_len = 0;

public:
	unsorted_array_inventory();

	bool add(const record& rec);
	bool remove(int number);
};
