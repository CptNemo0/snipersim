#ifndef EVICTION_HISTORY_H_
#define EVICTION_HISTORY_H_

#include "fixed_types.h"
#include "size_queue.h"
#include "cache_state.h"
#include "cache_block_info.h"

constexpr UInt32 gSharerThreshold = 1;

struct EvictionHistoryEntry {
   IntPtr m_tag = 0;
   CacheState::cstate_t m_state = CacheState::cstate_t::INVALID;
   bool m_above_threshold = false;
   EvictionHistoryEntry() = default;
   explicit EvictionHistoryEntry(CacheBlockInfo* info) : EvictionHistoryEntry(info->getTag(), info->getCState(), info->getSharerCount()) {}
   EvictionHistoryEntry(IntPtr tag, CacheState::cstate_t state, UInt32 sharer_count) : m_tag(tag), m_state(state), m_above_threshold(sharer_count > gSharerThreshold) {}
   EvictionHistoryEntry(const EvictionHistoryEntry& other) : EvictionHistoryEntry(other.m_tag, other.m_state, other.m_above_threshold) {}

   void operator =(const EvictionHistoryEntry& other) {
      m_tag = other.m_tag;
      m_state = other.m_state;
      m_above_threshold = other.m_above_threshold;
   }

   bool operator ==(const EvictionHistoryEntry other) const 
   {
      return m_tag == other.m_tag && m_state == other.m_state && m_above_threshold == other.m_above_threshold;
   }
};

struct EvictionHistoryEntryTagComparer {
   bool operator()(const EvictionHistoryEntry& a, const EvictionHistoryEntry& b) const  {
      return a.m_tag == b.m_tag;   
   }
};

using EvictionHistory = SizeQueue<EvictionHistoryEntry, EvictionHistoryEntryTagComparer>;

#endif // !EVICTION_HISTORY_H_
