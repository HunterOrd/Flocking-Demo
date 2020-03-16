# Flocking-Demo
Graphic demonstration and Flocking simulation with planes and objects. Texture images credited in readme.

This demonstation is to showcase a flocking algorithim along with graphic work in OpenGL. 
The planes work with the forces added by priority. Centering and alignment are disregared depending on distance.
All of the textures are images convered to ppms and credit is given at the bottom of the readme.

The GLFW, GLEW, and GMTL libraries are used in the code.

# Install instruction if needed:

-Extract both zip files. Two different directories will be created. Move them to a location outside of your coding directory.

-Copy the following into VC's include directory (default C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.14.26428\include).  

-“GL” from GLEW’s include directory

-“GLFW” from GLFW’s include directory

-Copy the following Into VC's lib directory

-(default C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.14.26428\lib\x86). 

-glew32.lib and glew32s.lib from GLEW's lib\Release\Win32 directory.

-glfw3.lib from GLFW’s lib-vc2017 directory

-Copy the following Into C:\Windows\SysWOW64

-glew32.dll from GLEW's bin\Release\Win32. 

-glfw3.dll from GLFW's lib-vc2017 directory. 

-GMTL zip has a floder named "gmtl". Put this floder into VC\include the same way as GLFW and GLEW.

# Controls 

"w" - sets it to world view

"e" - sets it to the O plane location

"z" - rotates the camera around 90 degrees when centerd on the plane

"mouse movements" - when the button is held will move the camera

"N" - Zooms the camera out

"M" - Zooms the camera in

"A" - Turns off the global lighting

"D" - Turns off the Diffuse lighting


# Image credits

-Middle Sphere: Saki from the anime Zombie Land Saga

-The three smaller spheres are:

  -Thoru from Mrs. Kobayashi's Dragon Maid
  
  -A Padoru of shinobu
  
  -Wagner from Undernight In-Birth Exe:Late[st]
  
-The skysphere texture is a Drawing from Twitter handle: @astroid_ill
