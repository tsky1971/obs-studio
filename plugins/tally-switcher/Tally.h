#pragma once

#ifndef FAKEOBS
#include <obs.hpp>
#else
typedef void* calldata_t;

class OBSSignal
{
public:
   void Connect(void *handler_, const char *signal_, void *callback_, void *param_)
   {
   }
};
#endif

#include "TallySettings.h"

class Tally
{

private:   
   TallySettings Settings;
   
public:
   Tally();
   
   TallySettings *GetSettings();
};
