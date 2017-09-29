/*
    File: gc_interface.h
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
#ifndef GC_INTERFACE_H
#define GC_INTERFACE_H

//
// All class forward declarations
//
namespace core {
class T_O;
class WrappedPointer_O;
class Function_O;
class Creator_O;
class Iterator_O;
class SequenceStepper_O;
};
namespace asttooling {
namespace internal {
  class MatcherDescriptor_O;
};
};
namespace clbind {
class ConstructorCreator_O;
};


//////////////////////////////////////////////////////////////////////
//
// Forward declarations
//
//
namespace clbind {
  namespace detail {
    class class_map;
  }
}
namespace core {
  class KeywordArgument;
  class RequiredArgument;
  class SymbolClassPair;
  class SymbolStorage;
  class TranslationFunctor;
  class DynamicBinding;
  class AuxArgument;
  class OptionalArgument;
  class CacheRecord;
  class ExceptionEntry;
};
namespace asttooling {
  class DerivableSyntaxOnlyAction;
  class DerivableASTFrontendAction;
  class DerivableMatchCallback;
  class DerivableFrontendActionFactory;
};


#ifdef BUILD_EXTENSION
#define GC_INTERFACE_FORWARD
#include <project_headers.h>
#undef GC_INTERFACE_FORWARD

#define GC_INTERFACE_GC_MANAGED_TYPES
#include <project_headers.h>
#undef GC_INTERFACE_GC_MANAGED_TYPES
#endif

#if 1
 #ifndef SCRAPING
 #define DECLARE_FORWARDS
 #include INIT_CLASSES_INC_H // REPLACED CLASP_GC_FILENAME // "main/clasp_gc.cc"
 #undef DECLARE_FORWARDS
#endif
#else
#ifdef USE_MPS
#ifndef RUNNING_GC_BUILDER // when running the static analyzer - don't include the following
#define DECLARE_FORWARDS
#include CLASP_GC_FILENAME
#undef DECLARE_FORWARDS
#endif // ifndef RUNNING_GC_BUILDER
#endif
#ifdef USE_BOEHM
#ifdef USE_CXX_DYNAMIC_CAST
// nothing
#else
#define DECLARE_FORWARDS
#include CLASP_GC_FILENAME
#undef DECLARE_FORWARDS
#endif
#endif
#endif

namespace gctools {

#ifndef SCRAPING
#define GC_STAMP_SELECTORS
#include INIT_CLASSES_INC_H // REPLACED CLASP_GC_FILENAME
#undef GC_STAMP_SELECTORS
#endif

};

#include <clasp/gctools/other_tagged_casts.h>

extern "C" {
const char *obj_name(gctools::stamp_t kind);
extern void obj_dump_base(void *base);
extern void obj_deallocate_unmanaged_instance(gctools::smart_ptr<core::T_O> obj);

extern int global_symbol_count;
extern gctools::smart_ptr<core::Symbol_O> global_symbols[];

extern void client_validate_internal(void* tagged_client);
extern void client_validate_recursive(void* tagged_client, std::set<void*>& seen );

};


void initialize_clasp_Kinds();
void initialize_clasp();

void initialize_functions();
void initialize_source_info();
void initialize_classes_and_methods();
core::T_sp generate_type_header_value_map();

#endif
