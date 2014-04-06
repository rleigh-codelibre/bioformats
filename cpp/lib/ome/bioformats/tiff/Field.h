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

#include <ome/bioformats/tiff/IFD.h>
#include <ome/bioformats/tiff/Tags.h>

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

      template<typename Tag>
      class Field;

      template<typename T, typename F>
      class FieldBase
      {
      public:
        typedef F field_type;
        typedef T tag_category;
        typedef typename detail::TagProperties<tag_category>::value_type value_type;
        typedef std::weak_ptr<IFD> ifd_weakref;
        typedef FieldBase base_type;

      protected:
        FieldBase(std::shared_ptr<IFD> ifd,
                  tag_category         tag):
          ifd(ifd),
          tag(tag)
        {}

        field_type&
        operator=(const field_type& field)
        {
          set(field);
          return static_cast<field_type>(*this);
        }

        operator value_type()
        {
          value_type v;
          get(v);
          return v;
        }

      private:
        ifd_weakref ifd;
        tag_category tag;
      };

      template<>
      class Field<StringTag1> : public FieldBase<StringTag1, Field<StringTag1> >
      {
      public:
        Field(std::shared_ptr<IFD> ifd,
              tag_category         tag):
          base_type(ifd, tag)
        {}

        ~Field()
        {}

        void
        get(value_type& value) const;

        void
        set(const value_type& value) const;

      };

      // template<typename Field>
      // typename Field::value_type&
      // operator= (typename Field::value_type& value,
      //            Field& field)
      // {
      //   field.get(value);
      //   return value;
      // }

    }
  }
}

#endif // OME_BIOFORMATS_TIFF_FIELD_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
