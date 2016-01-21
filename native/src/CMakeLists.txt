
set(NATRIVEIMAGEPROXY_SRC
    NativeImageProxy.cpp
    NativeImageByteBufferProxy.cpp
)

set(NATIVEIMAGEPROXY_HEADERS
    ../../target/jni/net_imagej_itk_ImageProxy.h
    ../../target/jni/net_imagej_itk_ImageByteBufferProxy.h
)

add_library(NativeImageProxy SHARED ${NATRIVEIMAGEPROXY_SRC} ${NATIVEIMAGEPROXY_HEADERS})

if(WIN32)
  set_target_properties(NativeImageProxy PROPERTIES
    COMPILE_FLAGS "-Wall -Wl,--kill-at -D_JNI_IMPLEMENTATION_ ${SHARED_FLAGS}"
  )
else()
  set_target_properties(NativeImageProxy PROPERTIES
    COMPILE_FLAGS "-D_JNI_IMPLEMENTATION_"
  )
endif()

target_compile_features(NativeImageProxy PUBLIC cxx_nullptr)
target_compile_features(NativeImageProxy PUBLIC cxx_constexpr)
target_link_libraries(NativeImageProxy ${JNI_LIBRARY} ${SimpleITK_LIBRARIES})
target_include_directories(NativeImageProxy PUBLIC ${JNI_INCLUDE_DIRS})
