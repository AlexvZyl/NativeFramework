/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include "resource.h"
#include <iostream>

/*=======================================================================================================================================*/
/* Functions.                                                                                                                            */
/*=======================================================================================================================================*/

// Return the current module for resource loading (from the .exe).
HMODULE getCurrentModule()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)getCurrentModule,
		&hModule);
	return hModule;
}

//bool LoadPNG()
//{
//    HMODULE hInst = NULL;
//    HGLOBAL m_hBuffer = NULL;
//    HRSRC hResource = ::FindResource(hInst, MAKEINTRESOURCE(ARIAL_SDF_PNG), MAKEINTRESOURCE(PNG));
//    // Check if loaded.
//    if (!hResource){ return false; }
//
//    DWORD imageSize = ::SizeofResource(hInst, hResource);
//    if (!imageSize)
//    {
//        return false;
//    }
//    const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
//    if (!pResourceData)
//    {
//        return false;
//    }
//    m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
//    if (m_hBuffer)
//    {
//        void* pBuffer = ::GlobalLock(m_hBuffer);
//
//        if (pBuffer)
//        {
//            CopyMemory(pBuffer, pResourceData, imageSize);
//            HRESULT hr;
//            IStream* pStream = NULL;
//            
//            if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
//            {
//                hr = m_Image_ics.Load(pStream);
//            }
//
//            ::GlobalUnlock(m_hBuffer);
//        }
//
//        ::GlobalFree(m_hBuffer);
//        m_hBuffer = NULL;
//    }
//    return false;
//}

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/