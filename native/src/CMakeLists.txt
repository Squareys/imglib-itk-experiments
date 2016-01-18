
set(NATRIVEIMAGEPROXY_SRC
	NativeImageProxy.cpp
)

set(NATIVEIMAGEPROXY_HEADERS
    ../../target/jni/net_imagej_itk_ImageProxy.h
)

add_library(NativeImageProxy SHARED ${NATRIVEIMAGEPROXY_SRC} ${NATIVEIMAGEPROXY_HEADERS})

SET_TARGET_PROPERTIES(NativeImageProxy PROPERTIES
  COMPILE_FLAGS "-Wall -Wl,--kill-at -D_JNI_IMPLEMENTATION_ ${SHARED_FLAGS}"
)

target_compile_features(NativeImageProxy PUBLIC cxx_nullptr)
target_compile_features(NativeImageProxy PUBLIC cxx_constexpr)
target_link_libraries(NativeImageProxy ${JNI_LIBRARY} ${SimpleITK_LIBRARIES})
target_include_directories(NativeImageProxy PUBLIC ${JNI_INCLUDE_DIRS})
