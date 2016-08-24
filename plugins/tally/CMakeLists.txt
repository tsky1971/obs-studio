project(tally)

if(MSVC)
	set(tally_PLATFORM_DEPS
		w32-pthreads)
endif()

set(tally_SOURCES
	tally-source.c)

add_library(tally MODULE
	${tally_SOURCES})
target_link_libraries(tally
	libobs
	${tally_PLATFORM_DEPS})

install_obs_plugin_with_data(tally data)
