#include <Windows.h>
#include <GdiPlus.h>
#include "smith.h"
#include <stdio.h>

using namespace Gdiplus;

SMITHCALLS *smith = nullptr;


void DoTextureExperiment(double dt, double truedt)
{
	// for one, we'll use a GDI+ bitmap and do some drawing and replace the texture on kyle's back...
	if(true)
	{
		Bitmap* bmp = new Bitmap(256, 128);
		Graphics* gfx = Graphics::FromImage(bmp);

		SolidBrush brsh(Color::Black);
		gfx->FillRectangle(&brsh, Rect(0, 0, bmp->GetWidth(), bmp->GetHeight()));

		Pen pen(Color::White, 3.0f);
		gfx->DrawEllipse(&pen, Rect(30, 30, 10, 10));

		delete gfx;

		HBITMAP hBitmap;
		bmp->GetHBITMAP(Color::Black, &hBitmap);
		smith->GenerateMaterialBitmap("kybodyb.mat", "dflt.cmp", 0, false, hBitmap, NULL);
		DeleteObject(hBitmap);

		delete bmp;
	}


	// next, we'll take a pixeldata buffer and use that directly
	if(true)
	{
		int texWidth = 128;
		int texHeight = 128;
		int texDepth = 24;
		int stride = texWidth * (texDepth / 8);

		unsigned char* pBits = new unsigned char[stride*texHeight];

		for (int y = 0; y < texHeight; y++)
		{
			unsigned char* pDst = pBits + stride * y;
			for (int x = 0; x < texWidth; x++)
			{
				pDst[0] = rand()%128;			// b
				pDst[1] = rand()%255;			// g
				pDst[2] = 0;	// r

				pDst += 3;
			}
		}

		smith->GenerateMaterial("kybutt.mat", "dflt.cmp", 0, texWidth, texHeight, texDepth, stride, pBits, nullptr);

		delete[] pBits;
	}


}





extern "C" {

	void __cdecl OnMainLoop(double dt, double truedt)
	{
		if (smith == nullptr)
			return;


	}

	void __cdecl OnPrepareMainRender(double dt, double truedt)
	{
		if (smith == nullptr)
			return;

		DoTextureExperiment(dt, truedt);
	}

	void __cdecl OnLevelShutdownPreObject()
	{
		if (smith == nullptr)
			return;



	}

	void __cdecl OnLevelShutdownPostObject()
	{
		if (smith == nullptr)
			return;


	}

	void __cdecl OnLevelStartPreObject()
	{
		if (smith == nullptr)
			return;

		//smith->ExecuteCOG("PlaySoundLocal(\"activate02.wav\", 1.0, 0.0, 0x00);", 0, 0, 0, 0, 0, 0, 0, 0);
	}

	void __cdecl OnLevelStartPostObject()
	{
		if (smith == nullptr)
			return;


		//char masterColormap[16] = { 0 };
		//smith->ExecuteCOG("returnex(getsectorcolormap(0));", 0, 0, 0, 0, 0, 0, 0, masterColormap);
		//MessageBox(0, masterColormap, 0, 0);



		//char playerModel[16];
		//smith->ExecuteCOG("GetThingModel(GetLocalPlayerThing());", 0, 0, 0, 0, 0, 0, 0, playerModel);

	}

	int __cdecl OnExecuteCOGVerb(const char* szName, int nNumParams, const char** pszParams, char* szReturn)
	{
		//if (stricmp(szName, "PluginTest") == 0 && nNumParams == 1)
		//{
		//	MessageBox(0, pszParams[0], "SmithPlugin", MB_ICONINFORMATION);
		//	return 1;// handled
		//}

		return 0;// not handled;  let smith deal with it
	}

	int _smith_formatversion(int major, int minor)
	{
		return major * 100 + minor;
	}

	int __cdecl SmithQueryPlugin(PLUGININFO& p)
	{
		// fill out the details
		strcpy(p.name, "Sample Plugin");
		strcpy(p.author, "bahstrike");
		strcpy(p.authorEmail, "strike@bah.wtf");
		strcpy(p.attributions, "BAH_Strike&BAH Main Page https://bah.wtf|Leethaxxor&Leet haxxin support https://github.com/blahblah   This dude did stuff.|Bobs Tacos&Excellent tacos. Kept us goin|Homie A|Homie B");
		strcpy(p.desc, "mostly an example plugin project. contains demonstrations of things possible with smith engine mod development");
		strcpy(p.homepageURL, "https://github.com/bahstrike/SmithPlugin");
		p.ver = _smith_formatversion(1, 0);
		p.smithRequiredVer = SMITHVERSION;

		//p.authoritykey   // leave alone if u do not have agreed-upon algorithm

		return 1337;
	}

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken = 0;

	int __cdecl InitializePlugin(SMITHCALLS* _smith)
	{
		smith = _smith;

		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		

		return 1;
	}

	void __cdecl ShutdownPlugin()
	{



		if (gdiplusToken != 0)
		{
			GdiplusShutdown(gdiplusToken);
			gdiplusToken = 0;
		}

		// our smith interface is no longer valid
		smith = nullptr;
	}

}