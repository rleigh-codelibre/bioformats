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

#ifndef OME_BIOFORMATS_TIFF_TIFF_H
#define OME_BIOFORMATS_TIFF_TIFF_H

#include <string>

#include <ome/compat/cstdint.h>
#include <ome/compat/memory.h>

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

      class IFD;

      class TIFF : public std::enable_shared_from_this<TIFF>
      {

      private:
        class Impl;
        /// Private implementation details.
        std::shared_ptr<Impl> impl;

      protected:
        TIFF(const std::string& filename,
             const std::string& mode);

      private:
        /// Copy constructor (deleted).
        TIFF (const TIFF&);

        /// Assignment operator (deleted).
        TIFF&
        operator= (const TIFF&);

      public:
        ~TIFF();

        static std::shared_ptr<TIFF>
        open(const std::string& filename,
             const std::string& mode);


        void
        close();

        operator bool ();

        typedef uint16_t directory_index_type;
        typedef uint32_t offset_type;

        std::shared_ptr<IFD>
        getDirectoryByIndex(directory_index_type index);

        std::shared_ptr<IFD>
        getDirectoryByOffset(offset_type offset);
      };

      class IFD
      {
      private:
        class Impl;
        /// Private implementation details.
        std::shared_ptr<Impl> impl;

      protected:
        IFD(std::shared_ptr<TIFF>&     tiff,
            TIFF::directory_index_type index);

      private:
        /// Copy constructor (deleted).
        IFD (const IFD&);

        /// Assignment operator (deleted).
        IFD&
        operator= (const IFD&);

      public:
        ~IFD();

        static std::shared_ptr<IFD>
        open(std::shared_ptr<TIFF>&     tiff,
             TIFF::directory_index_type index);
      };

    }
  }
}

#endif // OME_BIOFORMATS_TIFF_TIFF_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
