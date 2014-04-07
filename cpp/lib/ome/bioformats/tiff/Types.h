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

#ifndef OME_BIOFORMATS_TIFF_TYPES_H
#define OME_BIOFORMATS_TIFF_TYPES_H

#include <string>

#include <ome/compat/cstdint.h>
#include <ome/compat/memory.h>

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

      /// IFD index.
      typedef uint16_t directory_index_type;

      /// IFD offset.
      typedef uint32_t offset_type;

      /// Tag number.
      typedef unsigned int tag_type;

      enum Type
        {
          NOTYPE = 0,      ///< Placeholder.
          BYTE = 1,        ///< 8-bit unsigned integer.
          ASCII = 2,       ///< 8-bit bytes with NUL terminator.
          SHORT = 3,       ///< 16-bit unsigned integer.
          LONG = 4,        ///< 32-bit unsigned integer.
          RATIONAL = 5,    ///< 64-bit unsigned fraction.
          SBYTE = 6,       ///< 8-bit signed integer.
          UNDEFINED = 7,   ///< 8-bit untyped data.
          SSHORT = 8,      ///< 16-bit signed integer.
          SLONG = 9,       ///< 32-bit signed integer.
          SRATIONAL = 10,  ///< 64-bit signed fraction.
          FLOAT = 11,      ///< 32-bit IEEE floating point.
          DOUBLE = 12,     ///< 64-bit IEEE floating point.
          IFD = 13,        ///< 32-bit unsigned integer (offset).
          LONG8 = 16,      ///< BigTIFF 64-bit unsigned integer.
          SLONG8 = 17,     ///< BigTIFF 64-bit signed integer.
          IFD8 = 18        ///< BigTIFF 64-bit unsigned integer (offset).
        };

    }
  }
}

#endif // OME_BIOFORMATS_TIFF_TYPES_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
