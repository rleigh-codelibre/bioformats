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

#include <algorithm>
#include <cmath>

#include <ome/bioformats/in/TIFFReader.h>

using ome::bioformats::detail::ReaderProperties;
using ome::bioformats::tiff::TIFF;
using ome::bioformats::tiff::IFD;
using ome::bioformats::tiff::tag_type;

namespace ome
{
  namespace bioformats
  {
    namespace in
    {

      namespace
      {

        const char *suffixes[] = {"tif", "tiff", "tf2", "tf8", "btf"};
        const char *companion_suffixes_array[] = {"txt", "xml"};

        ReaderProperties
        tiff_properties()
        {
          ReaderProperties p;

          p.name = "TIFF";
          p.description = "Tagged Image File Format";
          p.suffixes = std::vector<std::string>(suffixes,
                                                suffixes + (sizeof(suffixes) / sizeof(suffixes[0])));
          p.compression_suffixes.push_back("gz");
          p.metadata_levels.insert(MetadataOptions::METADATA_MINIMUM);
          p.metadata_levels.insert(MetadataOptions::METADATA_NO_OVERLAYS);
          p.metadata_levels.insert(MetadataOptions::METADATA_ALL);

          return p;
        }

        const ReaderProperties props(tiff_properties());

        std::vector<std::string> companion_suffixes(companion_suffixes_array,
                                                    companion_suffixes_array + (sizeof(companion_suffixes_array) / sizeof(companion_suffixes_array[0])));

        tag_type imagej(50839);
      }

      TIFFReader::TIFFReader():
        ::ome::bioformats::detail::FormatReader(props)
      {
        domains.push_back("Test domain");
      }

      TIFFReader::~TIFFReader()
      {
      }

      void
      TIFFReader::initFile(const std::string& id)
      {
        ::ome::bioformats::detail::FormatReader::initFile(id);

        tiff = ome::bioformats::tiff::TIFF::open(id, "r");

        // Read metadata from IFDs.
      }

      void
      TIFFReader::openBytes(dimension_size_type no,
                            PixelBufferRaw&     buf) const
      {
        ::ome::bioformats::detail::FormatReader::openBytes(no, buf);
      }

      void
      TIFFReader::openBytes(dimension_size_type /* no */,
                            PixelBufferRaw&     /* buf */,
                            dimension_size_type /* x */,
                            dimension_size_type /* y */,
                            dimension_size_type /* w */,
                            dimension_size_type /* h */) const
      {
        assertId(currentId, true);
        return;
      }

    }
  }
}
