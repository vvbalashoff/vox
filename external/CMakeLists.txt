# libnoise options
option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" TRUE)
option(LIBNOISE_BUILD_SHARED_LIBS "" ${BUILD_SHARED_LIBS})

add_subdirectory(glm)
add_subdirectory(glfw)
add_subdirectory(libnoise)
add_subdirectory(simplex)
#add_subdirectory(lua)
