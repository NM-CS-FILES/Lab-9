#include "record.h"

#pragma once

// represents a basic inventory using some iterator
// type checking is not performed, sorry :(

template<typename IterT>
class inventory {
public:
	using iterator = IterT;

	virtual bool add(const record& rec) = 0;
	virtual bool remove(int number) = 0;
	virtual bool update(const record& rec) = 0;

	virtual iterator begin() = 0;
	virtual iterator end() = 0;
	virtual iterator find(int number) = 0;
};
