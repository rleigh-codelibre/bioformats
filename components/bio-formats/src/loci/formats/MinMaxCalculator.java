//
// MinMaxCalculator.java
//

/*
OME Bio-Formats package for reading and converting biological file formats.
Copyright (C) 2005-@year@ UW-Madison LOCI and Glencoe Software, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

package loci.formats;

import java.io.IOException;
import java.util.Arrays;

import loci.common.DataTools;
import loci.formats.meta.IMinMaxStore;

/**
 * Logic to compute minimum and maximum values for each channel.
 *
 * <dl><dt><b>Source code:</b></dt>
 * <dd><a href="https://skyking.microscopy.wisc.edu/trac/java/browser/trunk/components/bio-formats/src/loci/formats/MinMaxCalculator.java">Trac</a>,
 * <a href="https://skyking.microscopy.wisc.edu/svn/java/trunk/components/bio-formats/src/loci/formats/MinMaxCalculator.java">SVN</a></dd></dl>
 */
public class MinMaxCalculator extends ReaderWrapper {

  // -- Utility methods --

  /** Converts the given reader into a MinMaxCalculator, wrapping if needed. */
  public static MinMaxCalculator makeMinMaxCalculator(IFormatReader r) {
    if (r instanceof MinMaxCalculator) return (MinMaxCalculator) r;
    return new MinMaxCalculator(r);
  }

  // -- Fields --

  /** Min values for each channel. */
  protected double[][] chanMin;

  /** Max values for each channel. */
  protected double[][] chanMax;

  /** Min values for each plane. */
  protected double[][] planeMin;

  /** Max values for each plane. */
  protected double[][] planeMax;

  /** Number of planes for which min/max computations have been completed. */
  protected int[] minMaxDone;

  /** Consumer of channel global minima and maxima */
  protected IMinMaxStore minMaxStore;

  // -- Constructors --

  /** Constructs a MinMaxCalculator around a new image reader. */
  public MinMaxCalculator() { super(); }

  /** Constructs a MinMaxCalculator with the given reader. */
  public MinMaxCalculator(IFormatReader r) { super(r); }

  /**
   * Sets the active min-max store for the calculator. Whenever a channel's
   * global minimum and maximum calculation has been completed this store is
   * notified.
   * @param store See above.
   */
  public void setMinMaxStore(IMinMaxStore store) {
    minMaxStore = store;
  }

  /**
   * Retrieves the current active min-max store for the calculator.
   * @return See above.
   */
  public IMinMaxStore getMinMaxStore() {
    return minMaxStore;
  }

  // -- MinMaxCalculator API methods --

  /**
   * Retrieves a specified channel's global minimum.
   * Returns null if some of the image planes have not been read.
   */
  public Double getChannelGlobalMinimum(int theC)
    throws FormatException, IOException
  {
    FormatTools.assertId(getCurrentFile(), true, 2);
    if (theC < 0 || theC >= getSizeC()) {
      throw new FormatException("Invalid channel index: " + theC);
    }

    int series = getSeries();

    // check that all planes have been read
    if (minMaxDone == null || minMaxDone[series] < getImageCount()) {
      return null;
    }
    return new Double(chanMin[series][theC]);
  }

  /**
   * Retrieves a specified channel's global maximum.
   * Returns null if some of the image planes have not been read.
   */
  public Double getChannelGlobalMaximum(int theC)
    throws FormatException, IOException
  {
    FormatTools.assertId(getCurrentFile(), true, 2);
    if (theC < 0 || theC >= getSizeC()) {
      throw new FormatException("Invalid channel index: " + theC);
    }

    int series = getSeries();

    // check that all planes have been read
    if (minMaxDone == null || minMaxDone[series] < getImageCount()) {
      return null;
    }
    return new Double(chanMax[series][theC]);
  }

  /**
   * Retrieves the specified channel's minimum based on the images that have
   * been read.  Returns null if no image planes have been read yet.
   */
  public Double getChannelKnownMinimum(int theC)
    throws FormatException, IOException
  {
    FormatTools.assertId(getCurrentFile(), true, 2);
    return chanMin == null ? null : new Double(chanMin[getSeries()][theC]);
  }

  /**
   * Retrieves the specified channel's maximum based on the images that
   * have been read.  Returns null if no image planes have been read yet.
   */
  public Double getChannelKnownMaximum(int theC)
    throws FormatException, IOException
  {
    FormatTools.assertId(getCurrentFile(), true, 2);
    return chanMax == null ? null : new Double(chanMax[getSeries()][theC]);
  }

  /**
   * Retrieves the minimum pixel value for the specified plane.
   * If each image plane contains more than one channel (i.e.,
   * {@link #getRGBChannelCount()} &gt; 1), returns the maximum value for each
   * embedded channel. Returns null if the plane has not already been read.
   */
  public Double[] getPlaneMinimum(int no) throws FormatException, IOException {
    FormatTools.assertId(getCurrentFile(), true, 2);
    if (planeMin == null) return null;

    int numRGB = getRGBChannelCount();
    int pBase = no * numRGB;
    int series = getSeries();
    if (planeMin[series][pBase] != planeMin[series][pBase]) {
      return null;
    }

    Double[] min = new Double[numRGB];
    for (int c=0; c<numRGB; c++) {
      min[c] = new Double(planeMin[series][pBase + c]);
    }
    return min;
  }

  /**
   * Retrieves the maximum pixel value for the specified plane.
   * If each image plane contains more than one channel (i.e.,
   * {@link #getRGBChannelCount()} &gt; 1), returns the maximum value for each
   * embedded channel. Returns null if the plane has not already been read.
   */
  public Double[] getPlaneMaximum(int no) throws FormatException, IOException {
    FormatTools.assertId(getCurrentFile(), true, 2);
    if (planeMax == null) return null;

    int numRGB = getRGBChannelCount();
    int pBase = no * numRGB;
    int series = getSeries();
    if (planeMax[series][pBase] != planeMax[series][pBase]) {
      return null;
    }

    Double[] max = new Double[numRGB];
    for (int c=0; c<numRGB; c++) {
      max[c] = new Double(planeMax[series][pBase + c]);
    }
    return max;
  }

  /**
   * Returns true if the values returned by
   * getChannelGlobalMinimum/Maximum can be trusted.
   */
  public boolean isMinMaxPopulated() throws FormatException, IOException {
    FormatTools.assertId(getCurrentFile(), true, 2);
    return minMaxDone != null && minMaxDone[getSeries()] == getImageCount();
  }

  // -- IFormatReader API methods --

  /* @see IFormatReader#openBytes(int) */
  public byte[] openBytes(int no) throws FormatException, IOException {
    return openBytes(no, 0, 0, getSizeX(), getSizeY());
  }

  /* @see IFormatReader#openBytes(int, byte[]) */
  public byte[] openBytes(int no, byte[] buf)
    throws FormatException, IOException
  {
    return openBytes(no, buf, 0, 0, getSizeX(), getSizeY());
  }

  /* @see IFormatReader#openBytes(int, int, int, int, int) */
  public byte[] openBytes(int no, int x, int y, int w, int h)
    throws FormatException, IOException
  {
    byte[] buf = new byte[w * h * getRGBChannelCount() *
      FormatTools.getBytesPerPixel(getPixelType())];
    return openBytes(no, buf, x, y, w, h);
  }

  /* @see IFormatReader#openBytes(int, byte[], int, int, int, int) */
  public byte[] openBytes(int no, byte[] buf, int x, int y, int w, int h)
    throws FormatException, IOException
  {
    FormatTools.assertId(getCurrentFile(), true, 2);
    super.openBytes(no, buf, x, y, w, h);
    updateMinMax(buf, no);
    return buf;
  }

  /* @see IFormatReader#close(boolean) */
  public void close(boolean fileOnly) throws IOException {
    reader.close(fileOnly);
    if (!fileOnly) {
      chanMin = null;
      chanMax = null;
      planeMin = null;
      planeMax = null;
      minMaxDone = null;
    }
  }

  // -- Helper methods --

  /** Updates min/max values based on the given byte array. */
  protected void updateMinMax(byte[] b, int ndx)
    throws FormatException, IOException
  {
    if (b == null) return;
    initMinMax();

    int numRGB = getRGBChannelCount();
    int series = getSeries();
    // check whether min/max values have already been computed for this plane
    if (planeMin[series][ndx * numRGB] == planeMin[series][ndx * numRGB]) {
      return;
    }

    boolean little = isLittleEndian();
    int bytes = FormatTools.getBytesPerPixel(getPixelType());
    int pixels = getSizeX() * getSizeY();
    boolean interleaved = isInterleaved();

    int[] coords = getZCTCoords(ndx);
    int cBase = coords[1] * numRGB;
    int pBase = ndx * numRGB;
    for (int c=0; c<numRGB; c++) {
      planeMin[series][pBase + c] = Double.POSITIVE_INFINITY;
      planeMax[series][pBase + c] = Double.NEGATIVE_INFINITY;
    }

    int pixelType = getPixelType();

    boolean fp = pixelType == FormatTools.FLOAT ||
      pixelType == FormatTools.DOUBLE;
    boolean signed = pixelType == FormatTools.INT8 ||
      pixelType == FormatTools.INT16 || pixelType == FormatTools.INT32;

    long threshold = (long) Math.pow(2, bytes * 8 - 1);
    for (int i=0; i<pixels; i++) {
      for (int c=0; c<numRGB; c++) {
        int idx = bytes * (interleaved ? i * numRGB + c : c * pixels + i);
        long bits = DataTools.bytesToLong(b, idx, bytes, little);
        if (signed) {
          if (bits >= threshold) bits -= 2*threshold;
        }
        double v = (double) bits;
        if (pixelType == FormatTools.FLOAT) {
          v = Float.intBitsToFloat((int) bits);
        }
        else if (pixelType == FormatTools.DOUBLE) {
          v = Double.longBitsToDouble(bits);
        }

        if (v > chanMax[series][cBase + c]) {
          chanMax[series][cBase + c] = v;
        }
        if (v < chanMin[series][cBase + c]) {
          chanMin[series][cBase + c] = v;
        }
      }
    }

    for (int c=0; c<numRGB; c++) {
      if (chanMin[series][cBase + c] < planeMin[series][pBase + c]) {
        planeMin[series][pBase + c] = chanMin[series][cBase + c];
      }
      if (chanMax[series][cBase + c] > planeMax[series][pBase + c]) {
        planeMax[series][pBase + c] = chanMax[series][cBase + c];
      }
    }
    minMaxDone[series]++;

    if (minMaxDone[getSeries()] == getImageCount() && minMaxStore != null) {
      for (int c=0; c<getSizeC(); c++) {
        minMaxStore.setChannelGlobalMinMax(c, chanMin[getSeries()][c],
          chanMax[getSeries()][c], getSeries());
      }
    }
  }

  /** Ensures internal min/max variables are initialized properly. */
  protected void initMinMax() throws FormatException, IOException {
    int seriesCount = getSeriesCount();
    int oldSeries = getSeries();

    if (chanMin == null) {
      chanMin = new double[seriesCount][];
      for (int i=0; i<seriesCount; i++) {
        setSeries(i);
        chanMin[i] = new double[getSizeC()];
        Arrays.fill(chanMin[i], Double.POSITIVE_INFINITY);
      }
      setSeries(oldSeries);
    }
    if (chanMax == null) {
      chanMax = new double[seriesCount][];
      for (int i=0; i<seriesCount; i++) {
        setSeries(i);
        chanMax[i] = new double[getSizeC()];
        Arrays.fill(chanMax[i], Double.NEGATIVE_INFINITY);
      }
      setSeries(oldSeries);
    }
    if (planeMin == null) {
      planeMin = new double[seriesCount][];
      for (int i=0; i<seriesCount; i++) {
        setSeries(i);
        int numRGB = getRGBChannelCount();
        planeMin[i] = new double[getImageCount() * numRGB];
        Arrays.fill(planeMin[i], Double.NaN);
      }
      setSeries(oldSeries);
    }
    if (planeMax == null) {
      planeMax = new double[seriesCount][];
      for (int i=0; i<seriesCount; i++) {
        setSeries(i);
        int numRGB = getRGBChannelCount();
        planeMax[i] = new double[getImageCount() * numRGB];
        Arrays.fill(planeMax[i], Double.NaN);
      }
      setSeries(oldSeries);
    }
    if (minMaxDone == null) minMaxDone = new int[seriesCount];
  }

}