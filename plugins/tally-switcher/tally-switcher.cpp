
#include <obs-module.h>
#include <obs-data.h>

#include <fstream>
#include <string>
#include <sstream>


OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("tallySwitcher", "en-US")

struct tally_destination {
	obs_source_t *source;

	char         *file;
	bool         persistent;
	time_t       file_timestamp;
	float        update_time_elapsed;
	uint64_t     last_time;
	bool         active;

	gs_image_file_t image;
};

struct obs_source_info tallySwitcher;

const char *tallySwitcher_GetName(void *type_data) {
	UNUSED_PARAMETER(type_data);
	return "Scene Switcher";
}

void tallySwitcher_Destroy(void *data) {
	UNUSED_PARAMETER(data);
}

uint32_t tallySwitcher_GetTally(void *data) {
	UNUSED_PARAMETER(data);
	return 0;
}

uint32_t tallySwitcher_SetTally(void *data) {
	UNUSED_PARAMETER(data);
	return 0;
}

void tallySwitcher_Setup() {
	tallySwitcher.id = "Tally Switcher Options";
	tallySwitcher.type = OBS_SOURCE_TYPE_INPUT;
	tallySwitcher.get_name = tallySwitcher_GetName;
	tallySwitcher.destroy = tallySwitcher_Destroy;
	tallySwitcher.get_tally = tallySwitcher_GetTally;
	tallySwitcher.set_tally = tallySwitcher_SetTally;
	tallySwitcher.update = tallySwitcher_Update;	
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
