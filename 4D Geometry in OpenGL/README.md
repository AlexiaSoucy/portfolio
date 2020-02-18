# 4D Geometry in OpenGL
A header file to create and render 4D geometry in OpenGL by generating a 3D cross-section of the hyperobject. Contains a Hypercube class to create sample geometry for demo purposes. Utilizes a tetrahedral mesh approach inspired by Mark ten Bosch's and Jeff Weber's Miegakure and 4D transformations inspired by Daniel Shiffman's 4D projection Coding Challenge.

Also contains the 18-DOP bounding volume implementation utilized in the project, adapted from Christer Ericson's 8-DOP approach detailed in Real-Time Collision Detection.

This 4D functionality was developed as part of an AI pathfinding demo in collaboration with Keven Presseau-St-Laurent and Maude Braunstein as part of Serguei Mokhov's COMP 477 at Concordia University in the fall of 2019.

## Controls
To control the 4D object 

SPACEBAR - Pause
Q/ESCAPE - Quit
F1 - Default camera
F2 - Overhead camera
F3 - Eye Level camera
ARROW KEYS - Rotate view
F - Toggle wireframe on 4D objects
RMB/LMB - Morph 4D objects