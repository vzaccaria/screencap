/*******************************************************************************
 * Copyright (c) 2014, Jean-David Gadina - www.xs-labs.com / www.digidna.net
 * Distributed under the Boost Software License, Version 1.0.
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

/*!
 * @header      CFPP-Data.h
 * @copyright   (c) 2014 - Jean-David Gadina - www.xs-labs.com / www.digidna.net
 * @abstract    CoreFoundation++ CFDataRef wrapper
 */

#ifndef CFPP_DATA_H
#define CFPP_DATA_H

namespace CF
{
    class CFPP_EXPORT Data: public PropertyListType< Data >
    {
        public:
            
            #ifdef _WIN32
            
            typedef unsigned __int8 Byte;
            
            #else
            
            typedef uint8_t Byte;
            
            #endif
            
            Data( void );
            Data( const Data & value );
            Data( CFTypeRef value );
            Data( CFDataRef value );
            Data( CFStringRef value );
            Data( std::string value );
            Data( Byte * value, CFIndex length );
            
            #ifdef CFPP_HAS_CPP11
            Data( Data && value );
            #endif
            
            virtual ~Data( void );
            
            Data & operator = ( Data value );
            Data & operator = ( CFTypeRef value );
            Data & operator = ( CFDataRef value );
            Data & operator = ( CFStringRef value );
            Data & operator = ( std::string value );
            
            operator const Byte * () const;
            operator std::string  () const;
            
            Byte operator [] ( int index ) const;
            
            Data & operator += ( Byte value );
            Data & operator += ( CFStringRef value );
            Data & operator += ( CFDataRef value );
            Data & operator += ( std::string value );
            
            virtual CFTypeID  GetTypeID( void ) const;
            virtual CFTypeRef GetCFObject( void ) const;
            
            CFIndex      GetLength( void ) const;
            void         SetLength( CFIndex length ) const;
            void         IncreaseLength( CFIndex extraLength ) const;
            const Byte * GetBytePtr( void ) const;
            Byte       * GetMutableBytePtr( void ) const;
            void         GetBytes( CFRange range, Byte * bytes ) const;
            void         AppendBytes( Byte * bytes, CFIndex length ) const;
            void         ReplaceBytes( CFRange range, Byte * newBytes, CFIndex newLength ) const;
            void         DeleteBytes( CFRange range ) const;
            
            friend void swap( Data & v1, Data & v2 );
            
        private:
            
            CFMutableDataRef _cfObject;
    };
}

#endif /* CFPP_DATA_H */
