#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>
#include <cstdlib>
#include <exception>
#include <algorithm>

class Heap_exception : public std::exception {
};

template<typename T, bool (*comp)(T, T)>
class Heap {
private:
    std::vector<T *> data{};

    const size_t FIRST_ELEMENT = 0;

    const size_t PARENT_ADDITIVE_FACTOR = 1;
    const size_t PARENT_MULTIPLICATIVE_FACTOR = 2;

    const size_t CHILD_MULTIPLICATIVE_FACTOR = 2;
    const size_t LEFT_CHILD_ADDITIVE_FACTOR = 1;
    const size_t RIGHT_CHILD_ADDITIVE_FACTOR = 2;

    const size_t LEAFS_MULTIPLICATIVE_FACTOR = 2;

    // Pre: both indexes must be less than the number of elements.
    // Post: swaps the elements stored in data at the given indexes.
    void swap(size_t index_1, size_t index_2);

    // Pre: both indexes must be less than the number of elements.
    // Post: makes an upheap on the given indexes.
    void upheap(size_t inserted_index, size_t parent_index);

    // Pre: index must be less than the number of elements.
    // Post: makes a downheap on the given index.
    void downheap(size_t moved_index);

public:
    // Constructor.
    Heap() = default;

    // Copy constructor.
    Heap(const Heap<T, comp> &heap);

    // Assign operator.
    Heap<T, comp> &operator=(const Heap<T, comp> &heap);

    // Pre: -
    // Post: adds new element to the heap.
    void add(T new_element);

    // Pre: the heap can´t be empty.
    // Post: removes and returns the first element.
    T remove();

    // Pre: the heap can´t be empty.
    // Post: returns the first element (the one with the higher priority).
    T first();

    // Pre: the heap can´t be empty.
    // Post: returns the last element (the one with the lowest priority).
    T last();

    // Pre: -
    // Post: returns true if the heap is empty.
    bool empty();

    // Pre: -
    // Post: returns the number of elements in the heap.
    size_t size();

    // Destructor.
    ~Heap();
};

template<typename T, bool (*comp)(T, T)>
void Heap<T, comp>::swap(size_t index_1, size_t index_2) {
    if (index_1 >= size() || index_2 >= size()) throw Heap_exception();

    T *aux = data[index_1];
    data[index_1] = data[index_2];
    data[index_2] = aux;
}

template<typename T, bool (*comp)(T, T)>
void Heap<T, comp>::upheap(size_t inserted_index, size_t parent_index) {
    if (inserted_index >= size() || parent_index >= size()) throw Heap_exception();

    if (comp(*data[inserted_index], *data[parent_index])) {
        swap(inserted_index, parent_index);

        if (parent_index > FIRST_ELEMENT) {
            upheap(parent_index, (parent_index - PARENT_ADDITIVE_FACTOR) / PARENT_MULTIPLICATIVE_FACTOR);
        }
    }
}

template<typename T, bool (*comp)(T, T)>
void Heap<T, comp>::downheap(size_t moved_index) {
    if (moved_index >= size()) throw Heap_exception();

    size_t swap_index = moved_index;

    if ((moved_index * CHILD_MULTIPLICATIVE_FACTOR) + LEFT_CHILD_ADDITIVE_FACTOR < size() &&
        !comp(*data[moved_index], *data[(moved_index * CHILD_MULTIPLICATIVE_FACTOR) + LEFT_CHILD_ADDITIVE_FACTOR])) {

        swap_index = (moved_index * CHILD_MULTIPLICATIVE_FACTOR) + LEFT_CHILD_ADDITIVE_FACTOR;
    }

    if ((moved_index * CHILD_MULTIPLICATIVE_FACTOR) + RIGHT_CHILD_ADDITIVE_FACTOR < size() &&
        !comp(*data[swap_index], *data[(moved_index * CHILD_MULTIPLICATIVE_FACTOR) + RIGHT_CHILD_ADDITIVE_FACTOR])) {

        swap_index = (moved_index * CHILD_MULTIPLICATIVE_FACTOR) + RIGHT_CHILD_ADDITIVE_FACTOR;
    }

    if (swap_index != moved_index) {
        swap(moved_index, swap_index);
        downheap(moved_index);
    }

}

template<typename T, bool (*comp)(T, T)>
Heap<T, comp>::Heap(const Heap<T, comp> &heap) {
    for (size_t i = 0; i < heap.size(); i++) {
        data.emplace_back(*heap.data[i]);
    }
}

template<typename T, bool (*comp)(T, T)>
Heap<T, comp> &Heap<T, comp>::operator=(const Heap<T, comp> &heap) {
    if (this != &heap) {
        for (size_t i = 0; i < size(); i++) {
            delete data[i];
        }

        data.clear();

        for (size_t i = 0; i < heap.size(); i++) {
            data.emplace_back(*heap.data[i]);
        }
    }

    return *this;
}

template<typename T, bool (*comp)(T, T)>
void Heap<T, comp>::add(T new_element) {
    data.emplace_back(new_element);

    if (size() - 1 > FIRST_ELEMENT) {
        upheap(size() - 1, (size() - PARENT_ADDITIVE_FACTOR) / PARENT_MULTIPLICATIVE_FACTOR);
    }
}

template<typename T, bool (*comp)(T, T)>
T Heap<T, comp>::remove() {
    if (empty()) throw Heap_exception();

    T removed_element = *data[FIRST_ELEMENT];
    delete data[FIRST_ELEMENT];
    data[FIRST_ELEMENT] = data[size() - 1];
    data.pop_back();

    if (!empty()) {
        downheap(FIRST_ELEMENT);
    }

    return removed_element;
}

template<typename T, bool (*comp)(T, T)>
T Heap<T, comp>::first() {
    if (empty()) throw Heap_exception();

    return *data[FIRST_ELEMENT];
}

template<typename T, bool (*comp)(T, T)>
T Heap<T, comp>::last() {
    if (empty()) throw Heap_exception();

    T last_element = *data[size() / LEAFS_MULTIPLICATIVE_FACTOR];
    for (size_t i = size() / LEAFS_MULTIPLICATIVE_FACTOR; i < size(); i++) {
        if (!comp(last_element, *data[i])) last_element = *data[i];
    }

    return last_element;
}

template<typename T, bool (*comp)(T, T)>
bool Heap<T, comp>::empty() {
    return data.empty();
}

template<typename T, bool (*comp)(T, T)>
size_t Heap<T, comp>::size() {
    return data.size();
}

template<typename T, bool (*comp)(T, T)>
Heap<T, comp>::~Heap() {
    for (size_t i = 0; i < size(); i++) {
        delete data[i];
    }
}

#endif