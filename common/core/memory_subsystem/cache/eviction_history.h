#ifndef EVICTION_HISTORY_H_
#define EVICTION_HISTORY_H_

#include "fixed_types.h"
#include "size_queue.h"

struct EvictionHistoryEntry {
   IntPtr m_tag = 0;
   EvictionHistoryEntry() = default;
   EvictionHistoryEntry(IntPtr tag) : m_tag(tag) {}
   bool operator ==(const EvictionHistoryEntry other) const 
   {
      return m_tag == other.m_tag;
   }
};

using EvictionHistory = SizeQueue<EvictionHistoryEntry>;

#endif // !EVICTION_HISTORY_H_
