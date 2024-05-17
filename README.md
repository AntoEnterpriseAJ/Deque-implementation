# Deque Implementation in C++

## Overview

This project is an implementation of a double-ended queue (deque) in C++. The deque is a dynamic data structure that allows insertion and deletion of elements from both ends efficiently. This implementation starts with a single block of memory containing 8 elements and dynamically allocates new blocks as needed. When two successive blocks become empty, the furthest one is deallocated.

## Features

- **Dynamic Memory Allocation:** The deque starts with a single block of 8 elements and dynamically resizes by adding new blocks when needed. After resizing, elements are recentered in the vector of blocks to maintain balance.
  
- **Efficient Memory Management:** Unused blocks are deallocated when two successive blocks become empty, optimizing memory usage.

- **Support for Common Operations:**
  - `push_front`: Insert an element at the front of the deque.
  - `push_back`: Insert an element at the back of the deque.
  - `pop_front`: Remove an element from the front of the deque.
  - `pop_back`: Remove an element from the back of the deque.
  - `insert`: Insert an element at a specified position.
  - `deleteAtPos`: Delete an element from a specified position.
  - `front`: Access the front element.
  - `back`: Access the back element.
  - `operator[]`: Access elements by index.
  - `empty`: Check if the deque is empty.
  - `clear`: Clear all elements from the deque.
  - `print`: Print all elements of the deque.
  - `size`: Get the number of elements in the deque.

## Contributing

This project was created for study purposes. Contributions are welcome but not actively sought.


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
