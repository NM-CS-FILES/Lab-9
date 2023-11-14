#include "unsorted_list_inventory.h"

bool unsorted_list_inventory::add(const record& rec) {
	m_container.push_back(rec);
	return true;
}

bool unsorted_list_inventory::remove(int number) {
	iterator i = find(number);

	if (i == end()) {
		return false;
	}

	m_container.erase(i);

	return true;
}