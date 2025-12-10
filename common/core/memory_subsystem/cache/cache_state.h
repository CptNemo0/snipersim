#ifndef CACHE_STATE_H
#define CACHE_STATE_H

#include <cassert>
#include <string_view>

#include "fixed_types.h"

class CacheState
{
   public:
      enum cstate_t
      {
         CSTATE_FIRST = 0,
         INVALID = CSTATE_FIRST,
         SHARED = 1,
         SHARED_UPGRADING = 2,
         EXCLUSIVE = 3,
         OWNED = 4,
         MODIFIED = 5,
         NUM_CSTATE_STATES = 6,
         /* Below are special states, used only for reporting */
         INVALID_COLD = NUM_CSTATE_STATES,
         INVALID_EVICT = 7,
         INVALID_COHERENCY = 8,
         NUM_CSTATE_SPECIAL_STATES = 9
      };

      static std::string_view to_string(cstate_t state) 
      {
         switch(state) 
         {
         case cstate_t::CSTATE_FIRST:
            return "CSTATE_FIRST || INVALID";
         case cstate_t::SHARED:
            return "SHARED";
         case cstate_t::SHARED_UPGRADING:
            return "SHARED_UPGRADING";
         case cstate_t::EXCLUSIVE:
            return "EXCLUSIVE";
         case cstate_t::OWNED:
            return "OWNDER";
         case cstate_t::MODIFIED:
            return "MODIFIED";
         case cstate_t::INVALID_COLD:
            return "INVALID_COLD";
         case cstate_t::INVALID_EVICT:
            return "INVALID_EVICT";
         case cstate_t::INVALID_COHERENCY:
            return "INVALID_COHERENCY";
         case cstate_t::NUM_CSTATE_SPECIAL_STATES:
            return "NUM_CSTATE_SPECIAL_STATES";
         }
         return "SOMETHING WENT WRONG WITH STATE PARSING";
      }

      CacheState(cstate_t state = INVALID) : cstate(state) {}
      ~CacheState() {}

      bool readable()
      {
         return (cstate == MODIFIED) || (cstate == OWNED) || (cstate == SHARED) || (cstate == EXCLUSIVE);
      }

      bool writable()
      {
         return (cstate == MODIFIED);
      }

   private:
      cstate_t cstate;

};

#endif /* CACHE_STATE_H */
