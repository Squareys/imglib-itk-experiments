package net.imagej.itk.test;

import org.junit.Test;

import net.imagej.itk.ImageProxy;

public class TestImageProxy {
	
	@Test
	public void testImageProxy() {
		long[] pixelData = new long[100];
		long[] dims = new long[]{10, 10};
		
		ImageProxy proxy = new ImageProxy(pixelData, dims);
		
	}

}
