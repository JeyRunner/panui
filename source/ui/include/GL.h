/* 
 * File:   GL.h
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * GL CLASS
 * basic OpenGl functionality
 * static class
 * ---------------------------------------
 */


#ifndef GL_H
#define	GL_H 

// include
#if defined pl_pi || defined ANDROID
#include <GLES2/gl2.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#ifndef pl_pi
#include <SDL.h>
#endif

#include <stdlib.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "Log.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
namespace ui
{

/* GL Class
 */
class GL : protected Log
{
    public:
      GL ();
      
      // var ------------------------------------------
      // handel
      // -- SHADER_VIEW_BACKGROUND
      static GLint SHADER_VIEW_BACKGROUND,
                   SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS,
                   SHADER_VIEW_BACKGROUND_UNIF_TRANSFORM_MATIX,
                   SHADER_VIEW_BACKGROUND_UNIF_COLOR;
      
      // -- SHADER_TEXT_CHARACTER
      static GLint SHADER_TEXT_CHARACTER,
                   SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS,
                   SHADER_TEXT_CHARACTER_UNIF_TRANSFORM_MATIX,
                   SHADER_TEXT_CHARACTER_UNIF_COLOR,
                   SHADER_TEXT_CHARACTER_UNIF_TEXTURE;
      
      // Transform Matrix
      static glm::mat4 transformMatrix,
              projectionMatrix;
      
      // Freetype
      static FT_Library ftLib;
      
      
      // function ------------------------------------
      
      /* initialisation of pre View creation stuff \n
       * create Freetype lib   */ 
      static void initPre(); 
      
      /* initialisation of gl \n
       * creates shaders      */ 
      static void init(); 
      
      /* create shaderProgram 
       * by vertex + Fragemnt shader */
      static void createShader(const GLchar *vertexShader, const GLchar *fragmentShader, GLint *shaderProgramHandle, GLint *shaderAttributeVertexPosHandle, GLint *shaderUniformColorHandle, GLint *shaderUniformTransformMatixHandle);

      // check if shader causes error
      static void checkShaderError(GLuint shader, bool isProgram, GLint flag, string message) ;


    private:
      static Log log;
};


};     /* END NAMESPACE */
#endif	/* GL_H */

