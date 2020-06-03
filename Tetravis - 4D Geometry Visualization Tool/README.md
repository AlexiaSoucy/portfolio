# Tetravis
## A 4D geometry visualization tool written in OpenGL.

Currently in a prototype phase, Tetravis is intended to eventually serve as part of a 4D gamedev pipeline as a tool for importing a 3D mesh, formatting it as necessary for 4D geometry (notably the mesh must undergo a process called tetrahedralization to function as a 4D mesh), and displaying as a proper 4D model ready to be exported and added to a game environment. In its current state, Tetravis is a 4D visualizer using a 4D hypertetrahedron to demonstrate the behaviour of 4D geometry.

## Background
I had the opportunity in 2019 to work on a university project that made use of 4D geometry and was tasked with writing the implementation for the generation and rendering of the 4D elements. This used the OpenGL fixed function pipeline and was not usable as a development tool and was primarily an AI demo with a fun 4D feature on the side. In order to one day give developers the ability to create more interesting applications with 4D elements, I decided to create Tetravis and rewrote the implementation using a more modern OpenGL approach. My primary inspiration for doing so is Marc ten Bosch's Miegakure.

## Running
All source code written by me is included in src, but the only code necessary to run the .exe on Windows x64 is the shaders (namely the .vs and .fs files). In addition, recompiling the code to run on a different platform requires GLAD, GLFW, and GLM libraries.

Contact:
* Email: soucy.alexia@gmail.com
* LinkedIn: https://www.linkedin.com/in/alexia-soucy-077935192/