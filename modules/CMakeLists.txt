file(GLOB MODULES_FOLDERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
LIST(REMOVE_ITEM MODULES_FOLDERS "CMakeFiles")
#Remove kate's stupid temp files
LIST(REMOVE_ITEM MODULES_FOLDERS "*~")

# if we're on windows, tell it to do MODULE_COMPILE define so CoreExport can reverse for dll's and such
if(WIN32)
  add_definitions(-DMODULE_COMPILE)
endif(WIN32)

# Iterate through the directories
foreach(MODULE_FOLDER ${MODULES_FOLDERS})
  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_FOLDER}")

    # if a cmake list file exists, we need to extend the build instead of treating it like a module
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_FOLDER}/CMakeLists.txt)
      # include the file
      include(${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_FOLDER}/CMakeLists.txt)
    else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_FOLDER}/CMakeLists.txt)

    # Get a list of all .cpp files in this directory
    file(GLOB MODULES_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${MODULE_FOLDER}/*.cpp")
    sort_list(MODULES_SRCS)

    # Set all the files to use C++ as well as set their compile flags (use the Module-specific compile flags, though)
    set_source_files_properties(${MODULES_SRCS} PROPERTIES LANGUAGE CXX COMPILE_FLAGS "${CXXFLAGS}")

    # Create an empty list to store extra include directories
    set(EXTRA_INCLUDES)

    # Get the length of the folder name
    string(LENGTH ${MODULE_FOLDER} FOLDER_LEN)
    # Add one (the /)
    math(EXPR FOLDER_LEN "${FOLDER_LEN} + 1")

    # Iterate through all the source files
    foreach(SRC ${MODULES_SRCS})
      # Get the length of the new source file
      string(LENGTH ${SRC} SRC_LEN)
      # Set FILE_LEN to the length of the source file minus folder length
      math(EXPR FILE_LEN "${SRC_LEN} - ${FOLDER_LEN}")
      # Get the real name of the source file now
      string(SUBSTRING ${SRC} ${FOLDER_LEN} ${FILE_LEN} SRC_REALNAME)
      # Convert the real source file extension to have a .so extension
      string(REGEX REPLACE "\\.cpp$" "${CMAKE_SHARED_LIBRARY_SUFFIX}" SO ${SRC_REALNAME})
      # Reset skip_depends
      set(SKIP_DEPENDS)
      # Temporary variable for the current source's include directories
      set(TEMP_INCLUDES)
      # Calculate the header file dependencies for the given source file
      calculate_depends(${SRC} SKIP_DEPENDS TEMP_INCLUDES)
      # If there were some extra include directories, add them to the list
      if(TEMP_INCLUDES)
        append_to_list(EXTRA_INCLUDES ${TEMP_INCLUDES})
      endif(TEMP_INCLUDES)
      # Reset linker flags
      set(TEMP_LDFLAGS)
      # Reset extra dependencies
      set(TEMP_DEPENDENCIES)
      # Reset skip_libraries
      set(SKIP_LIBRARIES)
      # Calculate the library dependencies for the given source file
      calculate_libraries(${SRC} SKIP_LIBRARIES TEMP_LDFLAGS TEMP_DEPENDENCIES)
      if(NOT SKIP_DEPENDS AND NOT SKIP_LIBRARIES)
        # Reset has_function
        set(HAS_FUNCTION)
        # Check the function dependencies for the given source file
        check_functions(${SRC} HAS_FUNCTION)
        # Only continue if this module.has all of the required functions
        if(HAS_FUNCTION)
          # For Visual Studio only, include win32_memory static library, required to override Visual Studio's overrides of the new/delete operators
          if(MSVC)
            set(WIN32_MEMORY win32_memory)
          else(MSVC)
            set(WIN32_MEMORY)
          endif(MSVC)
          # Generate the Module and set it's linker flags, also set it to depend on the main Navn executable to be built beforehand
          add_library(${SO} MODULE ${SRC})
          # Windows requires this because it's weird
          if(WIN32)
            set(WIN32_NO_LIBS "/nodefaultlib:\"libcmt.lib\"")
          else(WIN32)
            set(WIN32_NO_LIBS)
          endif(WIN32)
          set_target_properties(${SO} PROPERTIES LINKER_LANGUAGE CXX PREFIX "" SUFFIX "" LINK_FLAGS "${TEMP_LDFLAGS} ${WIN32_NO_LIBS}")
          add_dependencies(${SO} ${PROGRAM_NAME})
          # For Windows only, have the Module link to the export library of Navn as well as the wsock32 library (most of the Modules probably don't need this, but this is to be on the safe side), also set it's version
          if(WIN32)
            target_link_libraries(${SO} ${PROGRAM_NAME} wsock32 Ws2_32 ${WIN32_MEMORY} ${TEMP_DEPENDENCIES})
            set_target_properties(${PROGRAM_NAME} PROPERTIES VERSION "${VERSION_SIMPLE}")
          endif(WIN32)
          # Set the Module to be installed to the Module directory under the data directory
          install(TARGETS ${SO}
            DESTINATION Modules
          )
        endif(HAS_FUNCTION)
      else(NOT SKIP_DEPENDS AND NOT SKIP_LIBRARIES)
        message("  This is not a fatal error - ${SRC} will not be built.")
      endif(NOT SKIP_DEPENDS AND NOT SKIP_LIBRARIES)
    endforeach(SRC)

    # Get a list of ALL files and directories within this Modules directory
    file(GLOB SUBMODULE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${MODULE_FOLDER}/*")
    remove_item_from_list(SUBMODULE_DIRS "CMakeFiles")

    foreach(SUBDIR ${SUBMODULE_DIRS})
      if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}")
        file(GLOB MODULES_SUBDIR_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SUBDIR}/*.cpp")
        sort_list(MODULES_SUBDIR_SRCS)

        # Set all the files to use C++ as well as set their compile flags (use the Module-specific compile flags, though)
        set_source_files_properties(${MODULES_SUBDIR_SRCS} PROPERTIES LANGUAGE CXX COMPILE_FLAGS "${CXXFLAGS}")

        # Get the length of this subdir
        string(LENGTH ${SUBDIR} SUBDIR_LEN)
        # Calculate the length of the folder
        math(EXPR FILE_LEN "${SUBDIR_LEN} - ${FOLDER_LEN}")
        # Extract this subfolders name to use to generate the .so file
        string(SUBSTRING ${SUBDIR} ${FOLDER_LEN} ${FILE_LEN} SUBDIR_REALNAME)
        # Add .so to the end of the directory name, this will be the Module's name
        set(SO "${SUBDIR_REALNAME}${CMAKE_SHARED_LIBRARY_SUFFIX}")

        # Temporary linker flags for this subdirectory
        set(SUBDIR_LDFLAGS "${LDFLAGS}")
        # Temporary extra dependencies for this subdirectory
        set(SUBDIR_EXTRA_DEPENDS)
        # Reset skip_depends
        set(SKIP_DEPENDS)
        # Reset skip_libraries
        set(SKIP_LIBRARIES)
        # Reset has_function
        set(HAS_FUNCTION TRUE)

        # Iterate through the source files in the subdirectory
        foreach(SRC ${MODULES_SUBDIR_SRCS})
          if(NOT SKIP_DEPENDS AND NOT SKIP_LIBRARIES AND HAS_FUNCTION)
            # Temporary variable for the current source's include directories
            set(TEMP_INCLUDES)
            # Calculate the header file dependencies for the given source file
            calculate_depends(${SRC} SKIP_DEPENDS TEMP_INCLUDES)
            # If there were some extra include directories, add them to the list
            if(TEMP_INCLUDES)
              append_to_list(EXTRA_INCLUDES ${TEMP_INCLUDES})
            endif(TEMP_INCLUDES)
            # Reset linker flags
            set(TEMP_LDFLAGS)
            # Reset extra dependencies
            set(TEMP_DEPENDENCIES)
            # Calculate the library dependencies for the given source file
            calculate_libraries(${SRC} SKIP_LIBRARIES TEMP_LDFLAGS TEMP_DEPENDENCIES)
            # Check the function dependencies for the given source file
            check_functions(${SRC} HAS_FUNCTION)

            # Append this source file's linker flags to the subdirectoy's linker flags, if there are any to append
            if(TEMP_DEPENDENCIES)
              append_to_list(SUBDIR_EXTRA_DEPENDS ${TEMP_DEPDENCIES})
            endif(TEMP_DEPENDENCIES)
          endif(NOT SKIP_DEPENDS AND NOT SKIP_LIBRARIES AND HAS_FUNCTION)
        endforeach(SRC)

        # Continue if library and function requirements are met
        if(NOT SKIP_DEPENDS AND NOT SKIP_LIBRARIES AND HAS_FUNCTION)
          # Remove duplicates from the linker flags
          if(SUBDIR_LDFLAGS)
            remove_list_duplicates(SUBDIR_LDFLAGS)
          endif(SUBDIR_LDFLAGS)
          # Remove duplicates from the extra dependencies
          if(SUBDIR_EXTRA_DEPENDS)
            remove_list_duplicates(SUBDIR_EXTRA_DEPENDS)
          endif(SUBDIR_EXTRA_DEPENDS)


          # Generate the Module and set it's linker flags, also set it to depend on the main Navn executable to be built beforehand
          add_library(${SO} MODULE ${MODULES_SUBDIR_SRCS})
          set_target_properties(${SO} PROPERTIES LINKER_LANGUAGE CXX PREFIX "" SUFFIX "" LINK_FLAGS "${SUBDIR_LDFLAGS}")
          add_dependencies(${SO} ${PROGRAM_NAME})
          # For Windows only, have the Module link to the export library of Navn as well as the wsock32 library (most of the Modules probably don't need this, but this is to be on the safe side), also set it's version
          if(WIN32)
             target_link_libraries(${SO} ${PROGRAM_NAME} wsock32 ${WIN32_MEMORY} ${SUBDIR_EXTRA_DEPENDS})
             set_target_properties(${PROGRAM_NAME} PROPERTIES VERSION "${VERSION_SIMPLE}")
          endif(WIN32)
          # Set the Module to be installed to the Module directory under the data directory
          install(TARGETS ${SO}
            DESTINATION data/Modules
          )
        else(NOT SKIP_DEPENDS AND NOT SKIP_LIBRARIES AND HAS_FUNCTION)
          message("  This is not a fatal error - ${SUBDIR} will not be built.")
        endif(NOT SKIP_DEPENDS AND NOT SKIP_LIBRARIES AND HAS_FUNCTION)
      endif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}")
    endforeach(SUBDIR)
   endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_FOLDER}/CMakeLists.txt)
  endif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_FOLDER}")
endforeach(MODULE_FOLDER)

# If there were extra include directories, remove the duplicates and add the directories to the include path
if(EXTRA_INCLUDES)
  remove_list_duplicates(EXTRA_INCLUDES)
  include_directories(${EXTRA_INCLUDES})
endif(EXTRA_INCLUDES)