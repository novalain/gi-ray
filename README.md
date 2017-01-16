# gi-ray
A Monte-Carlo ray tracer from scratch in C++.

### Key Features:
* Path tracing where the only source of bias comes from path termination
* Explicit light sampling
* Ray-triangle intersection using Möller-Trumbore
* Ray-sphere intersection
* Lambertian, Specular and Transparent BRDFs
* Multi-threading

### To compile and run on UNIX system
* Cd to root folder
* Run ```make && make run```

### To set up Visual Studio 2015
* Download glm http://glm.g-truc.net/0.9.8/index.html
* Open Visual Studio
* File -> New -> Project From Existing Code...
* Make sure "Visual C++" is chosen then hit next
* Locate gi-ray folder, name project gi-ray. Hit next
* Select "Use Visual Studio" abd set project type to "Console application project". Press finish
* Go Project -> gi-ray Properties...
* Select C/C++ in left column
* In "Additional Include Directories", add the path to both "gi-ray/include" and glm 
