package net.imagej.itk;

import org.itk.simple.Image;

public class SneakyImage extends Image {
	
	public SneakyImage(long pointer, boolean ownsMemory) {
		super(pointer, ownsMemory);
	}
}
