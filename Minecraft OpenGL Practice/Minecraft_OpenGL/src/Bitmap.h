#pragma once

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

// -------------------------------------------------------

class CBitmap
{
public:
	unsigned char* m_pBytes;
	int								m_nWidth;
	int								m_nHeight;
	int								m_nBits;
	BITMAPINFO* m_pInfo;
	BITMAP						bm;
	HBITMAP						phBitmap;
	Gdiplus::Bitmap* m_pBitmap;

public:
	CBitmap();
	~CBitmap(void);

	virtual bool LoadBitmap(std::string strFileName);
	virtual bool LoadFromFile(std::string strFileName);
};
