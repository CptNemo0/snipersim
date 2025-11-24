#include "rl_extension.h"
#include <iostream>

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
void RlExtension::OnAccess(IntPtr tag) 
{
   if (m_history->contains({tag}))
   {
      // negative reward
   }
   else
   {
      // positive reward
   } 
}

void RlExtension::OnEviction(IntPtr evicted_tag, IntPtr new_tag)
{
   EvictionHistoryEntry evicted_entry {evicted_tag};
   EvictionHistoryEntry new_entry {new_tag};   
   
   if(evicted_tag == new_tag) 
   {
      //std::cout<<"Evicted and new tag are the same\n";
      return;
   }

   if(m_history->remove_if_present({new_tag}))
   {
      std::cout<<"Removing new tag from the history\n";      
   }
}
