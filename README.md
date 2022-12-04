# HelloEngine

HelloEngine is a Game Engine made by videogame development students [Zhida Chen](https://github.com/Xidashuaige) & [Adrià Sellarés](https://github.com/AdriaSeSa).

This is a 3D game engine. The engine state is under development right now.

The goal is to create a complete game engine with Entity Component System, fast 3D rendering, resource manager, scripting language and other utilities.

# Most important features to be reviewed

These are some additional features that are important to know about:

* Undo / Redo implementation.

HelloEngine allows to use Ctrl + Z and Ctrl + Y to Undo / Redo changes on ImGui Widgets. In this version, it only applies to Transform Component widgets inside Inspector.

* Instance Rendering

Instance Rendering. To improve performance, HelloEngine uses Instance rendering to render every Mesh. It is not finished yet, but it can be noticable when instancing multiple meshes.

* Efficiency focused ECS

Efficience focused Entity Component System. An implementation of the ECS where the GameObjects do not use an Update method to iterate over every Component. Instead, every Component is linked to an object inside the Engine that is Updated by a corresponding manager. This way, constantly updating every object is not necessary and performance can be improved. In this version, there are major improvements to be done, but we will not implement them until we are concerned about performance in the future.

## How to use

While hovering the Scene Window:

* Right Click + WASD to move.
* Right Click + Mouse movement to look around.
* Alt + Left Click to orbitate around selected Game Object. If no game object is selected, orbitate around {0,0,0}.
* Mouse wheel to change scene camera zoom.
* Shift to move faster.

### Loading assets

Currently you can load FBX and PNG/DDS files. To do so, drag and drop the file you want to load into the engine. This will create a copy of the dragged file into the Assets folder. FILES THAT ARE NOT PLACED INSIDE ASSETS USING THIS METHOD WILL NOT BE PROCESSED BY THE ENGINE.

Once the file is imported, drag and drop the file from the Project window inside the Engine. Drop inside Scene to load an FBX. Drop inside MaterialComponent Inspector image if it is a PNG/DDS image.

### Current version

**Current state** : Level Editor (v0.5):

**Basic Features**

Custom file format for Mesh and Model. Named "hmesh" and "hmodel".

Resource Manager that:

* Makes sure every asset gets loaded only once in memory, and unloads when no other references are using it.
* Creates Meta files beside assets to connect to the custom file format files inside Resources.
* Destroys any Resources data that no longer has an Asset.

Scene Serialization. Now scenes can be saved and loaded into an "HScene" file. 

Camera Component added Orthographic camera. Updated camera component variables to be changed inside inspector.

Project window with icons and correct form.

Start/Stop/Pause/Next frame buttons that control an internal timer. This can be seen inside Configuration Window!

### Versions

Early in development (v0.1)

3D rendering available, but no mesh loading yet.

Scene structure created, but no Entity component system yet.

Added XML utilities to load Int, Float, Bool and String variables from a default XML file. This system is called QuickSave and mimics the PlayerPrefs system form Unity Engine.

Geometry viewer: v0.2

**Basic Features**

3D rendering with FBX loading.

Texture diffuse rendering applied to any mesh. 

Entity Component System with Hierarchy. Allows change of parents and entities and components can be enabled/disabled.

Basic Project window to manage files inside Assets

Inspector window to manage component variables of selected entity.

Console to log information about the engine.

Transform component that uses child/parent relationship to apply transformations.

Drag & drop to Import files into Project folder. This can be then used to be applied to Entities.

Drag & drop of FBX files into the scene to load the Mesh.

Drag & drop of png files into a MaterialComponent inside Inspector to change the texture currently being used.

Viewing of memory consumption. 
