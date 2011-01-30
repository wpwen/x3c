/*! \file XComCreator.h
 *  \brief Implement xCreateObject() to use Cx_Interface. XModuleMacro.h and XModuleImpl.h are not needed.
 *
 *  \note This file is used in projects which need not to 
 *        implement interface and want use interface only, 
 *        so XModuleMacro.h and XModuleImpl.h are not need.\n
 *        These projects are usual unit-test applications, COM/Active controls, 
 *        simple applications or non plugin-system applications. \n
 *
 *    Usage: Include this file in a cpp file such as StdAfx.cpp, so you can use Cx_Interface.
 *    (1) Load plugins using PluginManager.h before using those interfaces.\n
 *    (2) or, if just use only one plugin, you can do as following and not need PluginManager.h :
 *    define USE_ONE_PLUGIN then include this file, and assign module handle (using LoadLibrary) to g_hPluginDll.
 *
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2010.10.19
 */

#ifndef USE_ONE_PLUGIN

#include "Ix_ObjectFactory.h"

HRESULT xCreateObject(const XCLSID& clsid, Ix_Object** ppv)
{
    if (NULL == ppv)
        return E_POINTER;
    *ppv = NULL;

    typedef Ix_ObjectFactory* (*FUNC_GET)();
    FUNC_GET pfn = (FUNC_GET)GetProcAddress(
        GetModuleHandleW(L"PluginManagerX3.dll"), "xGetRegisterBank");

    Ix_ObjectFactory* pFactory = pfn ? (*pfn)() : NULL;
    if (NULL == pFactory)
        return E_FAIL;  // plugins must are already loaded using PluginManager.h

    return pFactory->CreateObject(clsid, ppv, NULL);
}

#else // USE_ONE_PLUGIN

// Define USE_ONE_PLUGIN then include this file, and assign module handle (using LoadLibrary) to g_hPluginDll,
// thus xCreateObject() can work and Cx_Interface can be used.

HMODULE     g_hPluginDll = NULL;

HRESULT xCreateObject(const XCLSID& clsid, Ix_Object** ppv)
{
    if (NULL == ppv)
        return E_POINTER;
    *ppv = NULL;

    typedef HRESULT (*FUNC_CREATE)(const char*, Ix_Object**, HMODULE);
    FUNC_CREATE pfn = (FUNC_CREATE)GetProcAddress(
        g_hPluginDll, "_xInternalCreateObject");

    return pfn ? (*pfn)(clsid.str(), ppv, NULL) : E_FAIL;
}

#endif // USE_ONE_PLUGIN

HMODULE xGetModuleHandle()
{
    return NULL;
}
