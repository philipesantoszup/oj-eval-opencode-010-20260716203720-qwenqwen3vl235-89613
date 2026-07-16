#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T data;
        node *prev;
        node *next;

        node() : prev(nullptr), next(nullptr) {}
        node(const T &value) : data(value), prev(nullptr), next(nullptr) {}
        node(const T &value, node *prev, node *next) : data(value), prev(prev), next(next) {}
        ~node() {}
    };

protected:
    node *head;
    node *tail;
    size_t listSize;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        if (pos == head) {
            cur->next = head;
            cur->prev = nullptr;
            if (head) head->prev = cur;
            head = cur;
            if (!tail) tail = cur;
        } else {
            cur->next = pos;
            cur->prev = pos->prev;
            if (pos->prev) pos->prev->next = cur;
            pos->prev = cur;
        }
        listSize++;
        return cur;
    }
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        if (pos == head) {
            head = pos->next;
            if (head) head->prev = nullptr;
            if (pos == tail) tail = nullptr;
        } else if (pos == tail) {
            tail = pos->prev;
            if (tail) tail->next = nullptr;
        } else {
            pos->prev->next = pos->next;
            pos->next->prev = pos->prev;
        }
        listSize--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    public:
        friend class list;
        node *ptr;

        iterator() : ptr(nullptr) {}
        iterator(node *p) : ptr(p) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            if (ptr) ptr = ptr->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            if (ptr) ptr = ptr->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (!ptr) throw invalid_iterator();
            return ptr->data;
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (!ptr) throw invalid_iterator();
            return &(ptr->data);
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    public:
        friend class list;
        node *ptr;

        const_iterator() : ptr(nullptr) {}
        const_iterator(node *p) : ptr(p) {}
        const_iterator(const iterator &it) : ptr(it.ptr) {}

        /**
         * iter++
         */
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        /**
         * ++iter
         */
        const_iterator & operator++() {
            if (ptr) ptr = ptr->next;
            return *this;
        }
        /**
         * iter--
         */
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }
        /**
         * --iter
         */
        const_iterator & operator--() {
            if (ptr) ptr = ptr->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        const T & operator *() const {
            if (!ptr) throw invalid_iterator();
            return ptr->data;
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        const T * operator ->() const {
            if (!ptr) throw invalid_iterator();
            return &(ptr->data);
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : head(nullptr), tail(nullptr), listSize(0) {}
    list(const list &other) : head(nullptr), tail(nullptr), listSize(0) {
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
    }
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
    }
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) throw container_is_empty();
        return head->data;
    }
    const T & back() const {
        if (empty()) throw container_is_empty();
        return tail->data;
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head);
    }
    const_iterator cbegin() const {
        return const_iterator(head);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(nullptr);
    }
    const_iterator cend() const {
        return const_iterator(nullptr);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return listSize == 0;
    }
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return listSize;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (head) {
            node *tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        listSize = 0;
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (!pos.ptr && !head && !tail) {
            node *newNode = new node(value);
            head = tail = newNode;
            listSize++;
            return iterator(newNode);
        }
        if (!pos.ptr) {
            node *newNode = new node(value);
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
            listSize++;
            return iterator(newNode);
        }
        if (pos.ptr == head) {
            node *newNode = new node(value);
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
            listSize++;
            return iterator(newNode);
        }
        node *newNode = new node(value);
        newNode->next = pos.ptr;
        newNode->prev = pos.ptr->prev;
        pos.ptr->prev->next = newNode;
        pos.ptr->prev = newNode;
        listSize++;
        return iterator(newNode);
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty() || !pos.ptr) throw invalid_iterator();
        node *removed = erase(pos.ptr);
        iterator result = iterator(removed->next);
        delete removed;
        return result;
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        if (empty()) {
            head = tail = new node(value);
            listSize++;
        } else {
            node *newNode = new node(value);
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
            listSize++;
        }
    }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) throw container_is_empty();
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            listSize--;
        } else {
            node *tmp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete tmp;
            listSize--;
        }
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        if (empty()) {
            head = tail = new node(value);
            listSize++;
        } else {
            node *newNode = new node(value);
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
            listSize++;
        }
    }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) throw container_is_empty();
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            listSize--;
        } else {
            node *tmp = head;
            head = head->next;
            head->prev = nullptr;
            delete tmp;
            listSize--;
        }
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (listSize <= 1) return;
        std::vector<T> vec;
        vec.reserve(listSize);
        for (iterator it = begin(); it != end(); ++it) {
            vec.push_back(*it);
        }
        auto cmp = [](const T& a, const T& b) { return a < b; };
        sjtu::sort(vec.data(), vec.data() + vec.size(), std::function<bool(const T&, const T&)>(cmp));
        int index = 0;
        for (iterator it = begin(); it != end(); ++it) {
            *it = vec[index++];
        }
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (&other == this) return;
        if (other.empty()) return;
        if (empty()) {
            *this = other;
            other.clear();
            return;
        }

        node *thisPtr = head;
        node *otherPtr = other.head;
        node *result = nullptr;
        node *resultTail = nullptr;

        while (thisPtr && otherPtr) {
            if (thisPtr->data < otherPtr->data) {
                if (!result) {
                    result = thisPtr;
                    resultTail = thisPtr;
                } else {
                    resultTail->next = thisPtr;
                    thisPtr->prev = resultTail;
                    resultTail = thisPtr;
                }
                thisPtr = thisPtr->next;
            } else {
                if (!result) {
                    result = otherPtr;
                    resultTail = otherPtr;
                } else {
                    resultTail->next = otherPtr;
                    otherPtr->prev = resultTail;
                    resultTail = otherPtr;
                }
                otherPtr = otherPtr->next;
            }
        }

        while (thisPtr) {
            if (!result) {
                result = thisPtr;
                resultTail = thisPtr;
            } else {
                resultTail->next = thisPtr;
                thisPtr->prev = resultTail;
                resultTail = thisPtr;
            }
            thisPtr = thisPtr->next;
        }

        while (otherPtr) {
            if (!result) {
                result = otherPtr;
                resultTail = otherPtr;
            } else {
                resultTail->next = otherPtr;
                otherPtr->prev = resultTail;
                resultTail = otherPtr;
            }
            otherPtr = otherPtr->next;
        }

        head = result;
        tail = resultTail;
        head->prev = nullptr;
        tail->next = nullptr;
        listSize += other.listSize;
        other.head = other.tail = nullptr;
        other.listSize = 0;
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (listSize <= 1) return;
        node *current = head;
        node *temp = nullptr;
        while (current) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }
        temp = head;
        head = tail;
        tail = temp;
        if (head) head->prev = nullptr;
        if (tail) tail->next = nullptr;
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (listSize <= 1) return;
        iterator it = begin();
        iterator next = it;
        ++next;
        while (next != end()) {
            if (*it == *next) {
                next = erase(next);
            } else {
                ++it;
                ++next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
