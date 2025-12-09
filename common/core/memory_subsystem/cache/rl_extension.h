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
      enum class MODE 
      {
         DISABLED = 0,
         STATE = 1,
         BITSET = 2,
         BOTH = 3
      };

      RlExtension(CacheSet* cache_set, UInt32 associativity, RlExtension::MODE mode);
      ~RlExtension();
   
      virtual void OnAccess(CacheBlockInfo* block) override;
      virtual void OnEviction(CacheBlockInfo* evicted_block, CacheBlockInfo* new_block) override;

      bool ShouldEvict(const CacheBlockInfo* block) const;

   private:
      // Updates the counts of Modified, Exclusive and Shared found in the m_history.
      void UpdateMESCount();      

      CacheSet* m_set;
      const UInt32 m_associativity;
      std::unique_ptr<EvictionHistory> m_history;
      std::unordered_map<CacheState::cstate_t, std::uint32_t> m_mes;
      MODE m_mode = MODE::BOTH;

      float m_m = 0.0f;
      float m_e = 1.0f;
      float m_s = 0.0f;
      float m_at = 0.0f;
      float m_bt = 1.0f;
};

#endif // !RL_EXTENSION_H_
