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
      // negative reward
   }
   else
   {
      // positive reward
   } 
}

// On every eviction the algorithm places evicted block on the head of the size queue.
// At the same time it keeps track of the amount of entries with each state by.
void RlExtension::OnEviction(CacheBlockInfo* evicted_block, CacheBlockInfo* new_block)
{
   if(evicted_block->getTag() == new_block->getTag()) 
   {
      //std::cout<<"Evicted and new tag are the same\n";
      return;
   }

   // This looks confusig, however the evicted (old cache block) 
   // block bocomes new history entry.
   EvictionHistoryEntry new_entry {evicted_block};      
   EvictionHistoryEntry old_entry;

   [[maybe_unused]] const bool match_found = m_history->remove_if_present(new_entry, old_entry);
   m_history->push(new_entry);

   UpdateMESCount();
}

void RlExtension::UpdateMESCount()
{
   m_mes.clear();   
   const auto& data = m_history->data();
   for(const auto& entry : data) {
      m_mes[entry.m_state]++;
   }

   std::cout<<std::format("associativity: {}, size: {}, modified: {}, exclusive: {}, shared: {}\n", 
                           m_associativity,  
                           m_history->size(),
                           m_mes[CacheState::cstate_t::MODIFIED], 
                           m_mes[CacheState::cstate_t::EXCLUSIVE], 
                           m_mes[CacheState::cstate_t::SHARED]);
}
