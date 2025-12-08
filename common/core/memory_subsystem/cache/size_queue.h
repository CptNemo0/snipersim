#ifndef SIZE_QUEUE_H_
#define SIZE_QUEUE_H_

#include "fixed_types.h"
#include <vector>
#include <concepts>
#include <algorithm>
#include <iostream>

template<typename T>
concept VectorFullyCompatible = requires(std::vector<T> v, T obj) 
{
   v.resize(1);
   v.push_back(obj);
   v.push_back(std::move(obj));
};

template <VectorFullyCompatible T, typename BinaryPredicate = std::equal_to<T>>
class SizeQueue
{
   public:
   SizeQueue(UInt32 size, BinaryPredicate compare = BinaryPredicate()) : m_compare(std::move(compare)) 
   {
      m_data.resize(size);
   }

   SizeQueue(BinaryPredicate compare = BinaryPredicate()) : m_compare(std::move(compare)) 
   {
      m_data.resize(16);
   }

   const T& head() {
      return m_data[m_head];   
   }

   T push(const T& value) 
   {
      auto old = m_data[m_head];
      m_data[m_head] = value;
      m_head = (m_head + 1) % m_data.size();
      return old;
   }

   bool contains(const T& value) const 
   {
      return m_data.end() != std::find_if(m_data.begin(), m_data.end(), [&](const T& element){return this->m_compare(element, value);});
   }

   void remove(const T& value) 
   {   
      auto iterator = std::find_if(m_data.begin(), m_data.end(), [&](const T& element){return this->m_compare(element, value);});
      if(iterator == m_data.end()) return;
      *iterator = T();
   }

   bool remove_if_present(const T& value, T old_value) 
   {
      auto iterator = std::find_if(m_data.begin(), m_data.end(), [&](const T& element){return this->m_compare(element, value);});
      if(iterator == m_data.end()) return false;
      old_value = *iterator;
      *iterator = T();
      return true;
   }

   std::size_t size() 
   {
      return m_data.size();
   }
   
   const std::vector<T>& data() const 
   {
      return m_data;
   }

   private: 
      UInt32 m_head = 0;
      std::vector<T> m_data;
      [[no_unique_address]] BinaryPredicate m_compare;
};

#endif // !SIZE_QUEUE_H_
