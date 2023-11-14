#include "rand_access_inventory.h"

rand_access_inventory_value::rand_access_inventory_value(std::fstream& fio, size_t idx) :
	m_fio(fio),
	m_idx(idx)
{ }

rand_access_inventory_value& rand_access_inventory_value::operator=(const record& rec) {
	m_fio.seekp(m_idx * sizeof(record));
	m_fio.write((const char*)&rec, sizeof(record));
	return *this;
}

//
//
//

rand_access_inventory_iterator::rand_access_inventory_iterator() :
	m_fio(nullptr),
	m_idx(0)
{ }

rand_access_inventory_iterator::rand_access_inventory_iterator(std::fstream& fio, size_t idx) :
	m_fio(&fio),
	m_idx(idx)
{ }

rand_access_inventory_value rand_access_inventory_iterator::operator*() {
	return rand_access_inventory_value(*m_fio, m_idx);
}

rand_access_inventory_iterator& rand_access_inventory_iterator::operator++() {
	++m_idx;
	return *this;
}

rand_access_inventory_iterator rand_access_inventory_iterator::operator++(int) {
	rand_access_inventory_iterator cpy = *this;
	++(*this);
	return cpy;
}

rand_access_inventory_iterator& rand_access_inventory_iterator::operator--() {
	--m_idx;
	return *this;
}

rand_access_inventory_iterator& rand_access_inventory_iterator::operator--(int) {
	rand_access_inventory_iterator cpy = *this;
	--(*this);
	return *this;
}

bool operator==(
	const rand_access_inventory_iterator& l,
	const rand_access_inventory_iterator& r
	) {
	return l.m_fio == r.m_fio && l.m_idx == r.m_idx;
}

bool operator!=(
	const rand_access_inventory_iterator& l,
	const rand_access_inventory_iterator& r
	) {
	return !(l == r);
}

//
//
//

rand_access_inventory::rand_access_inventory(const std::string& file_name) {
	std::ofstream fout(file_name);
	record rec;

	// initialize to 100 empty records
	for (size_t i = 0; i != 100; i++) {
		fout.write((const char*)&rec, sizeof(record));
	}

	fout.close();

	m_fio = std::fstream(file_name, std::ios::in | std::ios::out);
}


bool rand_access_inventory::add(const record& rec) {
	if (rec.number > 100 || rec.number <= 0) {
		return false;
	}

	iterator i(m_fio, rec.number - 1);
	*i = rec;

	return true;
}

bool rand_access_inventory::remove(int number) {
	if (number > 100 || number <= 0) {
		return false;
	}

	iterator i(m_fio, number - 1);
	*i = record();

	return true;
}

bool rand_access_inventory::update(const record& rec) {
	return add(rec);
}

rand_access_inventory::iterator rand_access_inventory::find(int number) {
	if (number > 100 || number <= 0) {
		return end();
	}

	return iterator(m_fio, number - 1);
}