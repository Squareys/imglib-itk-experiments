#include "SimpleITK.h"

#include "../../target/jni/net_imagej_itk_ImageProxy.h"

jclass imageProxyClass = nullptr;
jfieldID imageProxy_internalImagePointer = nullptr;
jfieldID imageProxy_internalArrayPointer = nullptr;
jfieldID imageProxy_pixelData = nullptr;

namespace sitk = itk::simple;

JNIEXPORT void JNICALL Java_net_imagej_itk_ImageProxy_init(JNIEnv* env, jclass cls) {
  printf("Finding ImageProxy class:\n");
  imageProxyClass = cls;
  printf("-- internalImagePointer\n");
  imageProxy_internalImagePointer = env->GetFieldID(imageProxyClass, "internalImagePointer", "J");
  printf("-- internalArrayPointer\n");
  imageProxy_internalArrayPointer = env->GetFieldID(imageProxyClass, "internalArrayPointer", "J");
  printf("-- pixelData\n");
  imageProxy_pixelData = env->GetFieldID(imageProxyClass, "pixelData", "Ljava/lang/Object;");
  printf("-- DONE!\n");
}

template<typename TYPE, typename ARRAY_TYPE>
TYPE* getArrayElements(JNIEnv* env, ARRAY_TYPE array, jboolean* isCopy);

template<typename TYPE, typename ARRAY_TYPE>
void releaseArrayElements(JNIEnv* env, ARRAY_TYPE array, TYPE* pointer, jint flag);

template<typename TYPE>
void setBuffer(sitk::ImportImageFilter& importFilter, TYPE* data, jint length);

// jlong

template<>
jlong* getArrayElements<jlong, jlongArray>(JNIEnv* env, jlongArray array, jboolean* isCopy) {
  return static_cast<jlong*>(env->GetLongArrayElements(array, isCopy));
}

template<>
void releaseArrayElements(JNIEnv* env, jlongArray array, jlong* pointer, jint flag) {
  env->ReleaseLongArrayElements(array, pointer, flag);
}

template<>
void setBuffer(sitk::ImportImageFilter& importFilter, jlong* data, jint length) {
  importFilter.SetBufferAsInt64(data, length);
}

// jint

template<>
jint* getArrayElements<jint, jintArray>(JNIEnv* env, jintArray array, jboolean* isCopy) {
  return static_cast<jint*>(env->GetIntArrayElements(array, isCopy));
}

template<>
void releaseArrayElements(JNIEnv* env, jintArray array, jint* pointer, jint flag) {
  env->ReleaseIntArrayElements(array, pointer, flag);
}

template<>
void setBuffer(sitk::ImportImageFilter& importFilter, jint* data, jint length) {
  importFilter.SetBufferAsInt32(data, length);
}


template<typename TYPE, typename ARRAY_TYPE>
void acquire(JNIEnv* env, jobject o, ARRAY_TYPE pixelData, jlongArray dimensions) {
  jboolean isCopy = false;
  TYPE* pixelDataPointer = getArrayElements<TYPE, ARRAY_TYPE>(env, pixelData, &isCopy);
  jint pixelDataLength = env->GetArrayLength(pixelData);

  if (isCopy) {
    printf("WARNING: input array was copied!\n");
    isCopy = false;
  }

  jlong* dimensionsPointer = getArrayElements<jlong, jlongArray>(env, dimensions, &isCopy);
  jint dimensionsLength = env->GetArrayLength(dimensions);

  if (isCopy) {
    printf("WARNING: dimensions array was copied!\n");
  }

  sitk::ImportImageFilter importFilter;
  importFilter.SetSpacing(std::vector<double>{dimensionsLength, 1.0f});
  importFilter.SetOrigin(std::vector<double>{dimensionsLength, 0.0f});

  importFilter.SetSize(std::vector<unsigned int>{2, 100});
  setBuffer<TYPE>(importFilter, pixelDataPointer, pixelDataLength);

  sitk::Image* img = new sitk::Image(std::move(importFilter.Execute()));

  env->SetLongField(o, imageProxy_internalImagePointer, reinterpret_cast<jlong>(img));
  env->SetLongField(o, imageProxy_internalArrayPointer, reinterpret_cast<jlong>(pixelDataPointer));

  releaseArrayElements<jlong, jlongArray>(env, dimensions, dimensionsPointer, 0);
}

/*
 * Class:     net_imagej_itk_ImageProxy
 * Method:    acquire
 * Signature: ([J[J)V
 */
JNIEXPORT void JNICALL Java_net_imagej_itk_ImageProxy_acquire___3J_3J
  (JNIEnv * env, jobject o, jlongArray pixelData, jlongArray dimensions) {
  acquire<jlong, jlongArray>(env, o, pixelData, dimensions);
}

/*
 * Class:     net_imagej_itk_ImageProxy
 * Method:    acquire
 * Signature: ([I[J)V
 */
JNIEXPORT void JNICALL Java_net_imagej_itk_ImageProxy_acquire___3I_3J
  (JNIEnv * env, jobject o, jintArray pixelData, jlongArray dimensions) {
  acquire<jint, jintArray>(env, o, pixelData, dimensions);
}

/*
 * Class:     net_imagej_itk_ImageProxy
 * Method:    release
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_imagej_itk_ImageProxy_release(JNIEnv * env, jobject o) {
    // get values of fields
    jlong* arrayPointer = reinterpret_cast<jlong*>(env->GetLongField(o, imageProxy_internalArrayPointer));
    jlongArray pixelData = jlongArray(env->GetObjectField(o, imageProxy_pixelData));

    // release the array.
    env->ReleaseLongArrayElements(pixelData, arrayPointer, 0);
}
