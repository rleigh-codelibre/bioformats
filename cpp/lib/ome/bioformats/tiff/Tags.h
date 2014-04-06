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

#ifndef OME_BIOFORMATS_TIFF_TAGS_H
#define OME_BIOFORMATS_TIFF_TAGS_H

#include <string>

#include <ome/bioformats/tiff/Types.h>

#include <ome/compat/array.h>
#include <ome/compat/cstdint.h>
#include <ome/compat/tuple.h>

#include <boost/multi_array.hpp>

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

      enum StringTag1
        {
          ARTIST,
          COPYRIGHT,
          DATETIME,
          DOCUMENTNAME,
          HOSTCOMPUTER,
          IMAGEDESCRIPTION,
          INKNAMES,
          MAKE,
          MODEL,
          PAGENAME,
          SOFTWARE,
          TARGETPRINTER
        };

      enum UInt16Tag1
        {
          BITSPERSAMPLE,
          CLEANFAXDATA,
          COMPRESSION,
          DATATYPE,
          FILLORDER,
          INKSET,
          MATTEING,
          MAXSAMPLEVALUE,
          MINSAMPLEVALUE,
          ORIENTATION,
          PHOTOMETRIC,
          PLANARCONFIG,
          PREDICTOR,
          RESOLUTIONUNIT,
          SAMPLEFORMAT,
          SAMPLESPERPIXEL,
          THRESHHOLDING,
          YCBCRPOSITIONING,
          YCBCRSUBSAMPLING
        };

      enum UInt16Tag2
        {
          DOTRANGE,
          HALFTONEHINTS,
          PAGENUMBER
        };

      enum UInt16UInt16TagArray
        {
          EXTRASAMPLES
        };

      enum UInt16UInt32TagArray
        {
          SUBIFD
        };

      enum UInt16TagArray2
        {
          COLORMAP,
          TRANSFERFUNCTION
        };

      enum UInt32Tag1
        {
          BADFAXLINES,
          CONSECUTIVEBADFAXLINES,
          GROUP3OPTIONS,
          GROUP4OPTIONS,
          IMAGEDEPTH,
          IMAGELENGTH,
          IMAGEWIDTH,
          ROWSPERSTRIP,
          SUBFILETYPE,
          TILEDEPTH,
          TILELENGTH,
          TILEWIDTH
        };

      enum UInt32UInt32TagArray
        {
          STRIPBYTECOUNTS,
          STRIPOFFSETS,
          TILEBYTECOUNTS,
          TILEOFFSETS
        };

      enum UInt32RawDataTag
        {
          ICCPROFILE,
          JPEGTABLES,
          PHOTOSHOP,
          RICHTIFFIPTC,
          XMLPACKET
        };

      enum FloatTag1
        {
          XRESOLUTION,
          YRESOLUTION,
          XPOSITION,
          YPOSITION,
        };

      enum FloatTagArray2
        {
          WHITEPOINT
        };

      enum FloatTagArray3
        {
          YCBCRCOEFFICIENTS
        };

      enum FloatTagArray6
        {
          PRIMARYCHROMATICITIES,
          REFERENCEBLACKWHITE
        };

      /**
       * Implementation details.
       */
      namespace detail
      {

        /**
         * Map a given tag category enum type to the corresponding
         * language value type.
         */
        template<typename TagCategory>
        struct TagProperties;

        /// Properties of StringTag1 tags.
        template<>
        struct TagProperties<StringTag1>
        {
          /// string type.
          typedef std::string value_type;
        };

        /// Properties of UInt16Tag1 tags.
        template<>
        struct TagProperties<UInt16Tag1>
        {
          /// uint16_t type.
          typedef uint16_t value_type;
        };

        /// Properties of UInt16Tag2 tags.
        template<>
        struct TagProperties<UInt16Tag2>
        {
          /// uint16_t array type.
          typedef std::tuple<uint16_t, uint16_t> value_type;
        };

        /// Properties of UInt16UInt16TagArray tags.
        template<>
        struct TagProperties<UInt16UInt16TagArray>
        {
          /// uint16_t vector type.
          typedef std::vector<uint16_t> value_type;
          /// uint16_t count type.
          typedef uint16_t count_type;
        };

        /// Properties of UInt16UInt32TagArray tags.
        template<>
        struct TagProperties<UInt16UInt32TagArray>
        {
          /// uint32_t vector type.
          typedef std::vector<uint32_t> value_type;
          /// uint16_t count type.
          typedef uint16_t count_type;
        };

        /// Properties of UInt16TagArray2 tags.
        template<>
        struct TagProperties<UInt16TagArray2>
        {
          /// uint16_t array type.
          typedef std::array<uint16_t, 2> value_type;
        };

        /// Properties of UInt32Tag1 tags.
        template<>
        struct TagProperties<UInt32Tag1>
        {
          /// uint32_t type.
          typedef uint32_t value_type;
        };

        /// Properties of UInt32UInt32TagArray tags.
        template<>
        struct TagProperties<UInt32UInt32TagArray>
        {
          /// uint32_t vector type.
          typedef std::vector<uint32_t> value_type;
          /// uint32_t count type.
          typedef uint32_t count_type;
        };

        /// Properties of UInt32RawDataTag tags.
        template<>
        struct TagProperties<UInt32RawDataTag>
        {
          /// uint32_t vector type.
          typedef std::vector<uint8_t> value_type;
          /// uint32_t count type.
          typedef uint32_t count_type;
        };

        /// Properties of FloatTag1 tags.
        template<>
        struct TagProperties<FloatTag1>
        {
          /// float type.
          typedef float value_type;
        };

        /// Properties of FloatTagArray2 tags.
        template<>
        struct TagProperties<FloatTagArray2>
        {
          /// float array type.
          typedef std::array<float, 2> value_type;
        };

        /// Properties of FloatTagArray3 tags.
        template<>
        struct TagProperties<FloatTagArray3>
        {
          /// float array type.
          typedef std::array<float, 3> value_type;
        };

        /// Properties of FloatTagArray6 tags.
        template<>
        struct TagProperties<FloatTagArray6>
        {
          /// float array type.
          typedef std::array<float, 6> value_type;
        };

      }

      // Unhandled tags

// OSUBFILETYPE
// CELLWIDTH
// CELLLENGTH
// FREEOFFSETS
// FREEBYTECOUNTS
// GRAYRESPONSEUNIT
// GRAYRESPONSECURVE
// T4OPTIONS
// T6OPTIONS
// COLORRESPONSEUNIT
// NUMBEROFINKS
// SMINSAMPLEVALUE
// SMAXSAMPLEVALUE
// CLIPPATH
// XCLIPPATHUNITS
// YCLIPPATHUNITS
// INDEXED
// OPIPROXY
// GLOBALPARAMETERSIFD
// PROFILETYPE
// FAXPROFILE
// CODINGMETHODS
// VERSIONYEAR
// MODENUMBER
// DECODE
// IMAGEBASECOLOR
// T82OPTIONS
// JPEGPROC
// JPEGIFOFFSET
// JPEGIFBYTECOUNT
// JPEGRESTARTINTERVAL
// JPEGLOSSLESSPREDICTORS
// JPEGPOINTTRANSFORM
// JPEGQTABLES
// JPEGDCTABLES
// JPEGACTABLES
// STRIPROWCOUNTS
// OPIIMAGEID
// REFPTS
// REGIONTACKPOINT
// REGIONWARPCORNERS
// REGIONAFFINE
// PIXAR_IMAGEFULLLENGTH
// YPOSITION
// PIXAR_IMAGEFULLWIDTH
// PIXAR_IMAGEFULLLENGTH
// PIXAR_TEXTUREFORMAT
// PIXAR_WRAPMODES
// PIXAR_FOVCOT
// PIXAR_MATRIX_WORLDTOSCREEN
// PIXAR_MATRIX_WORLDTOCAMERA
// WRITERSERIALNUMBER
// IT8SITE
// IT8COLORSEQUENCE
// IT8HEADER
// IT8RASTERPADDING
// IT8BITSPERRUNLENGTH
// IT8BITSPEREXTENDEDRUNLENGTH
// IT8COLORTABLE
// IT8IMAGECOLORINDICATOR
// IT8BKGCOLORINDICATOR
// IT8IMAGECOLORVALUE
// IT8BKGCOLORVALUE
// IT8PIXELINTENSITYRANGE
// IT8TRANSPARENCYINDICATOR
// IT8COLORCHARACTERIZATION
// IT8HCUSAGE
// IT8TRAPINDICATOR
// IT8CMYKEQUIVALENT
// FRAMECOUNT
// EXIFIFD
// IMAGELAYER
// JBIGOPTIONS
// GPSIFD
// FAXRECVPARAMS
// FAXSUBADDRESS
// FAXRECVTIME
// FAXDCS
// STONITS
// FEDEX_EDR
// INTEROPERABILITYIFD
// DNGVERSION
// DNGBACKWARDVERSION
// UNIQUECAMERAMODEL
// LOCALIZEDCAMERAMODEL
// CFAPLANECOLOR
// CFALAYOUT
// LINEARIZATIONTABLE
// BLACKLEVELREPEATDIM
// BLACKLEVEL
// BLACKLEVELDELTAH
// BLACKLEVELDELTAV
// WHITELEVEL
// DEFAULTSCALE
// DEFAULTCROPORIGIN
// DEFAULTCROPSIZE
// COLORMATRIX1
// COLORMATRIX2
// CAMERACALIBRATION1
// CAMERACALIBRATION2
// REDUCTIONMATRIX1
// REDUCTIONMATRIX2
// ANALOGBALANCE
// ASSHOTNEUTRAL
// ASSHOTWHITEXY
// BASELINEEXPOSURE
// BASELINENOISE
// BASELINESHARPNESS
// BAYERGREENSPLIT
// LINEARRESPONSELIMIT
// CAMERASERIALNUMBER
// LENSINFO
// CHROMABLURRADIUS
// ANTIALIASSTRENGTH
// SHADOWSCALE
// DNGPRIVATEDATA
// MAKERNOTESAFETY
// CALIBRATIONILLUMINANT1
// CALIBRATIONILLUMINANT2
// BESTQUALITYSCALE
// RAWDATAUNIQUEID
// ORIGINALRAWFILENAME
// ORIGINALRAWFILEDATA
// ACTIVEAREA
// MASKEDAREAS
// ASSHOTICCPROFILE
// ASSHOTPREPROFILEMATRIX
// CURRENTICCPROFILE
// CURRENTPREPROFILEMATRIX
// DCSHUESHIFTVALUES
// FAXMODE
// JPEGQUALITY
// JPEGCOLORMODE
// JPEGTABLESMODE
// FAXFILLFUNC
// PIXARLOGDATAFMT
// DCSIMAGERTYPE
// DCSINTERPMODE
// DCSBALANCEARRAY
// DCSCORRECTMATRIX
// DCSGAMMA
// DCSTOESHOULDERPTS
// DCSCALIBRATIONFD
// ZIPQUALITY
// PIXARLOGQUALITY
// DCSCLIPRECTANGLE
// SGILOGDATAFMT
// SGILOGENCODE
// LZMAPRESET
// PERSAMPLE

      tag_type
      getWrappedTag(StringTag1 tag);

    }
  }
}

#endif // OME_BIOFORMATS_TIFF_TAGS_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
