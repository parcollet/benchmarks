# All test
FILE(GLOB_RECURSE TestList RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
message( ${TestList} )
FOREACH( TestName1  ${TestList} )
 STRING(REPLACE ".cpp" "" TestName ${TestName1})
 add_executable( ${TestName}  ${CMAKE_CURRENT_SOURCE_DIR}/${TestName}.cpp )
 target_link_libraries(${TestName} triqs benchmark ) 
 triqs_set_rpath_for_target(${TestName})
ENDFOREACH( TestName1  ${TestList} )

