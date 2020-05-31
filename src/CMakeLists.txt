cmake_minimum_required(VERSION 3.17)

project(RockRaiders)

set(var_main_sources 
	main.cpp
	Entity.h
	Block.h
	GameMap.cpp
	MapLoader.cpp
	MapTextureLookup.h
	CharBlockLookup.h
	TestEntity.cpp)

find_package(SFML 2.5.1 COMPONENTS audio
graphics window system REQUIRED)

add_executable(RockRaiders ${var_main_sources})

target_link_libraries(RockRaiders sfml-window sfml-graphics sfml-audio sfml-network)