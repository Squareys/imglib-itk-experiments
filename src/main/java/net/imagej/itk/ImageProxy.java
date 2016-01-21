package net.imagej.itk;

import org.itk.simple.Image;

public class ImageProxy {

	static {
		System.loadLibrary("NativeImageProxy");
		init();
	}

	/* Some members only used from the C side */
	private long internalImagePointer;
	private long internalArrayPointer;
	private long[] pixelData;

	/**
	 * Constructor
	 * 
	 * @param pixelData
	 * @param dimensions
	 */
	public ImageProxy(long[] pixelData, long[] dimensions) {
		acquire(pixelData, dimensions);
		this.pixelData = pixelData;

		System.out.println("Image: " + internalImagePointer);
		System.out.println("Array: " + internalArrayPointer);
		assert internalImagePointer != 0;
		assert internalArrayPointer != 0;
	}

	@Override
	protected void finalize() throws Throwable {
		release();
		super.finalize();
	}

	/**
	 * One-time initialization of the native side of ImageProxy.
	 */
	private static native void init();

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

		Image newInstance = new SneakyImage(internalImagePointer, false);
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
