#ifndef EVICTION_HISTORY_H_
#define EVICTION_HISTORY_H_

#include "fixed_types.h"
#include "size_queue.h"
#include "cache_state.h"
#include "cache_block_info.h"

struct EvictionHistoryEntry {
   IntPtr m_tag = 0;
   CacheState::cstate_t m_state = CacheState::cstate_t::INVALID;
   EvictionHistoryEntry() = default;
   explicit EvictionHistoryEntry(CacheBlockInfo* info) : EvictionHistoryEntry(info->getTag(), info->getCState()) {}
   EvictionHistoryEntry(IntPtr tag, CacheState::cstate_t state) : m_tag(tag), m_state(state) {}
   EvictionHistoryEntry(const EvictionHistoryEntry& other) : EvictionHistoryEntry(other.m_tag, other.m_state) {}

   void operator =(const EvictionHistoryEntry& other) {
      m_tag = other.m_tag;
      m_state = other.m_state;
   }

   bool operator ==(const EvictionHistoryEntry other) const 
   {
      return m_tag == other.m_tag && m_state == other.m_state;
   }
};

struct EvictionHistoryEntryTagComparer {
   bool operator()(const EvictionHistoryEntry& a, const EvictionHistoryEntry& b) const  {
      return a.m_tag == b.m_tag;   
   }
};

using EvictionHistory = SizeQueue<EvictionHistoryEntry, EvictionHistoryEntryTagComparer>;

#endif // !EVICTION_HISTORY_H_
