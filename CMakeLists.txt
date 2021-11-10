cmake_minimum_required(VERSION 3.18.0)
project(aravisgrabber VERSION 0.1.0)

include(CTest)
enable_testing()
include_directories(SYSTEM /usr/local/include/aravis-0.8)
include_directories(SYSTEM /usr/include/glib-2.0)
include_directories(SYSTEM /usr/lib/glib-2.0/include)

add_executable(aravisgrabber main.cpp)

add_subdirectory(src)

set_property(TARGET aravisgrabber PROPERTY CXX_STANDARD 20)
#target_include_directories(aravisgrabber PRIVATE /usr/local/include/aravis-0.8)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_library(aravis-8.0 SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(aravis-8.0 PROPERTIES
  IMPORTED_LOCATION "/usr/local/lib/libaravis-0.8.so"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/local/include/aravis-0.8"
)

#set(FOO_SRCS "foo.cpp")
#add_executable(foo ${FOO_SRCS})
target_link_libraries(aravisgrabber aravis-8.0)
target_link_libraries(aravisgrabber glib-2.0)
