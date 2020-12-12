# vox

Vox is an open source Voxel Engine, and game, that is written in C++ and OpenGL and can be used to create easy and fast voxel based games and applications. The engine is written to be highly functional and very efficient at allowing voxel game creation and provides a simple framework for creating 3D voxel games.

This is a fork of [AlwaysGeeky Vox Engine] (https://github.com/AlwaysGeeky/Vox)\
Main goals of this fork are the following:
- keep update outdated project;
- treat libraries as external (use git submodule command where it is possible);
- replace all not opensource libraries with an opensource ones (fmod to sdl and so on);
- replace all file formats that hardly editable with an easy editable ones (ms3d to dae and so on);
- fix memory leaks and improve code quality (https://github.com/AlwaysGeeky/Vox/pull/179);
- improve code base after completing all above.

## Features
* Look for original project, it's all there.

## Drawbacks
* FMOD has 3d sound support, while SDL_mixer don't by default. Have to dodge somehow. And there are some audio issues there. Have to be fixed.
* There is no support for Windows. Don't have it, don't like it.

## Building the Source Code

**Windows**

I don't support it, sorry. You have to do it by yourself.
Visual Studio projects are included inside the ```\project``` sub-folder (VS2012, VS2013, VS2015) of original Vox project.
But they are outdated a bit and need polish.

**Linux**

```
git clone <project>
cd vox
git submodule init
git submodule update --recursive
mkdir build
cd build
cmake ..
make
```

## Screenshots
![Vox](http://i.imgur.com/aNsZ9Jz.png)
![Vox](http://i.imgur.com/iGcUYSy.png)
![Vox](http://i.imgur.com/ai0qMfD.png)
![Vox](http://i.imgur.com/usPCqJt.png)
![Vox](http://i.imgur.com/mZ85dWP.png)

## License
This software is provided under the **GNU General Public License**, more information can be found in the [license file](https://github.com/AlwaysGeeky/Vox/blob/master/LICENSE.md).
