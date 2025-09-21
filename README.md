# PvZ-GW-Map-Loader
Level loader for Plants vs. Zombies: Garden Warfare on PC.

# How to Use It
* Download the latest release and inject it with a DLL injector of your choice. Alternatively, you can have the DLL load automatically by placing it in the game's installation directory and renaming it to "dinput8.dll". 
* If you want to use this offline, you can inject it after loading into a level with "-level" and "-Game.DefaultLayerInclusion" launch options.
* If you want to play with friends or be connected to EA servers, you must be the host and not in the main menu for this to work. 
* Select options within the boxes based on the level you want to load, then press "Load Level".

# Credits
PvZ-GW-Map-Loader utilizes the following open source projects:
* [minhook](https://github.com/TsudaKageyu/minhook)
* [imgui](https://github.com/ocornut/imgui)
