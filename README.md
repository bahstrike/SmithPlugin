# SmithPlugin
api for writing engine mods for Smith


the follow is an excerpt from [smith.h](https://github.com/bahstrike/SmithPlugin/blob/master/smith.h)

please refer to smith.h for the most up-to-date plugin development information




		smith plugin API
    
    
		basically just create a win32 C DLL which exports regular ass, non-mangled functions.
		ensure you export the minimum required symbols for smith to recognize u as a valid plugin.
		your dll also needs to specify file version information (eg. resource file / VERSIONINFO) with smith in the file description.
    
    
		follow either the sample project, or this header, to use the API. there is no .lib or additional DLL that
		you are required to link in; just export the right symbols and call the methods of SMITHCALLS in order
		to manipulate the engine.
    
    
		put your dll in the same directory as smith.exe and thats it.
