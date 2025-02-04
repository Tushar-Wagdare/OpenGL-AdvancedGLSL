# OpenGL-AdvancedGLSL

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Description

This repository showcases advanced OpenGL Shading Language (GLSL) techniques implemented in C++. It demonstrates various rendering effects using custom shaders, including post-processing, lighting, and complex shader-based animations. This project serves as a practical learning resource for developers interested in exploring modern graphics programming.

## Screenshots

<!-- Include screenshots here -->
![Screenshot 1](screenshot1.png)
![Screenshot 2](screenshot2.png)

## Dependencies

Before building and running this project, you need to install the following dependencies:

*   **GLEW (OpenGL Extension Wrangler Library):**  Download GLEW from [https://github.com/nigels-com/glew](https://github.com/nigels-com/glew). Build GLEW and add the `include` directory and library files (e.g., `glew32.lib`) to your system environment variables.
*   **GLM (OpenGL Mathematics):**  Download GLM from [https://github.com/g-truc/glm](https://github.com/g-truc/glm). GLM is a header-only library, so simply copy the `glm` directory into your project's include path.

**Note:** Adding GLEW and GLM to your system environment variables ensures that Visual Studio can find them during the build process. This typically involves adding paths to the `INCLUDE` and `LIB` environment variables.

## Build Instructions (Visual Studio 2022)

Follow these steps to build the project using Visual Studio 2022:

1.  **Clone the Repository:**  Clone this repository to your local machine.
    ```bash
    git clone [Repository URL]
    ```
2.  **Open the Solution:**  Open the `OGLvcvproj.sln` file in Visual Studio 2022.
3.  **Set Build Configuration:**  Select either "Debug" or "Release" configuration from the dropdown menu. Ensure that you have the correct platform (e.g., x64) selected.
4.  **Build the Solution:**  Go to `Build` -> `Build Solution` (or press `Ctrl+Shift+B`).
5.  **Resolve Dependencies:**
    *   **Include Directories:**  Add the GLEW `include` directory and the GLM directory to the project's include directories. (Project -> Properties -> C/C++ -> General -> Additional Include Directories).
    *   **Library Directories:**  Add the GLEW library directory to the project's library directories. (Project -> Properties -> Linker -> General -> Additional Library Directories).
    *   **Linker Input:**  Add `glew32.lib` to the project's linker input dependencies. (Project -> Properties -> Linker -> Input -> Additional Dependencies).
6.  **Build Again:** Build the solution again after setting the include directories, library directories, and dependencies.

## Usage

After successfully building the project, you can run the executable located in the `x64/Debug` or `x64/Release` directory.

The demo showcases various advanced OpenGL techniques. Experiment with different shader parameters and explore the source code to understand the implementation details.

**Exploring the Techniques:**

*   **[Technique 1]:** (e.g., Screen-Space Ambient Occlusion): Look for the relevant shader files in the `shaders` directory and the C++ code in the `OGL.cpp` file.
*   **[Technique 2]:** (e.g., Deferred Shading): Examine the shader setup and rendering passes in the `OGL.cpp` file to understand the deferred shading pipeline.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
