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

#ifndef OME_BIOFORMATS_TIFF_FIELD_H
#define OME_BIOFORMATS_TIFF_FIELD_H

#include <string>

#include <ome/bioformats/tiff/Tags.h>
#include <ome/bioformats/tiff/Exception.h>

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

      class IFD;

      template<typename Tag>
      class Field
      {
      public:
        typedef Tag tag_category;
        typedef typename detail::TagProperties<tag_category>::value_type value_type;
        typedef std::weak_ptr<IFD> ifd_weakref;

        friend class IFD;

      protected:
        Field(std::shared_ptr<IFD> ifd,
              tag_category         tag):
          ifd(ifd),
          tag(tag)
        {}

        std::shared_ptr<IFD>
        getIFD() const
        {
          std::shared_ptr<IFD> ifd = std::shared_ptr<IFD>(this->ifd);
          if (!ifd)
            throw Exception("Field reference to IFD no longer valid");

          return ifd;
        }

      public:
        virtual ~Field()
        {}

        void
        get(value_type& value) const
        {
          getIFD()->getField(getWrappedTag(tag), &value);
        }

        void
        set(const value_type& value)
        {
          getIFD()->getField(getWrappedTag(tag), value);
        }

        Field&
        operator=(const Field& field)
        {
          set(field);
          return *this;
        }

        operator value_type()
        {
          value_type v;
          get(v);
          return v;
        }

      protected:
        ifd_weakref ifd;
        tag_category tag;
      };

      template<>
      void
      Field<StringTag1>::get(value_type& value) const;

      template<>
      void
      Field<StringTag1>::set(const value_type& value);

      template<typename V>
      class ValueProxy
      {
      public:
        typedef V value_type;

      private:
        V& value;

      public:
        explicit
        ValueProxy(value_type& value):
          value(value)
        {}

        ~ValueProxy()
        {}

        ValueProxy&
        operator= (const ValueProxy& value)
        { this->value = value.value; }

        ValueProxy&
        operator= (const value_type& value)
        {
          this->value = value;
        }

        template<typename F>
        ValueProxy&
        operator= (const Field<F>& field)
        {
          field.get(value);
          return *this;
        }

        value_type&
        get()
        {
          return value;
        }

        const value_type&
        get() const
        {
          return value;
        }
      };

      template<typename V>
      class Value
      {
      public:
        typedef V value_type;

      private:
        V value;

      public:
        explicit
        Value():
          value()
        {}

        ~Value()
        {}

        Value&
        operator= (const Value& value)
        { this->value = value.value; }

        Value&
        operator= (const value_type& value)
        {
          this->value = value;
        }

        template<typename F>
        Value&
        operator= (const Field<F>& field)
        {
          field.get(value);
          return *this;
        }

        value_type&
        get()
        {
          return value;
        }

        const value_type&
        get() const
        {
          return value;
        }
      };

    }
  }
}

#endif // OME_BIOFORMATS_TIFF_FIELD_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
