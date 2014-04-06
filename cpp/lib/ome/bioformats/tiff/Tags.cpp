/*
 * #%L
 * OME-BIOFORMATS C++ library for image IO.
 * Copyright © 2006 - 2014 Open Microscopy Environment:
 *   - Massachusetts Institute of Technology
 *   - National Institutes of Health
 *   - University of Dundee
 *   - Board of Regents of the University of Wisconsin-Madison
 *   - Glencoe Software, Inc.
 * %%
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of any organization.
 * #L%
 */

#include <ome/bioformats/tiff/Tags.h>

#include <tiffio.h>

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

    // No switch default to avoid -Wunreachable-code errors.
    // However, this then makes -Wswitch-default complain.  Disable
    // temporarily.
#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wswitch-default"
#endif

      tag_type
      getWrappedTag(StringTag1 tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case ARTIST:
#ifdef TIFFTAG_ARTIST
            ret = TIFFTAG_ARTIST;
#endif
            break;
          case COPYRIGHT:
#ifdef TIFFTAG_COPYRIGHT
            ret = TIFFTAG_COPYRIGHT;
#endif
            break;
          case DATETIME:
#ifdef TIFFTAG_DATETIME
            ret = TIFFTAG_DATETIME;
#endif
            break;
          case DOCUMENTNAME:
#ifdef TIFFTAG_DOCUMENTNAME
            ret = TIFFTAG_DOCUMENTNAME;
#endif
            break;
          case HOSTCOMPUTER:
#ifdef TIFFTAG_HOSTCOMPUTER
            ret = TIFFTAG_HOSTCOMPUTER;
#endif
            break;
          case IMAGEDESCRIPTION:
#ifdef TIFFTAG_IMAGEDESCRIPTION
            ret = TIFFTAG_IMAGEDESCRIPTION;
#endif
            break;
          case INKNAMES:
#ifdef TIFFTAG_INKNAMES
            ret = TIFFTAG_INKNAMES;
#endif
            break;
          case MAKE:
#ifdef TIFFTAG_MAKE
            ret = TIFFTAG_MAKE;
#endif
            break;
          case MODEL:
#ifdef TIFFTAG_MODEL
            ret = TIFFTAG_MODEL;
#endif
            break;
          case PAGENAME:
#ifdef TIFFTAG_PAGENAME
            ret = TIFFTAG_PAGENAME;
#endif
            break;
          case SOFTWARE:
#ifdef TIFFTAG_SOFTWARE
            ret = TIFFTAG_SOFTWARE;
#endif
            break;
          case TARGETPRINTER:
#ifdef TIFFTAG_TARGETPRINTER
            ret = TIFFTAG_TARGETPRINTER;
#endif
            break;
          }

        return ret;
      }

      tag_type
      getWrappedTag(UInt16Tag1 tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case BITSPERSAMPLE:
#ifdef TIFFTAG_BITSPERSAMPLE
            ret = BITSPERSAMPLE;
#endif
            break;
          case CLEANFAXDATA:
#ifdef TIFFTAG_CLEANFAXDATA
            ret = CLEANFAXDATA;
#endif
            break;
          case COMPRESSION:
#ifdef TIFFTAG_COMPRESSION
            ret = COMPRESSION;
#endif
            break;
          case DATATYPE:
#ifdef TIFFTAG_DATATYPE
            ret = DATATYPE;
#endif
            break;
          case FILLORDER:
#ifdef TIFFTAG_FILLORDER
            ret = FILLORDER;
#endif
            break;
          case INKSET:
#ifdef TIFFTAG_INKSET
            ret = INKSET;
#endif
            break;
          case MATTEING:
#ifdef TIFFTAG_MATTEING
            ret = MATTEING;
#endif
            break;
          case MAXSAMPLEVALUE:
#ifdef TIFFTAG_MAXSAMPLEVALUE
            ret = MAXSAMPLEVALUE;
#endif
            break;
          case MINSAMPLEVALUE:
#ifdef TIFFTAG_MINSAMPLEVALUE
            ret = MINSAMPLEVALUE;
#endif
            break;
          case ORIENTATION:
#ifdef TIFFTAG_ORIENTATION
            ret = ORIENTATION;
#endif
            break;
          case PHOTOMETRIC:
#ifdef TIFFTAG_PHOTOMETRIC
            ret = PHOTOMETRIC;
#endif
            break;
          case PLANARCONFIG:
#ifdef TIFFTAG_PLANARCONFIG
            ret = PLANARCONFIG;
#endif
            break;
          case PREDICTOR:
#ifdef TIFFTAG_PREDICTOR
            ret = PREDICTOR;
#endif
            break;
          case RESOLUTIONUNIT:
#ifdef TIFFTAG_RESOLUTIONUNIT
            ret = RESOLUTIONUNIT;
#endif
            break;
          case SAMPLEFORMAT:
#ifdef TIFFTAG_SAMPLEFORMAT
            ret = SAMPLEFORMAT;
#endif
            break;
          case SAMPLESPERPIXEL:
#ifdef TIFFTAG_SAMPLESPERPIXEL
            ret = SAMPLESPERPIXEL;
#endif
            break;
          case THRESHHOLDING:
#ifdef TIFFTAG_THRESHHOLDING
            ret = THRESHHOLDING;
#endif
            break;
          case YCBCRPOSITIONING:
#ifdef TIFFTAG_YCBCRPOSITIONING
            ret = YCBCRPOSITIONING;
#endif
            break;
          case YCBCRSUBSAMPLING:
#ifdef TIFFTAG_YCBCRSUBSAMPLING
            ret = YCBCRSUBSAMPLING;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(UInt16Tag2 tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case DOTRANGE:
#ifdef TIFFTAG_DOTRANGE
            ret = DOTRANGE;
#endif
            break;
          case HALFTONEHINTS:
#ifdef TIFFTAG_HALFTONEHINTS
            ret = HALFTONEHINTS;
#endif
            break;
          case PAGENUMBER:
#ifdef TIFFTAG_PAGENUMBER
            ret = PAGENUMBER;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(UInt16UInt16TagArray tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case EXTRASAMPLES:
#ifdef TIFFTAG_EXTRASAMPLES
            ret = EXTRASAMPLES;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(UInt16UInt32TagArray tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case SUBIFD:
#ifdef TIFFTAG_SUBIFD
            ret = SUBIFD;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(UInt16TagArray2 tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case COLORMAP:
#ifdef TIFFTAG_COLORMAP
            ret = COLORMAP;
#endif
            break;
          case TRANSFERFUNCTION:
#ifdef TIFFTAG_TRANSFERFUNCTION
            ret = TRANSFERFUNCTION;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(UInt32Tag1 tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case BADFAXLINES:
#ifdef TIFFTAG_BADFAXLINES
            ret = BADFAXLINES;
#endif
            break;
          case CONSECUTIVEBADFAXLINES:
#ifdef TIFFTAG_CONSECUTIVEBADFAXLINES
            ret = CONSECUTIVEBADFAXLINES;
#endif
            break;
          case GROUP3OPTIONS:
#ifdef TIFFTAG_GROUP3OPTIONS
            ret = GROUP3OPTIONS;
#endif
            break;
          case GROUP4OPTIONS:
#ifdef TIFFTAG_GROUP4OPTIONS
            ret = GROUP4OPTIONS;
#endif
            break;
          case IMAGEDEPTH:
#ifdef TIFFTAG_IMAGEDEPTH
            ret = IMAGEDEPTH;
#endif
            break;
          case IMAGELENGTH:
#ifdef TIFFTAG_IMAGELENGTH
            ret = IMAGELENGTH;
#endif
            break;
          case IMAGEWIDTH:
#ifdef TIFFTAG_IMAGEWIDTH
            ret = IMAGEWIDTH;
#endif
            break;
          case ROWSPERSTRIP:
#ifdef TIFFTAG_ROWSPERSTRIP
            ret = ROWSPERSTRIP;
#endif
            break;
          case SUBFILETYPE:
#ifdef TIFFTAG_SUBFILETYPE
            ret = SUBFILETYPE;
#endif
            break;
          case TILEDEPTH:
#ifdef TIFFTAG_TILEDEPTH
            ret = TILEDEPTH;
#endif
            break;
          case TILELENGTH:
#ifdef TIFFTAG_TILELENGTH
            ret = TILELENGTH;
#endif
            break;
          case TILEWIDTH:
#ifdef TIFFTAG_TILEWIDTH
            ret = TILEWIDTH;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(UInt32UInt32TagArray tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case STRIPBYTECOUNTS:
#ifdef TIFFTAG_STRIPBYTECOUNTS
            ret = STRIPBYTECOUNTS;
#endif
            break;
          case STRIPOFFSETS:
#ifdef TIFFTAG_STRIPOFFSETS
            ret = STRIPOFFSETS;
#endif
            break;
          case TILEBYTECOUNTS:
#ifdef TIFFTAG_TILEBYTECOUNTS
            ret = TILEBYTECOUNTS;
#endif
            break;
          case TILEOFFSETS:
#ifdef TIFFTAG_TILEOFFSETS
            ret = TILEOFFSETS;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(UInt32RawDataTag tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case ICCPROFILE:
#ifdef TIFFTAG_ICCPROFILE
            ret = ICCPROFILE;
#endif
            break;
          case JPEGTABLES:
#ifdef TIFFTAG_JPEGTABLES
            ret = JPEGTABLES;
#endif
            break;
          case PHOTOSHOP:
#ifdef TIFFTAG_PHOTOSHOP
            ret = PHOTOSHOP;
#endif
            break;
          case RICHTIFFIPTC:
#ifdef TIFFTAG_RICHTIFFIPTC
            ret = RICHTIFFIPTC;
#endif
            break;
          case XMLPACKET:
#ifdef TIFFTAG_XMLPACKET
            ret = XMLPACKET;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(FloatTag1 tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case XRESOLUTION:
#ifdef TIFFTAG_XRESOLUTION
            ret = XRESOLUTION;
#endif
            break;
          case YRESOLUTION:
#ifdef TIFFTAG_YRESOLUTION
            ret = YRESOLUTION;
#endif
            break;
          case XPOSITION:
#ifdef TIFFTAG_XPOSITION
            ret = XPOSITION;
#endif
            break;
          case YPOSITION:
#ifdef TIFFTAG_YPOSITION
            ret = YPOSITION;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(FloatTagArray2 tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case WHITEPOINT:
#ifdef TIFFTAG_WHITEPOINT
            ret = WHITEPOINT;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(FloatTagArray3 tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case YCBCRCOEFFICIENTS:
#ifdef TIFFTAG_YCBCRCOEFFICIENTS
            ret = YCBCRCOEFFICIENTS;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(FloatTagArray6 tag)
      {
        tag_type ret = 0;

        switch(tag)
          {
          case PRIMARYCHROMATICITIES:
#ifdef TIFFTAG_PRIMARYCHROMATICITIES
            ret = PRIMARYCHROMATICITIES;
#endif
            break;
          case REFERENCEBLACKWHITE:
#ifdef TIFFTAG_REFERENCEBLACKWHITE
            ret = REFERENCEBLACKWHITE;
#endif
            break;
          };
        return ret;
      }

      tag_type
      getWrappedTag(UnimplementedTag /* tag */)
      {
        return 0;
      }

#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

    }
  }
}
