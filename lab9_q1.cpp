/*
Note, it's 3:52am at the moment
I'm just trying to get this done
I'm not trying to over-engineer or
be crazy with my implementation,
this just seemed like the laziest approach,
or maybe just the most abstract and oo'd.
Better than rewriting the same code 5 times.

it's now 6:50am,
previous abstractions failed, now I'm left with this
I'm adding sparse documentation now, maybe it would have been
easier to just write the same code 5 times

Fun Note, just read "Would your conclusion change if you used a doubly-linked list?",
I've not been using `std::forward_list` and will not at this point.
My implementation uses doubly linked lists and that's a lot better for insertions
Knock my grade down all you want...
*/

#include <iostream>

#include "prompt.h"

#include "unsorted_list_inventory.h"
#include "sorted_list_inventory.h"

#include "unsorted_array_inventory.h"
#include "sorted_array_inventory.h"

#include "rand_access_inventory.h"

//
//

record user_get_record() {
	record rec;
	std::string rec_name_buf;

	// get number from user
	rec.number = prompt_ensure<int>("Enter Record Number: ", [](int& i) { return i > 0 && i <= 100; });

	// get name from user, truncate if needed
	rec_name_buf = prompt_ensure<std::string>("Enter Record Name: ", [](std::string& s) { return !s.empty(); });
	std::copy_n(rec_name_buf.data(), std::min(sizeof(rec.name), rec_name_buf.size() + 1), rec.name);

	// get quantity from user
	rec.quantity = prompt_ensure<int>("Enter Record Quantity: ");

	// get cost from user
	rec.cost = (int)(prompt_ensure<double>("Enter Record Price: ") * 100);

	return rec;
}

template<typename IterT>
void user_loop(inventory<IterT>& inv) {
	inv.add(record(68,	"Screwdriver",	   106, 699));
	inv.add(record(17,	"Hammer",		   76,	1199));
	inv.add(record(56,	"Power saw",	   18,	9999));
	inv.add(record(3,	"Electric Sander", 7,	5700));
	inv.add(record(83,	"Wrench",		   34,	750));
	inv.add(record(24,	"Jig Saw",		   21,	1100));
	inv.add(record(39,	"Lawn mower",	   3,	7950));
	inv.add(record(77,	"Sledge hammer",   11,	2150));

	bool flag = true;
	bool status = false;
	int number = 0;
	IterT beg = { };
	IterT end = { };

	while (flag) {
		printf("1: Add\n2: Update\n3: Delete\n4: Show\n5: Show all\nq: Quit\n");

		char in = tolower(prompt<char>("Action: ", '\0'));

		// "Use switch statements" they said,
		// "The compiler isn't smarter enough to optimize if-elif" they said
		switch (tolower(in))
		{
		case '1':
			status = inv.add(user_get_record());
			printf(status ? "Added Record\n" : "Unable To Add Record\n");
			break;

		case '2':
			status = inv.update(user_get_record());
			printf(status ? "Updated Record\n" : "Unable to Update Record\n");
			break;

		case '3':
			number = prompt_ensure<int>("Enter Record Number: ", [](int& i) { return i > 0 && i <= 100; });
			status = inv.remove(number);
			printf(status ? "Deleted Record\n" : "Unable to Delete Record\n");
			break;

		case '4':
			number = prompt_ensure<int>("Enter Record Number: ", [](int& i) { return i > 0 && i <= 100; });
			beg = inv.find(number);
			if (beg == inv.end()) {
				printf("Record Does Not Exist\n");
			}
			else {
				end = beg;
				++end;
				record_print_all(beg, end);
			}
			break;

		case '5':
			record_print_all(inv.begin(), inv.end());
			break;

		case 'q':
			flag = false;
			break;
		}
	}

}

int main(int argc, char** argv) {
	unsorted_list_inventory		unsorted_list_inv;
	sorted_list_inventory		sorted_list_inv;
	
	unsorted_array_inventory	unsorted_arr_inv;
	sorted_array_inventory		sorted_arr_inv;

	rand_access_inventory		rand_access_inv;

	printf("1: unsorted list\n2: sorted list\n3: unsorted array\n4: sorted array\n5: random access\n");
	char opt = prompt<char>("Enter inventory type: ", '1');

	switch (opt)
	{
	case '1':	user_loop(unsorted_list_inv);	break;
	case '2':	user_loop(sorted_list_inv);		break;
	case '3':	user_loop(unsorted_arr_inv);	break;
	case '4':	user_loop(sorted_arr_inv);		break;
	case '5':	user_loop(rand_access_inv);		break;

	default:
		printf("Invalid Option Entered\n");
	}
}
