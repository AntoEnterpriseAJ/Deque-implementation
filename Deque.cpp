#include <iostream>
#include <vector>

template <typename T, int blockSize = 8>
class Deque
{
	struct Block
	{
		T* elements = new T[blockSize];
		bool used = false;
	};

	std::vector<Block*> blocks;
	int frontBlockIndex;
	int backBlockIndex;
	int frontElementIndex;
	int backElementIndex;
	int m_size;

	//we resize when we hit left/right bound in the blocks vector
	void resizeBlocks()
	{
		int newSize;
		if (blocks.size() == 1)
		{
			newSize = 4;
		}
		else
		{
			newSize = blocks.size() * 2;
		}

		int numberOfElements = backBlockIndex - frontBlockIndex + 1;
		std::vector<Block*> temp(numberOfElements);
		int index = 0;

		for (int i = frontBlockIndex; i <= backBlockIndex; ++i)
		{
			temp[index++] = blocks[i];
		}

		blocks.clear();
		blocks.resize(newSize, nullptr);
		for (int i = 0; i < temp.size(); ++i)
		{
			blocks[(newSize - numberOfElements) / 2 + i] = temp[i];
		}

		frontBlockIndex = (newSize - numberOfElements) / 2;
		backBlockIndex = frontBlockIndex + numberOfElements - 1;
	}

public:
	Deque()
		: frontBlockIndex{ 0 }, backBlockIndex{ 0 }, frontElementIndex{ -1 }, backElementIndex{ -1 }
	{
		blocks.resize(1);
		blocks[0] = new Block;
	}

	~Deque()
	{
		for (int i = frontBlockIndex; i < backBlockIndex; ++i)
		{
			delete[] blocks[i];
		}
	}

	void push_front(T element)
	{
		if (frontElementIndex == -1) //first push
		{
			frontElementIndex = blockSize - 1;
			if (backElementIndex == -1)
			{
				backElementIndex = frontElementIndex;
			}

			blocks[frontBlockIndex]->elements[frontElementIndex] = element;
		}
		else if (frontElementIndex == 0) //the block we are trying to push into is full
		{
			if (frontBlockIndex > 0) //there's space left in the blocks vector
			{
				blocks[--frontBlockIndex] = new Block;
				frontElementIndex = blockSize - 1;
				blocks[frontBlockIndex]->elements[frontElementIndex] = element;
			}
			else //no space left in the blocks vector, we resize
			{
				resizeBlocks();
				frontElementIndex = blockSize - 1;
				blocks[--frontBlockIndex] = new Block;
				blocks[frontBlockIndex]->elements[frontElementIndex] = element;
			}
		}
		else
		{
			frontElementIndex--;
			blocks[frontBlockIndex]->elements[frontElementIndex] = element;
		}
		m_size++;
		blocks[frontBlockIndex]->used = true;
	}

	void push_back(T element)
	{
		if (backElementIndex == -1) //first push
		{
			backElementIndex = 0;
			if (frontElementIndex == -1)
			{
				frontElementIndex = backElementIndex;
			}
			blocks[backBlockIndex]->elements[backElementIndex] = element;
		}
		else if (backElementIndex == blockSize - 1) //the block we are trying to push into is full
		{
			if (backBlockIndex < blocks.size() - 1) //there's space left in the blocks vector
			{
				blocks[++backBlockIndex] = new Block;
				backElementIndex = 0;
				blocks[backBlockIndex]->elements[backElementIndex] = element;
			}
			else //no space left in the blocks vector, we resize
			{
				resizeBlocks();
				backElementIndex = 0;
				blocks[++backBlockIndex] = new Block;
				blocks[backBlockIndex]->elements[backElementIndex] = element;
			}
		}
		else
		{
			backElementIndex++;
			blocks[backBlockIndex]->elements[backElementIndex] = element;
		}
		m_size++;
		blocks[backBlockIndex]->used = true;
	}

	void pop_back()
	{
		if (m_size == 0)
		{
			std::cout << "pop_back: already empty\n";
			return;
		}

		if (frontBlockIndex == backBlockIndex && frontElementIndex == backElementIndex) // it's the only element
		{
			/*debug:*/ std::cout << "poppedBack: " << blocks[backBlockIndex]->elements[backElementIndex] << '\n';
			frontElementIndex = backElementIndex = -1;
			m_size--;
		}
		else if (backElementIndex != -1)
		{
			/*debug:*/ std::cout << "poppedBack: " << blocks[backBlockIndex]->elements[backElementIndex] << '\n';

			if (backElementIndex == 0) //the only element in the block
			{
				int next = backBlockIndex + 1;
				//if there's an unused next block, we deallocate it
				if (backBlockIndex < blocks.size() - 1 && blocks[next] != nullptr && blocks[next]->used == false)
				{
					delete[] blocks[backBlockIndex + 1]->elements;
					delete blocks[backBlockIndex + 1];
					blocks[backBlockIndex + 1] == nullptr;
				}

				blocks[backBlockIndex]->used = false;
				backBlockIndex--;

				backElementIndex = blockSize - 1;
			}
			else
			{
				backElementIndex--;
			}
			m_size--;
		}

	}

	void pop_front()
	{
		if (m_size == 0)
		{
			std::cout << "pop_front: already empty\n";
			return;
		}

		if (frontBlockIndex == backBlockIndex && frontElementIndex == backElementIndex) //it's the only element
		{
			/*debug:*/ std::cout << "poppedFront: " << blocks[frontBlockIndex]->elements[frontElementIndex] << '\n';
			frontElementIndex = backElementIndex = -1;
			m_size--;
		}
		else if (frontElementIndex != -1)
		{
			/*debug:*/ std::cout << "poppedFront: " << blocks[frontBlockIndex]->elements[frontElementIndex] << '\n';

			if (frontElementIndex == blockSize - 1) //the only element in the block
			{
				int prev = frontBlockIndex - 1;
				//if there's an unused previous block, we deallocate it
				if (frontBlockIndex > 0 && blocks[prev] != nullptr && blocks[prev]->used == false)
				{
					delete[] blocks[frontBlockIndex - 1]->elements;
					delete blocks[frontBlockIndex - 1];
					blocks[frontBlockIndex - 1] == nullptr;
				}

				blocks[frontBlockIndex]->used = false;
				frontBlockIndex++;

				frontElementIndex = 0;
			}
			else
			{
				frontElementIndex++;
			}
			m_size--;
		}
	}

	void insert(T element, int pos)
	{
		if (pos < 0 || pos > m_size)
		{
			std::cout << "insert: invalid position\n";
			return;
		}

		if (pos == 0)
		{
			push_front(element);
		}
		else if (pos == m_size)
		{
			push_back(element);
		}
		else
		{
			T lastElement = (*this)[m_size - 1];
			for (int i = m_size - 1; i >= pos + 1; --i)
			{
				(*this)[i] = (*this)[i - 1];
			}
			(*this)[pos] = element;


			push_back(lastElement);
		}
	}

	void deleteAtPos(int pos)
	{
		if (pos < 0 || pos >= m_size)
		{
			std::cout << "delete: invalid position\n";
			return;
		}

		if (pos == 0)
		{
			pop_front();
		}
		else if (pos == m_size - 1)
		{
			pop_back();
		}
		else
		{
			for (int i = pos; i < m_size - 1; ++i)
			{
				(*this)[i] = (*this)[i + 1];
			}
			pop_back();
		}
	}

	const T& front()
	{
		if (frontElementIndex != -1)
		{
			return blocks[frontBlockIndex]->elements[frontElementIndex];
		}
		throw std::out_of_range("front: container is empty");
	}

	const T& back()
	{
		if (backElementIndex != -1)
		{
			return blocks[backBlockIndex]->elements[backElementIndex];
		}
		throw std::out_of_range("front: container is empty");
	}

	T& operator[](int index)
	{
		int elementIndex = (frontElementIndex + index) % blockSize;
		int blockIndex = frontBlockIndex + (frontElementIndex + index) / blockSize;
		return blocks[blockIndex]->elements[elementIndex];
	}

	bool empty()
	{
		return m_size == 0;
	}

	void clear()
	{
		for (int i = frontBlockIndex; i < backBlockIndex; ++i)
		{
			delete[] blocks[i];
		}
		blocks.clear();
		blocks.resize(1);
		blocks[0] = new Block;
		frontBlockIndex = backBlockIndex = 0;
		frontElementIndex = backElementIndex = -1;
		m_size = 0;
	}

	void print()
	{
		for (int i = 0; i < m_size; ++i)
		{
			std::cout << (*this)[i] << " ";
		}
		std::cout << '\n';
	}

	int size()
	{
		return m_size;
	}
};

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