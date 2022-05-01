# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (game_engine)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(
#		MAIN
		main
		main/main.cpp
#		UTILS
		src/utils/meshloader.cpp src/utils/meshloader.hpp
		src/utils/texture.cpp src/utils/texture.hpp
		src/utils/objloader.cpp src/utils/objloader.hpp
		src/utils/printer.cpp src/utils/printer.hpp
		src/utils/Transform.cpp src/utils/Transform.hpp
		src/utils/Geometry3D.cpp src/utils/Geometry3D.hpp
#		SHADERS
        src/shader/ShadersDataManager.cpp src/shader/ShadersDataManager.hpp
        src/shader/VAODataManager.cpp src/shader/VAODataManager.hpp
		src/shader/Shaders.cpp src/shader/Shaders.hpp
#		MESH
		src/mesh/Mesh.cpp src/mesh/Mesh.hpp
		src/mesh/LODMesh.cpp src/mesh/LODMesh.hpp
#		SCENE GRAPH
		src/scene_graph/NodeSG.cpp src/scene_graph/NodeSG.hpp
		src/scene_graph/NodeOnTopSG.cpp src/scene_graph/NodeOnTopSG.hpp
		src/scene_graph/RootSG.cpp src/scene_graph/RootSG.hpp
		src/scene_graph/ElementSG.cpp src/scene_graph/ElementSG.hpp
		src/scene_graph/NodeGameSG.cpp src/scene_graph/NodeGameSG.hpp
#		SCENE
		src/scene/SolarSystem.cpp src/scene/SolarSystem.hpp
		src/scene/SceneLand.cpp src/scene/SceneLand.hpp
		src/scene/Scene.cpp src/scene/Scene.hpp
		src/scene/BounceOBBScene.cpp src/scene/BounceOBBScene.hpp
		src/scene/BounceSphereBBScene.cpp src/scene/BounceSphereBBScene.hpp
		src/scene/LabScene.cpp src/scene/LabScene.hpp
#		MATERIAL
		src/material/Material.cpp src/material/Material.hpp
		src/material/MaterialColor.cpp src/material/MaterialColor.hpp
		src/material/MaterialTexture.cpp src/material/MaterialTexture.hpp
#		LIGHT
		src/light/Light.cpp src/light/Light.hpp
		src/light/LightShader.hpp
        src/light/light_behavior/LightBehavior.cpp src/light/light_behavior/LightBehavior.hpp
		src/light/PositionLight.cpp src/light/PositionLight.hpp
        src/light/DirectionLight.cpp src/light/DirectionLight.hpp
		src/light/SpotLight.cpp src/light/SpotLight.hpp
        src/light/light_behavior/DirectionLightBehavior.cpp src/light/light_behavior/DirectionLightBehavior.hpp
        src/light/light_behavior/PositionLightBehavior.cpp src/light/light_behavior/PositionLightBehavior.hpp
		src/light/light_behavior/SpotLightBehavior.cpp src/light/light_behavior/SpotLightBehavior.hpp
#		PHYSICS
		src/physics/bounding_box/BoundingBox.cpp src/physics/bounding_box/BoundingBox.hpp
		src/physics/bounding_box/AABB.cpp src/physics/bounding_box/AABB.hpp
		src/physics/bounding_box/OBB.cpp src/physics/bounding_box/OBB.hpp
		src/physics/bounding_box/RCBB.cpp src/physics/bounding_box/RCBB.hpp
		src/physics/bounding_box/SphereBB.cpp src/physics/bounding_box/SphereBB.hpp
		src/physics/bounding_box/BBFactory.cpp src/physics/bounding_box/BBFactory.hpp
		src/physics/RigidBodyVolume.cpp src/physics/RigidBodyVolume.hpp
		src/physics/Collision.cpp src/physics/Collision.hpp
		src/physics/PhysicsSystem.cpp src/physics/PhysicsSystem.hpp
		src/physics/force/Force.cpp src/physics/force/Force.hpp
		src/physics/force/GravityForce.cpp src/physics/force/GravityForce.hpp
#		CHARACTER
		src/Character.cpp src/Character.hpp
		)
target_link_libraries(main
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(main PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/main/")
create_target_launcher(main WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/main/")




SOURCE_GROUP(src REGULAR_EXPRESSION ".*/src/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/shader/.*" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/main${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/main/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

