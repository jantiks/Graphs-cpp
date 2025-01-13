//
//  Array.hpp
//  Array_1
//
//  Created by Tigran on 11.12.23.
//

#ifndef Array_hpp
#define Array_hpp
#define __DEBUG__

#include <iostream>
#include <vector>
#include <array>
#include <exception>
#include "iterator"

constexpr const size_t DEFAULT_BLOCK_SIZE = 10;

template <typename element>
class Array {
private:
    std::vector<element*> data_;
    size_t blockSize_;
    size_t size_;
    size_t capacity_;

    void addBlock();
    void removeBlock();
public:
    Array(std::initializer_list<element> items);
    Array(size_t blockSize = DEFAULT_BLOCK_SIZE);
    Array(size_t count, const element& value);
    Array(const Array<element>& other);
    Array(Array<element>&& other) noexcept;
    ~Array() noexcept;

    size_t size() const;
    size_t capacity() const;

    void print(std::ostream& stream = std::cout) const;
    void push_back(const element& item);
    void push_back(element&& item);
    void pop_back();
    void resize(size_t newSize, const element& e);

    Array<element>& operator=(const Array<element>& other);
    Array<element>& operator=(const Array<element>&& other);
    void clearData();

    element& at(size_t index);
    const element& at(size_t index) const;
    element& operator[](size_t index);
    const element& operator[](size_t index) const;
    
    class iterator;
    iterator begin();
    iterator end();

    class const_iterator {
    private:
        size_t position_;
        const Array<element>* array_;
    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = const element;
        using pointer = const element*;
        using reference = const element&;
        using difference_type = std::ptrdiff_t;


        const_iterator(const Array<element>* array, size_t position)
            : array_(array), position_(position) {}

        bool operator==(const const_iterator& other) const {
            return position_ == other.position_;
        }

        bool operator!=(const const_iterator& other) const {
            return position_ != other.position_;
        }

        const_iterator& operator++() {
            ++position_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        reference operator*() const {
            return (*array_)[position_];
        }

        pointer operator->() const {
            return &(*array_)[position_];
        }
    };

    const_iterator begin() const {
        return const_iterator(this, 0);
    }

    const_iterator end() const {
        return const_iterator(this, size_);
    }
};

template <typename element>
Array<element>::Array(size_t blockSize):blockSize_(blockSize), size_(0), capacity_(0) {}

template <typename element>
Array<element>::Array(std::initializer_list<element> items) {
    size_ = 0;
    capacity_ = 0;
    blockSize_ = DEFAULT_BLOCK_SIZE;
    for (auto&& item : items) {
        try {
            push_back(item);
        } catch (...) {
            clearData();
            throw ;
        }
    }
}

template <typename element>
Array<element>::Array(const Array<element>& other) {
    size_ = 0;
    capacity_ = 0;
    blockSize_ = other.blockSize_;
    for (int i = 0; i < other.size(); ++i) {
        try {
            push_back(other[i]);
        } catch (...) {
            clearData();
            throw ;
        }
    }
}

template <typename element>
Array<element>::Array(size_t count, const element& value) {
    for (int i = 0; i < count; ++i) {
        push_back(value);
    }
}

template <typename element>
Array<element>::Array(Array<element>&& other) noexcept {
    size_ = other.size_;
    capacity_ = other.capacity_;
    blockSize_ = other.blockSize_;
    std::swap(data_, other.data_);
}

template <typename element>
void Array<element>::resize(size_t newSize, const element& e) {
    size_t difference = newSize - size_;
    for (size_t i = 0; i < difference; ++i) {
        push_back(e);
    }
}

template <typename element>
inline size_t Array<element>::size() const {
    return size_;
}

template <typename element>
inline size_t Array<element>::capacity() const {
    return capacity_;
}

template <typename element>
void Array<element>::print(std::ostream& stream) const {
    stream << "[";
    for (size_t i = 0; i < size_; ++i) {
        stream << data_[i / blockSize_][i % blockSize_];
        if (i < size_ - 1) {
            stream << ", ";
        }
    }
    stream << "]";
}

template <typename element>
std::ostream& operator<<(std::ostream& stream, const Array<element>& array) {
    array.print(stream);
    return stream;
}

template <typename element>
void Array<element>::addBlock() {
    void* ptr = malloc(sizeof(element) * blockSize_);
    if (ptr == nullptr) {
        throw std::bad_alloc();
    }

    element* castedPtr = static_cast<element*>(ptr);
    try {
        data_.push_back(castedPtr);
    } catch (const std::bad_alloc&) {
        free(castedPtr);
        throw ;
    }

    capacity_ += blockSize_;
}

template <typename element>
void Array<element>::removeBlock() {
    if (data_.size() == 0) {
        return;
    }

    free(data_.back());
    data_.pop_back();

    capacity_ -= blockSize_;
}

template <typename element>
void Array<element>::push_back(const element& item) {
    bool addedNewBlock = false;
    if (size_ == capacity_) {
        addBlock();
        addedNewBlock = true;
    }

    try {
        new (&data_[size_ / blockSize_][size_ % blockSize_]) element(item);
    } catch (...) {
        if (addedNewBlock) {
            removeBlock();
        }
        throw ;
    }

    ++size_;
}

template <typename element>
void Array<element>::push_back(element&& item) {
    bool addedNewBlock = false;
    if (size_ == capacity_) {
        addBlock();
        addedNewBlock = true;
    }

    try {
        new (&data_[size_ / blockSize_][size_ % blockSize_]) element(item);
    } catch (...) {
        if (addedNewBlock) {
            removeBlock();
        }
        throw ;
    }

    ++size_;
}

template <typename element>
void Array<element>::pop_back() {
    #ifdef __DEBUG__
    if (size_ == 0) {
        throw std::invalid_argument("Empty array");
    }
    #endif

    element* item = &(data_[size_ / blockSize_][size_ % blockSize_]);
    item->~element();
    --size_;

    if (size_ == capacity_ - blockSize_) {
        removeBlock();
    }
}

template <typename element>
Array<element>::~Array() noexcept {
    clearData();
    // check that size_, capacity_ are 0 also the data_ should be empty
}

template <typename element>
element& Array<element>::at(size_t index) {
    #ifdef __DEBUG__
        if (index >= size_) {
            throw std::out_of_range("Invalid index");
        }
    #endif
    return data_[index / blockSize_][index % blockSize_];
}

template <typename element>
const element& Array<element>::at(size_t index) const {
    #ifdef __DEBUG__
        if (index >= size_) {
            throw std::out_of_range("Invalid index");
        }
    #endif
    return data_[index / blockSize_][index % blockSize_];
}

template <typename element>
element& Array<element>::operator[](size_t index) {
    return at(index);
}

template <typename element>
const element& Array<element>::operator[](size_t index) const {
    return at(index);
}

template <typename element>
Array<element>& Array<element>::operator=(const Array<element>& other) {
    clearData();

    blockSize_ = other.blockSize_;
    // handle errors
    for (size_t i = 0; i < other.size(); ++i) {
        push_back(other[i]);
    }

    return *this;
}

template <typename element>
Array<element>& Array<element>::operator=(const Array<element>&& other) {
    clearData();

    blockSize_ = other.blockSize_;
    // handle errors
    data_ = std::move(other.data_);

    return *this;
}

template <typename element>
void Array<element>::clearData() {
    for (size_t i = 0; i < size_; ++i) {
        pop_back();
    }
}

/*  ITERATOR  */

template<typename element>
class Array<element>::iterator {
private:
    size_t position_;
    Array<element>* array_;
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = element;
    using pointer = element*;
    using reference = element&;
    using difference_type = std::ptrdiff_t;

    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;
    iterator& operator++();
    iterator operator++(int);
    reference operator*() const;
    pointer operator->() const;
    iterator(Array<element>* array, size_t position):array_(array), position_(position){};
};

template<typename element>
Array<element>::iterator Array<element>::begin() {
    return iterator(this, 0);
}

template<typename element>
Array<element>::iterator Array<element>::end() {
    return iterator(this, size_);
}

template<typename element>
inline bool Array<element>::iterator::operator==(const iterator& other) const {
    return position_ == other.position_;
}

template<typename element>
inline bool Array<element>::iterator::operator!=(const iterator& other) const {
    return position_ != other.position_;
}

template<typename element>
inline Array<element>::iterator& Array<element>::iterator::operator++() {
    ++position_;
    return *this;
}

template<typename element>
inline Array<element>::iterator Array<element>::iterator::operator++(int) {
    return iterator(array_, position_++);
}

template<typename element>
inline Array<element>::iterator::reference Array<element>::iterator::operator*() const {
    return (*array_)[position_];
}

template<typename element>
inline Array<element>::iterator::pointer Array<element>::iterator::operator->() const {
    return &(*array_)[position_];
}
#endif /* Array_hpp */
