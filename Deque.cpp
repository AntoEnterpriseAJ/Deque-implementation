#include "Deque.h"
#include <iostream>

void printMenu()
{
	std::cout << "0.Print menu\t1. Push front\t2. Push back\t3. Pop front\t4. Pop back\n";
	std::cout << "5. Insert at position\t6. Delete at position\t7. Front\t8. Back\n";
	std::cout << "9. Print\t10. Size\t11. Clear\t12. Exit\n";
}

int main()
{
	Deque<int> myDeque;

	int choice;
	printMenu();
	do {
		std::cout << "Enter your choice: ";
		std::cin >> choice;

		switch (choice) {
		case 0:
		{
			printMenu();
			break;
		}
		case 1:
		{
			int element;
			std::cout << "Enter element to push front: ";
			std::cin >> element;
			myDeque.push_front(element);
			break;
		}
		case 2:
		{
			int element;
			std::cout << "Enter element to push back: ";
			std::cin >> element;
			myDeque.push_back(element);
			break;
		}
		case 3:
			myDeque.pop_front();
			break;
		case 4:
			myDeque.pop_back();
			break;
		case 5:
		{
			int element, pos;
			std::cout << "Enter element to insert: ";
			std::cin >> element;
			std::cout << "Enter position to insert at: ";
			std::cin >> pos;
			myDeque.insert(element, pos);
			break;
		}
		case 6:
		{
			int pos;
			std::cout << "Enter position to delete: ";
			std::cin >> pos;
			myDeque.deleteAtPos(pos);
			break;
		}
		case 7:
			try
			{
				std::cout << "Front element: " << myDeque.front() << std::endl;
			}
			catch (const std::out_of_range& e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 8:
			try
			{
				std::cout << "Back element: " << myDeque.back() << std::endl;
			}
			catch (const std::out_of_range& e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 9:
			std::cout << "Deque elements: ";
			myDeque.print();
			break;
		case 10:
			std::cout << "Deque size: " << myDeque.size() << std::endl;
			break;
		case 11:
			myDeque.clear();
			std::cout << "Deque cleared\n";
			break;
		case 12:
			std::cout << "Exiting...\n";
			break;
		default:
			std::cout << "Invalid choice\n";
		}
	} while (choice != 12);

	return 0;
}