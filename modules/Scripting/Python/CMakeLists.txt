# Custom compile folder for Python scripting

# Find Python
find_package(PythonLibs)

# If python was found
if(PythonLibs_FOUND)
  # Get all files for python compilation
  file(GLOB MODULES_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

  # Sort the files
  sort_list(MODULES_SRCS)

  # Add include directories for compile
  include_directories(${PYTHON_INCLUDE_DIRS})

  # Set it to use module include crap with python crap
  set_source_files_properties(${MODULES_SRCS} PROPERTIES LANGUAGE CXX COMPILE_FLAGS "${CXXFLAGS}")

  # iterate through the source files and compile them
  foreach(SRC ${MODULES_SRCS})

    # Convert the real source file extension to have a .so extension
    string(REGEX REPLACE "\\.cpp$" ".so" SO ${SRC})

    # Generate the Module and set it's linker flags, also set it to depend on the main Navn executable to be built beforehand
    add_library(${SO} MODULE ${SRC})

    # Set the library to be compiled with specific flags
    # old py flags: -L/usr/lib/python2.7/config -lpthread -ldl -lutil -lm -lpython2.7 -Xlinker -Wl,-O1 -Wl,-Bsymbolic-functions
    set_target_properties(${SO} PROPERTIES LINKER_LANGUAGE CXX PREFIX "" SUFFIX "" LINK_FLAGS "${PYTHON_LIBRARIES}")
    add_dependencies(${SO} ${PROGRAM_NAME})

    # Where to install the module
    install(TARGETS ${SO}
      DESTINATION Modules
    )

  endforeach(SRC ${MODULES_SRCS})
elseif(PythonLibs_FOUND)
  # We don't have python :(
  message("  This is not a fatal error - Python interpreter will not be built.")
endif(PythonLibs_FOUND)