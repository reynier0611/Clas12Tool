
set(INCLUDES
	${CMAKE_CURRENT_SOURCE_DIR}/Hipo4
	${CMAKE_CURRENT_SOURCE_DIR}/Clas12Banks4	
	${CMAKE_CURRENT_SOURCE_DIR}/Clas12Root
	${LZ4_INCLUDE_DIR}
)

include_directories(${INCLUDES})
add_executable(helloWord helloWord.cpp)
target_link_libraries(helloWord ${ROOT_LIBRARIES} Hipo4 Clas12Banks4)

add_executable(clasReactions clasReactions.cpp)
target_link_libraries(clasReactions ${ROOT_LIBRARIES} Hipo4 Clas12Banks4)

#add_executable(readFile readFile.cpp)
#target_link_libraries(readFile Hipo4 ${ROOT_LIBRARIES} )
