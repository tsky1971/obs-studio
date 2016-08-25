
#include <obs-module.h>
#include <obs-data.h>

#include <fstream>
#include <string>
#include <sstream>

#include "Tally.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("tallySwitcher", "en-US")

const char *setting_CameraName = "Camera Name";
const char *setting_CameraIndex = "Camera Index";
const char *setting_TallyActive = "Tally Active";

struct obs_source_info tallySwitcher;


const char *tallySwitcher_GetName(void *type_data) 
{
	UNUSED_PARAMETER(type_data);
	return "Tally Switcher";
}

void * tallySwitcher_Create(obs_data_t *settings, obs_source_t *source)
{
	Tally *pTally = new Tally();

	UNUSED_PARAMETER(source);
	UNUSED_PARAMETER(settings);

	return pTally;
}

void tallySwitcher_Destroy(void *data) 
{
	Tally *pTally = static_cast<Tally *>(data);
	if (pTally != nullptr) {
		delete pTally;
	}
}

obs_properties_t *tallySwitcher_Properties(void *data)
{
	Tally *pTally = static_cast<Tally *>(data);

	obs_properties_t *props = obs_properties_create();

	obs_properties_add_text(props, setting_CameraName, obs_module_text("CameraName"), OBS_TEXT_DEFAULT);
	obs_properties_add_int(props, setting_CameraIndex, obs_module_text("CameraIndex"), 0, 255, 1);
	obs_properties_add_bool(props, setting_TallyActive, obs_module_text("TallyActive"));

	return props;
}

void tallySwitcher_GetDefaults(obs_data_t *settings)
{
	obs_data_set_default_string(settings, setting_CameraName, "Camera XX");
	obs_data_set_default_int(settings, setting_CameraIndex, 99);
	obs_data_set_default_bool(settings, setting_TallyActive, false);
}

void tallySwitcher_Update(void *data, obs_data_t *settings)
{
	Tally *pTally = static_cast<Tally *>(data);

	const char *cameraName = obs_data_get_string(settings, setting_CameraName);
	const int cameraIndex = obs_data_get_int(settings, setting_CameraIndex);
	const bool tallyActive = obs_data_get_bool(settings, setting_TallyActive);

	pTally->GetSettings()->SetCameraName(cameraName);
	pTally->GetSettings()->SetCameraIndex(cameraIndex);
	pTally->GetSettings()->SetTallyActive(tallyActive);		
}

void tallySwitcher_VideoTick(void *data, float seconds)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(seconds);
}

void tallySwitcher_Activate(void *data)
{
	Tally *pTally = static_cast<Tally *>(data);
		
	pTally->GetSettings()->SetTallyActive(true);

	blog(LOG_INFO, "Activate %s Index=%i", pTally->GetSettings()->GetCameraName().c_str(), pTally->GetSettings()->GetCameraIndex());
}

void tallySwitcher_Deactivate(void *data)
{	
	Tally *pTally = static_cast<Tally *>(data);

	pTally->GetSettings()->SetTallyActive(false);

	blog(LOG_INFO, "Deactivate %s Index=%i", pTally->GetSettings()->GetCameraName().c_str(), pTally->GetSettings()->GetCameraIndex());
}

void tallySwitcher_Show(void *data)
{
	UNUSED_PARAMETER(data);
	blog(LOG_INFO, "Show");
}

void tallySwitcher_Hide(void *data)
{
	UNUSED_PARAMETER(data);
	blog(LOG_INFO, "Hide");
}


void tallySwitcher_Setup() 
{
	tallySwitcher.id = "Tally Switcher Options";
	tallySwitcher.type = OBS_SOURCE_TYPE_INPUT;
	tallySwitcher.get_name = tallySwitcher_GetName;
	tallySwitcher.create = tallySwitcher_Create;
	tallySwitcher.destroy = tallySwitcher_Destroy;
	tallySwitcher.get_defaults = tallySwitcher_GetDefaults;
	tallySwitcher.get_properties = tallySwitcher_Properties;
	tallySwitcher.update = tallySwitcher_Update;
	tallySwitcher.video_tick = tallySwitcher_VideoTick;
	tallySwitcher.activate = tallySwitcher_Activate;
	tallySwitcher.deactivate = tallySwitcher_Deactivate;
	tallySwitcher.show = tallySwitcher_Show;
	tallySwitcher.hide = tallySwitcher_Hide;

	obs_register_source(&tallySwitcher);
}

bool obs_module_load(void) 
{	
	tallySwitcher_Setup();
	return true;
}

void obs_module_unload(void)
{

}

const char *obs_module_author(void) 
{
	return "tsky";
}

const char *obs_module_name(void) 
{
	return "Tally Switcher";
}

const char *obs_module_description(void) 
{
	return "Automatic tally switching";
}
