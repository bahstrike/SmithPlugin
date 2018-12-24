#include <Windows.h>
#include <GdiPlus.h>
#include "smith.h"
#include <stdio.h>

using namespace Gdiplus;

SMITHCALLS *smith = nullptr;

/*
================================== =
Engine Hooks For AArcade FE Support
================================== =

CreateProceduralTexture(texture_id, width, height, format, pixelData) :
	-Creates a texture with the given ID using the given pixel data.
	- Returns a POINTER to the texture that AArcade will ONLY write to during
	- Called by AArcade to create a texture that is going to be used on an object later on.
	- The

	CreateProceduralMaterial(material_id) :
	-Called by AArcade to setup the engine to give OnMaterialBind callbacks related to the material_id.
	- This material_id will match a material channel used on the dynamic object models.AArcade dynamic object models are created using material_id's that AArcade is expected to register with the engine.  These include, but are not limited to: DynMarquee, DynMarqueeGlow, DynScreen, DynScreenGlow, DynScreenGlowHologram.
	- The engine needs to notify AArcade when objects are being rendered that use this material_id.

	OnMaterialBind(material_id, object_id) :
	-AArcade registers which material_id's it wants to listen for MaterialBind events.
	- Called by the engine when an object is about to be rendered that uses a registered material_id.
	- The material_id and the object_id are provided as arguments.

	SwapMaterialTexture(material_id, texture_id) :
	-Called by AArcade during its response to OnMaterialBind.
	- The engine swaps in the correct pixel data pointer for the texture being used on the object_id by the provided material_id.
*/


typedef void* (__cdecl* dsmk_open_file)(const char* filename, unsigned char mode);
typedef void(__cdecl* dsmk_close)(void* smk);
typedef char(__cdecl* dsmk_info_all)(void* smk, unsigned int* frame, unsigned int* frame_count, double* usf);
typedef char(__cdecl* dsmk_info_video)(void* smk, unsigned int* w, unsigned int* h, unsigned char* y_scale_mode);
typedef char(__cdecl* dsmk_enable_video)(void* smk, unsigned char enable);
typedef char(__cdecl* dsmk_enable_audio)(void* smk, unsigned char track, unsigned char enable);
typedef unsigned char* (__cdecl*dsmk_get_palette)(void* smk);
typedef unsigned char*(__cdecl*dsmk_get_video)(void* smk);
typedef char(__cdecl* dsmk_first)(void* smk);
typedef char(__cdecl*dsmk_next)(void* smk);

HMODULE hSmackerDLL = NULL;
dsmk_open_file psmk_open_file = nullptr;
dsmk_close psmk_close = nullptr;
dsmk_info_all psmk_info_all = nullptr;
dsmk_info_video psmk_info_video = nullptr;
dsmk_enable_video psmk_enable_video = nullptr;
dsmk_enable_audio psmk_enable_audio = nullptr;
dsmk_get_palette psmk_get_palette = nullptr;
dsmk_get_video psmk_get_video = nullptr;
dsmk_first psmk_first = nullptr;
dsmk_next psmk_next = nullptr;


/*
IntPtr smk = smk_open_file

smk_info_all(smk, null, &numframes, &usecPerFrame);

smk_info_video(smk, &width, &height, &yScaleMode);

				smk_enable_video(smk, 1);
				smk_enable_audio(smk, 0, 0);

				smk_first(smk);

					byte* palette = smk_get_palette(smk);
					byte* video = smk_get_video(smk);


smk_next(smk);

smk_close(smk);
*/


void InitVideoExperiment()
{
	if (psmk_open_file == nullptr)
		return;

	char farts[16];
	strcpy(farts, "16a.smk");

	char smkpath[MAX_PATH];
	if (smith->LocateDiskFile(farts, smkpath) == 0)
		return;

	MessageBox(0, smkpath, farts, 0);

	void* smk = (*psmk_open_file)(smkpath, 0);

	if (smk != nullptr)
		MessageBox(0, "Well shit dude", 0, 0);
}

void ShutdownVideoExperiment()
{
	if (psmk_open_file == nullptr)
		return;


}



void DoTextureExperiment()
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

		DoTextureExperiment();
	}

	void __cdecl OnLevelShutdownPreObject()
	{
		if (smith == nullptr)
			return;


		ShutdownVideoExperiment();
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

		//char playerModel[16];
		//smith->ExecuteCOG("GetThingModel(GetLocalPlayerThing());", 0, 0, 0, 0, 0, 0, 0, playerModel);

		InitVideoExperiment();
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

	// do not distribute this algorithm so long as u want to ensure that your mod identity is protected
	void _smith_authenticate_smithplugin(const char* szName, int& key)
	{
		// if you do not have, then leave blank. do not modify key
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
		strcpy(p.desc, "mostly an example plugin project. contains demonstrations of things possible with smith engine mod development");
		p.ver = _smith_formatversion(1, 0);
		p.smithRequiredVer = SMITHVERSION;
		_smith_authenticate_smithplugin(p.name, p.authoritykey);

		return 1337;
	}

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken = 0;

	int __cdecl OnInitializePlugin(SMITHCALLS* _smith)
	{
		smith = _smith;

		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


		hSmackerDLL = LoadLibrary("libSmacker.dll");
		if (hSmackerDLL != NULL)
		{
			psmk_open_file = (dsmk_open_file)GetProcAddress(hSmackerDLL, "smk_open_file");
			psmk_close = (dsmk_close)GetProcAddress(hSmackerDLL, "smk_close");
			psmk_info_all = (dsmk_info_all)GetProcAddress(hSmackerDLL, "smk_info_all");
			psmk_info_video = (dsmk_info_video)GetProcAddress(hSmackerDLL, "smk_info_video");
			psmk_enable_video = (dsmk_enable_video)GetProcAddress(hSmackerDLL, "smk_enable_video");
			psmk_enable_audio = (dsmk_enable_audio)GetProcAddress(hSmackerDLL, "smk_enable_audio");
			psmk_get_palette = (dsmk_get_palette)GetProcAddress(hSmackerDLL, "smk_get_palette");
			psmk_get_video = (dsmk_get_video)GetProcAddress(hSmackerDLL, "smk_get_video");
			psmk_first = (dsmk_first)GetProcAddress(hSmackerDLL, "smk_first");
			psmk_next = (dsmk_next)GetProcAddress(hSmackerDLL, "smk_next");
		}


		//int thingID = 10;
		//char szThingID[16];
		//sprintf(szThingID, "%d", thingID);

		//char szReturn[16];

		//smith->ExecuteCOG("SetThingMass(GetSourceRef(), 999.0);", szThingID, 0, 0, 0, 0, 0, 0, szReturn);

		

		return 1;
	}

	void __cdecl OnShutdownPlugin()
	{
		psmk_open_file = nullptr;
		psmk_close = nullptr;
		psmk_info_all = nullptr;
		psmk_info_video = nullptr;
		psmk_enable_video = nullptr;
		psmk_enable_audio = nullptr;
		psmk_get_palette = nullptr;
		psmk_get_video = nullptr;
		psmk_first = nullptr;
		psmk_next = nullptr;
		if (hSmackerDLL != NULL)
		{
			FreeLibrary(hSmackerDLL);
			hSmackerDLL = NULL;
		}


		if (gdiplusToken != 0)
		{
			GdiplusShutdown(gdiplusToken);
			gdiplusToken = 0;
		}

		// our smith interface is no longer valid
		smith = nullptr;
	}

}