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

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdarg>

#include <ome/bioformats/tiff/TIFF.h>
#include <ome/bioformats/tiff/IFD.h>
#include <ome/bioformats/tiff/Sentry.h>
#include <ome/bioformats/tiff/Exception.h>

#include <ome/compat/thread.h>
#include <ome/compat/string.h>

#include <tiffio.h>

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

      void
      IFD::getField(tag_type tag,
                    ...) const
      {
        std::shared_ptr<TIFF> tiff = getTIFF();
        ::TIFF *tiffraw = reinterpret_cast<::TIFF *>(tiff->getWrapped());

        Sentry sentry;

        makeCurrent();

        va_list ap;
        va_start(ap, tag);

        if (!tag)
          throw Exception("Error getting field: Tag is not valid");

        std::cerr << "GET TAG " << tag << std::endl;

        if (!TIFFVGetField(tiffraw, tag, ap))
          sentry.error();
      }

      void
      IFD::setField(tag_type tag,
                    ...)
      {
        std::shared_ptr<TIFF> tiff = getTIFF();
        ::TIFF *tiffraw = reinterpret_cast<::TIFF *>(tiff->getWrapped());

        Sentry sentry;

        makeCurrent();

        va_list ap;
        va_start(ap, tag);

        if (!tag)
          throw Exception("Error getting field: Tag is not valid");

        if (!TIFFVSetField(tiffraw, tag, ap))
          sentry.error();
      }

      namespace
      {

        class IndexIFDConcrete : public IndexIFD
        {
        public:
          IndexIFDConcrete(std::shared_ptr<TIFF>& tiff,
                           directory_index_type   index):
            IndexIFD(tiff, index)
          {
          }

          virtual
          ~IndexIFDConcrete()
          {
          }
        };

        class OffsetIFDConcrete : public OffsetIFD
        {
        public:
          OffsetIFDConcrete(std::shared_ptr<TIFF>& tiff,
                            offset_type            offset):
            OffsetIFD(tiff, offset)
          {
          }

          virtual
          ~OffsetIFDConcrete()
          {
          }
        };

      }

      /**
       * Internal implementation details of IndexIFD.
       */
      class IndexIFD::Impl
      {
      public:
        /// Weak reference to the parent TIFF.
        std::weak_ptr<TIFF> tiff;
        /// Index of this IFD.
        directory_index_type index;

        /**
         * Constructor.
         *
         * @param tiff the parent TIFF.
         * @param index the IFD index.
         */
        Impl(std::shared_ptr<TIFF>& tiff,
             directory_index_type   index):
          tiff(tiff),
          index(index)
        {
        }

        /// Destructor.
        ~Impl()
        {
        }

      private:
        /// Copy constructor (deleted).
        Impl (const Impl&);

        /// Assignment operator (deleted).
        Impl&
        operator= (const Impl&);

      public:
      };

      IndexIFD::IndexIFD(std::shared_ptr<TIFF>& tiff,
                         directory_index_type   index):
        IFD(),
        // Note boost::make_shared makes arguments const, so can't use
        // here.
        impl(std::shared_ptr<Impl>(new Impl(tiff, index)))
      {
      }

      IndexIFD::~IndexIFD()
      {
      }

      std::shared_ptr<IFD>
      IndexIFD::open(std::shared_ptr<TIFF>& tiff,
                     directory_index_type   index)
      {
        // Note boost::make_shared makes arguments const, so can't use
        // here.
        return std::shared_ptr<IFD>(new IndexIFDConcrete(tiff, index));
      }

      std::shared_ptr<TIFF>
      IndexIFD::getTIFF() const
      {
        std::shared_ptr<TIFF> tiff = std::shared_ptr<TIFF>(impl->tiff);
        if (!tiff)
          throw Exception("IFD reference to TIFF no longer valid");

        return tiff;
      }

      void
      IndexIFD::makeCurrent() const
      {
        std::shared_ptr<TIFF> tiff = getTIFF();
        ::TIFF *tiffraw = reinterpret_cast<::TIFF *>(tiff->getWrapped());

        Sentry sentry;

        if (!TIFFSetDirectory(tiffraw, impl->index))
          sentry.error();
      }

      /**
       * Internal implementation details of OffsetIFD.
       */
      class OffsetIFD::Impl
      {
      public:
        /// Weak reference to the parent TIFF.
        std::weak_ptr<TIFF> tiff;
        /// Offset of this IFD.
        offset_type offset;

        /**
         * Constructor.
         *
         * @param tiff the parent TIFF.
         * @param offset the IFD offset.
         */
        Impl(std::shared_ptr<TIFF>& tiff,
             offset_type            offset):
          tiff(tiff),
          offset(offset)
        {
        }

        /// Destructor.
        ~Impl()
        {
        }

      private:
        /// Copy constructor (deleted).
        Impl (const Impl&);

        /// Assignment operator (deleted).
        Impl&
        operator= (const Impl&);

      public:
      };

      OffsetIFD::OffsetIFD(std::shared_ptr<TIFF>& tiff,
                           offset_type            offset):
        IFD(),
        // Note boost::make_shared makes arguments const, so can't use
        // here.
        impl(std::shared_ptr<Impl>(new Impl(tiff, offset)))
      {
      }

      OffsetIFD::~OffsetIFD()
      {
      }

      std::shared_ptr<IFD>
      OffsetIFD::open(std::shared_ptr<TIFF>& tiff,
                     offset_type             offset)
      {
        // Note boost::make_shared makes arguments const, so can't use
        // here.
        return std::shared_ptr<IFD>(new OffsetIFDConcrete(tiff, offset));
      }

      std::shared_ptr<TIFF>
      OffsetIFD::getTIFF() const
      {
        std::shared_ptr<TIFF> tiff = std::shared_ptr<TIFF>(impl->tiff);
        if (!tiff)
          throw Exception("IFD reference to TIFF no longer valid");

        return tiff;
      }

      void
      OffsetIFD::makeCurrent() const
      {
        std::shared_ptr<TIFF> tiff = getTIFF();
        ::TIFF *tiffraw = reinterpret_cast<::TIFF *>(tiff->getWrapped());

        Sentry sentry;

        if (!TIFFSetSubDirectory(tiffraw, impl->offset))
          sentry.error();
      }

    }
  }
}
