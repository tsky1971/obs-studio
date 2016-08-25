#include "Tally.h"


#ifndef FAKEOBS
#include <callback/calldata.h>
#else
void *obs_get_signal_handler()
{
   return nullptr;
}
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

Tally::Tally()
{
  
}


TallySettings * Tally::GetSettings()
{
   return &Settings;
}
