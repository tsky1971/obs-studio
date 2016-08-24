#include <obs-module.h>
#include <graphics/image-file.h>
#include <util/platform.h>
#include <util/dstr.h>
#include <sys/stat.h>

#define blog(log_level, format, ...) \
	blog(log_level, "[tally: '%s'] " format, \
			obs_source_get_name(context->source), ##__VA_ARGS__)

#define debug(format, ...) \
	blog(LOG_DEBUG, format, ##__VA_ARGS__)
#define info(format, ...) \
	blog(LOG_INFO, format, ##__VA_ARGS__)
#define warn(format, ...) \
	blog(LOG_WARNING, format, ##__VA_ARGS__)

#define T_CAMERA	T_("CAMERA")
	
struct tally {
	obs_source_t *source;

	char         *file;
	bool         persistent;
	time_t       file_timestamp;
	float        update_time_elapsed;
	uint64_t     last_time;
	bool         active;
	
	uint32_t	camera;

	gs_image_file_t image;
};


static time_t get_modified_timestamp(const char *filename)
{
	struct stat stats;
	if (os_stat(filename, &stats) != 0)
		return -1;
	return stats.st_mtime;
}

static const char *tally_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("TallyInput");
}

static void tally_load(struct tally *context)
{
	char *file = context->file;

	obs_enter_graphics();
	gs_image_file_free(&context->image);
	obs_leave_graphics();

	if (file && *file) {
		debug("loading texture '%s'", file);
		context->file_timestamp = get_modified_timestamp(file);
		gs_image_file_init(&context->image, file);
		context->update_time_elapsed = 0;

		obs_enter_graphics();
		gs_image_file_init_texture(&context->image);
		obs_leave_graphics();

		if (!context->image.loaded)
			warn("failed to load texture '%s'", file);
	}
}

static void tally_unload(struct tally *context)
{
	obs_enter_graphics();
	gs_image_file_free(&context->image);
	obs_leave_graphics();
}

static void tally_update(void *data, obs_data_t *settings)
{
	struct tally *context = data;
	const char *file = obs_data_get_string(settings, "file");
	const bool unload = obs_data_get_bool(settings, "unload");
	int camera_index = context->camera = obs_data_get_int(settings, "camera");

	if (context->file)
		bfree(context->file);
	context->file = bstrdup(file);
	context->persistent = !unload;

	/* Load the image if the source is persistent or showing */
	if (context->persistent || obs_source_showing(context->source))
		tally_load(data);
	else
		tally_unload(data);

	blog(LOG_DEBUG, "tally camera_index=%i", camera_index);
}

static void tally_defaults(obs_data_t *settings)
{
	obs_data_set_default_bool(settings, "unload", false);
}

static void tally_show(void *data)
{
	struct tally *context = data;

	if (!context->persistent)
		tally_load(context);
}

static void tally_hide(void *data)
{
	struct tally *context = data;

	if (!context->persistent)
		tally_unload(context);
}

static void *tally_create(obs_data_t *settings, obs_source_t *source)
{
	struct tally *context = bzalloc(sizeof(struct tally));
	context->source = source;

	tally_update(context, settings);
	return context;
}

static void tally_destroy(void *data)
{
	struct tally *context = data;

	tally_unload(context);

	if (context->file)
		bfree(context->file);
	bfree(context);
}

static uint32_t tally_getwidth(void *data)
{
	struct tally *context = data;
	return context->image.cx;
}

static uint32_t tally_getheight(void *data)
{
	struct tally *context = data;
	return context->image.cy;
}

static void tally_render(void *data, gs_effect_t *effect)
{
	struct tally *context = data;

	int camera_index = context->camera;

	if (!context->image.texture)
		return;

	gs_reset_blend_state();
	gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"),
			context->image.texture);
	gs_draw_sprite(context->image.texture, 0,
			context->image.cx, context->image.cy);

	blog(LOG_DEBUG, "tally camera_index=%i", camera_index);
}

static void tally_tick(void *data, float seconds)
{
	struct tally *context = data;
	uint64_t frame_time = obs_get_video_frame_time();

	if (obs_source_active(context->source)) {
		if (!context->active) {
			if (context->image.is_animated_gif)
				context->last_time = frame_time;
			context->active = true;
		}

	} else {
		if (context->active) {
			if (context->image.is_animated_gif) {
				context->image.cur_frame = 0;
				context->image.cur_loop = 0;
				context->image.cur_time = 0;

				obs_enter_graphics();
				gs_image_file_update_texture(&context->image);
				obs_leave_graphics();
			}

			context->active = false;
		}

		return;
	}

	if (context->last_time && context->image.is_animated_gif) {
		uint64_t elapsed = frame_time - context->last_time;
		bool updated = gs_image_file_tick(&context->image, elapsed);

		if (updated) {
			obs_enter_graphics();
			gs_image_file_update_texture(&context->image);
			obs_leave_graphics();
		}
	}

	context->last_time = frame_time;

	context->update_time_elapsed += seconds;

	if (context->update_time_elapsed >= 1.0f) {
		time_t t = get_modified_timestamp(context->file);
		context->update_time_elapsed = 0.0f;

		if (context->file_timestamp < t) {
			tally_load(context);
		}
	}
}


static const char *image_filter =
	"All formats (*.bmp *.tga *.png *.jpeg *.jpg *.gif);;"
	"BMP Files (*.bmp);;"
	"Targa Files (*.tga);;"
	"PNG Files (*.png);;"
	"JPEG Files (*.jpeg *.jpg);;"
	"GIF Files (*.gif)";

static obs_properties_t *tally_properties(void *data)
{
	struct tally *s = data;
	struct dstr path = {0};

	obs_properties_t *props = obs_properties_create();

	if (s && s->file && *s->file) {
		const char *slash;

		dstr_copy(&path, s->file);
		dstr_replace(&path, "\\", "/");
		slash = strrchr(path.array, '/');
		if (slash)
			dstr_resize(&path, slash - path.array + 1);
	}

	obs_properties_add_path(props,
			"file", obs_module_text("File"),
			OBS_PATH_FILE, image_filter, path.array);
	obs_properties_add_bool(props,
			"unload", obs_module_text("UnloadWhenNotShowing"));

	obs_properties_add_int(props,
		"camera", obs_module_text("CameraIndex"), 0, 4, 1);

	dstr_free(&path);

	return props;
}


static struct obs_source_info tally_info = {
	.id = "tally",
	.type = OBS_SOURCE_TYPE_INPUT,
	.output_flags = OBS_SOURCE_VIDEO,
	.get_name = tally_get_name,
	.create = tally_create,
	.destroy = tally_destroy,
	.update = tally_update,
	.get_defaults = tally_defaults,
	.show = tally_show,
	.hide = tally_hide,
	.get_width = tally_getwidth,
	.get_height = tally_getheight,
	.video_render = tally_render,
	.video_tick = tally_tick,
	.get_properties = tally_properties
};

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("tally", "en-US")

extern struct obs_source_info slideshow_info;

bool obs_module_load(void)
{
	obs_register_source(&tally_info);
	return true;
}
