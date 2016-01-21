# imagej-itk
Proof of concept for how to integration SimpleITK with Imglib2 while trying to avoid copying data whereever possible.

## Introduction
This repository is of no use to the majority of the imagej user base but rather tries to be a basis for future work of imagej/imglib2 developers.

The code tries to integration SimpleITK with Imglib2 in two ways: Using access to java array and using a ByteBuffer.
The two approaches will be elaborated on in the next sections.

## Copy the Data Approach
The straight-forward approach of creating a SimpleITK `Image` from any kind of imglib2 image structure would be to create the `org.itk.simple.Image`
and copy the data pixel by pixel.

## Java Array Access Approach
The idea behind this approach is to access an `ArrayImg` data storage array from JNI and create a SimpleITK Image from it.
Theoretically, this will only copy the data when needed (see JNI documentation), but practically, the array data is allways copied, 
which therefore does not improve on the straight-forward approach very much.

## DirectByteBuffer Approach
With `java.nio.DirectByteBuffer`, you can get direct access to data in the JVM in JNI. This approach does work, but requires `ArrayImg` to be created with
a backing `DirectByteBuffer`, so that data does not need to be copied into a new buffer and imglib2 does allow this. A drawback is that `DirectByteBuffer`
supposedly (performs a bit worse than `byte[]`)[http://www.evanjones.ca/software/java-bytebuffers.html], which is relevant for other uses of `ArrayImg` other
than sharing data with SimpleITK.
