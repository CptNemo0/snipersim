#ifndef CACHE_SET_RANDOM_MY_H_
#define CACHE_SET_RANDOM_MY_H_

#include "cache_base.h"
#include "cache_set.h"

class CacheSetRandomMy : public  CacheSet {
   public:
      CacheSetRandomMy(String cfgname, CacheBase::cache_t cache_type, UInt32 associativity, UInt32 block_size);
      ~CacheSetRandomMy();
      UInt32 getReplacementIndex(CacheCntlr *cntlr);
      void updateReplacementIndex(UInt32 accessed_index);
};

#endif // CACHE_SET_RANDOM_MY_M_
