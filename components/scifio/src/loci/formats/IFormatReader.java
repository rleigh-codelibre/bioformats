//
// IFormatReader.java
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
import java.util.Hashtable;

import loci.common.RandomAccessInputStream;
import loci.formats.meta.MetadataStore;

/**
 * Interface for all biological file format readers.
 *
 * <dl><dt><b>Source code:</b></dt>
 * <dd><a href="http://trac.openmicroscopy.org.uk/ome/browser/bioformats.git/components/bio-formats/src/loci/formats/IFormatReader.java">Trac</a>,
 * <a href="http://git.openmicroscopy.org/?p=bioformats.git;a=blob;f=components/bio-formats/src/loci/formats/IFormatReader.java;hb=HEAD">Gitweb</a></dd></dl>
 */
public interface IFormatReader extends IFormatHandler, IMetadataConfigurable {

  // -- Constants --

  /** File grouping options. */
  int MUST_GROUP = 0;
  int CAN_GROUP = 1;
  int CANNOT_GROUP = 2;

  // -- IFormatReader API methods --

  /**
   * Checks if the given file is a valid instance of this file format.
   *
   * @param open If true, and the file extension is insufficient to determine
   *   the file type, the file may be opened for further analysis, or other
   *   relatively expensive file system operations (such as file existence
   *   tests and directory listings) may be performed.  If false, file system
   *   access is not allowed.
   */
  boolean isThisType(String name, boolean open);

  /** Checks if the given block is a valid header for this file format. */
  boolean isThisType(byte[] block);

  /** Checks if the given stream is a valid stream for this file format. */
  boolean isThisType(RandomAccessInputStream stream) throws IOException;

  /** Determines the number of image planes in the current file. */
  int getImageCount();

  /**
   * Checks if the image planes in the file have more than one channel per
   * {@link #openBytes} call.
   * This method returns true if and only if {@link #getRGBChannelCount()}
   * returns a value greater than 1.
   */
  boolean isRGB();

  /** Gets the size of the X dimension. */
  int getSizeX();

  /** Gets the size of the Y dimension. */
  int getSizeY();

  /** Gets the size of the Z dimension. */
  int getSizeZ();

  /** Gets the size of the C dimension. */
  int getSizeC();

  /** Gets the size of the T dimension. */
  int getSizeT();

  /**
   * Gets the pixel type.
   * @return the pixel type as an enumeration from {@link FormatTools}
   * <i>static</i> pixel types such as {@link FormatTools#INT8}.
   */
  int getPixelType();

  /**
   * Gets the number of valid bits per pixel. The number of valid bits per
   * pixel is always less than or equal to the number of bits per pixel
   * that correspond to {@link #getPixelType()}.
   */
  int getBitsPerPixel();

  /**
   * Gets the effective size of the C dimension, guaranteeing that
   * getEffectiveSizeC() * getSizeZ() * getSizeT() == getImageCount()
   * regardless of the result of isRGB().
   */
  int getEffectiveSizeC();

  /**
   * Gets the number of channels returned with each call to openBytes.
   * The most common case where this value is greater than 1 is for interleaved
   * RGB data, such as a 24-bit color image plane. However, it is possible for
   * this value to be greater than 1 for non-interleaved data, such as an RGB
   * TIFF with Planar rather than Chunky configuration.
   */
  int getRGBChannelCount();

  /**
   * Gets whether the image planes are indexed color.
   * This value has no impact on {@link #getSizeC()},
   * {@link #getEffectiveSizeC()} or {@link #getRGBChannelCount()}.
   */
  boolean isIndexed();

  /**
   * Returns false if {@link #isIndexed()} is false, or if {@link #isIndexed()}
   * is true and the lookup table represents "real" color data. Returns true
   * if {@link #isIndexed()} is true and the lookup table is only present to aid
   * in visualization.
   */
  boolean isFalseColor();

  /**
   * Gets the 8-bit color lookup table associated with
   * the most recently opened image.
   * If no image planes have been opened, or if {@link #isIndexed()} returns
   * false, then this may return null. Also, if {@link #getPixelType()} returns
   * anything other than {@link FormatTools#INT8} or {@link FormatTools#UINT8},
   * this method will return null.
   */
  byte[][] get8BitLookupTable() throws FormatException, IOException;

  /**
   * Gets the 16-bit color lookup table associated with
   * the most recently opened image.
   * If no image planes have been opened, or if {@link #isIndexed()} returns
   * false, then this may return null. Also, if {@link #getPixelType()} returns
   * anything other than {@link FormatTools#INT16} or {@link
   * FormatTools#UINT16}, this method will return null.
   */
  short[][] get16BitLookupTable() throws FormatException, IOException;

  /**
   * Gets the lengths of each subdimension of C,
   * in fastest-to-slowest rasterization order.
   */
  int[] getChannelDimLengths();

  /**
   * Gets the name of each subdimension of C,
   * in fastest-to-slowest rasterization order.
   * Common subdimensional types are enumerated in {@link FormatTools}.
   */
  String[] getChannelDimTypes();

  /** Get the size of the X dimension for the thumbnail. */
  int getThumbSizeX();

  /** Get the size of the Y dimension for the thumbnail. */
  int getThumbSizeY();

  /** Gets whether the data is in little-endian format. */
  boolean isLittleEndian();

  /**
   * Gets a five-character string representing the
   * dimension order in which planes will be returned. Valid orders are:<ul>
   *   <li>XYCTZ</li>
   *   <li>XYCZT</li>
   *   <li>XYTCZ</li>
   *   <li>XYTZC</li>
   *   <li>XYZCT</li>
   *   <li>XYZTC</li>
   * </ul>
   * In cases where the channels are interleaved (e.g., CXYTZ), C will be
   * the first dimension after X and Y (e.g., XYCTZ) and the
   * {@link #isInterleaved()} method will return true.
   */
  String getDimensionOrder();

  /**
   * Gets whether the dimension order and sizes are known, or merely guesses.
   */
  boolean isOrderCertain();

  /**
   * Gets whether the current series is a lower resolution copy of a different
   * series.
   */
  boolean isThumbnailSeries();

  /**
   * Gets whether or not the channels are interleaved. This method exists
   * because X and Y must appear first in the dimension order. For
   * interleaved data, {@link #getDimensionOrder()} returns XYCTZ or XYCZT,
   * and this method returns true.
   *
   * Note that this flag returns whether or not the data returned by
   * {@link #openBytes(int)} is interleaved.  In most cases, this will
   * match the interleaving in the original file, but for some formats (e.g.
   * TIFF) channel re-ordering is done internally and the return value of
   * this method will not match what is in the original file.
   */
  boolean isInterleaved();

  /**
   * Gets whether or not the given sub-channel is interleaved. This method
   * exists because some data with multiple rasterized sub-dimensions within
   * C have one sub-dimension interleaved, and the other not&mdash;e.g.,
   * {@link loci.formats.in.SDTReader} handles spectral-lifetime data with
   * interleaved lifetime bins and non-interleaved spectral channels.
   */
  boolean isInterleaved(int subC);

  /**
   * Obtains the specified image plane from the current file as a byte array.
   * @see #openBytes(int, byte[])
   */
  byte[] openBytes(int no) throws FormatException, IOException;

  /**
   * Obtains a sub-image of the specified image plane,
   * whose upper-left corner is given by (x, y).
   */
  byte[] openBytes(int no, int x, int y, int w, int h)
    throws FormatException, IOException;

  /**
   * Obtains the specified image plane from the current file into a
   * pre-allocated byte array of
   * (sizeX * sizeY * bytesPerPixel * RGB channel count).
   *
   * @param no the image index within the file.
   * @param buf a pre-allocated buffer.
   * @return the pre-allocated buffer <code>buf</code> for convenience.
   * @throws FormatException if there was a problem parsing the metadata of the
   *   file.
   * @throws IOException if there was a problem reading the file.
   */
  byte[] openBytes(int no, byte[] buf)
    throws FormatException, IOException;

  /**
   * Obtains a sub-image of the specified image plane
   * into a pre-allocated byte array.
   *
   * @param no the image index within the file.
   * @param buf a pre-allocated buffer.
   * @param x X coordinate of the upper-left corner of the sub-image
   * @param y Y coordinate of the upper-left corner of the sub-image
   * @param w width of the sub-image
   * @param h height of the sub-image
   * @return the pre-allocated buffer <code>buf</code> for convenience.
   * @throws FormatException if there was a problem parsing the metadata of the
   *   file.
   * @throws IOException if there was a problem reading the file.
   */
  byte[] openBytes(int no, byte[] buf, int x, int y, int w, int h)
    throws FormatException, IOException;

  /**
   * Obtains the specified image plane (or sub-image thereof) in the reader's
   * native data structure. For most readers this is a byte array; however,
   * some readers call external APIs that work with other types such as
   * {@link java.awt.image.BufferedImage}. The openPlane method exists to
   * maintain generality and efficiency while avoiding pollution of the API
   * with AWT-specific logic.
   *
   * @see loci.formats.FormatReader
   * @see loci.formats.gui.BufferedImageReader
   */
  Object openPlane(int no, int x, int y, int w, int h)
    throws FormatException, IOException;

  /**
   * Obtains a thumbnail for the specified image plane from the current file,
   * as a byte array.
   */
  byte[] openThumbBytes(int no) throws FormatException, IOException;

  /**
   * Closes the currently open file. If the flag is set, this is all that
   * happens; if unset, it is equivalent to calling
   * {@link IFormatHandler#close()}.
   */
  void close(boolean fileOnly) throws IOException;

  /** Gets the number of series in this file. */
  int getSeriesCount();

  /** Activates the specified series. */
  void setSeries(int no);

  /** Gets the currently active series. */
  int getSeries();

  /** Specifies whether or not to normalize float data. */
  void setNormalized(boolean normalize);

  /** Returns true if we should normalize float data. */
  boolean isNormalized();

  /**
   * Specifies whether or not to save proprietary metadata
   * in the MetadataStore.
   */
  void setOriginalMetadataPopulated(boolean populate);

  /**
   * Returns true if we should save proprietary metadata
   * in the MetadataStore.
   */
  boolean isOriginalMetadataPopulated();

  /** Specifies whether or not to force grouping in multi-file formats. */
  void setGroupFiles(boolean group);

  /** Returns true if we should group files in multi-file formats.*/
  boolean isGroupFiles();

  /** Returns true if this format's metadata is completely parsed. */
  boolean isMetadataComplete();

  /**
   * Returns an int indicating that we cannot, must, or might group the files
   * in a given dataset.
   */
  int fileGroupOption(String id) throws FormatException, IOException;

  /** Returns an array of filenames needed to open this dataset. */
  String[] getUsedFiles();

  /**
   * Returns an array of filenames needed to open this dataset.
   * If the 'noPixels' flag is set, then only files that do not contain
   * pixel data will be returned.
   */
  String[] getUsedFiles(boolean noPixels);

  /** Returns an array of filenames needed to open the current series. */
  String[] getSeriesUsedFiles();

  /**
   * Returns an array of filenames needed to open the current series.
   * If the 'noPixels' flag is set, then only files that do not contain
   * pixel data will be returned.
   */
  String[] getSeriesUsedFiles(boolean noPixels);

  /**
   * Returns an array of FileInfo objects representing the files needed
   * to open this dataset.
   * If the 'noPixels' flag is set, then only files that do not contain
   * pixel data will be returned.
   */
  FileInfo[] getAdvancedUsedFiles(boolean noPixels);

  /**
   * Returns an array of FileInfo objects representing the files needed to
   * open the current series.
   * If the 'noPixels' flag is set, then only files that do not contain
   * pixel data will be returned.
   */
  FileInfo[] getAdvancedSeriesUsedFiles(boolean noPixels);

  /** Returns the current file. */
  String getCurrentFile();

  /** Returns the list of domains represented by the current file. */
  String[] getDomains();

  /**
   * Gets the rasterized index corresponding
   * to the given Z, C and T coordinates.
   */
  int getIndex(int z, int c, int t);

  /**
   * Gets the Z, C and T coordinates corresponding
   * to the given rasterized index value.
   */
  int[] getZCTCoords(int index);

  /**
   * Obtains the specified metadata field's value for the current file.
   * @param field the name associated with the metadata field
   * @return the value, or null if the field doesn't exist
   */
  Object getMetadataValue(String field);

  /**
   * Obtains the specified metadata field's value for the current series
   * in the current file.
   * @param field the name associated with the metadata field
   * @return the value, or null if the field doesn't exist
   */
  Object getSeriesMetadataValue(String field);

  /**
   * Obtains the hashtable containing the metadata field/value pairs from
   * the current file.
   * @return the hashtable containing all non-series-specific metadata
   * from the file
   */
  Hashtable<String, Object> getGlobalMetadata();

  /**
   * Obtains the hashtable containing metadata field/value pairs from the
   * current series in the current file.
   */
  Hashtable<String, Object> getSeriesMetadata();

  /** Obtains the core metadata values for the current file. */
  CoreMetadata[] getCoreMetadata();

  /**
   * Specifies whether ugly metadata (entries with unprintable characters,
   * and extremely large entries) should be discarded from the metadata table.
   */
  void setMetadataFiltered(boolean filter);

  /**
   * Returns true if ugly metadata (entries with unprintable characters,
   * and extremely large entries) are discarded from the metadata table.
   */
  boolean isMetadataFiltered();

  /**
   * Sets the default metadata store for this reader.
   * @param store a metadata store implementation.
   */
  void setMetadataStore(MetadataStore store);

  /**
   * Retrieves the current metadata store for this reader. You can be
   * assured that this method will <b>never</b> return a <code>null</code>
   * metadata store.
   * @return A metadata store implementation.
   */
  MetadataStore getMetadataStore();

  /**
   * Retrieves the current metadata store's root object. It is guaranteed that
   * all file parsing has been performed by the reader prior to retrieval.
   * Requests for a full populated root object should be made using this method.
   * @return Current metadata store's root object fully populated.
   */
  Object getMetadataStoreRoot();

  /**
   * Retrieves all underlying readers.
   * Returns null if there are no underlying readers.
   */
  IFormatReader[] getUnderlyingReaders();

  /** Returns true if this is a single-file format. */
  boolean isSingleFile(String id) throws FormatException, IOException;

  /** Returns a short description of the dataset structure. */
  String getDatasetStructureDescription();

  /** Returns a list of scientific domains in which this format is used. */
  String[] getPossibleDomains(String id) throws FormatException, IOException;

  /** Returns true if this format supports multi-file datasets. */
  boolean hasCompanionFiles();

  /** Returns the optimal sub-image width for use with openBytes. */
  int getOptimalTileWidth();

  /** Returns the optimal sub-image height for use with openBytes. */
  int getOptimalTileHeight();

  // -- Deprecated methods --

  /**
   * Specifies whether or not to collect metadata.
   * @deprecated Use {@link #setMetadataOptions(MetadataOptions)} instead.
   */
  void setMetadataCollected(boolean collect);

  /**
   * Returns true if we should collect metadata.
   * @deprecated Use {@link #getMetadataOptions()} instead.
   */
  boolean isMetadataCollected();

  /**
   * Returns a hashtable containing the union of all of the field/value pairs
   * in getGlobalMetadata() and getSeriesMetadata(). The series name is
   * prepended to fields in the getSeriesMetadata() hashtable.
   *
   * @deprecated Use #getGlobalMetadata() or #getSeriesMetadata() instead.
   */
  Hashtable<String, Object> getMetadata();

}
