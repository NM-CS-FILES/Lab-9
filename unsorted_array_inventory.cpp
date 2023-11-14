#include "unsorted_array_inventory.h"

unsorted_array_inventory::unsorted_array_inventory() {
	std::fill(begin(), end(), record());
}

bool unsorted_array_inventory::add(const record& rec) {
	if (rec.number > 100 || rec.number <= 0 || m_record_len == 100) {
		return false;
	}

	m_container.at(m_record_len) = rec;

	++m_record_len;

	return true;
}

bool unsorted_array_inventory::remove(int number) {
	iterator i = find(number);

	if (i == end()) {
		return false;
	}

	*i = record();
	--i;

	// shift elems in array to the left one index

	while (i != end()) {
		*(i - 1) = *i;
		++i;
	}

	--m_record_len;

	return true;
}