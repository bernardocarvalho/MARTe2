/**
 * @file StringHelper.h
 * @brief Header file for module StringHelper
 * @date 30/06/2015
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the module StringHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGHELPER_H_
#define STRINGHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "ErrorManagement.h"
#include "CString.h"
#include "CCString.h"
#include "StaticCString.h"
#include "DynamicCString.h"


/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{762} the functions are declared in the public header file and defined in the header file of the specific architecture.*/
namespace StringHelper {

/**
 * @brief Searches for a character in a string.
 * @param[in] string is the source string.
 * @param[in] c is the character to find in string.
 * @return a pointer to the first occurrence of c in the string, NULL if c is not found.
 */
DLL_API CCString SearchChar(CCString  const string,  char8 c);


/**
 * @brief Returns a pointer to the first occurrence of substring in string.
 * @param[in] string the source string.
 * @param[in] substring the string which must be searched in string.
 * @return a pointer to the first occurrence of substring in string.
 */
DLL_API CCString SearchString(CCString  const string, CCString  const substring);
/**
 * @brief Compares two strings.
 * @param[in] string1 the first string.
 * @param[in] string2 the second string.
 * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings).
 */
DLL_API int32 Compare(CCString  const string1, CCString  const string2);

/**
 * @brief Compares two strings for a maximum of 'size' characters.
 * @param[in] string1 is the first string.
 * @param[in] string2 is the second string.
 * @param[in] size is the maximum number of characters to compare.
 * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings).
 * @pre size <= largest length of the string1 or string2.
 */
DLL_API int32 CompareN(CCString  const string1,CCString  const string2,const uint32 size);

/**
 * @brief Get the token using characters as delimiters.
 * @param[in] string is the string to tokenize.
 * @param[in] delimiterd contains character delimiters.
 * @param[in] skip contains characters not to be included in token
 * @param[in] keepTerm if true moves the char pointer in the output up to the terminator, not up to the next character
 * @param[out] token is the substring between delimiters (including delimeters if not in skip).
 * @return a pointer to the next position after the delimiter for a successive tokenize operation or
 * the end of the string if terminator is not found. It returns NULL if at least one of the
 * arguments is NULL.
 */

DLL_API CCString  Tokenize(CCString  const string, DynamicCString &token, CCString const delimiters, CCString const skip,bool keepTerm=false);

#if 0
/**
 * @brief Get the token using a string as delimiter.
 * @param[in] string is the string to tokenize.
 * @param[in] terminator is the string delimiter.
 * @param[out] result is the next position for a successive operation (NULL if terminator is not found).
 * @return a pointer to the next position after the substring for a successive tokenize operation or NULL if the substring is not found.
 */
DLL_API CCString  TokenizeByString(CCString  const string,CCString  const terminator,CString  const result,uint32 resultStorageSize);
#endif

/** TODO */
DLL_API CCString  TokenizeByString(CCString  const string,CCString  const terminator,DynamicCString & result);

/**
 * @brief Returns the index position of the first character in string2 found in string1 (e.g. "abcde" "12d" returns 3).
 * @param[in] string1 is the source string.
 * @param[in] string2 contains the characters which must be searched in string1.
 * @return the index at the first occurrence of the first character from string2 found in string1.
 * If the stcppring1 or string2 is NULL returns -1.
 * If the string2 is not found in the string1 returns the length of the string1 (the last Index + 1).
 */
DLL_API int32 SearchIndex(CCString  const string1,   CCString  const string2);

#if 0
/**
 * @brief Copies the source into the destination for a maximum of 'size' chars.
 * @warning !! The destination string memory allocation MUST be able to hold the size of source
 * @warning No reallocation will be performed here!!
 * @param[in,out] destination the destination string.
 * @param[in] source the string to copy into the destination.
 * @param[in] size the maximum number of bytes to copy.
 * @return true if the source is copied to the destination.
 * @pre size <= length source
 */
DLL_API bool CopyN(CString destination, CCString source,  uint32 size);
#endif
/**
 * @brief Duplicates a string in the heap memory.
 * @param[in] s The pointer to the string which must be copied.
 * @return The pointer to the new allocated memory which contains a copy of s.
 * TODO all CCString  ---> CCString
 * TODO all CString  CString
 */
//DLL_API DynamicCString StringDup(CCString  const s);
//OBSOLETED use DynamicCString news(s)

/**
 * @brief Concatenates two strings. "destination" + "source"
 * @warning !! The destination string memory allocation MUST be able to hold both strings!!
 * @warning No reallocation will be performed here!!
 * @param[in,out] destination the string where the source will be appended.
 * @param[in] source the string to be appended to the destination.
 * @return true if all parameters were ok
 */
//DLL_API bool Concatenate(CString  const destination,uint32 destinationStorageSize, CCString  const source);

#if 0
template <uint32 sz>
inline bool Concatenate(StaticCString<sz> destination,CCString  const source){
//    return Concatenate(destination.GetList(),sz,source);
    return destination.Append(source);
}

inline bool Concatenate(DynamicCString destination,CCString  const source){
    return destination.Append(source);
}


/**
 * @brief Concatenates two strings until 'size' chars.
 * @warning !! The destination string memory allocation MUST be able to hold the extra amount of characters
 * @warning No reallocation will be performed here!!
 * @param[in,out] destination the string where the source will be appended.
 * @param[in] source the string to be appended to the destination.
 * @param[in] size is the maximum number of bytes to append.
 * @return true if all parameters were ok
 * @pre size <= length of the string source.
 */
DLL_API bool ConcatenateN(CString destination,
                          CCString source,
                          uint32 size);




/**
 * @brief Copies the source into the destination.
 * @warning !! The destination string memory allocation MUST be able to hold the size of source
 * @warning No reallocation will be performed here!!
 * @param[out] destination the destination string.
 * @param[in] source the string to copy into the destination.
 * @return true if the source is copied to the destination.
 */
DLL_API bool Copy(CString  const destination,
                  CCString  const source);


/**
 * @brief Returns the length of a string (not including the terminating \0 character).
 * @param[in] string the source string.
 * @return the length of the string.
 */
DLL_API uint32 Length(CCString const string);

/**
 * @brief Returns the pointer of the first character in string1 matched with one of chars in string2.
 * @param[in] string1 is the source string.
 * @param[in] string2 contains the characters which must be searched in string1.
 * @return a pointer to the first occurrence of the first character from string2 contained in string1.
 */
DLL_API CCString SearchChars(CCString  const string1,
                                 CCString  const string2);

/**
 * @brief Returns a pointer to the last occurrence of the character c in the string.
 * @param[in] string the source string.
 * @param[in] c the character to search.
 * @return a pointer to the last occurrence of c in string.
 */
DLL_API CCString SearchLastChar(CCString  const string,
                                    const char8 c);




/**
 * @brief Gets the substring between two indexes.
 * @param[in] begin is the first index.
 * @param[in] end is the second index.
 * @param[in] string is the string where to search the indexes
 * @param[out] result is the substring.
 * @return true if indexes are valid and the strings are not NULL.
 */
DLL_API bool Substr(const uint32 begin,
                    const uint32 end,
                    CCString  const string,
                    CString  const result);

/**
 * @brief Fills the string with a constant char.
 * @param[in,out] string to be filled with the char.
 * @param[in] size length of the string in bytes.
 * @param[in] c the char to fill the string.
 * @return true if the string is not null and the size > 0 and the operation succeeds.
 */
DLL_API bool SetChar(CString  const string,
                     const uint32 size,
                     const char8 c);




/**
 * @brief Gets the substring between two indexes.
 * @param[in] begin is the first index.
 * @param[in] end is the second index.
 * @param[in] string is the string where to search the indexes
 * @param[out] result is the substring.
 * @return true if indexes are valid and the strings are not NULL.
 */
DLL_API bool Substr(const uint32 begin,
                    const uint32 end,
                    CCString  const string,
                    CString  const result);

/**
 * @brief Concatenates two strings giving result in another string.
 * @warning !! The destination string memory allocation MUST be able to hold the size of string1+string2
 * @warning No reallocation will be performed here!!
 * @param[in] string1 is the string at the beginning.
 * @param[in] string2 is the string to append.
 * @param[out] result is the concatenated string.
 * @return true if strings are not NULL.
 */
DLL_API bool Concatenate(CCString  const string1,
                         CCString  const string2,
                         CString  const result);

/**
 * @brief Appends max 'size' characters of string2 to string1.
 * @warning !! The destination string memory allocation MUST be able to hold the size of string1+string2
 * @warning No reallocation will be performed here!!
 * @param[in] string1 is the string at the beginning.
 * @param[in] string2 is the string to append.
 * @param[out] result is the concatenated string.
 * @param[in] size is the maximum number of characters to append.
 * @return true if strings are not NULL.
 * @pre size <=  length of the string2
 */
DLL_API bool ConcatenateN(CCString  const string1,
                          CCString  const string2,
                          CString  const result,
                          const uint32 size);

#endif
}


}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGHELPER_H_ */

