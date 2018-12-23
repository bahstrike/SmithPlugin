#include <Windows.h>
#include "smith.h"
#include <stdio.h>

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

SMITHCALLS *smith = nullptr;


extern "C" {

	void __cdecl OnMainLoop(double dt, double truedt)
	{
		if (smith == nullptr)
			return;


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

		char playerModel[16];
		//smith->ExecuteCOG("GetThingModel(GetLocalPlayerThing());", 0, 0, 0, 0, 0, 0, 0, playerModel);

	}

	int __cdecl OnExecuteCOGVerb(const char* szName, int nNumParams, const char** pszParams, char* szReturn)
	{
		if (stricmp(szName, "PluginTest") == 0 && nNumParams == 1)
		{
			MessageBox(0, pszParams[0], "SmithPlugin", MB_ICONINFORMATION);
			return 1;// handled
		}

		return 0;// not handled;  let smith deal with it
	}

	// do not distribute this algorithm so long as u want to ensure that your mod identity is protected
	void _smith_authenticate_smithplugin(const char* szName, int& key)
	{
		// if you do not have
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

	int __cdecl OnInitializePlugin(SMITHCALLS* _smith)
	{
		smith = _smith;



		int thingID = 10;
		char szThingID[16];
		sprintf(szThingID, "%d", thingID);

		char szReturn[16];

		//smith->ExecuteCOG("SetThingMass(GetSourceRef(), 999.0);", szThingID, 0, 0, 0, 0, 0, 0, szReturn);


		return 1;
	}

	void __cdecl OnShutdownPlugin()
	{
		// our smith interface is no longer valid
		smith = nullptr;
	}

}