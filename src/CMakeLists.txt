cmake_minimum_required(VERSION 3.17)

project(RockRaiders)

set(var_main_sources 
	main.h
	Entity.cpp
	Block.cpp
	GameMap.cpp
	MapLoader.cpp
	BlockPropertiesLookup.cpp
	CharBlockLookup.h
	LocatedEntity.h
	TestEntity.cpp
	Graph.cpp
	Navigation.h
	World.cpp
	JobDoer.cpp
	Job.cpp
	Pickup.cpp
	Worker.cpp
	JobPickup.h
	JobWalk.h
	JobDrill.h
	TileJobs.cpp
	Ore.h
	Crystal.h
	JobDeliver.h)

find_package(SFML 2.5.1 COMPONENTS audio graphics window system REQUIRED)

add_executable(RockRaiders ${var_main_sources})

target_link_libraries(RockRaiders sfml-window sfml-graphics sfml-audio sfml-network)