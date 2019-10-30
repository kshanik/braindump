#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <stdio.h>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "common.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef struct
{
    SDL_Window*     window;
    SDL_GLContext   context;
    bool            render_quad;
    int             window_width;
    int             window_height;
} global_state;

global_state g_state = {
    0,
    {},
    true,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
};

#define _UI_IMPLEMENTATION
#include "ui.h"
#define _FONT_IMPLEMENTATION_
#include "font.h"
#define _RENDERER_IMPLEMENTATION_H
#include "renderer.h"



bool initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        //printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        success = false;
    }

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        //printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        success = false;
    }
    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        //printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        success = false;
    }
    
    return success;
}

bool init()
{
    load_font_file("res/font.fnt");

    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Use OpenGL 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

        //Create window
        g_state.window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( g_state.window == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create context
            g_state.context = SDL_GL_CreateContext( g_state.window );
            if( g_state.context == NULL )
            {
                printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Use Vsync
                if( SDL_GL_SetSwapInterval( 1 ) < 0 )
                {
                    printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
                }

                //Initialize OpenGL
                if( !initGL() )
                {
                    printf( "Unable to initialize OpenGL!\n" );
                    success = false;
                }
            }
        }
    }

    return success;
}


void handleKeys( unsigned char key, int x, int y )
{
    //Toggle quad
    if (key == 'q')
    {
        g_state.render_quad = !g_state.render_quad;
    }
}

void update()
{
    //No per frame update needed
}

void render()
{
    //Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Render quad
    if (g_state.render_quad)
    {
        vec2 pos  = {100, 100};
        vec2 size = {300, 400};
        graphics::draw_rect(pos, size);
    }
}

int main()
{
    init();
    SDL_StartTextInput();
    bool quit = false;
    while (!quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0 )
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_TEXTINPUT)
            {
                int x = 0, y = 0;
                SDL_GetMouseState(&x, &y);
                handleKeys(e.text.text[0], x, y);
            }
            else if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    printf("%s\n", __FUNCTION__);
                    g_state.window_width  = e.window.data1;
                    g_state.window_height = e.window.data2;
                }
            }
        }
        render();
        SDL_GL_SwapWindow(g_state.window);
    }
    SDL_StopTextInput();
    return 0;
}
