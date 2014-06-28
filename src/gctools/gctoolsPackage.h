
#ifndef	gctoolsCando_H
#define gctoolsCando_H

#include "core/common.h"
#include "gctoolsPackage.fwd.h"

NAMESPACE_PACKAGE_ASSOCIATION(gctools,GcToolsPkg,"GCTOOLS")


namespace gctools
{

    extern bool _GlobalDebugAllocations;

    class GcToolsExposer : public core::Exposer
    {
    private:
    public:
        DISABLE_NEW();
    GcToolsExposer(core::Lisp_sp lisp) : Exposer(lisp,GcToolsPkg) {};
        virtual void expose(core::Lisp_sp lisp,WhatToExpose what) const;
    };



class TestingClass
{
 public:
    int x;
    TestingClass() : x(0) {};
    TestingClass(int i) :x(i) {};
    void dump() const { printf("%d ",x);};
    DECLARE_onHeapScanGCRoots();
};
    



    void af_cleanup();

    void initialize_bootstrap_kinds();


};
#endif
