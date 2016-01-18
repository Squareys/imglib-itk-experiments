#include "SimpleITK.h"

#include "../../target/jni/net_imagej_itk_ImageProxy.h"

jclass imageProxyClass = nullptr;

namespace sitk = itk::simple;

/*
 * Class:     net_imagej_itk_ImageProxy
 * Method:    acquire
 * Signature: ([J[J)V
 */
JNIEXPORT void JNICALL Java_net_imagej_itk_ImageProxy_acquire(JNIEnv * env, jobject o, jlongArray pixelData, jlongArray dimensions) {
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
    if (imageProxyClass == nullptr) {
        imageProxyClass = env->FindClass("net/imagej/itk/ImageProxy");
    }

    // get fieldIDs
    jfieldID arrayPointerField = env->GetFieldID(imageProxyClass, "internalArrayPointer", "j");
    jfieldID pixelDataField = env->GetFieldID(imageProxyClass, "pixelData", "[j");

    // get values of fields
    jlong* arrayPointer = reinterpret_cast<jlong*>(env->GetLongField(o, arrayPointerField));
    jlongArray pixelData = jlongArray(env->GetObjectField(o, pixelDataField));

    // release the array.
    env->ReleaseLongArrayElements(pixelData, arrayPointer, (jint)0);
}
