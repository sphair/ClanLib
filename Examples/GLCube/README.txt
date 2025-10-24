         Name: GLCube
       Status: Windows(Y), Linux(Y)
        Level: Intermediate
   Maintainer: Core developers
      Summary: GL

The GL example shows how to use GL calls from within your ClanLib appplication.
Notice the call to CL_SetupGL::init(); before CL_SetupDisplay::init();. This makes
sure that a correct OpenGL context is being created for your application.

You should see a spinning pyramide (on the left side) and cube (on the right).
