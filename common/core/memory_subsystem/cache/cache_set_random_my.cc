#include "cache_set_random_my.h"

CacheSetRandomMy::CacheSetRandomMy(String cfgname, CacheBase::cache_t cache_type, UInt32 associativity, UInt32 block_size) : CacheSet(cfgname, cache_type, associativity, block_size) 
{
   srand(time(0));
}

CacheSetRandomMy::~CacheSetRandomMy(){}


UInt32 CacheSetRandomMy::getReplacementIndex(CacheCntlr *cntlr)
 {
   // Invalidations may mess up the LRU bits

   for (UInt32 i = 0; i < m_associativity; i++)
   {
       if (!m_cache_block_info_array[i]->isValid())
          return i;   // if there is an invalid line, use that line
   }

   UInt32 index = (rand() % m_associativity);
   if (isValidReplacement(index))
   {
      return index;
   }
   else
   {
      // Could not find valid victim, try again, due to randomness, it might work
      return getReplacementIndex(cntlr);
   }
}

void CacheSetRandomMy::updateReplacementIndex(UInt32 accessed_index){}

