#include <windows.h>

BOOL M2U(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
  // Get the required size of the buffer that receives the Unicode
  // string.
  DWORD dwMinSize;
  dwMinSize = MultiByteToWideChar (CP_UTF8, 0, lpcszStr, -1, NULL, 0);

  if(dwSize < dwMinSize)
  {
   return FALSE;
  }

  
  // Convert headers from ASCII to Unicode.
  MultiByteToWideChar (CP_UTF8, 0, lpcszStr, -1, lpwszStr, dwMinSize);  
  return TRUE;
}
 