Job/Task System
======

## System Requirements

- System starts up and shutsdown correctly
  - Startup spins up a fixed number of generic threads for running jobs
  - Startup takes a fixed number of non-generic categories for setting up work queues
  - Shutdown tries to finish all generic work, and asserts that all active work is finished before returning
    (important as the job pools are cleaned up at this point)
  - Give control of when/where non-generic jobs are run.
- Have a way to set up work dependencies between jobs (support for any non-cyclic graph)
- Be able to easily dispatch a job (similar to starting a thread)
- Be able to easily attach data to a job (job as it's own class, job free store, etc...)
- Be able to check for job status (just completion is enough) 


### Bonus Objectives
- Try to minimize allocations from the system outside of extreme outlier cases (ie, a job needs ot allocate a large amount of data, such as loading an image)
- Support async/overlapped non-cpu jobs (file-io is an example of this)


## Examples
- [./codesamples.md](./codesamples.md) - Some implementation suggestions
- [./examples.md](./examples.md) - Example tasks suitable to jobs; 


## Checklist
To help visualize this working - will have to do something with rendering - it has multiple categories involved.  Will
field other examples in class, and if they're off appropriate completity I'll add them; 

- [ ] 50%: All system requirements are met; 
- [ ] 30%: Screenshot Functionality with minimal frame impact
      - [ ] `save_screenshot "path/to/file.png"` console command
      - [ ] In `EndFrame`, just before `Present`
          - [ ] Create a staging texture
          - [ ] Copy current back buffer to staging texture
          - [ ] render job to copy staging texture to image, and free staging buffer
          - [ ] generic job to save image to file, and free image
          - [ ] main thread job to `ConsolePrintf` that it finished 
- [ ] 20%: One of following two demos works; 

### Texture Loading

- [ ] Have a textured quad in scene that you can see the entirety of.
- [ ] Have something animating so that we can see frame drops; 
- [ ] A console command to load a texture for that quad to use; 
      - *example:* `async_load_texture "path/to/file.png"`
      - Have some large textures around (1080p, 2K, and 4K)
- [ ] Be able to load them on the fly and see minimal or no frame drops
      - While loading, texture should switch to a 1x1 solid colour
      - If failed, texture should be 'invalid', like a magenta.


### Mandelbrot Generation
- [ ] Have a textured quad in scene that you can see the entirety of.
- [ ] Have something animating so we can see frame drops
- [ ] Have a mandelbrot generation console command
      - *syntax:* `async mandelbrot width height iterations`
      - *example:* `async_mandelbrot 1280 720 1000`
- [ ] Generation should break up the work by rows in the image
- [ ] As rows finish, update the rows in the texture on the main thread
- [ ] Should happen with minimal frame loss; 


## Appendix

### Mandelbrot Generation Code
```cpp
//--------------------------------------------------------------------
// Stolen from https://en.wikipedia.org/wiki/Mandelbrot_set
static uint CheckMandlebrotSet( float x0, float y0, uint const MAX_ITERATIONS ) 
{

   float x = 0.0f;
   float y = 0.0f;

   uint iteration = 0;
   while ((((x * x) + (y * y)) < 4.0f) && (iteration < MAX_ITERATIONS)) {
      float xtemp = x * x - y * y + x0;
      y = 2.0f * x * y + y0;
      x = xtemp;
      iteration++;
   }

   return iteration;
}

//------------------------------------------------------------------------
bool ImageGenerateMandlebrot( Image *image, uint width, uint height )
{
   if (!image->create_buffer( IMAGE_RGBA8, width, height )) {
      return false;
   }
   
   // number of iterations to check if item is in set
   uint MAX_ITERATIONS = 1000;

   for (uint x = 0; x < width; ++x) {
      // scale image width to a -2.5 to 1 range for mandelbrot
      float x0 = ((float)x / (float)width) * 3.5f - 2.5f;
      for (uint y = 0; y < height; ++y) {
         // scale image height to -1 to 1 range for mandelbrot
         float y0 = ((float)y / (float)height) * 2.0f - 1.0f;
         uint iterations = CheckMandlebrotSet( x0, y0, MAX_ITERATIONS );

         // feel free to tone-map by normlizing iteration count
         // for a more colourful image... but black and white
         // is fine for this assignment; 
         if (iterations == MAX_ITERATIONS) {
            image->set_pixel( x, y, rgba::BLACK );
         } else {
            image->set_pixel( x, y, rgba::WHITE );
         }
      }
   }

   return true;
}
```
