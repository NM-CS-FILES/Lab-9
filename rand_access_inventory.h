#include "inventory.h"
#include <fstream>

#pragma once


// wow, where do I begin, this class
// was made to provide iterator-like
// access to a file stream, treating
// it more like an array

class rand_access_inventory_value {
	std::fstream& m_fio;
	size_t m_idx;

public:
	rand_access_inventory_value(std::fstream& fio, size_t idx);

	// support for `*iter = record();`
	rand_access_inventory_value& operator=(const record& rec);

	// support for `record r = *iter;`
	operator record() {
		record rec;
		m_fio.seekg(m_idx * sizeof(record));
		m_fio.read((char*)&rec, sizeof(record));
		return rec;
	}
};


// as far as I know file streams do
// not have iterators, this class
// attempts to change that, it keeps
// a pointer to a file stream and it's
// index, again treating the stream as
// an array

class rand_access_inventory_iterator {
	std::fstream* m_fio;
	size_t m_idx;

public:
	rand_access_inventory_iterator();

	rand_access_inventory_iterator(std::fstream& fio, size_t idx);

	rand_access_inventory_value operator*();
	rand_access_inventory_iterator& operator++();
	rand_access_inventory_iterator operator++(int);
	rand_access_inventory_iterator& operator--();
	rand_access_inventory_iterator operator--(int);

	friend bool operator==(
		const rand_access_inventory_iterator& l,
		const rand_access_inventory_iterator& r
	);

	friend bool operator!=(
		const rand_access_inventory_iterator& l,
		const rand_access_inventory_iterator& r
	);
};


// specialization of inventory that deals
// with file streams

class rand_access_inventory : public inventory<rand_access_inventory_iterator> {
	std::fstream m_fio;

public:
	using iterator = rand_access_inventory_iterator;

	rand_access_inventory(const std::string& file_name = "rand_inv.dat");

	inline iterator begin() { return iterator(m_fio, 0); }
	inline iterator end() { return iterator(m_fio, 100); }

	bool add(const record& rec);
	bool remove(int number);
	bool update(const record& rec);
	iterator find(int number);
};

