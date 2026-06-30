
file(GLOB_RECURSE MY_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

set(CONFIG_FILES 
	"${CMAKE_SOURCE_DIR}/configuration.yaml" 
	"${CMAKE_SOURCE_DIR}/options.xml")

set(IMGUI_SOURCES 
	../ThirdParty/imgui/imgui.cpp
	../ThirdParty/imgui/imgui_draw.cpp
	../thirdparty/imgui/imgui_widgets.cpp
	../ThirdParty/imgui/imgui_tables.cpp
	../ThirdParty/imgui/imgui_demo.cpp
	../ThirdParty/imgui/backends/imgui_impl_glfw.cpp
    ../ThirdParty/imgui/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_HEADERS
    ../ThirdParty/imgui/imgui.h
    ../ThirdParty/imgui/imconfig.h
)

source_group("Source Files\\imgui" FILES ${IMGUI_SOURCES})
source_group("Header Files\\imgui" FILES ${IMGUI_HEADERS})
source_group("Header Files" FILES ${MY_HEADERS})
source_group("Config Files" FILES ${CONFIG_FILES})

add_library(KonstruktorLib STATIC 
    shaders.cpp 
    texture.cpp 
    optionsreader.cpp
)

add_executable(Konstruktor 
	main.cpp
)

target_sources(KonstruktorLib PRIVATE 
	${IMGUI_SOURCES} 
    ${IMGUI_HEADERS}
    ${MY_HEADERS}
	${CONFIG_FILES}
)

target_sources(Konstruktor PRIVATE 
    ${IMGUI_HEADERS}
    ${MY_HEADERS}
)

# Headers
target_include_directories(KonstruktorLib PRIVATE 

	"${CMAKE_SOURCE_DIR}/include"
    ../ThirdParty/imgui
    ../ThirdParty/imgui/backends
)

target_include_directories(Konstruktor PRIVATE 

	"${CMAKE_SOURCE_DIR}/include"
    ../ThirdParty/imgui
    ../ThirdParty/imgui/backends
)

# Link the imported target
target_link_libraries(KonstruktorLib PRIVATE 
		glfw
		glm::glm
		soil
		GLEW::GLEW
		freetype
		opengl32
        yaml-cpp::yaml-cpp)

target_link_libraries(Konstruktor PRIVATE 
		KonstruktorLib
        GLEW::GLEW)

# get_target_property(CURRENT_INCLUDES Konstruktor INCLUDE_DIRECTORIES)
# message(STATUS "Konstruktor target includes: ${CURRENT_INCLUDES}")

add_custom_command(TARGET Konstruktor POST_BUILD
    # Copy options.xml to both locations
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/options.xml" "${CMAKE_SOURCE_DIR}/build/src/options.xml"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/options.xml" "${CMAKE_SOURCE_DIR}/build/bin/options.xml"
    
    # Copy configuration.yaml to both locations
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/configuration.yaml" "${CMAKE_SOURCE_DIR}/build/src/configuration.yaml"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/configuration.yaml" "${CMAKE_SOURCE_DIR}/build/bin/configuration.yaml"
)

set_target_properties(Konstruktor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/bin"
	CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

#DOXYGEN

set(DOXYGEN_EXECUTABLE "${CMAKE_SOURCE_DIR}/ThirdParty/doxygen-1.17.0/bin/doxygen.exe")

if(EXISTS ${DOXYGEN_EXECUTABLE})
    message(STATUS "Using local Doxygen: ${DOXYGEN_EXECUTABLE}")
    
    # Configure your template
    configure_file("${CMAKE_SOURCE_DIR}/ThirdParty/doxygen-1.17.0/Doxyfile.in" 
                   "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)

    # Add the generator target
    add_custom_target(generate_docs
        COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Generating docs using local ThirdParty Doxygen"
    )
else()
    message(WARNING "Doxygen binary not found in ThirdParty folder!")
endif()


