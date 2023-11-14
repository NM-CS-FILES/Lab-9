#include "sorted_list_inventory.h"

bool sorted_list_inventory::add(const record& rec) {
	iterator i = begin();

	while (i != end() && i->number < rec.number) {
		++i;
	}

	if (i == end()) {
		m_container.push_back(rec);
	}
	else {
		m_container.insert(i, rec);
	}

	return true;
}