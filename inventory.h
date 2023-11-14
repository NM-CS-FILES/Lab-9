#include "record.h"

#pragma once

template<typename IterT>
class inventory {
public:
	using iterator = IterT;

	virtual bool add(const record& rec) = 0;
	virtual bool remove(int number) = 0;
	virtual bool update(const record& rec) = 0;

	virtual IterT begin() = 0;
	virtual IterT end() = 0;
	virtual IterT find(int number) = 0;
};