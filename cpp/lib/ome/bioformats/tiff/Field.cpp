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

#include <ome/bioformats/tiff/Field.h>
#include <ome/bioformats/tiff/IFD.h>
#include <ome/bioformats/tiff/Sentry.h>
#include <ome/bioformats/tiff/TIFF.h>

#include <tiffio.h>

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

      /**
       * Internal implementation details of FieldBase.
       */
      class FieldBase::Impl
      {
      public:
        /// Weak reference to the parent IFD.
        std::weak_ptr<IFD>  ifd;
        tag_type            tag;
        const ::TIFFField  *fieldinfo;

        /**
         * Constructor.
         *
         * @param tiff the parent TIFF.
         * @param index the IFD index.
         */
        Impl(std::shared_ptr<IFD>& ifd,
             tag_type              tag):
          ifd(ifd),
          tag(tag),
          fieldinfo()
        {
        }

        /// Destructor.
        ~Impl()
        {
        }

        std::shared_ptr<IFD>
        getIFD() const
        {
          std::shared_ptr<IFD> sifd = std::shared_ptr<IFD>(ifd);
          if (!sifd)
            throw Exception("Field reference to IFD no longer valid");

          return sifd;
        }

        /**
         * @note Needs wrapping in a sentry by the caller.
         */
        ::TIFF *
        getTIFF()
        {
          getIFD()->makeCurrent();
          ::TIFF *tiff = reinterpret_cast<::TIFF *>(getIFD()->getTIFF()->getWrapped());
          return tiff;
        }

        const ::TIFFField *
        getFieldInfo()
        {
          if (!fieldinfo)
            {
              Sentry sentry;

              fieldinfo = TIFFFindField(getTIFF(), tag, TIFF_ANY);
              if (!fieldinfo)
                sentry.error();
            }

          return fieldinfo;
        }
      };

      FieldBase::FieldBase(std::shared_ptr<IFD> ifd,
                           tag_type             tag):
        impl(std::shared_ptr<Impl>(new Impl(ifd, tag)))
      {
      }

      FieldBase::~FieldBase()
      {
      }

      std::string
      FieldBase::name() const
      {
        Sentry sentry;

        const ::TIFFField * field = impl->getFieldInfo();
        const char *name = TIFFFieldName(field);
        return name;
      }

      Type
      FieldBase::type() const
      {
        Sentry sentry;

        const ::TIFFField * field = impl->getFieldInfo();
        Type t = static_cast<Type>(TIFFFieldDataType(field));
        return t;
      }

      bool
      FieldBase::count() const
      {
        Sentry sentry;

        const ::TIFFField * field = impl->getFieldInfo();
        int count = TIFFFieldPassCount(field);
        return (count);
      }

      int
      FieldBase::readCount() const
      {
        Sentry sentry;

        const ::TIFFField * field = impl->getFieldInfo();
        int count = TIFFFieldReadCount(field);
        return count;
      }

      int
      FieldBase::writeCount() const
      {
        Sentry sentry;

        const ::TIFFField * field = impl->getFieldInfo();
        int count = TIFFFieldWriteCount(field);
        return count;
      }

      tag_type
      FieldBase::tagNumber() const
      {
        return impl->tag;
      }

      std::shared_ptr<IFD>
      FieldBase::getIFD() const
      {
        return impl->getIFD();
      }

      template<>
      void
      Field<StringTag1>::get(value_type& value) const
      {
        assert(type == ASCII);
        assert(count() == false);
        assert(readCount() == TIFF_VARIABLE);

        char *text;
        getIFD()->getField(impl->tag, &text);
        value = text;
      }

      template<>
      void
      Field<StringTag1>::set(const value_type& value)
      {
        assert(type == ASCII);
        assert(count() == false);
        assert(writeCount() == TIFF_VARIABLE);

        getIFD()->getField(impl->tag, value.c_str());
      }

    }
  }
}
