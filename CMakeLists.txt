
cmake_minimum_required (VERSION 2.6)
project (ColladaParser)


set (CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

option (BUILD_TESTS "Build unit tests and create test target?")
option (BUILD_INTROSPECTION "Build the parser with introspection for Myelin?")


configure_file (
	${PROJECT_SOURCE_DIR}/ColladaParserConfig.cmake.in
	${PROJECT_BINARY_DIR}/ColladaParserConfig.cmake
	@ONLY)



set (SOURCE_FILES
	src/Document.cpp
	src/Effect.cpp
	src/Geometry.cpp
	src/Input.cpp
	src/Material.cpp
	src/Node.cpp
	src/Profile.cpp
	src/Reader.cpp
	src/Source.cpp
	src/Transform.cpp
	src/VisualScene.cpp)


set (HEADER_FILES
	include/ColladaParser/Config.h
	include/ColladaParser/DataSource.h
	include/ColladaParser/Document.h
	include/ColladaParser/Effect.h
	include/ColladaParser/Geometry.h
	include/ColladaParser/Input.h
	include/ColladaParser/Material.h
	include/ColladaParser/Node.h
	include/ColladaParser/Profile.h
	include/ColladaParser/Reader.h
	include/ColladaParser/Source.h
	include/ColladaParser/Transform.h
	include/ColladaParser/Types.h
	include/ColladaParser/VisualScene.h)


set (TICPP_SOURCE_FILES
	src/ticpp/ticpp.cpp
	src/ticpp/tinystr.cpp
	src/ticpp/tinyxml.cpp
	src/ticpp/tinyxmlerror.cpp
	src/ticpp/tinyxmlparser.cpp)


set (TICPP_HEADER_FILES
	include/ticpp/ticpp.h
	include/ticpp/ticpprc.h
	include/ticpp/tinystr.h
	include/ticpp/tinyxml.h)


include_directories (
	include/
	include/ColladaParser/
	include/ticpp/)


# build library
add_library (ColladaParser SHARED
	${SOURCE_FILES}
	${HEADER_FILES}
	${TICPP_SOURCE_FILES}
	${TICPP_HEADER_FILES})


if (GCC AND NOT MINGW)
	set (EXPORT_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
endif ()


set_target_properties (ColladaParser PROPERTIES COMPILE_FLAGS "-DTIXML_USE_TICPP -DCOLLADA_PARSER_DLL -DCOLLADA_PARSER_DLL_EXPORTS ${EXPORT_FLAGS}")



install (TARGETS ColladaParser EXPORT ColladaParserTargets DESTINATION lib)
install (EXPORT ColladaParserTargets DESTINATION lib/cmake/ColladaParser)
install (FILES ${PROJECT_BINARY_DIR}/ColladaParserConfig.cmake DESTINATION lib/cmake/ColladaParser)

install (FILES ${HEADER_FILES} DESTINATION include/ColladaParser)

