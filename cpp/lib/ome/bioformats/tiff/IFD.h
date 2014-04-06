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

#ifndef OME_BIOFORMATS_TIFF_IFD_H
#define OME_BIOFORMATS_TIFF_IFD_H

#include <string>

#include <ome/compat/cstdint.h>
#include <ome/compat/memory.h>

#include <ome/bioformats/tiff/Types.h>

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

      class TIFF;

      /**
       * Image File Directory (IFD).
       *
       * An IFD represents a subfile within a TIFF.
       */
      class IFD
      {
      private:
        class Impl;
        /// Private implementation details.
        std::shared_ptr<Impl> impl;

      protected:
        /// Constructor (not public).
        IFD(std::shared_ptr<TIFF>& tiff,
            directory_index_type   index);

        std::shared_ptr<TIFF>
        getTIFF() const;

      private:
        /// Copy constructor (deleted).
        IFD (const IFD&);

        /// Assignment operator (deleted).
        IFD&
        operator= (const IFD&);

      public:
        /// Destructor.
        ~IFD();

        /**
         * Open an IFD.
         *
         * @param tiff the source TIFF.
         * @param index the directory index.
         * @returns the open IFD.
         */
        static std::shared_ptr<IFD>
        open(std::shared_ptr<TIFF>&     tiff,
             directory_index_type index);

        /// @todo get index / offset

        /**
         * Make this IFD the current directory.
         *
         * Internally this is simply a call to TIFFSetDirectory.
         */
        void
        makeCurrent() const;

        /**
         * Get a field by its tag number.
         *
         * @note This should not be used except internally.  Use
         * getField(Field) instead which offers a type-safe interface
         * on top of this lower-level TIFFGetField wrapper.
         *
         * @param tag the tag number.
         * @param ... pointers to variables to store the value(s) in.
         */
        void
        getField(tag_type tag,
                 ...) const;

        /**
         * Set a field by its tag number.
         *
         * @note This should not be used except internally.  Use
         * setField(Field) instead which offers a type-safe interface
         * on top of this lower-level TIFFSetField wrapper.
         *
         * @param tag the tag number.
         * @param ... variables containing the value(s) to set.
         */
        void
        setField(tag_type tag,
                 ...);

      };

    }
  }
}

#endif // OME_BIOFORMATS_TIFF_IFD_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
