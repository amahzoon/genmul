file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

#to generate the version.hpp

execute_process(COMMAND  
		sh ${CMAKE_CURRENT_SOURCE_DIR}/version.sh ${CMAKE_CURRENT_SOURCE_DIR}
    )

## main.cpp and GenMul.cpp are not needed if using Emscripten
if (EMSCRIPTEN)
	list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
	list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/GenMul.cpp)
else()
	list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/GenMul_Emscripten.cpp)
endif()

add_executable(genmul
        ${SOURCES}
        ${HEADERS})

INSTALL(TARGETS genmul RUNTIME DESTINATION bin)

## genmul.wasm is also needed for final html file
if (EMSCRIPTEN)
	INSTALL (FILES "../build/src/genmul.wasm" DESTINATION bin)
endif()
