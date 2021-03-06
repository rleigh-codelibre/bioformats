//
// MNGReader.java
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

package loci.formats.in;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.util.Hashtable;
import java.util.Vector;

import javax.imageio.ImageIO;

import loci.common.RandomAccessInputStream;
import loci.formats.CoreMetadata;
import loci.formats.FormatException;
import loci.formats.FormatTools;
import loci.formats.MetadataTools;
import loci.formats.gui.AWTImageTools;
import loci.formats.meta.MetadataStore;

/**
 * MNGReader is the file format reader for Multiple Network Graphics (MNG)
 * files.  Does not support JNG (JPEG Network Graphics).
 *
 * <dl><dt><b>Source code:</b></dt>
 * <dd><a href="http://trac.openmicroscopy.org.uk/ome/browser/bioformats.git/components/bio-formats/src/loci/formats/in/MNGReader.java">Trac</a>,
 * <a href="http://git.openmicroscopy.org/?p=bioformats.git;a=blob;f=components/bio-formats/src/loci/formats/in/MNGReader.java;hb=HEAD">Gitweb</a></dd></dl>
 */
public class MNGReader extends BIFormatReader {

  // -- Constants --

  public static final long MNG_MAGIC_BYTES = 0x8a4d4e470d0a1a0aL;

  // -- Fields --

  private Vector<SeriesInfo> seriesInfo;

  // -- Constructor --

  /** Constructs a new MNG reader. */
  public MNGReader() {
    super("Multiple Network Graphics", "mng");
    domains = new String[] {FormatTools.GRAPHICS_DOMAIN};
  }

  // -- IFormatReader API methods --

  /* @see loci.formats.IFormatReader#isThisType(RandomAccessInputStream) */
  public boolean isThisType(RandomAccessInputStream stream) throws IOException {
    final int blockLen = 8;
    if (!FormatTools.validStream(stream, blockLen, false)) return false;
    return stream.readLong() == MNG_MAGIC_BYTES;
  }

  /* @see loci.formats.IFormatReader#openPlane(int, int, int, int, int int) */
  public Object openPlane(int no, int x, int y, int w, int h)
    throws FormatException, IOException
  {
    FormatTools.checkPlaneParameters(this, no, -1, x, y, w, h);

    SeriesInfo info = seriesInfo.get(series);
    long offset = info.offsets.get(no);
    in.seek(offset);
    long end = info.lengths.get(no);
    BufferedImage img = readImage(end);

    // reconstruct the image to use an appropriate raster
    // ImageIO often returns images that cannot be scaled because a
    // BytePackedRaster is used
    return AWTImageTools.getSubimage(img, isLittleEndian(), x, y, w, h);
  }

  /* @see loci.formats.IFormatReader#close(boolean) */
  public void close(boolean fileOnly) throws IOException {
    super.close(fileOnly);
    if (!fileOnly) seriesInfo = null;
  }

  // -- Internal FormatReader API methods --

  /* @see loci.formats.FormatReader#initFile(String) */
  protected void initFile(String id) throws FormatException, IOException {
    super.initFile(id);
    in = new RandomAccessInputStream(id);
    in.order(false);

    LOGGER.info("Verifying MNG format");

    seriesInfo = new Vector<SeriesInfo>();
    seriesInfo.add(new SeriesInfo());

    in.skipBytes(12);

    if (!"MHDR".equals(in.readString(4))) {
      throw new FormatException("Invalid MNG file.");
    }

    LOGGER.info("Reading dimensions");

    in.skipBytes(32);

    Vector<Long> stack = new Vector<Long>();
    int maxIterations = 0;
    int currentIteration = 0;

    LOGGER.info("Finding image offsets");

    // read sequence of [len, code, value] tags

    while (in.getFilePointer() < in.length()) {
      int len = in.readInt();
      String code = in.readString(4);

      long fp = in.getFilePointer();

      if (code.equals("IHDR")) {
        seriesInfo.get(0).offsets.add(fp - 8);
      }
      else if (code.equals("IEND")) {
        seriesInfo.get(0).lengths.add(fp + len + 4);
      }
      else if (code.equals("LOOP")) {
        stack.add(fp + len + 4);
        in.skipBytes(1);
        maxIterations = in.readInt();
      }
      else if (code.equals("ENDL")) {
        long seek = stack.get(stack.size() - 1).longValue();
        if (currentIteration < maxIterations) {
          in.seek(seek);
          currentIteration++;
        }
        else {
          stack.remove(stack.size() - 1);
          maxIterations = 0;
          currentIteration = 0;
        }
      }

      in.seek(fp + len + 4);
    }

    LOGGER.info("Populating metadata");

    // easiest way to get image dimensions is by opening the first plane

    Hashtable<String, Vector> seriesOffsets = new Hashtable<String, Vector>();
    Hashtable<String, Vector> seriesLengths = new Hashtable<String, Vector>();

    SeriesInfo info = seriesInfo.get(0);
    addGlobalMeta("Number of frames", info.offsets.size());
    for (int i=0; i<info.offsets.size(); i++) {
      long offset = info.offsets.get(i);
      in.seek(offset);
      long end = info.lengths.get(i);
      if (end < offset) continue;
      BufferedImage img = readImage(end);
      String data = img.getWidth() + "-" + img.getHeight() + "-" +
        img.getRaster().getNumBands() + "-" + AWTImageTools.getPixelType(img);
      Vector<Long> v = new Vector<Long>();
      if (seriesOffsets.containsKey(data)) {
        v = seriesOffsets.get(data);
      }
      v.add(new Long(offset));
      seriesOffsets.put(data, v);

      v = new Vector();
      if (seriesLengths.containsKey(data)) {
        v = seriesLengths.get(data);
      }
      v.add(new Long(end));
      seriesLengths.put(data, v);
    }

    String[] keys = seriesOffsets.keySet().toArray(new String[0]);

    if (keys.length == 0) {
      throw new FormatException("Pixel data not found.");
    }

    core = new CoreMetadata[keys.length];

    seriesInfo.clear();
    for (int i=0; i<keys.length; i++) {
      core[i] = new CoreMetadata();
      String[] tokens = keys[i].split("-");
      core[i].sizeX = Integer.parseInt(tokens[0]);
      core[i].sizeY = Integer.parseInt(tokens[1]);
      core[i].sizeC = Integer.parseInt(tokens[2]);
      core[i].pixelType = Integer.parseInt(tokens[3]);
      core[i].rgb = core[i].sizeC > 1;
      core[i].sizeZ = 1;
      core[i].dimensionOrder = "XYCZT";
      core[i].interleaved = false;
      core[i].metadataComplete = true;
      core[i].indexed = false;
      core[i].littleEndian = false;
      core[i].falseColor = false;

      SeriesInfo inf = new SeriesInfo();
      inf.offsets = seriesOffsets.get(keys[i]);
      inf.lengths = seriesLengths.get(keys[i]);
      seriesInfo.add(inf);

      core[i].imageCount = inf.offsets.size();
      core[i].sizeT = core[i].imageCount;
    }

    MetadataStore store = makeFilterMetadata();
    MetadataTools.populatePixels(store, this);
    for (int i=0; i<getSeriesCount(); i++) {
      store.setImageName("Series " + (i + 1), i);
    }
  }

  // -- Helper methods --

  private BufferedImage readImage(long end) throws IOException {
    byte[] b = new byte[(int) (end - in.getFilePointer() + 8)];
    in.read(b, 8, b.length - 8);
    b[0] = (byte) 0x89;
    b[1] = 0x50;
    b[2] = 0x4e;
    b[3] = 0x47;
    b[4] = 0x0d;
    b[5] = 0x0a;
    b[6] = 0x1a;
    b[7] = 0x0a;
    return ImageIO.read(new ByteArrayInputStream(b));
  }

  // -- Helper class --

  private class SeriesInfo {
    public Vector<Long> offsets = new Vector<Long>();
    public Vector<Long> lengths = new Vector<Long>();
  }

}
