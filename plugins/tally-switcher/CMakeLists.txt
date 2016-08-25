project(tally-switcher)

if(MSVC)
	set(tally-switcher_PLATFORM_DEPS
		w32-pthreads)
endif()

set(tally-switcher_SOURCES
	tally-switcher.cpp
	Tally.cpp
	Tally.h
	TallySettings.cpp
	TallySettings.h
	)

add_library(tally-switcher MODULE
	${tally-switcher_SOURCES})
target_link_libraries(tally-switcher
	libobs
	${tally-switcher_PLATFORM_DEPS})

install_obs_plugin_with_data(tally-switcher data)
