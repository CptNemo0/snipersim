#ifndef SIZE_QUEUE_H_
#define SIZE_QUEUE_H_

#include "fixed_types.h"
#include <vector>
#include <concepts>
#include <algorithm>

template<typename T>
concept VectorFullyCompatible = requires(std::vector<T> v, T obj) 
{
   v.resize(1);
   v.push_back(obj);
   v.push_back(std::move(obj));
};

template <VectorFullyCompatible T>
class SizeQueue
{
   public:
   SizeQueue(UInt32 size) 
   {
      m_data.resize(size);
   }

   SizeQueue() 
   {
      m_data.resize(16);
   }

   void push(T&& value) 
   {
      m_data[m_head] = value;
      m_head = (m_head + 1) % m_data.size();
   }

   bool contains(const T& value) const 
   {
      return m_data.end() != std::find(m_data.begin(), m_data.end(), value);
   }

   bool remove_if_present(T&& value) 
   {
      auto iterator = std::find(m_data.begin(), m_data.end(), value);
      if(iterator == m_data.end()) return false;
      *iterator = T();
      return true;
   }

   private: 
      UInt32 m_head = 0;
      std::vector<T> m_data;
};

#endif // !SIZE_QUEUE_H_
