# Lab-1
The basis for this framework was https://github.com/ArthurSonzogni/OpenGL_CMake_Skeleton

Installation on Windows:
========================
It is recommended to use an IDE (CLion, Visual Studio, ...) but everything can be done without as well  
  
**Requirements**:  
- [git](https://gitforwindows.org/)
- [cmake](https://cmake.org/download/)
- [make for windows](http://gnuwin32.sourceforge.net/packages/make.htm)  
- compiler and cmake generator i.e. [MinGW-w64](https://sourceforge.net/projects/mingw-w64/)  
   
**Step 1**: clone the repository 
```
git clone --recursive https://github.com/Realtime-Rendering-II/Lab_1.git
```
**Step 2**: create a build folder in the project folder
```
cd Lab-1
mkdir build
```  
**Step 3**: open cmake-gui:  
![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/CMake-0.png) 
  
  select the source and build folder as seen in the image  
  click "generate" and select the cmake generator  
  (in this case MinGW Makefiles is chosen)  
  **Note**: make sure the environment variable for the generator is set  
  
  If the generation was successful it should look like this:  
  ![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/CMake-1.png) 

**Step4**: go into the build folder and make the project  
```
cd build
make
```
If the make command was executed successfully it should look like this:  
  ![Make](https://www.uni-weimar.de/~bexo5814/rrII/images/Make.png)   
  
**Step 5**: execute the application:
```
.\main.exe
```

**Note**: If your project is build in a different folder you have to take care of the shader-paths  
  
  
Installation on Linux:
========================
It is recommended to use an IDE (CLion, Visual Studio, ...) but everything can be done without as well  

**Step 1**: install dependencies  
```
sudo apt-get update
sudo apt-get install git cmake libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libglu1-mesa-dev
```

**Step 2**: clone the repository 
```
git clone --recursive https://github.com/Realtime-Rendering-II/Lab_1.git
```

**Step 3**: create a build folder in the project folder
```
cd Lab-1
mkdir build
```
**Step 4**: inside the build folder generate the cmake project
```
cd build
cmake ..
```
**Step 5**: make the project and execute the application
```
make
./main
```

About the code:
========================
**Wireframe-mode**: If the Key 1 is pressed the program switches between the wireframe and the normal rendering mode.  
This might help you to verify if the model is defined correctly.  
  
**First execution**: if the framework is executed for the first time it should look like this:  
  ![first](https://www.uni-weimar.de/~bexo5814/rrII/images/first_compile.png)  
  
**Final result**: the final result should look like this:   
  ![result](https://www.uni-weimar.de/~bexo5814/rrII/images/result.png)  
     
  with wireframe activated:  
  ![result](https://www.uni-weimar.de/~bexo5814/rrII/images/result_wireframe.png)  
    
  with rotation activated:  
  ![result](https://www.uni-weimar.de/~bexo5814/rrII/images/result_rotation.gif)   
