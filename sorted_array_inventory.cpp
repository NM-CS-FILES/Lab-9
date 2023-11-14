#include "sorted_array_inventory.h"

sorted_array_inventory::sorted_array_inventory() {
	std::fill(begin(), end(), record());
}

bool sorted_array_inventory::add(const record& rec) {
	if (rec.number > 100 || rec.number <= 0) {
		return false;
	}

	m_container.at(rec.number - 1) = rec;

	return true;
}

bool sorted_array_inventory::remove(int number) {
	if (number > 100 || number <= 0) {
		return false;
	}

	m_container.at(number - 1) = record();

	return true;
}