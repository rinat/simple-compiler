/*
* This code is under GNU Lesser General Public License.
* Simple C Compiler to Low Level Virtual Machine representation
*
* Authors: Rinat Shaikhutdinov (aka Sagit) - architecture, compiler core
*          Alexei Zhdanov (aka Alex) - parser
*          Timofey Tatarinov (aka Tikers) - code generator
*
* University Mari State University
* Faculty Informatics and Computer Science
* Course 4
* Specialization PS-41
* Subject Theory of programming languages and translation methods
* Yoshkar-Ola 2010
*/
#ifndef __Compiler_Conversions_h__
#define __Compiler_Conversions_h__

namespace utils
{
    namespace conversions
    {
        template <typename T>
        static std::string NumberToString(T inValue)
        {
            std::ostringstream oStream;
            oStream << inValue;
            return (oStream.str());
        }

        template <bool> struct CompileTimeError;
        template <> struct CompileTimeError<true> {};

        template <class To, class From>
        To safe_static_cast(From from)
        {
            CompileTimeError<(sizeof(from) <= sizeof(To)) != 0>();
            return (static_cast<To>(from));
        }
    }
}

#endif