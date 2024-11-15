#pragma once
// Copyright (c) .NET Foundation and contributors. All rights reserved. Licensed under the Microsoft Reciprocal License. See LICENSE.TXT file in the project root for full license information.


#ifdef __cplusplus
extern "C" {
#endif

#include "dutilsources.h"

#define WIXAPI __stdcall
#ifndef DUTIL_SOURCE_DEFAULT
#define DUTIL_SOURCE_DEFAULT DUTIL_SOURCE_EXTERNAL
#endif

#include "dutil.h"

#define MessageExitOnLastErrorSource(d, x, e, s, ...)      { x = ::GetLastError(); x = HRESULT_FROM_WIN32(x); if (FAILED(x)) { ExitTraceSource(d, x, s, __VA_ARGS__); WcaErrorMessage(e, x, MB_OK, -1, __VA_ARGS__, NULL);  goto LExit; } }
#define MessageExitOnFailureSource(d, x, e, s, ...)           if (FAILED(x)) { ExitTraceSource(d, x, s, __VA_ARGS__); WcaErrorMessage(e, x, INSTALLMESSAGE_ERROR | MB_OK, -1, __VA_ARGS__, NULL);  goto LExit; }
#define MessageExitOnNullWithLastErrorSource(d, p, x, e, s, ...) if (NULL == p) { x = ::GetLastError(); x = HRESULT_FROM_WIN32(x); if (!FAILED(x)) { x = E_FAIL; } ExitTraceSource(d, x, s, __VA_ARGS__); WcaErrorMessage(e, x, MB_OK, -1, __VA_ARGS__, NULL);  goto LExit; }

#define MessageExitOnLastError(x, e, s, ...) MessageExitOnLastErrorSource(DUTIL_SOURCE_DEFAULT, x, e, s, __VA_ARGS__)
#define MessageExitOnFailure(x, e, s, ...) MessageExitOnFailureSource(DUTIL_SOURCE_DEFAULT, x, e, s, __VA_ARGS__)
#define MessageExitOnNullWithLastError(p, x, e, s, ...) MessageExitOnNullWithLastErrorSource(DUTIL_SOURCE_DEFAULT, p, x, e, s, __VA_ARGS__)

// Generic action enum.
typedef enum WCA_ACTION
{
    WCA_ACTION_NONE,
    WCA_ACTION_INSTALL,
    WCA_ACTION_UNINSTALL,
} WCA_ACTION;

typedef enum WCA_CASCRIPT
{
    WCA_CASCRIPT_SCHEDULED,
    WCA_CASCRIPT_ROLLBACK,
} WCA_CASCRIPT;

typedef enum WCA_CASCRIPT_CLOSE
{
    WCA_CASCRIPT_CLOSE_PRESERVE,
    WCA_CASCRIPT_CLOSE_DELETE,
} WCA_CASCRIPT_CLOSE;

typedef enum WCA_TODO
{
    WCA_TODO_UNKNOWN,
    WCA_TODO_INSTALL,
    WCA_TODO_UNINSTALL,
    WCA_TODO_REINSTALL,
} WCA_TODO;

typedef struct WCA_CASCRIPT_STRUCT
{
    LPWSTR pwzScriptPath;
    HANDLE hScriptFile;
} *WCA_CASCRIPT_HANDLE;

typedef enum WCA_ENCODING
{
    WCA_ENCODING_UNKNOWN,
    WCA_ENCODING_UTF_16,
    WCA_ENCODING_UTF_8,
    WCA_ENCODING_ANSI,
} WCA_ENCODING;

void WIXAPI WcaGlobalInitialize(
    __in HINSTANCE hInst
    );
void WIXAPI WcaGlobalFinalize();

HRESULT WIXAPI WcaInitialize(
    __in MSIHANDLE hInstall,
    __in_z PCSTR szCustomActionLogName
    );
UINT WIXAPI WcaFinalize(
    __in UINT iReturnValue
    );
BOOL WIXAPI WcaIsInitialized();

MSIHANDLE WIXAPI WcaGetInstallHandle();
MSIHANDLE WIXAPI WcaGetDatabaseHandle();

const char* WIXAPI WcaGetLogName();

void WIXAPI WcaSetReturnValue(
    __in UINT iReturnValue
    );
BOOL WIXAPI WcaCancelDetected();

#define LOG_BUFFER 2048
typedef enum LOGLEVEL
{
    LOGMSG_TRACEONLY,  // Never written to the log file (except in DEBUG builds)
    LOGMSG_VERBOSE,    // Written to log when LOGVERBOSE
    LOGMSG_STANDARD    // Written to log whenever informational logging is enabled
} LOGLEVEL;

void __cdecl WcaLog(
    __in LOGLEVEL llv,
    __in_z __format_string PCSTR fmt, ...
    );
BOOL WIXAPI WcaDisplayAssert(
    __in LPCSTR sz
    );
void __cdecl WcaLogError(
    __in HRESULT hr,
    __in LPCSTR szMessage,
    ...
    );
void WIXAPI WcaLogErrorArgs(
    __in HRESULT hr,
    __in LPCSTR szMessage,
    __in va_list args
    );

UINT WIXAPI WcaProcessMessage(
    __in INSTALLMESSAGE eMessageType,
    __in MSIHANDLE hRecord
    );
UINT __cdecl WcaErrorMessage(
    __in int iError,
    __in HRESULT hrError,
    __in UINT uiType,
    __in INT cArgs,
    ...
    );
HRESULT WIXAPI WcaProgressMessage(
    __in UINT uiCost,
    __in BOOL fExtendProgressBar
    );

BOOL WIXAPI WcaIsInstalling(
    __in INSTALLSTATE isInstalled,
    __in INSTALLSTATE isAction
    );
BOOL WIXAPI WcaIsReInstalling(
    __in INSTALLSTATE isInstalled,
    __in INSTALLSTATE isAction
    );
BOOL WIXAPI WcaIsUninstalling(
    __in INSTALLSTATE isInstalled,
    __in INSTALLSTATE isAction
    );

HRESULT WIXAPI WcaSetComponentState(
    __in_z LPCWSTR wzComponent,
    __in INSTALLSTATE isState
    );

HRESULT WIXAPI WcaTableExists(
    __in_z LPCWSTR wzTable
    );

HRESULT WIXAPI WcaOpenView(
    __in_z LPCWSTR wzSql,
    __out MSIHANDLE* phView
    );
HRESULT WIXAPI WcaExecuteView(
    __in MSIHANDLE hView,
    __in MSIHANDLE hRec
    );
HRESULT WIXAPI WcaOpenExecuteView(
    __in_z LPCWSTR wzSql,
    __out MSIHANDLE* phView
    );
HRESULT WIXAPI WcaFetchRecord(
    __in MSIHANDLE hView,
    __out MSIHANDLE* phRec
    );
HRESULT WIXAPI WcaFetchSingleRecord(
    __in MSIHANDLE hView,
    __out MSIHANDLE* phRec
    );

HRESULT WIXAPI WcaGetProperty(
    __in_z LPCWSTR wzProperty,
    __inout LPWSTR* ppwzData
    );
HRESULT WIXAPI WcaGetFormattedProperty(
    __in_z LPCWSTR wzProperty,
    __out LPWSTR* ppwzData
    );
HRESULT WIXAPI WcaGetFormattedString(
    __in_z LPCWSTR wzString,
    __out LPWSTR* ppwzData
    );
HRESULT WIXAPI WcaGetIntProperty(
    __in_z LPCWSTR wzProperty,
    __inout int* piData
    );
HRESULT WIXAPI WcaGetTargetPath(
    __in_z LPCWSTR wzFolder,
    __out LPWSTR* ppwzData
    );
HRESULT WIXAPI WcaSetProperty(
    __in_z LPCWSTR wzPropertyName,
    __in_z LPCWSTR wzPropertyValue
    );
HRESULT WIXAPI WcaSetIntProperty(
    __in_z LPCWSTR wzPropertyName,
    __in int nPropertyValue
    );
BOOL WIXAPI WcaIsPropertySet(
    __in LPCSTR szProperty
    );
BOOL WIXAPI WcaIsUnicodePropertySet(
    __in LPCWSTR wzProperty
    );

HRESULT WIXAPI WcaGetRecordInteger(
    __in MSIHANDLE hRec,
    __in UINT uiField,
    __inout int* piData
    );
HRESULT WIXAPI WcaGetRecordString(
    __in MSIHANDLE hRec,
    __in UINT uiField,
    __inout LPWSTR* ppwzData
    );
HRESULT WIXAPI WcaGetRecordFormattedInteger(
    __in MSIHANDLE hRec,
    __in UINT uiField,
    __out int* piData
    );
HRESULT WIXAPI WcaGetRecordFormattedString(
    __in MSIHANDLE hRec,
    __in UINT uiField,
    __inout LPWSTR* ppwzData
    );

HRESULT WIXAPI WcaAllocStream(
    __deref_out_bcount_part(cbData, 0) BYTE** ppbData,
    __in SIZE_T cbData
    );
HRESULT WIXAPI WcaFreeStream(
    __in BYTE* pbData
    );

HRESULT WIXAPI WcaGetRecordStream(
    __in MSIHANDLE hRecBinary,
    __in UINT uiField,
    __deref_out_bcount_full(*pcbData) BYTE** ppbData,
    __out DWORD* pcbData
    );
HRESULT WIXAPI WcaSetRecordString(
    __in MSIHANDLE hRec,
    __in UINT uiField,
    __in_z LPCWSTR wzData
    );
HRESULT WIXAPI WcaSetRecordInteger(
    __in MSIHANDLE hRec,
    __in UINT uiField,
    __in int iValue
    );

HRESULT WIXAPI WcaDoDeferredAction(
    __in_z LPCWSTR wzAction,
    __in_z LPCWSTR wzCustomActionData,
    __in UINT uiCost
    );
DWORD WIXAPI WcaCountOfCustomActionDataRecords(
    __in_z LPCWSTR wzData
    );

HRESULT WIXAPI WcaReadStringFromCaData(
    __deref_in LPWSTR* ppwzCustomActionData,
    __deref_out_z LPWSTR* ppwzString
    );
HRESULT WIXAPI WcaReadIntegerFromCaData(
    __deref_in LPWSTR* ppwzCustomActionData,
    __out int* piResult
    );
HRESULT WIXAPI WcaReadStreamFromCaData(
    __deref_in LPWSTR* ppwzCustomActionData,
    __deref_out_bcount(*pcbData) BYTE** ppbData,
    __out DWORD_PTR* pcbData
    );
HRESULT WIXAPI WcaWriteStringToCaData(
    __in_z LPCWSTR wzString,
    __deref_inout_z LPWSTR* ppwzCustomActionData
    );
HRESULT WIXAPI WcaWriteIntegerToCaData(
    __in int i,
    __deref_out_z_opt LPWSTR* ppwzCustomActionData
    );
HRESULT WIXAPI WcaWriteStreamToCaData(
    __in_bcount(cbData) const BYTE* pbData,
    __in SIZE_T cbData,
    __deref_inout_z_opt LPWSTR* ppwzCustomActionData
    );

HRESULT __cdecl WcaAddTempRecord(
    __inout MSIHANDLE* phTableView,
    __inout MSIHANDLE* phColumns,
    __in_z LPCWSTR wzTable,
    __out_opt MSIDBERROR* pdbError,
    __in UINT uiUniquifyColumn,
    __in UINT cColumns,
    ...
    );

HRESULT WIXAPI WcaDumpTable(
    __in_z LPCWSTR wzTable
    );

HRESULT WIXAPI WcaDeferredActionRequiresReboot();
BOOL WIXAPI WcaDidDeferredActionRequireReboot();

HRESULT WIXAPI WcaCaScriptCreateKey(
    __out LPWSTR* ppwzScriptKey
    );

HRESULT WIXAPI WcaCaScriptCreate(
    __in WCA_ACTION action,
    __in WCA_CASCRIPT script,
    __in BOOL fImpersonated,
    __in_z LPCWSTR wzScriptKey,
    __in BOOL fAppend,
    __out WCA_CASCRIPT_HANDLE* phScript
    );

HRESULT WIXAPI WcaCaScriptOpen(
    __in WCA_ACTION action,
    __in WCA_CASCRIPT script,
    __in BOOL fImpersonated,
    __in_z LPCWSTR wzScriptKey,
    __out WCA_CASCRIPT_HANDLE* phScript
    );

void WIXAPI WcaCaScriptClose(
    __in_opt WCA_CASCRIPT_HANDLE hScript,
    __in WCA_CASCRIPT_CLOSE closeOperation
    );

HRESULT WIXAPI WcaCaScriptReadAsCustomActionData(
    __in WCA_CASCRIPT_HANDLE hScript,
    __out LPWSTR* ppwzCustomActionData
    );

HRESULT WIXAPI WcaCaScriptWriteString(
    __in WCA_CASCRIPT_HANDLE hScript,
    __in_z LPCWSTR wzValue
    );

HRESULT WIXAPI WcaCaScriptWriteNumber(
    __in WCA_CASCRIPT_HANDLE hScript,
    __in DWORD dwValue
    );

void WIXAPI WcaCaScriptFlush(
    __in WCA_CASCRIPT_HANDLE hScript
    );

void WIXAPI WcaCaScriptCleanup(
    __in_z LPCWSTR wzProductCode,
    __in BOOL fImpersonated
    );

HRESULT WIXAPI QuietExec(
    __inout_z LPWSTR wzCommand,
    __in DWORD dwTimeout,
    __in BOOL fLogCommand,
    __in BOOL fLogOutput
    );

HRESULT WIXAPI QuietExecCapture(
    __inout_z LPWSTR wzCommand,
    __in DWORD dwTimeout,
    __in BOOL fLogCommand,
    __in BOOL fLogOutput,
    __out_z_opt LPWSTR* psczOutput
    );

WCA_TODO WIXAPI WcaGetComponentToDo(
    __in_z LPCWSTR wzComponentId
    );

HRESULT WIXAPI WcaExtractBinaryToBuffer(
    __in LPCWSTR wzBinaryId,
    __out BYTE** pbData,
    __out DWORD* pcbData
    );
HRESULT WIXAPI WcaExtractBinaryToFile(
    __in LPCWSTR wzBinaryId,
    __in LPCWSTR wzPath
    );
HRESULT WIXAPI WcaExtractBinaryToString(
    __in LPCWSTR wzBinaryId,
    __deref_out_z LPWSTR* psczOutput,
    __out WCA_ENCODING* encoding
    );

#ifdef __cplusplus
}
#endif
