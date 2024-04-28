cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 17)

get_filename_component(Application ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" Application ${Application})
project(${Application})

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# ==============================================================
FILE(GLOB_RECURSE engineSources src/Engine/*.cpp src/Engine/*.h  Shaders/*.frag Shaders/*.vert Shaders/*.comp)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${engineSources})
add_library(Engine
${engineSources}
${ImGUISources}
)

message("VK_SDK_PATH : $ENV{VK_SDK_PATH}")

if(APPLE)
	target_include_directories(Engine PRIVATE 
	$ENV{VK_SDK_PATH}/include
	$ENV{GLFW_PATH}/include
	Libraries/glm
	Libraries/stb_image
	src/Engine
	)

	target_link_directories(Engine PRIVATE 
	$ENV{VK_SDK_PATH}/Lib
	$ENV{GLFW_PATH}/Lib
	)

	target_link_libraries(Engine
	vulkan.1
	glfw
	)
else()
	target_include_directories(Engine PRIVATE 
	$ENV{VK_SDK_PATH}/Include
	Libraries/glfw-3.3.8.bin.WIN64/include
	Libraries/glm
	Libraries/stb_image
	src/Engine
	)

	target_link_directories(Engine PRIVATE 
	$ENV{VK_SDK_PATH}/Lib
	Libraries/glfw-3.3.8.bin.WIN64/lib-vc2022
	)

	target_link_libraries(Engine
	vulkan-1
	glfw3
	)
endif()

# ==============================================================
FILE(GLOB SUBDIRECTORIES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/Samples/*)
foreach(SUBDIRECTORY ${SUBDIRECTORIES})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIRECTORY})
    	message("==============================================================")
        get_filename_component(LAST_ELEMENT ${SUBDIRECTORY} NAME)
        message("processing : ${LAST_ELEMENT}")

        FILE(GLOB_RECURSE sample1_sources ${SUBDIRECTORY}/*.cpp ${SUBDIRECTORY}/*.h  ${SUBDIRECTORY}/*.frag  ${SUBDIRECTORY}/*.vert  ${SUBDIRECTORY}/*.comp)
		add_executable(${LAST_ELEMENT} 
		${sample1_sources} 
		${ImGUISources}
		)

		if(APPLE)
			target_include_directories(${LAST_ELEMENT} PRIVATE 
			$ENV{VK_SDK_PATH}/include
			$ENV{GLFW_PATH}/include
			Libraries/glm
			Libraries/stb_image
			src/Engine
			)

			target_link_directories(${LAST_ELEMENT} PRIVATE 
			$ENV{VK_SDK_PATH}/Lib
			$ENV{GLFW_PATH}/Lib
			)

			target_link_libraries(${LAST_ELEMENT} 
			vulkan.1
			glfw
			Engine
			)

		else()
			target_include_directories(${LAST_ELEMENT} PRIVATE 
			$ENV{VK_SDK_PATH}/Include
			Libraries/glfw-3.3.8.bin.WIN64/include
			Libraries/glm
			Libraries/stb_image
			src/Engine
			)

			target_link_directories(${LAST_ELEMENT} PRIVATE 
			$ENV{VK_SDK_PATH}/Lib
			Libraries/glfw-3.3.8.bin.WIN64/lib-vc2022
			Windows_VS2022/Debug
			)

			target_link_libraries(${LAST_ELEMENT} 
			vulkan-1
			glfw3
			Engine
			)
		endif()
    endif()
endforeach()
# ==============================================================


message("==============================================================")


list(GET SUBDIRECTORIES 0 FIRST_ELEMENT)
get_filename_component(TARGET_NAME ${FIRST_ELEMENT} NAME)
message("set default target: ${TARGET_NAME}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})


message("complete.")
message("==============================================================")
