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

#include <ome/bioformats/tiff/TIFF.h>
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

        mutex tiff_lock;

        /**
         * Sentry for saving and restoring libtiff state.
         *
         * This acts primarily to save the state of the global error
         * and warning handlers, which are configured to work with the
         * currently active TIFF/IFD.  The original state is restored
         * when destroyed.
         */
        class Sentry
        {
        private:
          boost::lock_guard<boost::mutex> lock;
          TIFFErrorHandler oldErrorHandler;
          std::string message;

          static Sentry *currentSentry;

          static void
          errorHandler(const char *module,
                       const char *fmt,
                       va_list ap)
          {
            /*
             * Note that using the internal string buffer directly is
             * not standards-compliant until C++11 (not guaranteed
             * contiguous).  However, there are no known
             * implementations where this will fail.
             */

                std::cerr << "FMT: " << fmt << std::endl;
            std::string dest(" ");
            //            vfprintf(stderr, fmt, ap);
            int length = vsnprintf(&dest[0], 1, fmt, ap);
            if (length > 0)
              {
                std::cerr << "BSIZE: " << length<< std::endl;
                dest.resize(length+1, ' ');
                length = vsnprintf(&dest[0], length+1, fmt, ap);
              }
            if (length < 0)
              dest = "Unknown error (error formatting TIFF error message)";

            std::string message(module ? module : "");
            if (!message.empty())
              message += ": ";
            message += dest;

            std::cerr << "ERR TRIGGER: " << message<< std::endl;
            if (currentSentry)
              currentSentry->setMessage(message);
          }

          static TIFFErrorHandler
          setHandler(Sentry           *sentry,
                     TIFFErrorHandler  handler)
          {
            if (sentry)
              currentSentry = sentry;
            TIFFErrorHandler old = TIFFSetErrorHandler(handler);
            if (!sentry)
              currentSentry = sentry;
            return old;
          }

        public:
          Sentry():
            lock(tiff_lock),
            oldErrorHandler(setHandler(this, &Sentry::errorHandler)),
            message()
          {
          }

          ~Sentry()
          {
            setHandler(0, oldErrorHandler);
          }

          void
          setMessage(std::string const& message)
          {
            this->message = message;
          }

          std::string const&
          getMessage() const
          {
            return this->message;
          }

        };

        Sentry *Sentry::currentSentry = 0;

        class TIFFConcrete : public TIFF
        {
        public:
          TIFFConcrete(const std::string& filename,
                       const std::string& mode):
            TIFF(filename, mode)
          {
          }

          virtual
          ~TIFFConcrete()
          {
          }
        };

        class IFDConcrete : public IFD
        {
        public:
          IFDConcrete(std::shared_ptr<TIFF>&     tiff,
                      TIFF::directory_index_type index):
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
        TIFF::directory_index_type index;

        Impl(std::shared_ptr<TIFF>&     tiff,
             TIFF::directory_index_type index):
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

      class TIFF::Impl
      {
      public:
        ::TIFF *tiff;

        Impl(const std::string& filename,
             const std::string& mode):
          tiff()
        {
          Sentry sentry;

          tiff = TIFFOpen(filename.c_str(), mode.c_str());
          if (!tiff)
            throw Exception(sentry.getMessage());
        }

        ~Impl()
        {
          close();
        }

      private:
        /// Copy constructor (deleted).
        Impl (const Impl&);

        /// Assignment operator (deleted).
        Impl&
        operator= (const Impl&);

      public:
        void
        close()
        {
          if (tiff)
            TIFFClose(tiff);
        }
      };

      TIFF::TIFF(const std::string& filename,
                 const std::string& mode):
        impl(std::make_shared<Impl>(filename, mode))
      {
        Sentry sentry;
      }

      TIFF::~TIFF()
      {
      }

      std::shared_ptr<TIFF>
      TIFF::open(const std::string& filename,
                 const std::string& mode)
      {
        return std::make_shared<TIFFConcrete>(filename, mode);
      }

      void
      TIFF::close()
      {
        impl->close();
      }

      TIFF::operator bool ()
      {
        return impl && impl->tiff;
      }

      std::shared_ptr<IFD>
      TIFF::getDirectoryByIndex(directory_index_type index)
      {
        // @todo: Return IFD of correct index.
        std::shared_ptr<TIFF> t(shared_from_this());
        return IFD::open(t, directory_index_type(0U));
      }

      std::shared_ptr<IFD>
      TIFF::getDirectoryByOffset(offset_type offset)
      {
        // @todo: Return IFD of correct index.
        std::shared_ptr<TIFF> t(shared_from_this());
        return IFD::open(t, directory_index_type(0U));
      }


      IFD::IFD(std::shared_ptr<TIFF>&     tiff,
               TIFF::directory_index_type index):
        // Note boost::make_shared makes arguments const, so can't use
        // here.
        impl(std::shared_ptr<Impl>(new Impl(tiff, index)))
      {
      }

      IFD::~IFD()
      {
      }

      std::shared_ptr<IFD>
      IFD::open(std::shared_ptr<TIFF>&     tiff,
                TIFF::directory_index_type index)
      {
        // Note boost::make_shared makes arguments const, so can't use
        // here.
        return std::shared_ptr<IFD>(new IFDConcrete(tiff, index));
      }

    }
  }
}
