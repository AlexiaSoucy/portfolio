# LightMaze
### A pitch-black marble maze made in Unreal Engine

LightMaze lets the player navigate a pitch-black randomly-generated maze with a glowing marble as the only light source. The player can drop some of their light on the ground to mark parts of the maze, but lose some of their own light when they do.

This was a learning project to get accustomed to working in Unreal Engine, particularly as a first experiment using node-based programming. I decided to put together a maze generation algorithm in Blueprints to make things challenging and see how far I could push it. I took the light mechanics from a bigger game idea I had been thinking about, so the project doubles as a partial proof of concept.

The packaged game can be found at https://alexia-soucy.itch.io/lightmaze.

### Maze Generation

LightMaze uses a recursive division method to generate its mazes. This requires slicing the area with two perpendicular lines and giving 3 of the newly created walls a door so that every part of the maze is always accessible. This process then repeats with each quadrant that has now been created until the maze contains no more subdivisible areas. This functionality was implemented in Unreal's Blueprint framework through two functions: Slice and Generate.

The Slice function takes the indices of a new wall along the edge of the maze (which can be seen as an array) as well as its minimum and maximum indices as bounds for the wall generation. It also takes boolean values corresponding to whether the wall is vertical (rather than the default horizontal) and whether it has a door. If a door exists, an index is randomly generated for it and the wall is created from smaller wall assets corresponding to one index along the area of the maze except where a door should be.

![Image of the Slice function](https://github.com/AlexiaSoucy/portfolio/blob/master/LightMaze/Images/lightmaze-slice.png?raw=true)

The Generate function is much more complex; it takes four integer values corresponding to the minimum and maximum indices along the horizontal and vertical axes to randomly determine where new walls will be generated, then runs through a loop to generate the four appropriate Slice calls. After this, it uses the walls' generated indices to recursively call Generate four more times, once per new quadrant.

![Image of the Generate function](https://github.com/AlexiaSoucy/portfolio/blob/master/LightMaze/Images/lightmaze%20generate.png?raw=true)

Setting up each slice call requires a significant amount of boolean arithmetic, so it's worth taking a closer look at:

![Image of boolean arithmetic used to set up a Slice call](https://github.com/AlexiaSoucy/portfolio/blob/master/LightMaze/Images/lightmaze-slice-setup.png?raw=true)

This effectively determines which half of which wall is being generated (as they cross each other, forming four walls, three with doors) and makes sure the right values are passed to Slice simply by multiplying each possible value with its corresponding boolean value (0 or 1, meaning only the relevant value is retained.)

To recursively make four more Generate calls, the function checks if each new quadrant is big enough to be sliced again and if it is, passes this quadrant's boundaries to a new Generate call which will repeat the above process until no new walls can be created.

![Image of the first recursive Generate call](https://github.com/AlexiaSoucy/portfolio/blob/master/LightMaze/Images/lightmaze-generate-recursive.png?raw=true)

### Contact:
* Email: soucy.alexia@gmail.com
* LinkedIn: https://www.linkedin.com/in/alexia-soucy-077935192/
