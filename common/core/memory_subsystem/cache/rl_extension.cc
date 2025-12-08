#include "rl_extension.h"
#include <iostream>
#include <format>

RlExtension::RlExtension(CacheSet* cache_set, UInt32 associativity) :
                         m_set(cache_set), m_associativity(associativity)
{
   if(m_set)
   {
      m_set->AddObserver(this);
   }
   
m_history = std::make_unique<EvictionHistory>(2 * m_associativity);
}

RlExtension::~RlExtension()
{
   if(m_set)
   {
      m_set->RemoveObserver(this);
   }
}

// If tag is not found in the history this indicates favorable eviction,
// conversly, if found in the history this indicates suboptimal eviction.
void RlExtension::OnAccess(CacheBlockInfo* block) 
{
   const EvictionHistoryEntry entry {block};
   if (m_history->contains(entry))
   {
       m_history->remove(entry);        
      // negative reward
   }
   else
   {
      // positive reward
   } 
   UpdateMESCount();
}

// On every eviction the algorithm places evicted block on the head of the size queue.
// At the same time it keeps track of the amount of entries with each state by.
void RlExtension::OnEviction(CacheBlockInfo* evicted_block, CacheBlockInfo* new_block)
{
   static int ok_evicts = 0;
   static int bad_evicts = 0;
   if(evicted_block->getTag() == new_block->getTag()) 
   {
      return;
   }

   if(ShouldEvict(evicted_block))
   {
      ok_evicts++;
   }
   else 
   {
      bad_evicts++;
   }

   // This looks confusig, however the evicted (old cache block) 
   // block bocomes new history entry.
   EvictionHistoryEntry new_entry {evicted_block};      
   EvictionHistoryEntry old_entry;

   [[maybe_unused]] const bool match_found = m_history->remove_if_present(new_entry, old_entry);
   m_history->push(new_entry);

   std::cout<<std::format("ok: {}, bad: {}\n", ok_evicts, bad_evicts);
   //UpdateMESCount()i;
}

bool RlExtension::ShouldEvict(const CacheBlockInfo* block) const
{
   if(!block) 
   {
      std::cout<<"Block is nullptr\n";
      return true;
   }
   bool state = false;
   bool bitset = false;

   if(block->getCState() == CacheState::MODIFIED) 
   {
      state = m_m > m_e && m_m > m_s;
   } 
   else if(block->getCState() == CacheState::EXCLUSIVE) 
   {
      state = m_e > m_m && m_e > m_s;
   }
   else if(block->getCState() == CacheState::SHARED)
   {
      state = m_s > m_m && m_s > m_e;
   }
   else 
   {
      // This is most likely INVALID, which is always a correct eviction thus state = true
      std::cout<<"Different cache state: "<<CacheState::to_string(block->getCState())<<"\n";
      state = true;
   }
   
   if(block->getSharerCount() > gSharerThreshold)
   {
      bitset = m_at > m_bt;
   }
   else 
   {
      bitset = m_at <= m_bt;
   }

   return state && bitset;
}

void RlExtension::UpdateMESCount()
{
   m_mes.clear();   
   int shared = 0;
   float size = 0.0f;
   const auto& data = m_history->data();
   for(const auto& entry : data) 
   {
      shared += entry.m_above_threshold;   
      m_mes[entry.m_state]++;
      if(entry.m_tag) size++;
   }

   m_m = static_cast<float>(m_mes[CacheState::cstate_t::MODIFIED])/size;
   m_e = static_cast<float>(m_mes[CacheState::cstate_t::EXCLUSIVE])/size;
   m_s = static_cast<float>(m_mes[CacheState::cstate_t::SHARED])/size;
   m_at = static_cast<float>(shared)/size;
   m_bt = 1.0f - m_at;

   //std::cout<<std::format("m: {}, e: {}, s: {}, at: {}, bt: {}\n", m_m, m_e, m_s, m_at, m_bt);
}
