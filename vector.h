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
  

      T& at(int it) {
          if (it < 0 || avail < it) throw std::out_of_range{ "Vector::reference_at" };
          return avail[it];
      }
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
            void assign(int n, T value);
            void pop_back();
            void insert(int it, T value);
            void insert(int it, int n, T value);
            void clear();
            void erase(int it);
            void erase(int it1, int it2);
            void resize(int n, T val);
            void shrink_to_fit();
            void reserve(int n);
            bool empty() { if (avail > 0) return false; else return true; };
            T& back() { return beg[avail - 1]; }
            T* data() noexcept;

            reverse_iterator rbegin() noexcept {return reverse_iterator(avail); }
            const_reverse_iterator rbegin() const noexcept {return reverse_iterator(avail);}
            const_reverse_iterator crbegin() const noexcept {return reverse_iterator(avail); }
            reverse_iterator rend() noexcept {return reverse_iterator(beg);}
            const_reverse_iterator rend() const noexcept {return reverse_iterator(beg);}
            const_reverse_iterator crend() const noexcept { return reverse_iterator(beg);}
            size_type max_size() const noexcept;
            template<class ... Args> iterator emplace(const_iterator, Args&& ...);
            template<class ... Args> iterator emplace_back( Args&& ...);

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
  {   if (&rhs != this) {
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

      template <typename T>
    void Vector<T>::assign(int n, T value) {
       if (n < 0) throw std::out_of_range{ "Vector::assign" };
        uncreate();
        beg = new T[n];
        std::fill_n(beg, limit, value);
    }


    template <typename T>
    void Vector<T>::pop_back()
    {
        if (avail) {
            avail--;
        }
    }

    template <typename T>
    void Vector<T>::insert(int it, T value) {
        if (it < 1 || it > avail) throw std::out_of_range{ "Vector::insert" };
        T* beg2 = new T[avail + 1];
        for (int i = 0; i < it; i++) {
            beg2[i] = beg[i];
        }
        beg[it] = value;
        for (int i = it; i < avail + 1; i++) {
            beg2[i + 1] = beg[i];
        }
        uncreate();
        beg = beg2;
        delete[] beg2;
        avail++;
        if  (avail < limit) avail = limit;
    }

    template <typename T>
    void Vector<T>::insert(int it, int n, T value) {
        if (n < 1 || it < 1 || it > avail) throw std::out_of_range{ "Vector::insert" };
        T* beg2 = new T[avail+ n];
        for (int i = 0; i < it; i++) {
            beg2[i] = beg[i];
        }
        for (int i = it; i < it + n; i++) {
            beg2[i] = value;
        }
        for (int i = it; i < avail; i++) {
            beg2[i + n] = beg[i];
        }
        uncreate();
        beg = beg2;
        delete[] beg2;
        avail += n;
        if (avail < limit) avail = limit;
    }

    template <typename T>
    void Vector<T>::clear() {
        if (avail > 0)
        {
            avail = 0;
        }
    }

    template <typename T>
    void Vector<T>::erase(int it) {
       if (it < 1 || it > avail) throw std::out_of_range{ "Vector::erase" };
        T* beg2 = new T[avail - 1];
        for (int i = 0; i < it; i++) {
            beg2[i] = beg[i];
        }
        for (int i = it; i < avail - 1; i++) {
            beg2[i] = beg[i + 1];
        }
        uncreate();
        beg = beg2;
        delete[] beg2;
        avail--;
    }

    
template <typename T>
void Vector<T>::erase(int it1, int it2) {
	if (it1 - it2 > 0 || it2 > avail) throw std::out_of_range{ "Vector::erase" };
	T* beg2 = new T[avail - (it2 - it1)];
	for (int i = 0; i < it1; i++)
	{
		beg2 = beg[i];
	}
	for (int i = it1; i < avail - (it2 - it1); i++)
	{
		beg2 = beg[i + (it2 - it1)];
	}
    uncreate();
	beg = beg2;
	delete[] beg2;
	avail = avail - (it2 - it1);
}

template <typename T>
void Vector<T>::resize(int n, T val)
{
    if (n > 0) {
        T* beg2 = new T[n];
        int k;
        if (beg > n) k = n; else k = beg;
        for (int i = 0; i < k; i++) {
            beg2[i] = beg[i];
        }
        for (int i = k; i < n; i++) {
            beg2[i] = val;
        }
        uncreate();
        beg = beg2;
        delete[] beg2;
        if (n > limit) limit = n;
        avail = n;
    }
}

template <typename T>
void Vector<T>::shrink_to_fit()
{
    if (avail < limit) {
        T* beg2 = new T[avail];
        for (int i = 0; i < avail; i++)
        {
            beg2[i] = beg[i];
        }
        uncreate();
        beg = beg2;
        delete[] beg2;
        limit = avail;
    }
}

template <typename T>
void Vector<T>::reserve(int n)
{
    if (n > limit) {
        T* beg2 = new T[n];
        for (int i = 0; i < avail; i++)
        {
            beg2[i] = beg[i];
        }
        uncreate();
        T* beg = new T[n];
        beg = beg2;
        delete[] beg2;
        limit = n;
    }
}

template<typename T>
T* Vector<T>::data() noexcept {
    return beg;
}

template<typename T>
typename Vector<T>::size_type Vector<T>::max_size() const noexcept {
    T imin = std::numeric_limits<T>::min();
    T imax = std::numeric_limits<T>::max();
    T sum = fabs(imin) + fabs(imax);
    return sum;
}

template<typename T>
void Vector<T>::swap(Vector<T>& v) {
    iterator beg2 = beg , avail2 = avail, limit2 = limit; 
    beg = v.beg;
    limit = v.limit;
    avail = v.avail;

    v.beg = beg2;
    v.avail = avail2;
    v.limit = limit2;
}

template<typename T>
bool Vector<T>::operator==(const Vector<T>& v) const {
    if (v.avail != avail) return false;
    else {
        for (auto it = 0; it < avail; it++) {
            if (data[it] != v.data[it]) {
                return false; exit(0);
            }
        }
        return true;
    }
}

template<typename T>
bool Vector<T>::operator!=(const Vector<T>& v) const {
    if (v.avail != avail) return true;
    else {
        for (auto it = 0; it < avail; it++) {
            if (data[it] == v.data[it]) {
                return false; 
            }
        }
        return true;
    }
}

template<typename T>
bool Vector<T>::operator<(const Vector<T>& v) const {
    bool tr;
    tr = operator==(v);
    if ( tr == true) return false;
    else {
        if (beg < v.beg) return true;
        else return false;
    }
}

template<typename T>
bool Vector<T>::operator<=(const Vector<T>& v) const {
    bool tr;
    tr = operator==(v);
    if (tr == true) return true;
    else {
        if (beg < v.beg) return true;
        else return false;
    }
}

template<typename T>
bool Vector<T>::operator>(const Vector<T>& v) const {
    bool t;
    t = operator<=(v);
    if (t == true) return false;
    else return false;
}

template<typename T>
bool Vector<T>::operator>=(const Vector<T>& v) const {
    bool t;
    t = operator<(v);
    if (t == true) return false;
    else return true;
}

template<typename T>
template<class ... Args>
typename Vector<T>::iterator Vector<T>::emplace(typename Vector<T>::const_iterator it, Args&& ... args) {
    T* beg2 = new T[avail + 1];
    for (auto i = 0; i < it; i++) {
        beg2[i] = beg[i];
    }
    beg[it] = args;
    for (auto i = it; i < avail + 1; i++) {
        beg2[i + 1] = beg[i];
    }
    uncreate();
    beg = beg2;
    delete[] beg2;
    avail++;
    if (avail < limit) avail = limit;
}

template<typename T>
template<class ... Args>
typename Vector<T>::iterator Vector<T>::emplace_back(Args&& ... args) {
    T* beg2 = new T[avail + 1];
    for (auto i = 0; i < beg2.end(); i++) {
        beg2[i] = beg[i];
        if(++i == beg2.end()) beg2[i] = args;
    }
    uncreate();
    beg = beg2;
    delete[] beg2;
    avail++;
    if (avail < limit) avail = limit;
}
#endif