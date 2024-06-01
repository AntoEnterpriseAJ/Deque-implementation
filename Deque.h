#include <iostream>
#include <vector>

template <typename T, int blockSize = 8>
class Deque
{
public:
    Deque();
    Deque(const Deque& other);
    Deque& operator=(const Deque& other);
    ~Deque();

    void push_front(T element);
    void push_back(T element);
    void pop_back();
    void pop_front();
    void insert(T element, int pos);
    void deleteAtPos(int pos);
    const T& front() const;
    const T& back() const;
    T& operator[](int index);
    bool empty() const;
    void clear();
    void print();
    int size() const;

private:
    struct Block
    {
        T* elements = new T[blockSize];
        bool used = false;
    };

    void resizeBlocks();

    std::vector<Block*> m_blocks;
    int m_frontBlockIndex;
    int m_backBlockIndex;
    int m_frontElementIndex;
    int m_backElementIndex;
    int m_size;
};

template <typename T, int blockSize>
Deque<T, blockSize>::Deque()
    : m_frontBlockIndex{ 0 }, m_backBlockIndex{ 0 }, m_frontElementIndex{ -1 }, m_backElementIndex{ -1 }
{
    m_blocks.resize(1);
    m_blocks[0] = new Block;
}

template<typename T, int blockSize>
Deque<T, blockSize>::Deque(const Deque& other)
{
	m_blocks.resize(other.m_blocks.size());
    for (int i = 0; i < other.m_blocks.size(); ++i)
    {
        if (other.m_blocks[i] != nullptr && other.m_blocks[i]->used)
        {
			m_blocks[i] = new Block;
            for (int j = 0; j < blockSize; ++j)
            {
				m_blocks[i]->elements[j] = other.m_blocks[i]->elements[j];
			}
			m_blocks[i]->used = true;
		}
        else
        {
			m_blocks[i] = nullptr;
		}
	}

	m_frontBlockIndex = other.m_frontBlockIndex;
	m_backBlockIndex = other.m_backBlockIndex;
	m_frontElementIndex = other.m_frontElementIndex;
	m_backElementIndex = other.m_backElementIndex;
	m_size = other.m_size;
}

template<typename T, int blockSize>
Deque<T, blockSize>& Deque<T, blockSize>::operator=(const Deque& other)
{
    if (this != &other)
    {
        for (int i = 0; i < m_blocks.size(); ++i)
        {
            delete[] m_blocks[i];
        }

        m_blocks.resize(other.m_blocks.size());
        for (int i = 0; i < m_blocks.size(); ++i)
        {
            if (other.m_blocks[i] != nullptr && other.m_blocks[i]->used)
            {
                m_blocks[i] = new Block;
                for (int j = 0; j < blockSize; ++j)
                {
                    m_blocks[i]->elements[j] = other.m_blocks[i]->elements[j];
                }
                m_blocks[i]->used = true;
            }
            else
            {
				m_blocks[i] = nullptr;
			}
        }

        m_frontBlockIndex = other.m_frontBlockIndex;
        m_backBlockIndex = other.m_backBlockIndex;
        m_frontElementIndex = other.m_frontElementIndex;
        m_backElementIndex = other.m_backElementIndex;
        m_size = other.m_size;
    }

    return *this;
}

template <typename T, int blockSize>
Deque<T, blockSize>::~Deque()
{
    for (int i = m_frontBlockIndex; i < m_backBlockIndex; ++i)
    {
        delete[] m_blocks[i];
    }
}

template <typename T, int blockSize>
void Deque<T, blockSize>::resizeBlocks()
{
    int newSize;
    if (m_blocks.size() == 1)
    {
        newSize = 4;
    }
    else
    {
        newSize = m_blocks.size() * 2;
    }

    int numberOfElements = m_backBlockIndex - m_frontBlockIndex + 1;
    std::vector<Block*> temp(numberOfElements);
    int index = 0;

    for (int i = m_frontBlockIndex; i <= m_backBlockIndex; ++i)
    {
        temp[index++] = m_blocks[i];
    }

    m_blocks.clear();
    m_blocks.resize(newSize, nullptr);
    for (int i = 0; i < temp.size(); ++i)
    {
        m_blocks[(newSize - numberOfElements) / 2 + i] = temp[i];
    }

    m_frontBlockIndex = (newSize - numberOfElements) / 2;
    m_backBlockIndex = m_frontBlockIndex + numberOfElements - 1;
}

template <typename T, int blockSize>
void Deque<T, blockSize>::push_front(T element)
{
    if (m_frontElementIndex == -1) //first push
    {
        m_frontElementIndex = blockSize - 1;
        if (m_backElementIndex == -1)
        {
            m_backElementIndex = m_frontElementIndex;
        }

        m_blocks[m_frontBlockIndex]->elements[m_frontElementIndex] = element;
    }
    else if (m_frontElementIndex == 0) //the block we are trying to push into is full
    {
        if (m_frontBlockIndex > 0) //there's space left in the blocks vector
        {
            m_blocks[--m_frontBlockIndex] = new Block;
            m_frontElementIndex = blockSize - 1;
            m_blocks[m_frontBlockIndex]->elements[m_frontElementIndex] = element;
        }
        else //no space left in the blocks vector, we resize
        {
            resizeBlocks();
            m_frontElementIndex = blockSize - 1;
            m_blocks[--m_frontBlockIndex] = new Block;
            m_blocks[m_frontBlockIndex]->elements[m_frontElementIndex] = element;
        }
    }
    else
    {
        m_frontElementIndex--;
        m_blocks[m_frontBlockIndex]->elements[m_frontElementIndex] = element;
    }
    m_size++;
    m_blocks[m_frontBlockIndex]->used = true;
}

template <typename T, int blockSize>
void Deque<T, blockSize>::push_back(T element)
{
    if (m_backElementIndex == -1) //first push
    {
        m_backElementIndex = 0;
        if (m_frontElementIndex == -1)
        {
            m_frontElementIndex = m_backElementIndex;
        }
        m_blocks[m_backBlockIndex]->elements[m_backElementIndex] = element;
    }
    else if (m_backElementIndex == blockSize - 1) //the block we are trying to push into is full
    {
        if (m_backBlockIndex < m_blocks.size() - 1) //there's space left in the blocks vector
        {
            m_blocks[++m_backBlockIndex] = new Block;
            m_backElementIndex = 0;
            m_blocks[m_backBlockIndex]->elements[m_backElementIndex] = element;
        }
        else //no space left in the blocks vector, we resize
        {
            resizeBlocks();
            m_backElementIndex = 0;
            m_blocks[++m_backBlockIndex] = new Block;
            m_blocks[m_backBlockIndex]->elements[m_backElementIndex] = element;
        }
    }
    else
    {
        m_backElementIndex++;
        m_blocks[m_backBlockIndex]->elements[m_backElementIndex] = element;
    }
    m_size++;
    m_blocks[m_backBlockIndex]->used = true;
}

template <typename T, int blockSize>
void Deque<T, blockSize>::pop_back()
{
    if (m_size == 0)
    {
        std::cout << "pop_back: already empty\n";
        return;
    }

    if (m_frontBlockIndex == m_backBlockIndex && m_frontElementIndex == m_backElementIndex) // it's the only element
    {
        /*debug:*/ std::cout << "poppedBack: " << m_blocks[m_backBlockIndex]->elements[m_backElementIndex] << '\n';
        m_frontElementIndex = m_backElementIndex = -1;
        m_size--;
    }
    else if (m_backElementIndex != -1)
    {
        /*debug:*/ std::cout << "poppedBack: " << m_blocks[m_backBlockIndex]->elements[m_backElementIndex] << '\n';

        if (m_backElementIndex == 0) //the only element in the block
        {
            int next = m_backBlockIndex + 1;
            //if there's an unused next block, we deallocate it
            if (m_backBlockIndex < m_blocks.size() - 1 && m_blocks[next] != nullptr && m_blocks[next]->used == false)
            {
                delete[] m_blocks[m_backBlockIndex + 1]->elements;
                delete m_blocks[m_backBlockIndex + 1];
                m_blocks[m_backBlockIndex + 1] == nullptr;
            }

            m_blocks[m_backBlockIndex]->used = false;
            m_backBlockIndex--;

            m_backElementIndex = blockSize - 1;
        }
        else
        {
            m_backElementIndex--;
        }
        m_size--;
    }
}

template <typename T, int blockSize>
void Deque<T, blockSize>::pop_front()
{
    if (m_size == 0)
    {
        std::cout << "pop_front: already empty\n";
        return;
    }

    if (m_frontBlockIndex == m_backBlockIndex && m_frontElementIndex == m_backElementIndex) //it's the only element
    {
        /*debug:*/ std::cout << "poppedFront: " << m_blocks[m_frontBlockIndex]->elements[m_frontElementIndex] << '\n';
        m_frontElementIndex = m_backElementIndex = -1;
        m_size--;
    }
    else if (m_frontElementIndex != -1)
    {
        /*debug:*/ std::cout << "poppedFront: " << m_blocks[m_frontBlockIndex]->elements[m_frontElementIndex] << '\n';

        if (m_frontElementIndex == blockSize - 1) //the only element in the block
        {
            int prev = m_frontBlockIndex - 1;
            //if there's an unused previous block, we deallocate it
            if (m_frontBlockIndex > 0 && m_blocks[prev] != nullptr && m_blocks[prev]->used == false)
            {
                delete[] m_blocks[m_frontBlockIndex - 1]->elements;
                delete m_blocks[m_frontBlockIndex - 1];
                m_blocks[m_frontBlockIndex - 1] == nullptr;
            }

            m_blocks[m_frontBlockIndex]->used = false;
            m_frontBlockIndex++;

            m_frontElementIndex = 0;
        }
        else
        {
            m_frontElementIndex++;
        }
        m_size--;
    }
}

template <typename T, int blockSize>
void Deque<T, blockSize>::insert(T element, int pos)
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

        //push the last element back
        push_back(lastElement);
    }
}

template <typename T, int blockSize>
void Deque<T, blockSize>::deleteAtPos(int pos)
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
        //shift elements to the left
        for (int i = pos; i < m_size - 1; ++i)
        {
            (*this)[i] = (*this)[i + 1];
        }
        pop_back();
    }
}

template <typename T, int blockSize>
const T& Deque<T, blockSize>::front() const
{
    if (m_frontElementIndex != -1)
    {
        return m_blocks[m_frontBlockIndex]->elements[m_frontElementIndex];
    }
    throw std::out_of_range("front: container is empty");
}

template <typename T, int blockSize>
const T& Deque<T, blockSize>::back() const
{
    if (m_backElementIndex != -1)
    {
        return m_blocks[m_backBlockIndex]->elements[m_backElementIndex];
    }
    throw std::out_of_range("front: container is empty");
}

template <typename T, int blockSize>
T& Deque<T, blockSize>::operator[](int index)
{
    int elementIndex = (m_frontElementIndex + index) % blockSize;
    int blockIndex = m_frontBlockIndex + (m_frontElementIndex + index) / blockSize;
    return m_blocks[blockIndex]->elements[elementIndex];
}

template <typename T, int blockSize>
bool Deque<T, blockSize>::empty() const
{
    return m_size == 0;
}

template <typename T, int blockSize>
void Deque<T, blockSize>::clear()
{
    for (int i = m_frontBlockIndex; i < m_backBlockIndex; ++i)
    {
        delete[] m_blocks[i];
    }
    m_blocks.clear();
    m_blocks.resize(1);
    m_blocks[0] = new Block;
    m_frontBlockIndex = m_backBlockIndex = 0;
    m_frontElementIndex = m_backElementIndex = -1;
    m_size = 0;
}

template <typename T, int blockSize>
void Deque<T, blockSize>::print()
{
    for (int i = 0; i < m_size; ++i)
    {
        std::cout << (*this)[i] << " ";
    }
    std::cout << '\n';
}

template <typename T, int blockSize>
int Deque<T, blockSize>::size() const
{
    return m_size;
}