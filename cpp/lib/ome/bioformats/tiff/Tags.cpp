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

    }
  }
}
