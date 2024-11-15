#pragma once
// Copyright (c) .NET Foundation and contributors. All rights reserved. Licensed under the Microsoft Reciprocal License. See LICENSE.TXT file in the project root for full license information.


#define ExitOnGdipFailureSource(d, g, x, s, ...) { x = GdipHresultFromStatus(g); if (FAILED(x)) { Dutil_RootFailure(__FILE__, __LINE__, x); ExitTraceSource(d, x, s, __VA_ARGS__); goto LExit; } }
#define ExitOnGdipFailure(g, x, s, ...) ExitOnGdipFailureSource(DUTIL_SOURCE_DEFAULT, g, x, s, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

HRESULT DAPI GdipInitialize(
    __in const Gdiplus::GdiplusStartupInput* pInput,
    __out ULONG_PTR* pToken,
    __out_opt Gdiplus::GdiplusStartupOutput *pOutput
    );

void DAPI GdipUninitialize(
    __in ULONG_PTR token
    );

HRESULT DAPI GdipBitmapFromResource(
    __in_opt HINSTANCE hinst,
    __in_z LPCSTR szId,
    __out Gdiplus::Bitmap **ppBitmap
    );

HRESULT DAPI GdipBitmapFromFile(
    __in_z LPCWSTR wzFileName,
    __out Gdiplus::Bitmap **ppBitmap
    );

HRESULT DAPI GdipBitmapToGdiBitmap(
    __in Gdiplus::Bitmap* pBitmap,
    __out HBITMAP* phBitmap
    );

HRESULT DAPI GdipHresultFromStatus(
    __in Gdiplus::Status gs
    );

#ifdef __cplusplus
}
#endif
