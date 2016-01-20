package net.imagej.itk.test;

import org.itk.simple.Image;
import org.itk.simple.SimpleITK;
import org.junit.Test;

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

		proxy.release();
	}

}
