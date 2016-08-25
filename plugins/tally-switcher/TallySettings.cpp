#include "TallySettings.h"


bool TallySettings::GetTallyActive()
{
	return TallyActive;
}

std::string TallySettings::GetCameraName()
{
   return CameraName;
}

int TallySettings::GetCameraIndex()
{
   return CameraIndex;
}


void TallySettings::SetTallyActive(bool _value)
{
	TallyActive = _value;
}

void TallySettings::SetCameraName(std::string _name)
{
   CameraName = _name;
}

void TallySettings::SetCameraIndex(int _value)
{
   CameraIndex = _value;
}
