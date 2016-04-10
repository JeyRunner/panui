/* 
 * File:   GL.cpp
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * GL CLASS
 * basic OpenGl functionality
 * static class
 * ---------------------------------------
 */


//#include <GLES/gl.h>
#include "GL.h"
#include <sstream>

// static
GLint GL::SHADER_VIEW_BACKGROUND,
      GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS,
      GL::SHADER_VIEW_BACKGROUND_UNIF_TRANSFORM_MATIX,
      GL::SHADER_VIEW_BACKGROUND_UNIF_COLOR;

// -- SHADER_TEXT_CHARACTER
GLint GL::SHADER_TEXT_CHARACTER,
      GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS,
      GL::SHADER_TEXT_CHARACTER_UNIF_TRANSFORM_MATIX,
      GL::SHADER_TEXT_CHARACTER_UNIF_COLOR,
      GL::SHADER_TEXT_CHARACTER_UNIF_TEXTURE;

glm::mat4  GL::transfomMatix,
           GL::projectionMatix;

FT_Library GL::ftLib;
Log        GL::log;



// ###########################################
// -- CREATE OBJEKT ------------------
GL::GL()
{
    log.setLogName(" GL ");
}

// -- INIT PRE --------------------------------
void GL::initPre() 
{
    log.setLogName(" GL ");

    // -- init Freetype lib
    bool error = FT_Init_FreeType( &ftLib );
    log.out("init Freetype lib", error, "");
}


// -- INIT ------------------------------------
void GL::init() 
{
    // -- create view background shader ---------------------------------------------------
    createShader(/* -- vertexShader ---------------------------------------- */
                 "#version 100                                             \n" 
                 "attribute vec3 vertexPos;                                \n"
                 "uniform   mat4 transformMatrix;                          \n" // uniform can be set by cpu
                 "                                                         \n"
                 "void main() {                                            \n"
                 "                                                         \n"
                 "    gl_Position = transformMatrix * vec4(vertexPos, 1.0);\n"
                 "}                                                        \n",
            
                 /* -- fragmentShader ---------------------------------------- */
                 "#version 100                             \n" 
                 "precision highp float;                   \n" 
                 "uniform vec4 color;                      \n" // uniform can be set by cpu
                 "                                         \n"
                 "void main() {                            \n"
                 "                                         \n" 
                 "   gl_FragColor =  color;                \n"
                 "}                                        \n",
            
                 &SHADER_VIEW_BACKGROUND, 
                 &SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS, 
                 &SHADER_VIEW_BACKGROUND_UNIF_COLOR, 
                 &SHADER_VIEW_BACKGROUND_UNIF_TRANSFORM_MATIX);
    
    
    
    // -- create Text Character Shader ----------------------------------------------------
    createShader(/* -- vertexShader ---------------------------------------- */
                 "#version 100                                             \n" 
                 "attribute vec4 vertexPos;                                \n"
                 "uniform   mat4 transformMatrix;                          \n" // uniform can be set by cpu
                 "varying vec2 texcoord;                                   \n"
                 "                                                         \n"
                 "void main() {                                            \n"
                 "                                                         \n"
                 "    gl_Position = transformMatrix * vec4(vertexPos.xy, 0, 1.0);\n" // 
                 "    texcoord = vertexPos.zw;                             \n"
                 "}                                                        \n",
            
                 /* -- fragmentShader ---------------------------------------- */
                 "#version 100                             \n"  
                 "precision highp float;                   \n" 
                 "uniform vec4 color;                      \n" // uniform can be set by cpu
                 "varying vec2 texcoord;                   \n"
                 "uniform sampler2D tex;                   \n"
                 "                                         \n"
                 "void main() {                            \n"
                 "                                         \n" 
                 "   gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).a) * color; \n" // override alpha from tex with color    gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).a) * color;
                 "}                                        \n",
            
                 &SHADER_TEXT_CHARACTER, 
                 &SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS, 
                 &SHADER_TEXT_CHARACTER_UNIF_COLOR, 
                 &SHADER_TEXT_CHARACTER_UNIF_TRANSFORM_MATIX);
    
    // get extra attribute
//    SHADER_TEXT_CHARACTER_UNIF_TEXTURE = glGetU(SHADER_TEXT_CHARACTER, "tex");
//    if ( SHADER_TEXT_CHARACTER_UNIF_TEXTURE == -1  )
//        cout << "[ GL ] get Attribute/Uniform 'tex' [ERR]" << endl;
//    else 
//        cout << "[ GL ] get Attribute/Uniform 'tex' [OK]"  << endl;
//    
    
    
    
}


// -- CREATE SHADER ---------------------------
void GL::createShader(const GLchar* vertexShader, const GLchar* fragmentShader, GLint* shaderProgramHandle, GLint* shaderAttributeVertexPosHandle, GLint* shaderUniformColorHandle, GLint* shaderUniformTransformMatixHandle)
{
    GLchar prog, shaderVertex, shaderFragment, attrVertexPos, uniTransformMatix, uniColor;
    
    // create shader programm 
    prog = glCreateProgram();
    log.out("create shader program", (prog == 0), "");
    
    // create shader container --------
    shaderVertex   = glCreateShader(GL_VERTEX_SHADER);   // for vertext-shader
    shaderFragment = glCreateShader(GL_FRAGMENT_SHADER); // for fragment-shader
    log.out("create shader", (shaderVertex == 0  ||  shaderFragment == 0), "");
    
    // load shaders into char vars
    // => not necessary
    
    
    // load shader into container
    glShaderSource(shaderVertex,   1, &vertexShader,   NULL);
    glShaderSource(shaderFragment, 1, &fragmentShader, NULL);
    
    
    // compile shader
    glCompileShader(shaderVertex);
    glCompileShader(shaderFragment);    
    checkShaderError(shaderVertex,   false, GL_COMPILE_STATUS, "compile vertex shader");
    checkShaderError(shaderFragment, false, GL_COMPILE_STATUS, "compile fragment shader");
    
    
    // add shader to programm
    glAttachShader(prog, shaderVertex);
    glAttachShader(prog, shaderFragment);
    
    
    // bind shader programm to openGl
    glLinkProgram(prog);
    checkShaderError(prog, true, GL_LINK_STATUS, "link shader program");
    
    glValidateProgram(prog);
    checkShaderError(prog, true, GL_VALIDATE_STATUS, "validate shader program");
            
    
    // get attributes / uniformes
    attrVertexPos       = glGetAttribLocation( prog, "vertexPos");
    uniTransformMatix   = glGetUniformLocation(prog, "transformMatrix");  
    uniColor            = glGetUniformLocation(prog, "color");
    
    log.out( "get Attribute/Uniform",
             attrVertexPos     == -1
          || uniTransformMatix == -1
          || uniColor          == -1,
             "");
    
    
    // set var --
    *shaderProgramHandle                = prog;
    *shaderAttributeVertexPosHandle     = attrVertexPos;
    *shaderUniformColorHandle           = uniColor;
    *shaderUniformTransformMatixHandle  = uniTransformMatix;
}

// -- CHECK SHADER ERROR ----------------------
void GL::checkShaderError(GLuint shader, bool isProgram, GLint flag, string message) 
{
    // error var
    GLint  success         = GL_FALSE;
    GLchar errorText[1024] = { 0 };
    
    
    // is there an error
    if (isProgram)
    {
        glGetProgramiv(shader, flag, &success);
    } else
    {
        glGetShaderiv(shader, flag, &success);
    }
    
    
    // if there is error
    if (success == GL_FALSE)
    {
        if (isProgram)
            glGetProgramInfoLog(shader, sizeof(errorText), NULL, errorText);
        else
            glGetShaderInfoLog(shader, sizeof(errorText), NULL, errorText);
    }
    
    // out
    log.out(message, success == GL_FALSE, errorText);
}
