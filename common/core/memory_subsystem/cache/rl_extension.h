#ifndef RL_EXTENSION_H_
#define RL_EXTENSION_H_

#include <memory>

#include "cache_set.h"
#include "eviction_history.h"

class RlExtension : public CacheSet::CacheSetObserver
{
   public: 
      RlExtension(CacheSet* cache_set, UInt32 associativity);
      ~RlExtension();
   
      virtual void OnAccess(IntPtr tag) override;
      virtual void OnEviction(IntPtr old_tag, IntPtr new_tag) override;

   private:
      CacheSet* m_set;
      const UInt32 m_associativity;
      std::unique_ptr<EvictionHistory> m_history;
};

#endif // !RL_EXTENSION_H_
