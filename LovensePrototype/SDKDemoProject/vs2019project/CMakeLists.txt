cmake_minimum_required(VERSION 3.16...3.21)

set(PROJECT_NAME "SDKdemo")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_MFC_FLAG 1)
#mt/mtd
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_definitions(-D_AFXDLL)
add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-DNOMINMAX -DUNICODE -D_UNICODE)
add_definitions(-DLOVENSE_SDK_STATIC)
#-------------------------------------------------------------
IF(CMAKE_CL_64)
    set(BITS 64)
    set(BIT_LABEL x64)
ELSE(CMAKE_CL_64)
    set(BITS 32)
    set(BIT_LABEL x86)
ENDIF(CMAKE_CL_64)

include_directories(../WinSDK/include)
#link_directories(../WinSDK/)

file(GLOB PROJECT_FILES        
    *.cpp
    *.c
    *.hpp
    *.h
    *.rc
)

if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
endif()


source_group(
  "sources"
  FILES
   ${PROJECT_FILES}
)

source_group(
  "core"
  FILES
   ${PROJECT_CORE_FILES}
)

add_executable(${PROJECT_NAME} WIN32
    ${PROJECT_FILES}
    ${PROJECT_CORE_FILES}
    ${PROJECT_UI_FILES}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    lovense::toy_sdk
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "toys-control-sdk")

#-----------------------------------------------------------------------------------------
if(MSVC)
if(CMAKE_CL_64)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/  DESTINATION SDKDemoProject/vs2019project/)
endif()
endif()


install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION SDKDemoProject/bin/${BIT_LABEL}_$<CONFIGURATION>
        LIBRARY DESTINATION SDKDemoProject/bin/${BIT_LABEL}_$<CONFIGURATION>
        RUNTIME DESTINATION SDKDemoProject/bin/${BIT_LABEL}_$<CONFIGURATION>
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../toys-control-lib/$<CONFIGURATION>/shared/LovenseToys.dll
  DESTINATION SDKDemoProject/bin/${BIT_LABEL}_$<CONFIGURATION>)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../toys-control-lib/dep-libs/curl/libs/${BIT_LABEL}/libcurl.lib
  DESTINATION WinSDK/${BIT_LABEL}/$<CONFIGURATION>_Static OPTIONAL)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../toys-control-lib/dep-libs/zlib/libs/${BIT_LABEL}/zlib.lib
  DESTINATION WinSDK/${BIT_LABEL}/$<CONFIGURATION>_Static OPTIONAL)
