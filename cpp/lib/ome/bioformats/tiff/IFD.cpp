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
#include <cstdarg>

#include <ome/bioformats/tiff/TIFF.h>
#include <ome/bioformats/tiff/IFD.h>
#include <ome/bioformats/tiff/Exception.h>

#include <ome/compat/thread.h>
#include <ome/compat/string.h>

#include <tiffio.h>

using boost::mutex;
using boost::lock_guard;

namespace ome
{
  namespace bioformats
  {
    namespace tiff
    {

      namespace
      {

        class IFDConcrete : public IFD
        {
        public:
          IFDConcrete(std::shared_ptr<TIFF>& tiff,
                      directory_index_type   index):
            IFD(tiff, index)
          {
          }

          virtual
          ~IFDConcrete()
          {
          }
        };

      }

      class IFD::Impl
      {
      public:
        std::weak_ptr<TIFF> tiff;
        directory_index_type index;

        Impl(std::shared_ptr<TIFF>& tiff,
             directory_index_type   index):
          tiff(tiff),
          index(index)
        {
        }

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
      
      IFD::IFD(std::shared_ptr<TIFF>& tiff,
               directory_index_type   index):
        // Note boost::make_shared makes arguments const, so can't use
        // here.
        impl(std::shared_ptr<Impl>(new Impl(tiff, index)))
      {
      }

      IFD::~IFD()
      {
      }

      std::shared_ptr<IFD>
      IFD::open(std::shared_ptr<TIFF>& tiff,
                directory_index_type   index)
      {
        // Note boost::make_shared makes arguments const, so can't use
        // here.
        return std::shared_ptr<IFD>(new IFDConcrete(tiff, index));
      }

    }
  }
}
