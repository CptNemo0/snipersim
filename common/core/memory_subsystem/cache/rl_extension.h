#ifndef RL_EXTENSION_H_
#define RL_EXTENSION_H_

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "cache_block_info.h"
#include "cache_set.h"
#include "eviction_history.h"

class RlExtension : public CacheSet::CacheSetObserver
{
   public: 
      RlExtension(CacheSet* cache_set, UInt32 associativity);
      ~RlExtension();
   
      virtual void OnAccess(CacheBlockInfo* block) override;
      virtual void OnEviction(CacheBlockInfo* evicted_block, CacheBlockInfo* new_block) override;

   private:
      // Updates the counts of Modified, Exclusive and Shared found in the m_history.
      void UpdateMESCount();      

      CacheSet* m_set;
      const UInt32 m_associativity;
      std::unique_ptr<EvictionHistory> m_history;
      std::unordered_map<CacheState::cstate_t, std::uint32_t> m_mes;

      float m_m;
      float m_e;
      float m_s;
      float m_at;
      float m_bt;
};

#endif // !RL_EXTENSION_H_
