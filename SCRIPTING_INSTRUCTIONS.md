# Scripting Instructions

## Warning

For the scripting system to work correclty, you must be on a Windows system, and use Visual Studio 2019. The Engine has not been tested on Windows 11.

## Automatic Compilation

If you are looking for how to acivate the Automatic Compilation feature, go to the [README.md](README.md) file for detailed instructions. We recommend the github web page version to visualize with images.

## How to use

### Create a Script

On the Project Window, right click on an empty space and select "Create Script". Enter a valid name (cannot start with a number, cannot be a C++ keyword, cannot be the same name as another script), an press accept.

### Edit a Script

Open the ScriptingSLN.sln Visual Studio 2019 solution. You will find the script you have created already included on the project. 

Remember to save the file before returning to the Engine so the Automatic Compilation detects that a change has been made.

If Automatic compilation is enabled, once you return to the Engine window, the scripting project should automatically compile.

If Automatic Compilation is not enabled, you must compile the Scripting project manually. Right click on it on the Visual Studio file explorer, and press Compile.

### Add a Script to a GameObject

Add a ScriptComponent to a GameObject.

Drag either the .h or .cpp file of your new script onto the yellow field of the Script Component, on the Inspector Window.

If the Scripting solution has been compiled, the script should now appear on the Inspector window.

### Add Inspector variables

To add inspector variables, you must find the Create"ScriptName"() function inside your script .cpp file.

In the Create function, access the "script" variable passed as an argument. This is the ScriptComponent.

Use any of the methods available (AddDragFloat, AddDragBoxGameObject) to add your variable into the Inspector.

The only available variables for now are: int, float, bool, string, GameObject, Transform, MeshRenderer.

Return to the Engine (compile manually if Automatic Compilation is not enabled) and the inspector fields should be there.

### What are API_GameObject, API_Transform ... ?

The API_Object types are an interface class used to safely manage the current scene Entities and Components. These exist so you cannot accidentaly access a nullptr GameObject or Component.
A console message appears when you try to use one of these API objects with a nullptr value, but the application doesn't stop running.

### How to access other scripts

To access other scripts, include their header file on your .cpp file to access their methods. Then you will need a way to access that script instance. 

The recommended method is to use an inspector variable to access the other script's gameObject (API_GameObject). Then, use the GetScript method of the API_GameObject
and cast the return value to a pointer of your own. 

WARNING: This is not null-safe, meaning if the script stops existing without notice, you will get a nullptr access violation and the engine will crash. Use these references carefully, and
try not to use them without proper security measures. This shouldn't be a problem if, by design, you try to minimize the use of gameObject.Destroy(). Instead, use gameObject.SetActive(false).

