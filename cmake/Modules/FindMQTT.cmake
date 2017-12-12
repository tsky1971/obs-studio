# Once done these will be defined:
#
#  MQTT_FOUND
#  MQTT_INCLUDE_DIRS
#  MQTT_LIBRARIES
#
# For use in OBS: 
#
#  MQTT_INCLUDE_DIR

find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(_MQTT QUIET mqtt)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(_lib_suffix 64)
else()
	set(_lib_suffix 32)
endif()

find_path(MQTT_INCLUDE_DIR
	NAMES MQTTAsync.h
	NAMES MQTTClient.h
	NAMES MQTTClientPersistence.h
	HINTS
		ENV mqttPath${_lib_suffix}
		ENV mqttPath
		ENV DepsPath${_lib_suffix}
		ENV DepsPath
		${mqttPath${_lib_suffix}}
		${mqttPath}
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_MQTT_INCLUDE_DIRS}
	PATHS
		/usr/include /usr/local/include /opt/local/include /sw/include
	PATH_SUFFIXES
		include)

find_library(MQTT_LIBA
	NAMES ${_MQTT_LIBRARIES} paho-mqtt3a paho-mqtt3a-static
	HINTS
		ENV MQTTPath${_lib_suffix}
		ENV MQTTPath
		ENV DepsPath${_lib_suffix}
		ENV DepsPath
		${MQTTPath${_lib_suffix}}
		${MQTTPath}
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_MQTT_LIBRARY_DIRS}
	PATHS
		/usr/lib /usr/local/lib /opt/local/lib /sw/lib
	PATH_SUFFIXES
		lib${_lib_suffix} lib
		libs${_lib_suffix} libs
		bin${_lib_suffix} bin
		../lib${_lib_suffix} ../lib
		../libs${_lib_suffix} ../libs
		../bin${_lib_suffix} ../bin)

find_library(MQTT_LIBC
	NAMES ${_MQTT_LIBRARIES} paho-mqtt3c paho-mqtt3c-static
	HINTS
		ENV MQTTPath${_lib_suffix}
		ENV MQTTPath
		ENV DepsPath${_lib_suffix}
		ENV DepsPath
		${MQTTPath${_lib_suffix}}
		${MQTTPath}
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_MQTT_LIBRARY_DIRS}
	PATHS
		/usr/lib /usr/local/lib /opt/local/lib /sw/lib
	PATH_SUFFIXES
		lib${_lib_suffix} lib
		libs${_lib_suffix} libs
		bin${_lib_suffix} bin
		../lib${_lib_suffix} ../lib
		../libs${_lib_suffix} ../libs
		../bin${_lib_suffix} ../bin)

		
find_library(MQTT_LIBA_STATIC
	NAMES ${_MQTT_LIBRARIES} paho-mqtt3a-static
	HINTS
		ENV MQTTPath${_lib_suffix}
		ENV MQTTPath
		ENV DepsPath${_lib_suffix}
		ENV DepsPath
		${MQTTPath${_lib_suffix}}
		${MQTTPath}
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_MQTT_LIBRARY_DIRS}
	PATHS
		/usr/lib /usr/local/lib /opt/local/lib /sw/lib
	PATH_SUFFIXES
		lib${_lib_suffix} lib
		libs${_lib_suffix} libs
		bin${_lib_suffix} bin
		../lib${_lib_suffix} ../lib
		../libs${_lib_suffix} ../libs
		../bin${_lib_suffix} ../bin)

find_library(MQTT_LIBC_STATIC
	NAMES ${_MQTT_LIBRARIES} paho-mqtt3c-static
	HINTS
		ENV MQTTPath${_lib_suffix}
		ENV MQTTPath
		ENV DepsPath${_lib_suffix}
		ENV DepsPath
		${MQTTPath${_lib_suffix}}
		${MQTTPath}
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_MQTT_LIBRARY_DIRS}
	PATHS
		/usr/lib /usr/local/lib /opt/local/lib /sw/lib
	PATH_SUFFIXES
		lib${_lib_suffix} lib
		libs${_lib_suffix} libs
		bin${_lib_suffix} bin
		../lib${_lib_suffix} ../lib
		../libs${_lib_suffix} ../libs
		../bin${_lib_suffix} ../bin)

		
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MQTT DEFAULT_MSG MQTT_LIB MQTT_INCLUDE_DIR)
mark_as_advanced(MQTT_INCLUDE_DIR MQTT_LIB)

if(MQTT_FOUND)
	set(MQTT_INCLUDE_DIRS ${MQTT_INCLUDE_DIR})
	set(MQTT_LIBRARIES_A ${MQTT_LIBA})
	set(MQTT_LIBRARIES_C ${MQTT_LIBC})
	set(MQTT_LIBRARIES_A_STATIC ${MQTT_LIBA_STATIC})
	set(MQTT_LIBRARIES_C_STATIC ${MQTT_LIBC_STATIC})
endif()
