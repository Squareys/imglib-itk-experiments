package net.imagej.itk.test;

import static org.junit.Assert.assertEquals;

import java.nio.ByteBuffer;

import org.itk.simple.Image;
import org.itk.simple.SimpleITK;
import org.itk.simple.VectorInt64;
import org.itk.simple.VectorUInt32;
import org.junit.Test;

import net.imagej.itk.ImageByteBufferProxy;
import net.imagej.itk.ImageProxy;

public class TestImageProxy {

	@Test
	public void testImageProxy() {
		long[] pixelData = new long[100];
		long[] dims = new long[] { 10, 10 };

		for (int i = 0; i < 100; ++i) {
			pixelData[i] = i;
		}

		ImageProxy proxy = new ImageProxy(pixelData, dims);
		Image img = proxy.getImage();

		Image result = SimpleITK.sin(img);

		VectorUInt32 pos = new VectorUInt32(2);
		VectorInt64 pixel = result.getPixelAsVectorInt64(pos);

		for (int i = 0; i < 100; ++i) {
			assertEquals(pixel.get(i), (long) Math.sin(i));
		}

		proxy.release();
	}

	@Test
	public void testImageByteBufferProxy() {
		long[] dims = new long[] { 10, 10 };
		ByteBuffer buffer = ByteBuffer.allocateDirect(8 * 100);

		for (long i = 0; i < 100; ++i) {
			buffer.putLong(i);
		}

		ImageByteBufferProxy proxy = new ImageByteBufferProxy(buffer, dims);
		Image img = proxy.getImage();

		Image result = SimpleITK.sin(img);

		VectorUInt32 pos = new VectorUInt32(2);
		VectorInt64 pixel = result.getPixelAsVectorInt64(pos);

		for (int i = 0; i < 100; ++i) {
			assertEquals(pixel.get(i), (long) Math.sin(i));
		}

		proxy.release();
	}

}
