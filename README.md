# OpenGL-AdvancedGLSL

## Description

This repository showcases advanced OpenGL Shading Language (GLSL) techniques implemented in C++. It demonstrates various rendering effects using custom shaders.  It provides a practical learning resource for developers interested in exploring modern graphics programming.

## Screenshots

<!-- Include screenshots here -->
![Screenshot 1](screenshot1.png)
![Screenshot 2](screenshot2.png)

## Dependencies

Before building and running this project, you need to install the following dependencies:

*   **GLEW (OpenGL Extension Wrangler Library):**  Download GLEW and add the `include` directory and library files (e.g., `glew32.lib`) to your system environment variables.
*   **GLM (OpenGL Mathematics):**  Download GLM and add the `glm` directory to your system environment variables.

**Note:** Adding GLEW and GLM to your system environment variables ensures that Visual Studio can find them during the build process.

## Build Instructions (Visual Studio 2022)

Follow these steps to build the project using Visual Studio 2022:

1.  **Clone the Repository:**  Clone this repository to your local machine.
    ```bash
    git clone [Repository URL]
    ```
2.  **Open the Solution:**  Open the `OGLvcvproj.sln` file in Visual Studio 2022.
3.  **Set Build Configuration:**  Select either "Debug" or "Release" configuration from the dropdown menu.
4.  **Build the Solution:**  Go to `Build` -> `Build Solution` (or press `Ctrl+Shift+B`). Ensure that all the required dependencies are present.

## Usage

After successfully building the project, you can run the executable located in the `x64/Debug` or `x64/Release` directory.

Experiment with different shader parameters and explore the source code to understand the implementation details.

**Exploring the Techniques:**

*   Look for the relevant shader files in the `shaders` directory and the C++ code in the `OGL.cpp` file.
