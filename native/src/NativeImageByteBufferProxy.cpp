#include "SimpleITK.h"

#include "../../target/jni/net_imagej_itk_ImageByteBufferProxy.h"

jclass imageByteBufferProxyClass = nullptr;
jfieldID imageByteBufferProxy_internalImagePointer = nullptr;
jfieldID ImageByteBufferProxy_internalArrayPointer = nullptr;

namespace sitk = itk::simple;

JNIEXPORT void JNICALL Java_net_imagej_itk_ImageByteBufferProxy_init(JNIEnv * env, jclass cls) {
  printf("Finding ImageByteBufferProxy class:\n");
  imageByteBufferProxyClass = cls;
  printf("-- internalImagePointer\n");
  imageByteBufferProxy_internalImagePointer = env->GetFieldID(imageByteBufferProxyClass, "internalImagePointer", "J");
  printf("-- internalArrayPointer\n");
  ImageByteBufferProxy_internalArrayPointer = env->GetFieldID(imageByteBufferProxyClass, "internalArrayPointer", "J");
  printf("-- DONE!\n");
}

void setBuffer(sitk::ImportImageFilter& importFilter, jlong* data, jint length) {
  importFilter.SetBufferAsInt64(data, length);
}

void setBuffer(sitk::ImportImageFilter& importFilter, jint* data, jint length) {
  importFilter.SetBufferAsInt32(data, length);
}

template<typename TYPE>
void acquire(JNIEnv * env, jobject o, jobject pixelData, jlongArray dimensions) {
  TYPE* pixelDataPointer = reinterpret_cast<TYPE*>(env->GetDirectBufferAddress(pixelData));
  jint pixelDataLength = env->GetDirectBufferCapacity(pixelData) / sizeof(TYPE);

  jlong* dimensionsPointer = env->GetLongArrayElements(dimensions, nullptr);
  jint dimensionsLength = env->GetArrayLength(dimensions);

  sitk::ImportImageFilter importFilter;
  importFilter.SetSpacing(std::vector<double>{dimensionsLength, 1.0f});
  importFilter.SetOrigin(std::vector<double>{dimensionsLength, 0.0f});

  importFilter.SetSize(std::vector<unsigned int>{2, 100});
  setBuffer(importFilter, pixelDataPointer, pixelDataLength);

  sitk::Image* img = new sitk::Image(std::move(importFilter.Execute()));

  env->SetLongField(o, imageByteBufferProxy_internalImagePointer, reinterpret_cast<jlong>(img));
  env->SetLongField(o, ImageByteBufferProxy_internalArrayPointer, reinterpret_cast<jlong>(pixelDataPointer));

  env->ReleaseLongArrayElements(dimensions, dimensionsPointer, 0);
}

/*
 * Class:     net_imagej_itk_ImageByteBufferProxy
 * Method:    acquire
 * Signature: (Ljava/nio/ByteBuffer;[J)V
 */
JNIEXPORT void JNICALL Java_net_imagej_itk_ImageByteBufferProxy_acquire(JNIEnv * env, jobject o, jobject pixelData, jlongArray dimensions) {
  acquire<jlong>(env, o, pixelData, dimensions);
}


/*
 * Class:     net_imagej_itk_ImageByteBufferProxy
 * Method:    release
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_imagej_itk_ImageByteBufferProxy_release
  (JNIEnv *, jobject) {
}