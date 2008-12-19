#include <SDL/SDL.h>
#include <cstdlib>
#include <iostream>
#include <GL/gl.h>
#include <sys/time.h>

namespace
{

bool handleEvents ();
void draw         ();

void
setupGL (int width,
         int height)
{
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (-(float (width) / height), float (width) / height, -1, 1, 1, 0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glColor3f (1,1,1);
  glClearColor (0,0,0,1);
}

void
mainLoop ()
{
  unsigned long frames = 0;

  while (handleEvents ())
  {
    draw ();

    timeval t1, t2;

    gettimeofday (&t1, 0);
    SDL_GL_SwapBuffers ();
    gettimeofday (&t2, 0);

    double swap_time = ((t2.tv_sec - t1.tv_sec) * 1000.
                        + (t2.tv_usec - t1.tv_usec) / 1000.);

    if (swap_time > 2)
      std::cout << "(frame " << frames << ") " << swap_time << std::endl;

    ++frames;
  }
}

unsigned long
getTimeMs ()
{
  timeval t;

  gettimeofday (&t, 0);
  return (unsigned long)(t.tv_sec) * 1000 + (unsigned long)(t.tv_usec) / 1000;
}

void
draw ()
{
  glClear (GL_COLOR_BUFFER_BIT);

  glPushMatrix ();
  glRotatef ((getTimeMs () % 5000) * (360 / 5000.f), 0, 0, 1);

  glBegin (GL_TRIANGLE_FAN);
  glVertex2f (-.2f, -.9f);
  glVertex2f (.2f, -.9f);
  glVertex2f (.2f, .9f);
  glVertex2f (-.2f, .9f);
  glEnd ();

  glPopMatrix ();
}

bool
handleEvents ()
{
  bool continue_ = true;

  SDL_Event event;
  while (SDL_PollEvent (&event))
    switch (event.type)
    {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
      {
      case SDLK_ESCAPE: continue_ = false; break;
      default: break;
      }
      break;

    case SDL_QUIT: continue_ = false; break;
    default: break;
    }
  return continue_;
}

} //namespace

int
main (int    argc,
      char** argv)
{
  Uint32 sdl_flags = SDL_OPENGL;
  int    swidth    = 1280,
         sheight   = 800;

  for (int i = 1; i < argc; ++i)
    if (argv[i][0] == '-')
      switch (argv[i][1])
      {
      case 'w':
        ++i;
        if (i < argc)
          swidth = std::atoi (argv[i]);
        break;
      case 'h':
        ++i;
        if (i < argc)
          sheight = std::atoi (argv[i]);
        break;
      case 'f':
        sdl_flags ^= SDL_FULLSCREEN;
        break;
      }

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "Couldn't init SDL: " << SDL_GetError () << std::endl;
    return 1;
  }
  std::atexit (SDL_Quit);

  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  if (! SDL_SetVideoMode (swidth, sheight, 0, sdl_flags))
    return 1;
  if (! swidth || ! sheight)
  {
    SDL_Surface *surface = SDL_GetVideoSurface ();
    swidth  = surface->w;
    sheight = surface->h;
  }
  SDL_ShowCursor (0);

  setupGL (swidth, sheight);
  mainLoop ();

  return 0;
}
