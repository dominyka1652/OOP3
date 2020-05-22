#pragma once
#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <memory>
#include <algorithm>
#include<iostream>
#include<ctime>
#include<string>
#include<numeric>
#include<limits>
#include<fstream>
#include<random>
#include<chrono>
#include<sstream>
#include <memory>
#include <cstddef>
#include <cstring>
#include <utility>
#include <iterator>
#include <stdexcept>
#include <cmath>

template <class T>
class Vector
{
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef T* reference;
    typedef const T* const_reference;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;

    Vector() { create(); }
    explicit Vector(size_type n, const T& t = T{}) { create(n, t); }
    Vector(const Vector& v) { create(v.begin(), v.end()); }
    Vector& operator=(const Vector&);
    ~Vector() { uncreate(); }
    T& at(int it) { return beg[it]; }

    T& operator[](size_type i) { return beg[i]; }
    const T& operator[](size_type i) const { return  beg[i]; }
    void push_back(const T& t) {
        if (avail == limit)
            grow();
        unchecked_append(t);
    }

    size_type size() const { return avail - beg; }
    size_type capacity() const { return limit - beg; }
    iterator begin() { return beg; }
    iterator cbegin() { return beg; }
    const_iterator begin() const { return beg; }
    iterator end() { return avail; }
    iterator cend() { return avail; }
    const_iterator end() const { return avail; }
    void assign(T* t_start_ptr, T* t_end_ptr);
    void assign(unsigned int t_number, const T& t_value);
    void pop_back();
    void insert(const_iterator index, const T& item);
    void insert(const_iterator it, int n, T value);
    void clear();
    void erase(const_iterator it);
    void erase(const_iterator it1, const_iterator it2);
    void resize(int n, T val);
    void resize(int n);
    void shrink_to_fit();
    void reserve(const int cap);
    bool empty()
    {
        return  begin() == end();
    }
    T& back() { return *(avail - 1); }
    T* data() noexcept;

    reverse_iterator rbegin() noexcept { return reverse_iterator(avail); }
    const_reverse_iterator rbegin() const noexcept { return reverse_iterator(avail); }
    const_reverse_iterator crbegin() const noexcept { return reverse_iterator(avail); }
    reverse_iterator rend() noexcept { return reverse_iterator(beg); }
    const_reverse_iterator rend() const noexcept { return reverse_iterator(beg); }
    const_reverse_iterator crend() const noexcept { return reverse_iterator(beg); }
    size_type max_size() const noexcept;
    void emplace(Vector<T>::const_iterator it, T args);
    void emplace_back(T args);


    void swap(Vector<T>& v);
    bool operator==(const Vector<T>& v) const;
    bool operator!=(const Vector<T>& v) const;
    bool operator<(const Vector<T>& v) const;
    bool operator<=(const Vector<T>& v) const;
    bool operator>(const Vector<T>& v) const;
    bool operator>=(const Vector<T>& v) const;


private:
    iterator beg;
    iterator avail;
    iterator limit;
    std::allocator<T> alloc;
    void create();
    void create(size_type, const T&);
    void create(const_iterator, const_iterator);
    void uncreate();
    void grow();
    void unchecked_append(const T&);
};

template <class T>
void Vector<T>::create()
{
    beg = avail = limit = nullptr;
}
template <class T>
void Vector<T>::create(size_type n, const T& val)
{
    beg = alloc.allocate(n);
    limit = avail = beg + n;
    std::uninitialized_fill(beg, limit, val);
}
template <class T>
void Vector<T>::create(const_iterator i, const_iterator j)
{
    beg = alloc.allocate(j - i);
    limit = avail = std::uninitialized_copy(i, j, beg);
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
    if (&rhs != this) {
        uncreate();
        create(rhs.begin(), rhs.end());
    }
    return *this;
}

template <class T>
void Vector<T>::uncreate()
{
    if (beg) {
        iterator it = avail;
        while (it != beg)
            alloc.destroy(--it);
        alloc.deallocate(beg, limit - beg);
    }
    beg = limit = avail = nullptr;
}

template <class T>
void Vector<T>::grow()
{
    size_type new_size = std::max(2 * (limit - beg), ptrdiff_t(1));
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(beg, avail, new_data);
    uncreate();
    beg = new_data;
    avail = new_avail;
    limit = beg + new_size;
}

template <class T>
void Vector<T>::unchecked_append(const T& val)
{
    alloc.construct(avail++, val);
}

template<class T>
void Vector<T>::assign(T* t_start_ptr, T* t_end_ptr) {
    clear();
    resize(0);
    for (T* iter = t_start_ptr; iter < t_end_ptr; ++iter) {
        push_back(*iter);
    }
}

template<class T>
void Vector<T>::assign(unsigned int t_number, const T& t_value) {
    clear();
    reserve(t_number);
    for (unsigned int i = 0; i < t_number; ++i) {
        push_back(t_value);
    }
}

template <typename T>
void Vector<T>::pop_back()
{
    if (avail) {
        avail--;
    }
}

template <class T>
void Vector<T>::insert(Vector<T>::const_iterator index, const T& item) {
    if ((index < beg) || (index > avail)) {
        std::cout << "Throw exception" << std::endl;
    }
    int v = 0;
    for (auto k = begin(); k < index; k++) {
        v++;
    }
    size_type new_size = (size() + 1);
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(beg, avail + 1, new_data);
    for (int i = 0; i < v; i++) {
        new_data[i] = beg[i];
    }
    new_data[v] = item;
    int z = v + 1;
    int g = size() + 1;
    for (int i = z; i < g; i++) {
        new_data[i] = beg[i - 1];
    }
    uncreate();
    beg = new_data;
    avail = new_avail;
    limit = beg + new_size;

}

template <typename T>
void Vector<T>::insert(Vector<T>::const_iterator it, int n, T value) {
    if (it < beg || n < 1 || it > avail) throw std::out_of_range{ "Vector::insert" };
    avail += n;
    int v = 0;
    for (auto k = begin(); k < it; k++) {
        v++;
    }
    size_type new_size = (size() + n);
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(beg, avail, new_data);
    for (int i = 0; i < v; i++) {
        new_data[i] = beg[i];
    }
    for (int r = v; r <= v + n; r++)
        new_data[r] = value;
    int z = v + n;
    int g = size() + n;
    for (int i = z; i < g; i++) {
        new_data[i] = beg[i - n];
    }
    uncreate();
    beg = new_data;
    avail = new_avail;
    limit = beg + new_size;

}

template <typename T>
void Vector<T>::clear() {
    if (avail > 0)
    {
        avail = 0;
    }
}

template <typename T>
void Vector<T>::erase(Vector<T>::const_iterator it) {
    if (it < beg || it > avail) throw std::out_of_range{ "Vector::erase" };
    int v = 0;
    for (auto k = begin(); k < it; ++k) {
        v++;
    }
    if (v < size()) {
        for (v; v < size(); v++) {
            beg[v] = beg[v + 1];
        }
        avail--;
    }
}


template <typename T>
void Vector<T>::erase(Vector<T>::const_iterator it1, Vector<T>::const_iterator it2) {
    if (it1 < beg || it2 > avail) throw std::out_of_range{ "Vector::erase" };
    int v = 0, g = 0;
    for (auto k = begin(); k < it1; ++k) {
        v++;
    }
    for (auto k = it1; k < it2; ++k) {
        g++;
    }

    for (v; v < size() - g; v++) {
        beg[v] = beg[v + g];

    }
    avail -= g;
}

template <typename T>
void Vector<T>::resize(int n, T val)
{
    if (n > capacity()) {
        int a = size();
        size_type new_size = (size() + (n-size()));
        iterator new_data = alloc.allocate(new_size);
        iterator new_avail = std::uninitialized_copy(beg, avail, new_data);
        for (int i = 0; i < a; i++) {
            new_data[i] = beg[i];
        }
        uncreate();
        beg = new_data;
        avail = new_avail;
        limit = beg + new_size;
       
    }
    if (n > size()) {
        int o = size();
        int z = n - size();
        avail += z;

        for (int i = o; i < n; i++) {
            beg[i] = val;
        }
    }
    else resize(n);
}

template <typename T>
void Vector<T>::resize(int n) {
    int k = 0;
    if (n < size()) {
        for (int i = n; i < size(); i++) {
            beg[i] = 0;
            k++;
        }
        avail -= k;
    }
    else {
        if (n > capacity()) {
            int a = size();
           size_type new_size = (size() + (n - size()));
           iterator new_data = alloc.allocate(new_size);
           iterator new_avail = std::uninitialized_copy(beg, avail, new_data); 
           for (int i = 0; i < a; i++) {
               new_data[i] = beg[i];
           }
           uncreate();
           beg = new_data;
           avail = new_avail;
           limit = beg + new_size;
        }
        int o = size();
        int z = n - size();
        avail += z;
        for (int i = o; i < n; i++) {
            beg[i] = 0;
        }
    }
}

template <typename T>
void Vector<T>::shrink_to_fit()
{
    if (avail < limit) {
        limit = avail;
    }
}


template<typename T>
void Vector<T>::reserve(const int n) {
    int b = capacity();
    if (n > b) {
        size_type new_size = n;
        iterator new_data = alloc.allocate(new_size);
        iterator new_avail = std::uninitialized_copy(beg, avail, new_data);
        uncreate();
        beg = new_data;
        avail = new_avail;
        limit = beg + new_size;
    }
}

template<typename T>
T* Vector<T>::data() noexcept {
    return beg;
}

template<typename T>
typename Vector<T>::size_type Vector<T>::max_size() const noexcept {
    double imin = std::numeric_limits<T>::min();
    double imax = std::numeric_limits<T>::max();
    double sum = (imax - imin);
    double k = sizeof(T);
    return sum/k;
}

template<typename T>
void Vector<T>::swap(Vector<T>& v) {
    iterator beg2 = beg, avail2 = avail, limit2 = limit;
    beg = v.beg;
    limit = v.limit;
    avail = v.avail;

    v.beg = beg2;
    v.avail = avail2;
    v.limit = limit2;
}

template<typename T>
bool Vector<T>::operator==(const Vector<T>& v) const {
    if (v.size() != size()) return false;
    else {
        int g = v.size();
        for (int k = 0; k < g; k++) {
            if (beg[k] != v.beg[k]) { return false; }
        }
        return true;
    }
}

template<typename T>
bool Vector<T>::operator!=(const Vector<T>& v) const {
    if (v.size() != size()) return true;
    else {
        int g = v.size();
        for (int k = 0; k < g; k++) {
            if (beg[k] != v.beg[k]) { return true; }
        }
        return false;
    }
}

template<typename T>
bool Vector<T>::operator<(const Vector<T>& v) const {
    bool tr;
    tr = operator==(v);
    if (tr == true) return false;
    else {
        int g = v.size();
        for (int k = 0; k < g; k++) {
            if (beg[k] < v.beg[k]) return true;
            else if (beg[k] > v.beg[k]) return false;
        }
    }
}

template<typename T>
bool Vector<T>::operator<=(const Vector<T>& v) const {
    bool tr;
    tr = operator==(v);
    if (tr == true) return true;
    else {
        tr = operator<(v);
        if (tr == true) return true;
        else return false;
    }
}

template<typename T>
bool Vector<T>::operator>(const Vector<T>& v) const {
    bool t;
    t = operator<=(v);
    if (t == true) return false;
    else return true;
}

template<typename T>
bool Vector<T>::operator>=(const Vector<T>& v) const {
    bool t;
    t = operator<(v);
    if (t == true) return false;
    else return true;
}

template<typename T>
void Vector<T>::emplace(Vector<T>::const_iterator it, T args) {
    if ((it < beg) || (it > avail)) {
        std::cout << "Throw exception" << std::endl;
    }
    int v = 0;
    for (auto k = begin(); k < it; k++) {
        v++;
    }
    size_type new_size = (size() + 1);
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(beg, avail + 1, new_data);
    for (int i = 0; i < v; i++) {
        new_data[i] = beg[i];
    }
    new_data[v] = args;
    int z = v + 1;
    int g = size() + 1;
    for (int i = z; i < g; i++) {
        new_data[i] = beg[i - 1];
    }
    uncreate();
    beg = new_data;
    avail = new_avail;
    limit = beg + new_size;
}

template<typename T>
void Vector<T>::emplace_back(T args) {
    push_back(args);
}


#endif;


