// Disable this once we have List_sp working
#define USE_BAD_CAST_ERROR 1
//Turn these on only if x.nilp() are found in ASSERT(...) statements
#define ALLOW_NIL_OTHER 1
//#define ALLOW_CONS_NIL 1

/*
  File: smart_pointers.h
*/

/*
  Copyright (c) 2014, Christian E. Schafmeister
 
  CLASP is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  See directory 'clasp/licenses' for full details.
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
/* -^- */

//
// (C) 2004 Christian E. Schafmeister
//

#ifndef _core__pointer_tagging_H
#define _core__pointer_tagging_H

#include <boost/utility/binary.hpp>

#include <iostream>
#include <cstring>

#include <clasp/gctools/globals.h>

//#include "tagged_ptr.h"
//#define TAGGED_PTR_BASE tagged_ptr

//#define	IsUndefined(x) (x)
//#define	NotUndefined(x) (!(x))

//#define	_FWPLock(x)	(x)

//#define	TAGGED_PTR core::T_O*

extern void lisp_errorUnexpectedNil(type_info const &toType);
extern void lisp_errorBadCast(type_info const &toType, type_info const &fromType, core::T_O *objP);
extern void lisp_errorBadCastFromT_O(type_info const &toType, core::T_O *objP);
extern void lisp_errorBadCastToFixnum_O(type_info const &fromType, core::T_O *objP);
extern void lisp_errorBadCastFromT_OToCons_O(core::T_O *objP);
extern void lisp_errorBadCastFromSymbol_O(type_info const &toType, core::Symbol_O *objP);
extern void lisp_errorDereferencedNonPointer(core::T_O *objP);

namespace core {
  class VaList_S;
  class Code_S;
};

namespace gctools {

//    typedef core::T_O Fixnum_ty;

  template <class T>
    class smart_ptr;

  void initialize_smart_pointers();

#ifdef _ADDRESS_MODEL_64
  static const uintptr_t alignment = 8;    // 16 byte alignment for all pointers
  static const uintptr_t pointer_size = 8; // 8 byte words 64-bits
#if defined (CLASP_MS_WINDOWS_HOST)
#error "Define a 64bit Fixnum for windows"
#else
                                        //! Fixnum definition for 64 bit system
  typedef intptr_t Fixnum;
#endif
 
  typedef Fixnum cl_fixnum;
};

typedef gctools::Fixnum cl_index;

namespace gctools {
/*! A pointer that is already tagged can be passed to smart_ptr constructors
      by first reinterpret_casting it to Tagged */
  typedef uintptr_t Tagged;
  typedef uintptr_t TaggedVaList; // Used in situations where only a tagged VaList_S ptr is accepted
  static const int tag_shift = 3;
  static const int fixnum_bits = 62;
  static const int fixnum_shift = 2;
  static const long int most_positive_fixnum =  2305843009213693951;
  static const long int most_negative_fixnum = -2305843009213693952;
  static const size_t thread_local_cl_stack_min_size = THREAD_LOCAL_CL_STACK_MIN_SIZE;
  static const int most_positive_int = std::numeric_limits<int>::max();
  static const int most_negative_int = std::numeric_limits<int>::min();
  static const uint most_positive_uint = std::numeric_limits<unsigned int>::max();
  static const uint64_t most_positive_uint32 = std::numeric_limits<uint32_t>::max();
  static const uint64_t most_positive_uint64 = std::numeric_limits<uint64_t>::max();
  static const unsigned long long most_positive_unsigned_long_long = std::numeric_limits<unsigned long long>::max();
#define MOST_POSITIVE_FIXNUM gctools::most_positive_fixnum
#define MOST_NEGATIVE_FIXNUM gctools::most_negative_fixnum
#define FIXNUM_BITS gctools::fixnum_bits
#endif
#ifdef _ADDRESS_MODEL_32
#error "Add support for 32 bits - squeeze, Squeeze, SQUEEZE!"
#endif
/*! Pointer and immediate value tagging is set up here */
/* FIXNUM's have the lsb x00 set to zero - this allows addition and comparison to be fast */
/* The rest of the bits are the fixnum */
  static const uintptr_t tag_mask    = ZERO_TAG_MASK; // BOOST_BINARY(111);
  static const uintptr_t fixnum_tag  = BOOST_BINARY(00); // x00 means fixnum
  static const uintptr_t fixnum1_tag  = BOOST_BINARY(100); // x100 means fixnum odd
  static const uintptr_t fixnum_mask = BOOST_BINARY(11);
/*! The pointer tags, that point to objects that the GC manages are general_tag and cons_tag
Robert Strandh suggested a separate tag for CONS cells so that there would be a quick CONSP test
for a CONS cell*/
  static const uintptr_t ptr_mask    = ~ZERO_TAG_MASK;
  static const uintptr_t general_tag =  POINTER_GENERAL_TAG;  // means a GENERAL pointer
  static const uintptr_t cons_tag    =  POINTER_CONS_TAG;     // means a CONS cell pointer
  /*! A test for pointers has the form (potential_ptr&POINTER_TAG_MASK)==POINTER_TAG_EQ) */
  static const uintptr_t pointer_tag_mask = POINTER_TAG_MASK; 
  static const uintptr_t pointer_tag_eq   = POINTER_TAG_EQ;

 /*! gc_tag is used for headerless objects to indicate that this word is
used by the garbage collector */
  static const uintptr_t gc_tag = ZERO_TAG_MASK; //BOOST_BINARY(111);
 
/*! valist_tag is a tag for va_list(s) on the stack, it is used by Clasp to 
iterate over variable numbers of arguments passed to functions.
Pointers with this tag are NOT moved in memory, the objects valist_tag'd pointers
point to are only ever on the stack.
I hack the va_list structure in X86_64 ABI dependent ways and I will abstract all of the
ABI dependent behavior into a single header file so that it can be implemented for other
ABI's  */
  static const uintptr_t valist_tag = BOOST_BINARY(101); // means a valist
                                                       /*! Immediate value tags */
  static const uintptr_t immediate_mask   = BOOST_BINARY(111);
  static const uintptr_t character_tag    = BOOST_BINARY(010); // Character
  static const uintptr_t character_shift  = 3;
  static const uintptr_t single_float_tag = BOOST_BINARY(110); // single-float
  static const uintptr_t single_float_shift = 3;
  static const uintptr_t single_float_mask = 0x1FFFFFFFFF; // single-floats are in these 32+5bits

  struct Immediate_info {
    uintptr_t _kind;
    const char* _name;
  Immediate_info(uintptr_t k, const char* n) : _kind(k), _name(n) {};
  };

  std::vector<Immediate_info> get_immediate_info();

  /* These values define the Stamp ranges for different kinds of
     objects.  There are the following kinds of objects:
       Special objects that don't have headers (fixnum, single_float, character, cons)
       General objects that are stored on the heap and have a header
       Alien objects that are wrapped C++ classes and stored on the heap.
       Instance objects that are CLOS instances. 
       The ranges below define the kind/stamp values allowed for each
       and they allow the GC to quickly determine what kind of an object
       it is dealing with for fixing pointers.
       If the ranges aren't sufficiently large (general or alien)
       then move the numbers around.
       We need at least 62 bits to represent general CLOS objects.  */
  static const uintptr_t kind_fixnum = 1;
  static const uintptr_t kind_single_float = 2;
  static const uintptr_t kind_character = 3;
  static const uintptr_t kind_cons = 4;
  static const uintptr_t kind_va_list_s = 5;
  static const uintptr_t kind_first_general  = 6;
  static const uintptr_t kind_last_general   = 4095;
  static const uintptr_t kind_first_alien    = 4096;
  static const uintptr_t kind_last_alien     = 65535;
  static const uintptr_t kind_first_instance = 65536;
  static const uintptr_t kind_last_instance  = ((uintptr_t)most_positive_fixnum)<<1;
  
  template <class T>
    T tag(T ptr) { return reinterpret_cast<T>(reinterpret_cast<uintptr_t>(ptr) & tag_mask); };

  template <class T>
    inline bool tagged_consp(T ptr) {
    return (reinterpret_cast<uintptr_t>(tag(ptr)) == cons_tag);
  };

  template <class T>
    inline T tag_cons(T p) {
    GCTOOLS_ASSERT((reinterpret_cast<uintptr_t>(p) & tag_mask) == 0);
    return reinterpret_cast<T>(reinterpret_cast<uintptr_t>(p) + cons_tag);
  }

  template <class T>
    inline T untag_cons(T ptr) {
    GCTOOLS_ASSERT((reinterpret_cast<uintptr_t>(ptr) & tag_mask) == cons_tag);
    return reinterpret_cast<T>(reinterpret_cast<uintptr_t>(ptr) - cons_tag);
  }

  template <class T>
    inline T tag_general(T p) {
    GCTOOLS_ASSERT((reinterpret_cast<uintptr_t>(p) & tag_mask) == 0);
    return reinterpret_cast<T>(reinterpret_cast<uintptr_t>(p) + general_tag);
  }

  template <class T>
    inline T tag_object(T ptr) {
    return tag_general<T>(ptr);
  }
  template <>
    inline core::Cons_O *tag_object<core::Cons_O *>(core::Cons_O *ptr) {
    return tag_cons<core::Cons_O *>(ptr);
  }

  template <class T>
    inline bool tagged_nilp(T ptr) {
    return (reinterpret_cast<void *>(ptr) == global_tagged_Symbol_OP_nil);
  }
  template <class T>
    inline bool tagged_unboundp(T ptr) {
    return (reinterpret_cast<void *>(ptr) == global_tagged_Symbol_OP_unbound);
  }
  template <class T>
    inline bool tagged_deletedp(T ptr) {
    return (reinterpret_cast<void *>(ptr) == global_tagged_Symbol_OP_deleted);
  }
  template <class T>
    inline bool tagged_sameAsKeyp(T ptr) {
    return (reinterpret_cast<void *>(ptr) == global_tagged_Symbol_OP_sameAsKey);
  }

  template <class T>
    inline T tag_nil() {
    GCTOOLS_ASSERT(tagged_nilp(global_tagged_Symbol_OP_nil));
    return reinterpret_cast<T>(global_tagged_Symbol_OP_nil);
  }
  template <class T>
    inline T tag_unbound() {
    GCTOOLS_ASSERT(tagged_unboundp(global_tagged_Symbol_OP_unbound));
    return reinterpret_cast<T>(global_tagged_Symbol_OP_unbound);
  }
  template <class T>
    inline T tag_deleted() {
    GCTOOLS_ASSERT(tagged_deletedp(global_tagged_Symbol_OP_deleted));
    return reinterpret_cast<T>(global_tagged_Symbol_OP_deleted);
  }
  template <class T>
    inline T tag_sameAsKey() {
    GCTOOLS_ASSERT(tagged_sameAsKeyp(global_tagged_Symbol_OP_sameAsKey));
    return reinterpret_cast<T>(global_tagged_Symbol_OP_sameAsKey);
  }
  template <class T>
    inline T tag_valist(core::VaList_S *p) {
    GCTOOLS_ASSERT((reinterpret_cast<uintptr_t>(p) & tag_mask) == 0);
    return reinterpret_cast<T>(reinterpret_cast<uintptr_t>(p) + valist_tag);
  }


  template <class T>
    inline T untag_general(T ptr) {
    GCTOOLS_ASSERT((reinterpret_cast<uintptr_t>(ptr) & tag_mask) == general_tag);
    return reinterpret_cast<T>(reinterpret_cast<uintptr_t>(ptr) - general_tag);
  }
  template <class T>
    inline void *untag_valist(T ptr) {
    GCTOOLS_ASSERT((reinterpret_cast<uintptr_t>(ptr) & tag_mask) == valist_tag);
    return reinterpret_cast<core::VaList_S*>(reinterpret_cast<uintptr_t>(ptr) - valist_tag);
  }


  template <class T>
    inline T tag_fixnum(Fixnum fn) {
    return reinterpret_cast<T>((fn << fixnum_shift));
  }
  template <class T>
    inline Fixnum untag_fixnum(T const ptr) {
    GCTOOLS_ASSERT((reinterpret_cast<uintptr_t>(ptr) & fixnum_mask) == 0);
    return (Fixnum)(reinterpret_cast<Fixnum>(ptr) >> fixnum_shift);
  }
  template <class T>
    inline bool tagged_fixnump(T ptr) {
    return ((reinterpret_cast<uintptr_t>(ptr) & fixnum_mask) == fixnum_tag);
  };
  template <class T>
    inline T tag_character(int ch) {
    return reinterpret_cast<T>((ch << character_shift) | character_tag);
  }
  template <class T>
    inline int untag_character(T ptr) {
    GCTOOLS_ASSERT((reinterpret_cast<uintptr_t>(ptr) & immediate_mask) == character_tag);
    return (int)(reinterpret_cast<uintptr_t>(ptr) >> character_shift);
  }
  template <class T>
    inline bool tagged_characterp(T ptr) {
    return ((reinterpret_cast<uintptr_t>(ptr) & immediate_mask) == character_tag);
  };
  template <class T>
    inline T tag_single_float(float fn) {
    GCTOOLS_ASSERT(sizeof(uintptr_t) == 8);
    GCTOOLS_ASSERT(sizeof(float) == 4);
    uintptr_t val;
    memcpy(&val, &fn, sizeof(fn));
    return reinterpret_cast<T>((val << single_float_shift) + single_float_tag);
  }
  template <class T>
    inline uintptr_t tagged_single_float_masked(T const ptr) {
    return reinterpret_cast<uintptr_t>(reinterpret_cast<uintptr_t>(ptr) & single_float_mask);
  }
  template <class T>
    inline float untag_single_float(T const ptr) {
    GCTOOLS_ASSERT((reinterpret_cast<uintptr_t>(ptr) & immediate_mask) == single_float_tag);
    GCTOOLS_ASSERT(sizeof(uintptr_t) == 8);
    GCTOOLS_ASSERT(sizeof(float) == 4);
    uintptr_t val(reinterpret_cast<uintptr_t>(ptr));
    float result;
    val >>= single_float_shift;
    memcpy(&result, &val, sizeof(result));
    return result;
  }
  template <class T>
    inline bool tagged_single_floatp(T ptr) {
    return ((reinterpret_cast<uintptr_t>(ptr) & immediate_mask) == single_float_tag);
  };

  template <class T>
    inline bool tagged_generalp(T ptr) {
    return ((uintptr_t)(ptr) & tag_mask) == general_tag;
  }
  template <class T>
    inline bool tagged_valistp(T ptr) {
    return ((reinterpret_cast<uintptr_t>(ptr) & tag_mask) == valist_tag);
  };

  template <class T>
    inline bool tagged_objectp(T ptr) {
    return (reinterpret_cast<uintptr_t>(ptr) & pointer_tag_mask) == pointer_tag_eq;
  }

   
  template <class Type>
    inline Type untag_object(Type tagged_obj) {
    GCTOOLS_ASSERT(tagged_objectp(tagged_obj));
    return reinterpret_cast<Type>((uintptr_t)tagged_obj & ptr_mask);
  }
};

#endif // pointer_tagging
