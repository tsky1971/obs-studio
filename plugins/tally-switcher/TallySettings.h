#pragma once

#include <string>

class TallySettings
{
protected:
   std::string CameraName;
   int CameraIndex;
   bool TallyActive;

public:
	bool GetTallyActive();
	std::string GetCameraName();
	int GetCameraIndex();

	void SetTallyActive(bool _value);
	void SetCameraName(std::string _name);
	void SetCameraIndex(int _value);
};
