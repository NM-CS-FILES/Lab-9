#include "inventory.h"

#pragma once

template<typename Container>
class container_inventory : public inventory<typename Container::iterator> {
public:
	Container m_container;

public:
	using iterator = typename Container::iterator;

	iterator begin() { return m_container.begin(); }
	iterator end() { return m_container.end(); }

	bool update(const record& rec) {
		iterator i = this->find(rec.number);

		if (i == end()) {
			return false;
		}

		*i = rec;

		return true;
	}

	// linear search
	iterator find(int number) {
		iterator i = begin();

		while (i != end() && i->number != number) {
			++i;
		}

		return i;
	}
};