#include "Bitmap.h"
#include <stdio.h>

using namespace Gdiplus;
using namespace std;
#pragma comment (lib,"Gdiplus.lib")

CBitmap::CBitmap()
{
	m_pBytes = NULL;
	m_nWidth = m_nHeight = 0;
	m_pInfo = NULL;
	m_pBitmap = NULL;
}

CBitmap::~CBitmap(void)
{
}

bool CBitmap::LoadBitmap(string strFileName)
{
	BITMAPFILEHEADER header;

	ifstream theFile;
	theFile.open(strFileName, ios::in | ios::binary);

	if (theFile.is_open()) {
		theFile.read((char*)&header, sizeof(header));

		if (!theFile) {
			theFile.close();
			return false;
		}

		if (header.bfType != 'MB') {
			theFile.close();
			return false;
		}

		int nSize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
		if ((m_pInfo = (BITMAPINFO*)malloc(nSize)) == NULL) {
			theFile.close();
			return false;
		}

		theFile.read((char*)m_pInfo, nSize);
		if (!theFile) {
			free(m_pInfo);
			theFile.close();
			return false;
		}

		m_nWidth = m_pInfo->bmiHeader.biWidth;
		m_nHeight = m_pInfo->bmiHeader.biHeight;

		nSize = m_pInfo->bmiHeader.biWidth * m_pInfo->bmiHeader.biHeight * m_pInfo->bmiHeader.biBitCount / 8;
		m_pBytes = (unsigned char*)malloc(nSize);
		if ((m_pBytes = (unsigned char*)malloc(nSize)) == NULL) {
			free(m_pInfo);
			theFile.close();
			return false;
		}

		theFile.read((char*)m_pBytes, nSize);

		if (!theFile) {
			free(m_pInfo);
			free(m_pBytes);
			theFile.close();
			return false;
		}

		theFile.close();
	}
	return true;
}

bool CBitmap::LoadFromFile(string strFileName)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	// Convert filename to unicode
	int output_size = MultiByteToWideChar(CP_ACP, 0, strFileName.c_str(), -1, NULL, 0);
	LPWSTR wstring = (LPWSTR)malloc(output_size * sizeof(wchar_t));
	int size = MultiByteToWideChar(CP_ACP, 0, strFileName.c_str(), -1, wstring, output_size);

	// Load the bitmapfile
	m_pBitmap = new Bitmap(wstring);
	if (m_pBitmap == NULL) return false;
	Status res = m_pBitmap->GetHBITMAP(Color(255, 255, 255, 255), &phBitmap);

	free(wstring);

	if (res != 0) return false;

	// Get the bitmap data

	::GetObject(phBitmap, sizeof(BITMAP), &bm);
	m_pBytes = (unsigned char*)bm.bmBits;
	m_nWidth = bm.bmWidth;
	m_nHeight = bm.bmHeight;
	m_nBits = bm.bmBitsPixel;

	// Shutdown GDI+

	delete m_pBitmap;

	GdiplusShutdown(gdiplusToken);

	return true;
}

