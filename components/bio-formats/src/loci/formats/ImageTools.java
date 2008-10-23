//
// ImageTools.java
//

/*
OME Bio-Formats package for reading and converting biological file formats.
Copyright (C) 2005-@year@ UW-Madison LOCI and Glencoe Software, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
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

import loci.common.DataTools;

/**
 * A utility class with convenience methods for manipulating images
 * in primitive array form.
 *
 * To work with images in {@link java.awt.image.BufferedImage} form,
 * use the {@link AWTImageTools} class.
 *
 * <dl><dt><b>Source code:</b></dt>
 * <dd><a href="https://skyking.microscopy.wisc.edu/trac/java/browser/trunk/components/bio-formats/src/loci/formats/ImageTools.java">Trac</a>,
 * <a href="https://skyking.microscopy.wisc.edu/svn/java/trunk/components/bio-formats/src/loci/formats/ImageTools.java">SVN</a></dd></dl>
 *
 * @author Curtis Rueden ctrueden at wisc.edu
 */
public final class ImageTools {

  // -- Constructor --

  private ImageTools() { }

  // -- Image conversion --

  /**
   * Convert an arbitrary primitive type array with 3 samples per pixel to
   * a 3 x (width * height) byte array.
   */
  public static byte[][] make24Bits(Object pixels, int w, int h,
    boolean interleaved, boolean reverse)
  {
    int[] pix = make24Bits(pixels, w, h, interleaved);
    byte[][] rtn = new byte[3][pix.length];
    for (int i=0; i<pix.length; i++) {
      byte r = (byte) ((pix[i] >> 16) & 0xff);
      rtn[1][i] = (byte) ((pix[i] >> 8) & 0xff);
      byte b = (byte) (pix[i] & 0xff);
      rtn[0][i] = reverse ? b : r;
      rtn[2][i] = reverse ? r : b;
    }
    return rtn;
  }

  /**
   * Convert an arbitrary primitive type array with 3 samples per pixel to
   * an int array, i.e. RGB color with 8 bits per pixel.
   * Does not perform any scaling.
   */
  public static int[] make24Bits(Object pixels, int w, int h,
    boolean interleaved)
  {
    int[] rtn = new int[w * h];

    byte[] b = null;

    // adapted from ImageJ's TypeConverter methods

    if (pixels instanceof byte[]) b = (byte[]) pixels;
    else if (pixels instanceof short[]) {
      short[] s = (short[]) pixels;
      b = new byte[s.length];
      for (int i=0; i<s.length; i++) {
        int v = s[i] & 0xffff;
        b[i] = (byte) v;
      }
    }
    else if (pixels instanceof int[]) {
      int[] s = (int[]) pixels;
      b = new byte[s.length];
      for (int i=0; i<s.length; i++) {
        int value = s[i] & 0xffffffff;
        b[i] = (byte) value;
      }
    }
    else if (pixels instanceof float[]) {
      float[] s = (float[]) pixels;
      b = new byte[s.length];
      for (int i=0; i<s.length; i++) {
        float value = s[i];
        b[i] = (byte) (255 * value);
      }
    }
    else if (pixels instanceof double[]) {
      double[] s = (double[]) pixels;
      b = new byte[s.length];
      for (int i=0; i<s.length; i++) {
        double value = s[i];
        b[i] = (byte) Math.round(value);
      }
    }

    int c = b.length / rtn.length;

    for (int i=0; i<rtn.length; i++) {
      byte[] a = new byte[4];
      for (int j=c-1; j>=0; j--) {
        a[j] = b[interleaved ? i*c + j : i + j*w*h];
      }
      if (c == 1) {
        for (int j=1; j<a.length; j++) {
          a[j] = a[0];
        }
      }

      byte tmp = a[0];
      a[0] = a[2];
      a[2] = tmp;
      rtn[i] = DataTools.bytesToInt(a, true);
    }

    return rtn;
  }

  // -- Image manipulation --

  /**
   * Splits the given multi-channel array into a 2D array.
   * The "reverse" parameter is false if channels are in RGB order, true if
   * channels are in BGR order.
   */
  public static byte[] splitChannels(byte[] array, int index, int c, int bytes,
    boolean reverse, boolean interleaved)
  {
    byte[] rtn = new byte[array.length / c];

    if (reverse) index = c - index - 1;

    if (!interleaved) {
      System.arraycopy(array, rtn.length * index, rtn, 0, rtn.length);
    }
    else {
      int next = 0;
      for (int i=0; i<array.length; i+=c*bytes) {
        for (int k=0; k<bytes; k++) {
          if (next < rtn.length) rtn[next] = array[i + index*bytes + k];
          next++;
        }
      }
    }
    return rtn;
  }

  /**
   * Pads (or crops) the byte array to the given width and height.
   * The image will be centered within the new bounds.
   */
  public static byte[] padImage(byte[] b, boolean interleaved, int c,
    int oldWidth, int width, int height)
  {
    int oldHeight = b.length / (oldWidth * c);
    byte[] padded = new byte[height * width * c];

    int wClip = (width - oldWidth) / 2;
    int hClip = (height - oldHeight) / 2;

    int h = height < oldHeight ? height : oldHeight;

    if (interleaved) {
      int len = oldWidth < width ? oldWidth : width;
      if (h == oldHeight) {
        for (int y=0; y<h*c; y++) {
          int oldIndex = oldWidth * y;
          int index = width * y;
          System.arraycopy(b, oldIndex, padded, index, len);
        }
      }
      else {
        for (int ch=0; ch<c; ch++) {
          for (int y=0; y<h; y++) {
            int oldIndex = oldWidth * ch * oldHeight + oldWidth * y;
            int index = width * ch * height + width * y;
            System.arraycopy(b, oldIndex, padded, index, len);
          }
        }
      }
    }
    else {
      int len = oldWidth < width ? oldWidth * c : width * c;
      for (int oy=0, y=0; oy<oldHeight; oy++, y++) {
        int oldIndex = oldWidth * c * y;
        int index = width * c * (y + hClip) + c * wClip;
        System.arraycopy(b, oldIndex, padded, index, len);
      }
    }
    return padded;
  }

  /**
   * Pads (or crops) the short array to the given width and height.
   * The image will be centered within the new bounds.
   */
  public static short[] padImage(short[] b, boolean interleaved, int c,
    int oldWidth, int width, int height)
  {
    int oldHeight = b.length / (oldWidth * c);
    short[] padded = new short[height * width * c];

    int wClip = (width - oldWidth) / 2;
    int hClip = (height - oldHeight) / 2;

    int h = height < oldHeight ? height : oldHeight;

    if (interleaved) {
      int len = oldWidth < width ? oldWidth : width;
      if (h == oldHeight) {
        for (int y=0; y<h*c; y++) {
          int oldIndex = oldWidth * y;
          int index = width * y;
          System.arraycopy(b, oldIndex, padded, index, len);
        }
      }
      else {
        for (int ch=0; ch<c; ch++) {
          for (int y=0; y<h; y++) {
            int oldIndex = oldWidth * ch * oldHeight + oldWidth * y;
            int index = width * ch * height + width * y;
            System.arraycopy(b, oldIndex, padded, index, len);
          }
        }
      }
    }
    else {
      int len = oldWidth < width ? oldWidth * c : width * c;
      for (int oy=0, y=0; oy<oldHeight; oy++, y++) {
        int oldIndex = oldWidth * c * y;
        int index = width * c * (y + hClip) + c * wClip;
        System.arraycopy(b, oldIndex, padded, index, len);
      }
    }
    return padded;
  }

  /**
   * Pads (or crops) the int array to the given width and height.
   * The image will be centered within the new bounds.
   */
  public static int[] padImage(int[] b, boolean interleaved, int c,
    int oldWidth, int width, int height)
  {
    int oldHeight = b.length / (oldWidth * c);
    int[] padded = new int[height * width * c];

    int wClip = (width - oldWidth) / 2;
    int hClip = (height - oldHeight) / 2;

    int h = height < oldHeight ? height : oldHeight;

    if (interleaved) {
      int len = oldWidth < width ? oldWidth : width;
      if (h == oldHeight) {
        for (int y=0; y<h*c; y++) {
          int oldIndex = oldWidth * y;
          int index = width * y;
          System.arraycopy(b, oldIndex, padded, index, len);
        }
      }
      else {
        for (int ch=0; ch<c; ch++) {
          for (int y=0; y<h; y++) {
            int oldIndex = oldWidth * ch * oldHeight + oldWidth * y;
            int index = width * ch * height + width * y;
            System.arraycopy(b, oldIndex, padded, index, len);
          }
        }
      }
    }
    else {
      int len = oldWidth < width ? oldWidth * c : width * c;
      for (int oy=0, y=0; oy<oldHeight; oy++, y++) {
        int oldIndex = oldWidth * c * y;
        int index = width * c * (y + hClip) + c * wClip;
        System.arraycopy(b, oldIndex, padded, index, len);
      }
    }
    return padded;
  }

  /**
   * Pads (or crops) the float array to the given width and height.
   * The image will be centered within the new bounds.
   */
  public static float[] padImage(float[] b, boolean interleaved, int c,
    int oldWidth, int width, int height)
  {
    int oldHeight = b.length / (oldWidth * c);
    float[] padded = new float[height * width * c];

    int wClip = (width - oldWidth) / 2;
    int hClip = (height - oldHeight) / 2;

    int h = height < oldHeight ? height : oldHeight;

    if (interleaved) {
      int len = oldWidth < width ? oldWidth : width;
      if (h == oldHeight) {
        for (int y=0; y<h*c; y++) {
          int oldIndex = oldWidth * y;
          int index = width * y;
          System.arraycopy(b, oldIndex, padded, index, len);
        }
      }
      else {
        for (int ch=0; ch<c; ch++) {
          for (int y=0; y<h; y++) {
            int oldIndex = oldWidth * ch * oldHeight + oldWidth * y;
            int index = width * ch * height + width * y;
            System.arraycopy(b, oldIndex, padded, index, len);
          }
        }
      }
    }
    else {
      int len = oldWidth < width ? oldWidth * c : width * c;
      for (int oy=0, y=0; oy<oldHeight; oy++, y++) {
        int oldIndex = oldWidth * c * y;
        int index = width * c * (y + hClip) + c * wClip;
        System.arraycopy(b, oldIndex, padded, index, len);
      }
    }
    return padded;
  }

  /**
   * Pads (or crops) the double array to the given width and height.
   * The image will be centered within the new bounds.
   */
  public static double[] padImage(double[] b, boolean interleaved, int c,
    int oldWidth, int width, int height)
  {
    int oldHeight = b.length / (oldWidth * c);
    double[] padded = new double[height * width * c];

    int wClip = (width - oldWidth) / 2;
    int hClip = (height - oldHeight) / 2;

    int h = height < oldHeight ? height : oldHeight;

    if (interleaved) {
      int len = oldWidth < width ? oldWidth : width;
      if (h == oldHeight) {
        for (int y=0; y<h*c; y++) {
          int oldIndex = oldWidth * y;
          int index = width * y;
          System.arraycopy(b, oldIndex, padded, index, len);
        }
      }
      else {
        for (int ch=0; ch<c; ch++) {
          for (int y=0; y<h; y++) {
            int oldIndex = oldWidth * ch * oldHeight + oldWidth * y;
            int index = width * ch * height + width * y;
            System.arraycopy(b, oldIndex, padded, index, len);
          }
        }
      }
    }
    else {
      int len = oldWidth < width ? oldWidth * c : width * c;
      for (int oy=0, y=0; oy<oldHeight; oy++, y++) {
        int oldIndex = oldWidth * c * y;
        int index = width * c * (y + hClip) + c * wClip;
        System.arraycopy(b, oldIndex, padded, index, len);
      }
    }
    return padded;
  }

  /**
   * Perform autoscaling on the given byte array;
   * map min to 0 and max to 255.  If the number of bytes per pixel is 1, then
   * nothing happens.
   */
  public static byte[] autoscale(byte[] b, int min, int max, int bpp,
    boolean little)
  {
    if (bpp == 1) return b;

    byte[] out = new byte[b.length / bpp];

    for (int i=0; i<b.length; i+=bpp) {
      int s = DataTools.bytesToInt(b, i, bpp, little);

      if (s >= max) s = 255;
      else if (s <= min) s = 0;
      else {
        int diff = max - min;
        float dist = (s - min) / diff;

        s = (int) dist * 256;
      }

      out[i / bpp] = (byte) s;
    }
    return out;
  }

  /** Scan a plane for the channel min and max values. */
  public static Double[] scanData(byte[] plane, int bits, boolean littleEndian)
  {
    int max = 0;
    int min = Integer.MAX_VALUE;

    if (bits <= 8) {
      for (int j=0; j<plane.length; j++) {
        if (plane[j] < min) min = plane[j];
        if (plane[j] > max) max = plane[j];
      }
    }
    else if (bits == 16) {
      for (int j=0; j<plane.length; j+=2) {
        short s = DataTools.bytesToShort(plane, j, 2, littleEndian);
        if (s < min) min = s;
        if (s > max) max = s;
      }
    }
    else if (bits == 32) {
      for (int j=0; j<plane.length; j+=4) {
        int s = DataTools.bytesToInt(plane, j, 4, littleEndian);
        if (s < min) min = s;
        if (s > max) max = s;
      }
    }

    Double[] rtn = new Double[2];
    rtn[0] = new Double(min);
    rtn[1] = new Double(max);
    return rtn;
  }

  public static byte[] getSubimage(byte[] src, byte[] dest, int originalWidth,
    int originalHeight, int x, int y, int w, int h, int bpp, int channels,
    boolean interleaved)
  {
    for (int yy=y; yy<y + h; yy++) {
      for (int xx=x; xx<x + w; xx++) {
        for (int cc=0; cc<channels; cc++) {
          int oldNdx = -1, newNdx = -1;
          if (interleaved) {
            oldNdx = yy*originalWidth*bpp*channels + xx*bpp*channels + cc*bpp;
            newNdx = (yy - y)*w*bpp*channels + (xx - x)*bpp*channels + cc*bpp;
          }
          else {
            oldNdx =
              bpp*(cc*originalWidth*originalHeight + yy*originalWidth + xx);
            newNdx = bpp*(cc*w*h + (yy - y)*w + (xx - x));
          }
          System.arraycopy(src, oldNdx, dest, newNdx, bpp);
        }
      }
    }
    return dest;
  }

  // -- Indexed color conversion --

  /** Converts a LUT and an array of indices into an array of RGB tuples. */
  public static byte[][] indexedToRGB(byte[][] lut, byte[] b) {
    byte[][] rtn = new byte[lut.length][b.length];

    for (int i=0; i<b.length; i++) {
      for (int j=0; j<lut.length; j++) {
        rtn[j][i] = lut[j][b[i] & 0xff];
      }
    }
    return rtn;
  }

  /** Converts a LUT and an array of indices into an array of RGB tuples. */
  public static short[][] indexedToRGB(short[][] lut, byte[] b, boolean le) {
    short[][] rtn = new short[lut.length][b.length / 2];
    for (int i=0; i<b.length/2; i++) {
      for (int j=0; j<lut.length; j++) {
        rtn[j][i] = lut[j][DataTools.bytesToShort(b, i*2, 2, le)];
      }
    }
    return rtn;
  }

}
