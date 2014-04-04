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

#include <ome/bioformats/tiff/Types.h>

#include <ome/compat/cstdint.h>
#include <ome/compat/memory.h>

namespace ome
{
  namespace bioformats
  {
    /**
     * TIFF file format (libtiff wrapper).
     */
    namespace tiff
    {

      class IFD;

      /**
       * Tagged Image File Format (TIFF).
       *
       * This class is the primary class for reading and writing TIFF
       * files.  Use the static open() method to get a working
       * instance.  This instance may be used to get IFD instances and
       * then access to image metadata and pixel data.
       */
      class TIFF : public std::enable_shared_from_this<TIFF>
      {
      private:
        class Impl;
        /// Private implementation details.
        std::shared_ptr<Impl> impl;

      protected:
        /// Constructor (non-public).
        TIFF(const std::string& filename,
             const std::string& mode);

      private:
        /// Copy constructor (deleted).
        TIFF (const TIFF&);

        /// Assignment operator (deleted).
        TIFF&
        operator= (const TIFF&);

      public:
        /// Destructor.
        ~TIFF();

        /**
         * Open a TIFF file for reading or writing.
         *
         * @note There are additional open flags, documented in
         * TIFFOpen(3).
         *
         * @param filename the file to open.
         * @param mode the file open mode (@c r to read, @c w to write
         * or @c a to append).
         * @returns the the open TIFF.
         */
        static std::shared_ptr<TIFF>
        open(const std::string& filename,
             const std::string& mode);

        /**
         * Close the TIFF file.
         *
         * Note that this will be done automatically when the
         * destructor runs.  Any further method calls using this
         * object or any child IFD will throw an Exception.
         */
        void
        close();

        /**
         * Check if the TIFF file is valid.
         *
         * @returns @c true if the file is open and available for
         * reading and writing, or @c false if closed or invalid.
         */
        operator bool ();

        /**
         * Get an IFD by its index.
         *
         * @param index the directory index.
         * @returns the IFD.
         * @throws an Exception if the index is invalid or could not
         * be accessed.
         */
        std::shared_ptr<IFD>
        getDirectoryByIndex(directory_index_type index);

        /**
         * Get an IFD by its offset in the file.
         *
         * @param offset the directory offset.
         * @returns the IFD.
         * @throws an Exception if the offset is invalid or could not
         * be accessed.
         */
        std::shared_ptr<IFD>
        getDirectoryByOffset(offset_type offset);
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
