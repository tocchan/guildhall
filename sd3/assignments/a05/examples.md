Job System Examples
======

## Path Finding
Path finding (A* in this example) is a fairly easy task to job out;   

1. (immediate) acquire/create copy of current map state.
2. (generic) create cost map for current entity you want to path
3. (generic) run A* on your current map 
4. (main) Give finished path data to requesting entity (keep newest, discard old)

Optimizing this with shared information requires a little more sophisticate job system (allowing you to chain off "finished" jobs for example).   


## Saving Texture to Disk

### Preconditions
- Texture being passed in is valid
- Path to save is valid

### Post Conditions
- By the time function returns, save to edit/delete texture

### General Algorithm
1. (immediate) Make a staging copy of the texture (happens immediately from calling thread to satisfy post condition)
2. (render) Copy staging texture to CPU image, and release staging texture.
3. (generic) Compress image to desired format (png/jpg)
4. (io) Save to disk, and destroy CPU image. 

Steps 3 & 4 will likely be combined if using `stbi`.  But if you have a dedicated IO thread, you may want to split the two tasks up (the compression can easily be split ot multiple threads where IO may not), plus IO is non-CPU intensive.  



## Loading a Texture 
### Preconditions
- None

### Post Conditions
- Texture is immediatley useable (if not correct)
- Texture will update to correct image on correct load
- Texture will update to an invalid pattern/colour on failed load, logging a message

### General Algorithm 
1. (immediate) Create a stand-in texture for image (1x1 Texture2D, solid transparent white or black is usually good)
2. (io/generic) Load file to CPU image (load & decompression can be split up)
3. (render) Load raw image to new texture, or reference invalid texture if raw-image is invalid.
4. (render) Point originally created textures internal handle to resultant texture of step 3.  
   - Invalidate any views

Views can invalidate themselves by keeping track of the internal handle of the texture they were made for, as well as their parent texture (engine class).  If the handles don't match, regenerate the view lazily on next usage.


## Loading a CubeMap w/ Mips
Similar to above, but we can go more wide.  Cubemaps are made up of either one single texture that we load into 6 layers, 
or 6 seperate images for each face of the cube.  For simplicity let's do the 6 seperate image version as the single image version is nearly identical the above "Loading a Texture" case.

### General Algorithm
1. (immediate) Create a stand-in cube-map (again, 1x1 per side is probably fine)
2. (io/generic x6) Load each image
   - CPU mip loading could happen here
3. (render x6) Copy each layer into the correct sub-resource.
4. (render) Point original texture to newly created handle
   - Fix up views


## Chunk Generation (MineCraft)

Take **B** to be the number of biomes the chunk overlaps; 

1. (immediate) Mark chunk as being generated (so multiple callers don't try to generate it)
2. (generic) Determine if we're generating or streaming in - assume generating for now; 
3. (generic xB) Generate your specific chunk for the the selected biome
4. (generic) Generate cave structure 
5. (generic) Blend Biomes Together
6. (generic) Subtract Cave Structure 
7. (main) Swap in completed chunk data into the selected chunk, mark as 'ready'

Once ready, need to generate renderable geometry, so may kick off another thread anytime the chunk becomes 'dirty', or has changed.  There is probably another system on-top of this to help prioritize chunks for loading instead of letting the job system do it;  

1. (immediate) mark as generating & copy current state
2. (generic) generate opaque geoemetry 
3. (generic) generate translucent geometry (water, glass, etc)
4. (render) update GPU mesh (opaque and translucent layers)
5. (main) Mark as finished generating, attempt a restart if state has changed in the mean-time. 



