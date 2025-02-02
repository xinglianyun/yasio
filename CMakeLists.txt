# A cross platform socket APIs, support ios & android & wp8 & window store universal app.
# Copyright (c) 2012-2019 halx99.
# Licensed under the MIT License (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
# http://opensource.org/licenses/MIT
# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

cmake_minimum_required(VERSION 3.14.1)

if ( WIN32 AND NOT CYGWIN AND NOT ( CMAKE_SYSTEM_NAME STREQUAL "WindowsStore" ) AND NOT ANDROID)
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT" CACHE STRING "")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd" CACHE STRING "")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT" CACHE STRING "")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd" CACHE STRING "")
endif ()

project(yasio)

if ( IOS )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode")
endif ()

#option ( LUA_COMPAT_MODULE "compat module" ON)

find_path(YASIO_PROJECT_DIR NAMES SConstruct
    PATHS 
    ${CMAKE_SOURCE_DIR}
    NO_DEFAULT_PATH
    )

MARK_AS_ADVANCED(YASIO_PROJECT_DIR)

#set(LUA_SRC_PATH lua-5.3.5/src)
#set(LUAJIT_SRC_PATH luajit-2.1.0b3/src)

if (NOT WIN32 OR CYGWIN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if (ANDROID)
    if (NOT THIRDPART_LIB)
        set(THIRDPART_LIB "-llog")
    else()
        set(THIRDPART_LIB "${THIRDPART_LIB} -llog")
    endif()
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${THIRDPART_INC}
)

#~ if(WIN32)
#~     LINK_DIRECTORIES(
#~     "${CMAKE_SOURCE_DIR}\\lib"
#~     )
#~ endif()

set ( YASIO_CORE
    yasio/xxsocket.cpp
    yasio/yasio.cpp
    yasio/ibstream.cpp
    yasio/obstream.cpp
    yasio/impl/yasio_ni.cpp
)

macro(source_group_by_dir proj_dir source_files)
    if(MSVC OR APPLE)
        get_filename_component(sgbd_cur_dir ${proj_dir} ABSOLUTE)
        foreach(sgbd_file ${${source_files}})
			get_filename_component(sgbd_abs_file ${sgbd_file} ABSOLUTE)
            file(RELATIVE_PATH sgbd_fpath ${sgbd_cur_dir} ${sgbd_abs_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC OR APPLE)
endmacro(source_group_by_dir)

source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} YASIO_CORE)

if (APPLE)
    if (IOS)
        set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
        add_library(yasio STATIC
           ${YASIO_CORE}
           ${THIRDPART_SRC}
        )
		set_xcode_property (yasio IPHONEOS_DEPLOYMENT_TARGET "7.0" "all")
    else ()
        set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_64_BIT)")
        add_library(yasio MODULE
            ${YASIO_CORE}
            ${THIRDPART_SRC}
        )
        set_target_properties ( yasio PROPERTIES BUNDLE TRUE )
    endif ()
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Switch")
    add_library(yasio STATIC
        ${YASIO_CORE}
        ${THIRDPART_SRC}
    )
    target_compile_options(yasio PRIVATE -m64 -mcpu=cortex-a57+fp+simd+crypto+crc -fno-common -fno-short-enums -ffunction-sections -fdata-sections -fPIC -fms-extensions)
else ( )
    add_library(yasio SHARED
        ${YASIO_CORE}
        ${THIRDPART_SRC}
    )
endif ( )

if(UINT_ESPECIALLY)
    ADD_DEFINITIONS(-DUINT_ESPECIALLY)
endif()
	
if ( WIN32 AND NOT CYGWIN )
    target_link_libraries(yasio
        ws2_32
        ${THIRDPART_LIB} 
    )
else ( )
    target_link_libraries(yasio
        m
        ${THIRDPART_LIB} 
    )
endif ( )
