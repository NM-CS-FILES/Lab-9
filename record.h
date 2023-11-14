#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>

#pragma once

struct record {
	int number;
	char name[21];
	int quantity;
	int cost; // fixed point 1010 is $10.10

	//

	record() : 
		number(0),
		name{ },
		quantity(0),
		cost(0)
	{
		// fill with spaces
		std::fill_n(name, sizeof(name) - 1, ' ');

		// null terminate
		name[sizeof(name) - 1] = '\0';
	}

	record(int number, std::string_view name_view, int quantity, int cost) :
		number(number),
		name{ },
		quantity(quantity),
		cost(cost)
	{
		// copy into string, 12 bytes of overflow 
		// protection still copied safely
		std::copy_n(
			name_view.begin(),
			std::min(sizeof(name) - 1, name_view.size()),
			name
		);

		// null terminate
		name[sizeof(name) - 1] = '\0';
	}
};


// Prints records in a similiar style to 
// the text in tabeltop.pdf. The escaped 
// characters are part of the extended ascii
// box characters supported by the windows command prompt
// lots of lambdas, more so because I couldn't think
// of a proper place to put functions

template<typename InIter>
void record_print_all(InIter begin, InIter end) {
	using std::to_string;
	using std::string;

	const char* top = "\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xD1\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xD1\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xD1\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
	const char* mid = "\xC7\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xB6\n";
	const char* bot = "\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCF\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCF\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCF\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC";

	printf(top);
	printf("\xBA Record \xB3         Name         \xB3 Quantity \xB3  Cost  \xBA\n");

	// center text for some string size
	
	auto center = [](string name, size_t size) {
		if (size < name.size())
			name.resize(size);

		if (size == name.size())
			return name;

		size_t r_half_len = (size - name.size()) / 2;
		size_t l_half_len = (size - name.size()) - r_half_len;

		name.reserve(size);

		name.insert(name.begin(), l_half_len, ' ');
		name.insert(name.end(), r_half_len, ' ');

		return name;
	};

	// format fixed point integer as dollars and cents
	
	auto format_cost = [](int cost) {
		int sz = snprintf(nullptr, 0, "%d.%02d", cost / 100, cost % 100);
		string cost_buf(sz, ' ');
		snprintf(cost_buf.data(), sz + 1, "%d.%02d", cost / 100, cost % 100);
		return cost_buf;
	};

	while (begin != end) {
		record rec = *begin;

		if (rec.number != 0) {
			printf(mid);
			printf("\xBA %s \xB3 %s \xB3 %s \xB3 %s \xBA\n",
				center(to_string(rec.number), 6).c_str(),
				center(rec.name, 20).c_str(),
				center(to_string(rec.quantity), 8).c_str(),
				center(format_cost(rec.cost), 6).c_str()
			);
		}
		
		++begin;
	}

	printf(bot);
	putchar('\n');
}
