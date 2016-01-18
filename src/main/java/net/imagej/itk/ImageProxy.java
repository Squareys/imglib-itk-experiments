package net.imagej.itk;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

import org.itk.simple.Image;

public class ImageProxy {

	static {
		System.loadLibrary("lib/libNativeImageProxy");
	}

	/* Some members only used from the C side */
	private long internalImagePointer;
	private long internalArrayPointer;
	private long[] pixels;

	/**
	 * Constructor
	 * 
	 * @param pixelData
	 * @param dimensions
	 */
	public ImageProxy(long[] pixelData, long[] dimensions) {
		acquire(pixelData, dimensions);
		pixels = pixelData;
	}

	@Override
	protected void finalize() throws Throwable {
		release();
		super.finalize();
	}

	/**
	 * Acquire an Image from given data.
	 * 
	 * @param pixelData
	 * @param dimensions
	 */
	private native void acquire(long[] pixelData, long[] dimensions);

	/**
	 * @return the acquired Image.
	 */
	public Image getImage() {
		if (internalImagePointer == 0) {
			return null;
		}
		
		Image newInstance = null;
		try {
			Constructor<Image> constructor = Image.class.getConstructor(long.class, boolean.class);
			constructor.setAccessible(true);
			newInstance = constructor.newInstance(internalImagePointer, false);
		} catch (NoSuchMethodException e1) {
			e1.printStackTrace();
		} catch (SecurityException e1) {
			e1.printStackTrace();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}
		return newInstance;
	}

	/**
	 * Release the reference to the acquired array data.
	 */
	public native void release();
	
	public static long getCPtr(ImageProxy p) {
		return (p == null) ? 0 : p.internalImagePointer;
	}
	
}
