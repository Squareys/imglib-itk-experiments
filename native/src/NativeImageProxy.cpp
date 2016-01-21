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
  imageProxy_pixelData = env->GetFieldID(imageProxyClass, "pixelData", "[J");
  printf("-- DONE!\n");
}
}

/*
 * Class:     net_imagej_itk_ImageProxy
 * Method:    acquire
 * Signature: ([J[J)V
 */
JNIEXPORT void JNICALL Java_net_imagej_itk_ImageProxy_acquire
  (JNIEnv * env, jobject o, jlongArray pixelData, jlongArray dimensions) {
    
    jboolean isCopy = false;
    jlong* pixelDataPointer = static_cast<jlong*>(env->GetLongArrayElements(pixelData, &isCopy));
    jint pixelDataLength = env->GetArrayLength(pixelData);
    
    jlong* dimensionsPointer = static_cast<jlong*>(env->GetLongArrayElements(dimensions, &isCopy));
    jint dimensionsLength = env->GetArrayLength(dimensions);

    sitk::ImportImageFilter importFilter;
    importFilter.SetSpacing(std::vector<double>{dimensionsLength, 1.0f});
    importFilter.SetOrigin(std::vector<double>{dimensionsLength, 0.0f});

    importFilter.SetSize(std::vector<unsigned int>{2, 100});
    importFilter.SetBufferAsInt64(pixelDataPointer, pixelDataLength);
    
    sitk::Image* img = new sitk::Image(std::move(importFilter.Execute()));

    env->SetLongField(o, imageProxy_internalImagePointer, reinterpret_cast<jlong>(img));
    env->SetLongField(o, imageProxy_internalArrayPointer, reinterpret_cast<jlong>(pixelDataPointer));
    
    env->ReleaseLongArrayElements(dimensions, dimensionsPointer, 0);
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
