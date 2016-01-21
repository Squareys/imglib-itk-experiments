package net.imagej.itk;

import org.itk.simple.Image;
import org.itk.simple.VectorUInt32;

import net.imglib2.Interval;
import net.imglib2.Localizable;
import net.imglib2.Positionable;
import net.imglib2.RandomAccess;
import net.imglib2.RandomAccessibleInterval;
import net.imglib2.RealPositionable;
import net.imglib2.Sampler;
import net.imglib2.type.numeric.RealType;
import net.imglib2.type.numeric.integer.LongType;

public class RandomAccessibleImage<T extends RealType<T>> implements RandomAccessibleInterval<T> {

	private Image img;

	public RandomAccessibleImage(Image toWrap) {
		img = toWrap;
	}

	protected class ImageRandomAccess implements RandomAccess<LongType> {

		protected VectorUInt32 pos;

		public ImageRandomAccess(int x, int y) {
			pos = new VectorUInt32(2);
			pos.set(0, x);
			pos.set(1, y);
		}

		public ImageRandomAccess(int x, int y, int z) {
			pos = new VectorUInt32(3);
			pos.set(0, x);
			pos.set(1, y);
			pos.set(2, z);
		}

		public ImageRandomAccess(VectorUInt32 pos2) {
			pos = new VectorUInt32(pos2.capacity());
			for (int d = 0; d < pos2.capacity(); ++d) {
				pos.set(d, pos2.get(d));
			}
		}

		@Override
		public void localize(int[] position) {
			for (int d = 0; d < position.length; ++d) {
				position[d] = (int) pos.get(d);
			}
		}

		@Override
		public void localize(long[] position) {
			for (int d = 0; d < position.length; ++d) {
				position[d] = pos.get(d);
			}
		}

		@Override
		public int getIntPosition(int d) {
			return (int) pos.get(d);
		}

		@Override
		public long getLongPosition(int d) {
			return pos.get(d);
		}

		@Override
		public void localize(float[] position) {
			for (int d = 0; d < position.length; ++d) {
				position[d] = (int) pos.get(d);
			}
		}

		@Override
		public void localize(double[] position) {
			for (int d = 0; d < position.length; ++d) {
				position[d] = (int) pos.get(d);
			}
		}

		@Override
		public float getFloatPosition(int d) {
			return pos.get(d);
		}

		@Override
		public double getDoublePosition(int d) {
			return pos.get(d);
		}

		@Override
		public int numDimensions() {
			return (int) pos.capacity();
		}

		@Override
		public void fwd(int d) {
			pos.set(d, pos.get(d) + 1);
		}

		@Override
		public void bck(int d) {
			pos.set(d, pos.get(d) - 1);
		}

		@Override
		public void move(int distance, int d) {
			pos.set(d, pos.get(d) + distance);
		}

		@Override
		public void move(long distance, int d) {
			pos.set(d, pos.get(d) + distance);
		}

		@Override
		public void move(Localizable localizable) {
			for (int d = 0; d < localizable.numDimensions(); ++d) {
				pos.set(d, pos.get(d) + localizable.getLongPosition(d));
			}
		}

		@Override
		public void move(int[] distance) {
			for (int d = 0; d < distance.length; ++d) {
				pos.set(d, pos.get(d) + distance[d]);
			}
		}

		@Override
		public void move(long[] distance) {
			for (int d = 0; d < distance.length; ++d) {
				pos.set(d, pos.get(d) + distance[d]);
			}
		}

		@Override
		public void setPosition(Localizable localizable) {
			for (int d = 0; d < localizable.numDimensions(); ++d) {
				pos.set(d, localizable.getLongPosition(d));
			}
		}

		@Override
		public void setPosition(int[] position) {
			for (int d = 0; d < position.length; ++d) {
				pos.set(d, position[d]);
			}
		}

		@Override
		public void setPosition(long[] position) {
			for (int d = 0; d < position.length; ++d) {
				pos.set(d, position[d]);
			}
		}

		@Override
		public void setPosition(int position, int d) {
			pos.set(d, position);
		}

		@Override
		public void setPosition(long position, int d) {
			pos.set(d, position);
		}

		@Override
		public LongType get() {
			return new LongType(img.getPixelAsInt64(pos));
		}

		@Override
		public Sampler<LongType> copy() {
			return null; // what is this?
		}

		@Override
		public RandomAccess<LongType> copyRandomAccess() {
			return new ImageRandomAccess(pos);
		}

	}

	@Override
	public RandomAccess<T> randomAccess() {
		if (numDimensions() == 2) {
			return (RandomAccess<T>) new ImageRandomAccess(0, 0);
		} else if (numDimensions() == 3) {
			return (RandomAccess<T>) new ImageRandomAccess(0, 0, 0);
		}

		throw new RuntimeException("Invalid numDimensions in RandomAccessibleImage");
	}

	@Override
	public RandomAccess<T> randomAccess(Interval arg0) {
		return null;
	}

	@Override
	public int numDimensions() {
		return (int) img.getDimension();
	}

	@Override
	public long max(int d) {
		if (d == 0) {
			return img.getWidth();
		} else if (d == 1) {
			return img.getHeight();
		} else if (d == 2) {
			return img.getDepth();
		}
		throw new IndexOutOfBoundsException("ITK images have dimensions 1-3.");
	}

	@Override
	public void max(long[] max) {
		for (int d = 0; d < max.length; ++d) {
			max[d] = max(d);
		}
	}

	@Override
	public void max(Positionable max) {
		for (int d = 0; d < max.numDimensions(); ++d) {
			max.setPosition(max(d), d);
		}
	}

	@Override
	public long min(int arg0) {
		return 0;
	}

	@Override
	public void min(long[] min) {
		for (int d = 0; d < min.length; ++d) {
			min[d] = min(d);
		}
	}

	@Override
	public void min(Positionable min) {
		for (int d = 0; d < min.numDimensions(); ++d) {
			min.setPosition(min(d), d);
		}
	}

	@Override
	public double realMax(int arg0) {
		return 0;
	}

	@Override
	public void realMax(double[] arg0) {

	}

	@Override
	public void realMax(RealPositionable arg0) {
	}

	@Override
	public double realMin(int arg0) {
		return 0;
	}

	@Override
	public void realMin(double[] arg0) {
	}

	@Override
	public void realMin(RealPositionable arg0) {
	}

	@Override
	public long dimension(int d) {
		if (d == 0) {
			return img.getWidth();
		} else if (d == 1) {
			return img.getHeight();
		} else if (d == 2) {
			return img.getDepth();
		}
		throw new IndexOutOfBoundsException("ITK images have dimensions 1-3.");
	}

	@Override
	public void dimensions(long[] dims) {
		for (int d = 0; d < dims.length; ++d) {
			dims[0] = dimension(d);
		}
	}

}
