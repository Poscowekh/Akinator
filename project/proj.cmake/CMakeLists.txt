cmake_minimum_required (VERSION 3.5)
project(Akinator)

include_directories(../data data)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


##########################################################
#########################OXYGINE##########################
add_subdirectory(../../oxygine/oxygine-framework oxygine-framework)

add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})

link_directories(${OXYGINE_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")
###########################E#N#D############################


file(MAKE_DIRECTORY build/data)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../data/res.xml DESTINATION data)
configure_file(../data/res.xml data/res.xml COPYONLY)


##########################################################
##########################JSON############################
include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
###########################E#N#D############################


add_executable(Akinator
    ../src/example.cpp 
    ../src/main.cpp
    ../src/Presenter/Game_presenter.cpp
    ../src/View/Game_view.cpp
    ../src/Menu/Menu.cpp
    )

target_link_libraries(Akinator PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Akinator PRIVATE ${OXYGINE_CORE_LIBS})
include_directories("headers")

