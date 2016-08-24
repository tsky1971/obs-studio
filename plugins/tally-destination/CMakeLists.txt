project(tally-destination)

if(MSVC)
	set(tally-destination_PLATFORM_DEPS
		w32-pthreads)
endif()

set(tally-destination_SOURCES
	tally-destination.c)

add_library(tally-destination MODULE
	${tally-destination_SOURCES})
target_link_libraries(tally-destination
	libobs
	${tally-destination_PLATFORM_DEPS})

install_obs_plugin_with_data(tally-destination data)
