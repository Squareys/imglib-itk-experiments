#include "SimpleITK.h"

#include "../../target/jni/net_imagej_itk_ImageProxy.h"

jclass imageProxyClass = nullptr;
jfieldID imageProxy_internalImagePointer = nullptr;
jfieldID imageProxy_internalArrayPointer = nullptr;
jfieldID imageProxy_pixelData = nullptr;

namespace sitk = itk::simple;

void findImageProxyClass(JNIEnv* env) {
  if (imageProxyClass == nullptr) {
      printf("Finding ImageProxy class:\n");
      imageProxyClass = env->FindClass("net/imagej/itk/ImageProxy");
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
    findImageProxyClass(env);
    
    jboolean isCopy = false;
    long long* pixelDataPointer = static_cast<long long*>(env->GetLongArrayElements(pixelData, &isCopy));
    jint pixelDataLength = env->GetArrayLength(pixelData);

    sitk::ImportImageFilter importFilter;
    importFilter.SetSpacing( std::vector<double>{1.0f, 1.0f} );
    printf("Yay! 7\n");
    importFilter.SetBufferAsInt64(pixelDataPointer, pixelDataLength);
}

/*
 * Class:     net_imagej_itk_ImageProxy
 * Method:    release
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_imagej_itk_ImageProxy_release(JNIEnv * env, jobject o) {
    findImageProxyClass(env);

    // get values of fields
    jlong* arrayPointer = reinterpret_cast<jlong*>(env->GetLongField(o, imageProxy_internalArrayPointer));
    jlongArray pixelData = jlongArray(env->GetObjectField(o, imageProxy_pixelData));

    // release the array.
    env->ReleaseLongArrayElements(pixelData, arrayPointer, (jint)0);
}
