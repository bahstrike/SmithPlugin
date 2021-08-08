/*
		smith plugin API

		----------------------- OVERVIEW ----------------------------------------------------------------

		basically just create a win32 C DLL which exports regular ass, non-mangled functions.
		ensure you export the minimum required symbols for smith to recognize u as a valid plugin.
		your dll also needs to specify file version information (eg. resource file / VERSIONINFO) with smith in the file description.

		follow either the sample project, or this header, to use the API. there is no .lib or additional DLL that
		you are required to link in; just export the right symbols and call the methods of SMITHCALLS in order
		to manipulate the engine.

		put your dll in the same directory as smith.exe and thats it.



		-----------------------  API INDICATIONS FOR USE  ----------------------------------------------------------------

		the actual smith plugin API is literally just this smith.h  header file.

		the intended use of this smith.h header is for you to copy it to your project folder. however i do not
		suggest modifying it. should you decide to update your plugin in the future, just replace smith.h with the new
		version and rebuild your dll.



		--------------------  CONSIDERATION FOR DEVELOPERS  --------------------------------------------------------------------------

		there are basically two tricks to getting smith to recognize and load your plugin:
		1) you must create a resource file / version info and somewhere in the FileDescription include the word "smith" (case insensitive).
		   this serves a number of purposes for stability, performance and security by not even attempting to load DLLs that do not flag themselves as smith plugins.

		2) you must implement and export SmithQueryPlugin.  you must populate a name/author in the PLUGININFO struct.
		   you must return 1337 from the function. the authoritykey must not be modified unless you have special instruction to do so.
*/


///////////////////////////////////////////////////////////////////////////////////////////////
// START OF ACTUAL HEADER
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////
// REQUIRED INCLUDES
///////////////////////////////////////////////////////////////////////////////////////////////
#include <Windows.h>



///////////////////////////////////////////////////////////////////////////////////////////////
// USE TO POPULATE:       PLUGININFO::smithRequiredVer
#define SMITHVERSION		200
///////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////
// UR MODS IDENTIFICATION.   populate within  SmithQueryPlugin
///////////////////////////////////////////////////////////////////////////////////////////////

// optional flags for PLUGININFO::purpose
#define PP_TEXTUREUPSCALE		0x0001
#define PP_SMACKERCUTSCENE		0x0002
#define PP_SMUSHCUTSCENE		0x0004

struct PLUGININFO
{
	char name[16];			// required: name of ur plugin, eg. "Strikes Mod"
	char author[16];		// required: ur name
	char authorEmail[128];	// optional: ur email address
	char attributions[2048];// optional: fill this buffer with names[&info], each entry terminated by |.     for example, can do like "BAH_Strike&BAH Main Page https://bah.wtf|Leethaxxor&Leet haxxin support https://github.com/blahblah   This dude did stuff.|Bobs Tacos&Excellent tacos. Kept us goin|Homie A|Homie B"
	char desc[1024];		// optional: description of plugin
	char homepageURL[512];	// optional: URL for homepage of plugin
	char autoupdateURL[512];// optional: root URL for plugin autoupdate support  (LEAVE BLANK; NOT CURRENTLY SUPPORTED)
	int ver;				// required*: version of your plugin. follow a XYY standard wherein X is major and YY is minor version. eg: 207 -> version 2.7     the default provided is 100  (version 1.0)
	int smithRequiredVer;	// required*: minimum version of smith that your plugin requires to operate in. same version XYY version standard as described above.  please insert the SMITHVERSION constant
	int authoritykey;		// optional: key value to prevent spoofing;  prevents other ppl from making plugins that masquerade or try to override an official plugin. talk to strike to be provided an authority key. if you dont have an authority key algorithm, leave this value as the value passed in (do not change it! or your plugin will not load)
	unsigned int purpose;	// optional: flags to indicate what specific solutions this plugin provides.  populate with flag constants of PP_xxxxxxx, or leave 0 for generic plugin
};



///////////////////////////////////////////////////////////////////////////////////////////////
// PLUGIN -> SMITH   API CALLS
//
// this is all the stuff YOU can call on the engine via the SMITHCALLS* interface u are given
//
// please read like   "dExecuteCOG"  means you would call it like  "smith->ExecuteCOG()"
///////////////////////////////////////////////////////////////////////////////////////////////

extern "C"// this helps prevent name mangling on ur exported symbols
{


	// it simply just makes sense to allow plugins to freely execute cog;  this means you can do all sorts of crazy stuff in-game
	// without having to learn a new API and without me having to write one.
	//
	// note that the cog script should NOT include a symbols section, nor  code / end  keywords.
	// just put in one or more statements and they will be executed.
	//
	// you are responsible for providing the source/sender and params.  your cog script can use the typical
	// GetSourceRef(),  GetSenderID(), GetParam(0)  etc  to retrieve these values.
	//
	// if your cog calls ReturnEx(), then this value will be accessible to you via the returned szReturn  value.
	//
	// note: the smith cog VM operates under a Flex symbol type and generally is most compatible as strings.
	// so for simplicity of interface, all values are strings. you are responsible for formatting integer values into strings
	// and parsing return values etc.
	//
	// final note:  smith will spin up a discardable cog VM to execute this code and thats it.
	// you wont have access to any symbols or existing variables. as well, you should not record the (temporary) self cog ID.
	// your cog can, however, send triggers .. or messages to other cogs via their IDs .. and that should be fine.
	typedef void(__cdecl *dExecuteCOG)(const char* szCOGScript, const char* szSourceRef, const char* szSenderRef, const char* szSenderID, const char* szParam0, const char* szParam1, const char* szParam2, const char* szParam3, char* szReturn);



	// returns 1 if smith game engine is actually running with a level loaded etc, 0 otherwise.
	// you should probably check this if you decide to try to issue in-game related functions at a weird moment like
	// ShutdownPlugin (not recommended).  i cant gaurantee it wont crash if u do somethin like that. so basically call all
	// game stuff in a callback that you know the game is running, or call IsInGame  to check first.
	typedef int(__cdecl *dIsInGame)();



	// generates, or updates, an in-memory mat file using the specified parameters and pixeldata.
	// the mat name does not need to exist and therefore can be used to generate unique render targets
	// if the mat does exist, its pixeldata is simply updated. this also works on regular game textures that have already been loaded.
	//
	// note:  when replacing an existing mat's pixeldata, it is acceptable to change the texture dimensions. despite JK's texture coords
	// are in texels rather than 0-1,  smith remembers the original mat's dimensions so the scaling should work fine.  if you 
	// generate a new mat via this call, the original mat dimensions will be that which you specify here.
	typedef unsigned int(__cdecl *dGenerateMaterial)(const char* szMatName, const char* szColormap, int nCel, int width, int height, int depth, int stride, const void* pBitmap, const void* pEmissive);



	// generates, or updates, an in-memory mat file using the specified parameters and pixeldata.
	// the mat name does not need to exist and therefore can be used to generate unique render targets
	// if the mat does exist, its pixeldata is simply updated. this also works on regular game textures that have already been loaded.
	//
	// note:  when replacing an existing mat's pixeldata, it is acceptable to change the texture dimensions. despite JK's texture coords
	// are in texels rather than 0-1,  smith remembers the original mat's dimensions so the scaling should work fine.  if you 
	// generate a new mat via this call, the original mat dimensions will be that which you specify here.
	typedef unsigned int(__cdecl *dGenerateMaterialBitmap)(const char* szMatName, const char* szColormap, int nCel, int bAllowUpscale, HBITMAP hBitmap, HBITMAP hEmissive);

	// uses smith's resource system to locate the desired game file; presuming it exists as a full file path
	typedef int(__cdecl* dLocateDiskFile)(const char* szFileName, char* szFullPath);


}


// the actual struct that smith populates to provide you with function pointers to engine calls
struct SMITHCALLS
{
	dExecuteCOG ExecuteCOG;
	dIsInGame IsInGame;
	dGenerateMaterial GenerateMaterial;
	dGenerateMaterialBitmap GenerateMaterialBitmap;
	dLocateDiskFile LocateDiskFile;
};





///////////////////////////////////////////////////////////////////////////////////////////////
//  SMITH -> PLUGIN   CALLS
//
// SYMBOLS U SHOULD EXPORT FOR A GREAT AND HAPPY PLUGIN
///////////////////////////////////////////////////////////////////////////////////////////////


// REQUIRED:
// smith uses this to check if a DLL is a valid smith plugin. you must populate name
// and u must return 1337 from this function in order for smith to proceed in recognizing your plugin.
//
// NOTE: smith MAY only TEMPORARILY load this plugin DLL in order to call SmithQueryPlugin.
// if smith decides to actually use the plugin later on, it will then reload it a 2nd time
// and call InitializePlugin to get the ball rolling.
//int __cdecl SmithQueryPlugin(PLUGININFO& p)



// at the point smith decides it will load your plugin during the session, it will load this plugin dll again (if not already loaded)
// and call InitializePlugin, providing you with a pointer to the SMITHCALLS struct that contains all the available
// smith engine calls that you will have access to throughout the remainder of the session.
// return true (1) for success;  (false (0)  for fail to load associated components)
//int __cdecl InitializePlugin(SMITHCALLS* _smith)



// once the session is over, smith will call ShutdownPlugin to allow you to gracefully tie up any loose ends.
// generally, this will be followed by actually unloading this plugin DLL; however that's not gauranteed.
// if smith supports a more dynamic plugin load/unload system then its possible sessions can be started/stopped
// multiple times without the plugin being unloaded. so bear that in mind:  plz do not leak.
// 
// long story short;  kill any possible tasks you have that may either call smith API, or expect to be called by smith API.
// forget the SMITHCALLS interface; its not gauranteed to be valid anymore.
// the only functions smith will call after this point is either SmithQueryPlugin, or InitializePlugin again.
//void __cdecl ShutdownPlugin()



//-Called by the engine every cycle of the engine's main loop.
//- AArcade uses this to perform its own main loop.
//
// dt = deltatime, intended for use in all simulation and engine tasks. may be conditioned eg. if game runs worse than 20fps then dt will be clamped to 1/20.  as well, if there is any kind of world slowmo, this dt will be augmented to include that
// truedt = actual deltatime since last frame. please do not use this unless you really know why you need it over dt
//void __cdecl OnMainLoop(double dt, double truedt)



// called by the engine every cycle of the engine's main loop, at the very beginning of the render subsystem.
// the renderer context has just been bound at this point and so it is a good time to do any global, per-frame
// prep work like updating texture data
//void __cdecl OnPrepareMainRender(double dt, double truedt)



//-Called by the engine before it starts cleaning up any world objects from memory.
//- AArcade uses this to tell internal mutli - thread systems that they should be suspended.
//
//NOTE:  Shutdown functions may be called multiple times, or called even if a corresponding Start was never called.
//void __cdecl OnLevelShutdownPreObject()



//-Called by the engine after all objects are removed from memory.
//- AArcade uses this to perform general clean - up between sessions.
//
//NOTE:  Shutdown functions may be called multiple times, or called even if a corresponding Start was never called.
//void __cdecl OnLevelShutdownPostObject()



//-Called by the engine after the map is loaded but prior to any objects being created in it.
//- AArcade uses this to perform general start - up logic for the session.
//
//NOTE: Smith promises to call this 1 time, and will not call again until the corresponding Shutdown functions have been called
//void __cdecl OnLevelStartPreObject()



//-Called by the engine after all world - objects are spawned into the world & the world simulation has begun.
//- AArcade uses this to trigger its own start logic, which includes spawning many objects into the world.
//
////NOTE: Smith promises to call this 1 time, and will not call again until the corresponding Shutdown functions have been called
//void __cdecl OnLevelStartPostObject()



// called by smith whenever it is ready to execute a cog verb.  mods get first dibs.
// returning 1 indicates to smith that the verb was handled and it will not be passed to any other
// plugins or handled by smith itself.  returning 0 allows the verb to continue until its hopefully picked up by something else.
// 
// note: verb name in szName is always lowercase
//int __cdecl OnExecuteCOGVerb(const char* szName, int nNumParams, const char** pszParams, char* szReturn)



// called by smith after loading any imagefile and before GPU upload.
// can implement texture upscaling here.
// return true(1) if handled,  or false (0) if not
//int __cdecl OnProcessTexture(void* input, int inWidth, int inHeight, int inStride, void* output, int outStride)


// called by smith when initiating a smush cutscene.  pBuffer/len is the contents of a *.san file.
// return custom handle that will be passed to all further smushXXXXX functions
//void* __cdecl smushLoad(void* pBuffer, int len)


// called by smith when initiating a smush cutscene.
// return video properties
//void __cdecl smushGetInfo(void* smush, int& width, int& height, int& numframes, double& fps)


// called by smith continuously during smush cutscene playback. plugin is responsible for tracking time.
// return values:
// 0 - repeat frame (smith will redraw previous frame)
// 1 - new frame (smith will call smushGetFrame/smushGetCutsceneStringId for updated info)
// 2 - done (smith uses this to detect when cutscene playback has finished)
//int __cdecl smushFrame(void* smush)


// called by smith whenever smushFrame returns 1.
// format is 24bit RGB
//void __cdecl smushGetFrame(void* smush, void* scan0, int stride)


// called by audio driver whenever it requires the next streaming PCM sample buffer. may be threaded.
// stereo 16-bit 44100hz
//void __cdecl smushGetAudio(void* smush, void* buffer, int len)

// called by smith to determine the UNI string ID for cutscene subtitles of current frame.
//int __cdecl smushGetCutsceneStringId(void* smush)


// called by smith when finished playing smush cutscene.
//void __cdecl smushDestroy(void* smush)
