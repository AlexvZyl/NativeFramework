
if(NOT "C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-subbuild/boostorg_cmake-populate-prefix/src/boostorg_cmake-populate-stamp/boostorg_cmake-populate-gitinfo.txt" IS_NEWER_THAN "C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-subbuild/boostorg_cmake-populate-prefix/src/boostorg_cmake-populate-stamp/boostorg_cmake-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: 'C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-subbuild/boostorg_cmake-populate-prefix/src/boostorg_cmake-populate-stamp/boostorg_cmake-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe"  clone --no-checkout --depth 1 --no-single-branch --config "advice.detachedHead=false" "https://github.com/boostorg/cmake" "boostorg_cmake-src"
    WORKING_DIRECTORY "C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/boostorg/cmake'")
endif()

execute_process(
  COMMAND "C:/Program Files/Git/cmd/git.exe"  checkout develop --
  WORKING_DIRECTORY "C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'develop'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe"  submodule update --recursive --init 
    WORKING_DIRECTORY "C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-subbuild/boostorg_cmake-populate-prefix/src/boostorg_cmake-populate-stamp/boostorg_cmake-populate-gitinfo.txt"
    "C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-subbuild/boostorg_cmake-populate-prefix/src/boostorg_cmake-populate-stamp/boostorg_cmake-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'C:/Users/Admin/Documents/Ulrich/Automated-Electrical-Development/Lumen/ElecDev_Graphics_Application/Source/External/boost_1_77_0/libs/histogram/out/build/x64-Debug/_deps/boostorg_cmake-subbuild/boostorg_cmake-populate-prefix/src/boostorg_cmake-populate-stamp/boostorg_cmake-populate-gitclone-lastrun.txt'")
endif()

