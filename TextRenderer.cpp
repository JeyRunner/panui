/* 
 * File:   TextRenderer.cpp
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * TEXTRENDERER CLASS
 * render TEXT(VIEW)
 * extends RENDERER
 * ---------------------------------------
 */


#include "TextRenderer.h"
#include "Text.h"
#include "View.h"
#include "Screen.h"
#include "GL.h"
using namespace std;

// -- VAR ------
//TextRenderer::CharacterInfo charInfo[128];




// ###########################################
// -- CREATE OBJEKT ------------------
TextRenderer::TextRenderer(Text *view) : Renderer(view) 
{
}



// -- CALC TEXT --------------------------------
void TextRenderer::calcText() 
{
    // @TODO make calc text working
    // var
    const char *text = ((Text*)view)->text_str.c_str(); 
    const char *p; 
    int x = 0;
    int y = -fontRowHight;
    
    // Vertex
    struct Vertex
    {
        GLfloat x, y,
                u, v;
    }coords[6 * strlen(text)];
    
    charAmount = strlen(text);

    
    // delete old buffer
    // glDeleteBuffers(1, &HANDEL_VERTEX_BUFFER);
   
    
    int n = 0;
    // calc lines -> for each character
    for(p = text; *p; p++) {
        
      // check for new line
      if (*p == '\n' || ((x + charInfo[*p].advanceX) > renderAttributes.width))
      {
          // new line 
          x = 0;                // to row start
          y-= fontRowHight;     // one line deeper
      }  
        

      float x2 = x +  charInfo[*p].marginLeft; //ftGlyph->bitmap_left
      float y2 = -y - charInfo[*p].marginTop;  //ftGlyph->bitmap_top
      float w =       charInfo[*p].texWidht;   //ftGlyph->bitmap.width
      float h =       charInfo[*p].texHight;   //ftGlyph->bitmap.rows

      
      coords[n++] = (Vertex){x2,     -y2    ,           charInfo[*p].texPosX,                                                   0};
      coords[n++] = (Vertex){x2 + w, -y2    ,           charInfo[*p].texPosX + charInfo[*p].texWidht / fontRowWidht,         0};
      coords[n++] = (Vertex){x2,     -y2 - h,           charInfo[*p].texPosX,                                                charInfo[*p].texHight / fontRowHight}; //remember: each glyph occupies a different amount of vertical space
      coords[n++] = (Vertex){x2 + w, -y2    ,           charInfo[*p].texPosX + charInfo[*p].texWidht / fontRowWidht,         0};
      coords[n++] = (Vertex){x2,     -y2 - h,           charInfo[*p].texPosX,                                                charInfo[*p].texHight / fontRowHight};
      coords[n++] = (Vertex){x2 + w, -y2 - h,           charInfo[*p].texPosX + charInfo[*p].texWidht / fontRowWidht,         charInfo[*p].texHight / fontRowHight};

//      GLfloat box[4][4] = {
//          {x2,     -y2    ,  charInfo[*p].texPosX                               / fontRowWidht, 0},
//          {x2 + w, -y2    ,  (charInfo[*p].texPosX + charInfo[*p].texWidht)     / fontRowWidht, 0},
//          {x2,     -y2 - h,  charInfo[*p].texPosX                               / fontRowWidht, charInfo[*p].texHight / fontRowHight},
//          {x2 + w, -y2 - h,  (charInfo[*p].texPosX + charInfo[*p].texWidht)     / fontRowWidht, charInfo[*p].texHight / fontRowHight},
//      };

//      // give gl the Vertices via array
//        glVertexAttribPointer(
//            GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS       /* pass vertices to vertex Pos attribute of vertexShader */,
//            4                                               /* 3 Aruments: x,y,z */,
//            GL_FLOAT                                        /* Format: float     */,
//            GL_FALSE                                        /* take values as-is */,   
//            0                                               /* Entry lenght ?    */,
//            box                                             /* vertices Array    */ );
//
//        // draw with the vertices form the given Array
//        // make two connected triangle to create a rectangle
//        glDrawArrays(
//            GL_TRIANGLE_STRIP,
//            0 /* Array start pos   */,
//            4 /* how much vertices */);

      x += charInfo[*p].advanceX;
      y += charInfo[*p].advanceY;
    }
    n = 0;
    
//    coords[n++] = (Vertex){ 0,0, 0,0 };
//    coords[n++] = (Vertex){ 0,10, 0,0 };
//    coords[n++] = (Vertex){ 10,10, 0,0 };
//    
//    coords[n++] = (Vertex){ 0,0, 0,0 };
//    coords[n++] = (Vertex){ 10,10, 0,0 };
//    coords[n++] = (Vertex){ 0,10, 0,0 };
    
    // recrate buffer
//    glGenBuffers(1, &HANDEL_VERTEX_BUFFER);  
//    
//    glBindBuffer(GL_ARRAY_BUFFER, HANDEL_VERTEX_BUFFER);
//    glBufferData(GL_ARRAY_BUFFER,                                                       // type
//                 sizeof(coords),                                                        // size: num Characters * 6 Vertices per character * 4 Vaues per vertex * sizeof value
//                 coords,                                                                // data
//                 GL_DYNAMIC_DRAW);
//    
//    glVertexAttribPointer(GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS, 4, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS);
//    
//    
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //cout << "[TEXT] calc Text [OK]" << endl; 
}


// -- CALC TEXT FAMILY -----
void TextRenderer::calcTextFamily()
{
    int error;
    
    // load font face
    error = FT_New_Face( GL::ftLib, renderAttributes.text_family->stringValue.c_str(), 0, &ftFace );
    if ( error )
      cout << "[TEXT] load font face [ERR]" << endl;
    else
      cout << "[TEXT] load font face [OK]" << endl;
    
    ftGlyph = ftFace->glyph;
    
    // recalc TextAtlas
    addCalcTask(UI_CALCTASK_TEXT_ATLAS);
}

// -- CALC TEXT SIZE -------
void TextRenderer::calcTextSize() 
{
    int error;
    
    // set font size
    error = FT_Set_Pixel_Sizes(ftFace, 0, renderAttributes.text_size->intValue);
//    if ( error )
//      cout << "[TEXT] set textSize to " << renderAttributes.text_size->intValue << " [ERR]" << endl;
//    else
//      cout << "[TEXT] set textSize to " << renderAttributes.text_size->intValue << " [OK]" << endl;
    
        
    // recalc TextAtlas
    addCalcTask(UI_CALCTASK_TEXT_ATLAS);
}

// -- CALC TEXT - TEXTURE ATLAS -> CONTAINS CHARACTER BITMAPS
void TextRenderer::calcTextTexAtlas() 
{
    bool error = false;
    int  textureHight = 0,
         textureWidht = 0;
    
    // fill characterInfo width 0
    for (int i=0; i<128; i++)
    { charInfo[i] = {0, 0, 0, 0, 0}; }
    
    
    // get length of all characters together
    // get highest hight of characters
    // -> texture_atlas dimensions
    for (int i=32 /* not visible, control characters 0-32 */; i<128; i++)
    {
        // load character        
        if (FT_Load_Char(ftFace, i /* character number (ascii) */, FT_LOAD_RENDER)) {
            error = true;
            cout << "[TEXT] calcTextTexAtlas FT_Load_Char " << i << " '" << to_string((char)i) << "' [ERR]" << endl;
            continue; 
        }
        
        // textureHight, widht
        textureWidht += ftGlyph->bitmap.width;
        textureHight  = max(textureHight, (int)ftGlyph->bitmap.rows);
    }
//    cout << "[TEXT] calcTextTexAtlas  hight: " << textureHight << ", widht: " << textureWidht << endl;
    
    
    // delate old texture
    glDeleteTextures(1, &HANDEL_TEXTURE_ATLAS);
    
    // create texture --------------
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &HANDEL_TEXTURE_ATLAS);
    glBindTexture(GL_TEXTURE_2D, HANDEL_TEXTURE_ATLAS);
    
    glUniform1i(GL::SHADER_TEXT_CHARACTER_UNIF_TEXTURE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, textureWidht, textureHight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
    
    
    
    // paste all characters site by site into texture --------
    int xCursor = 0;
    
    for (int i=32 /* not visible, control characters */; i<128; i++)
    {
        // load character
        if (FT_Load_Char(ftFace, i /* character number (asci) */, FT_LOAD_RENDER)) {
            error = true;
            cout << "[TEXT] calcTextTexAtlas FT_Load_Char [ERR]" << endl;
            continue;
        }
        
        
        // load bitmap into texture -------------------
        glTexSubImage2D(GL_TEXTURE_2D, 0, 
                        xCursor               /* x-offset */,   0                       /* y-offset */, 
                        ftGlyph->bitmap.width /* width */,      ftGlyph->bitmap.rows    /* hight */,
                        GL_ALPHA,                               GL_UNSIGNED_BYTE,       /* format */
                        ftGlyph->bitmap.buffer /* pixels */);
        
        
        // set charInfo -------------------------------
        charInfo[i].texPosX    = xCursor;
        charInfo[i].texHight   = ftGlyph->bitmap.rows;
        charInfo[i].texWidht   = ftGlyph->bitmap.width;        
        charInfo[i].marginLeft = ftGlyph->bitmap_left;
        charInfo[i].marginTop  = ftGlyph->bitmap_top;
        charInfo[i].advanceX   = ftGlyph->advance.x >> 6;
        charInfo[i].advanceY   = ftGlyph->advance.y >> 6;
        
        
        // move cursor to next position
        xCursor += ftGlyph->bitmap.width;
    }
    
    
//    // print error
//    if (error)
//        cout << "[TEXT] calcTextTexAtlas [ERR] " << error << endl;
//    else
//        cout << "[TEXT] calcTextTexAtlas [OK]" << error << endl;
    
    
    // set row hight to max hight of font
    fontRowHight = textureHight;
    fontRowWidht = textureWidht;
}


// -- ADD CALC TASK
void TextRenderer::addCalcTask(int type) 
{
    Renderer::addCalcTask(type);
    
    // check for own possible task types
    switch (type)
    {
        case UI_CALCTASK_TEXT_SIZE:
        case UI_CALCTASK_TEXT_FAMILY:
        case UI_CALCTASK_TEXT_ATLAS:
        case UI_CALCTASK_TEXT_TEXT:
            calcTasks[type] = true;
    }
}


// -- EXE CALC TAKKS
int  TextRenderer::exeCaclTasks() 
{
    int lastTask = Renderer::exeCaclTasks();
    int i;
    
    // for each calc task
    for (i =lastTask; i <= UI_CALCTASK__SIZE+1; i++)
    {
        // if true 
        if (calcTasks[i])
        {
//            cout << "[TEXT] execute calc task num." << i << endl;
            
            // -> execute
            switch (i)
            {
                case UI_CALCTASK_TEXT_FAMILY:
                    calcTextFamily();
                    calcTasks[i] = false;
                    break;
                    
                case UI_CALCTASK_TEXT_SIZE:
                    calcTextSize();
                    calcTasks[i] = false;
                    break;   
                    
                case UI_CALCTASK_TEXT_ATLAS:
                    calcTextTexAtlas();
                    calcTasks[i] = false;
                    break;   
                    
                case UI_CALCTASK_TEXT_TEXT:
                    calcText();
                    calcTasks[i] = false;
                    break;  
            }
        }
    }
    
    return i;
}




//// -- CREATE VETEX BUFFER ----------------------------------
//void TextRenderer::createBuffer() 
//{
//    glGenBuffers(1, HANDEL_VERTEX_BUFFER);
//    glBindBuffer(GL_ARRAY_BUFFER, HANDEL_VERTEX_BUFFER);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(((Text*)view)->text_str.c_str() * 6 * sizeof(GLfloat)), NULL, GL_DYNAMIC_DRAW);
//    
//    cout << "[TEXT] create buffer [OK]" << endl;
//    bufferCreated = true;
//}



bool init=true;
// -- RENDER -----------------------------------------
void TextRenderer::render()
{
    // render self box
    Renderer::render();

    // ------ DRAW TEXT -----------------------------------------
    //glBindTexture(GL_TEXTURE_2D, HANDEL_TEXTURE);
    
    
    
    // bind view background shader
    glUseProgram(GL::SHADER_TEXT_CHARACTER);
    
    // draw a rectangle ----------------------------------------------
    // activate for client (to draw, not calculate) vertex Array
    // => tell gl we want to draw a something with a vertex Array -> to vertexPos attribute of vertexShader
    // => kind  of glBeginn()
    glEnableVertexAttribArray(GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS);
    
    // set attributes / uniformes  
    // set transform and projection matix
    // remove cursor -> to own position
    glm::mat4 transform =  glm::translate(glm::vec3( -(renderAttributes.width  /2 ) /* X */,
                                                     +(renderAttributes.height /2)  /* Y */,
                                                     +0.0f                                      /* Z */ ))
                                            *GL::transfomMatix;
    
    glm::mat4 model     = GL::projectionMatix * transform;
    
    glUniformMatrix4fv(GL::SHADER_TEXT_CHARACTER_UNIF_TRANSFORM_MATIX,
                       1                /* amount of matrix */,
                       GL_FALSE         /* convert format -> NO */,
                       &model[0][0]);
    
    //GL::transfomMatix = transform;
    
    
    // set shader color
    glUniform4f(GL::SHADER_TEXT_CHARACTER_UNIF_COLOR,
                renderAttributes.text_color->r,
                renderAttributes.text_color->g,
                renderAttributes.text_color->b,
                renderAttributes.text_color->alpha);
    
    
    
    // -- Test -------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, HANDEL_TEXTURE_ATLAS);
   
//    FT_Load_Char(ftFace, 'Z', FT_LOAD_RENDER);
//    
//    
//    glTexSubImage2D(
//        GL_TEXTURE_2D,
//        0,
//        0, 0,
//        ftGlyph->bitmap.width,
//        ftGlyph->bitmap.rows,
//        GL_ALPHA,
//        GL_UNSIGNED_BYTE,
//        ftGlyph->bitmap.buffer
//      );
//    
//     glTexSubImage2D(
//        GL_TEXTURE_2D,
//        0,
//        ftGlyph->bitmap.width, 0,
//        ftGlyph->bitmap.width,
//        ftGlyph->bitmap.rows,
//        GL_ALPHA,
//        GL_UNSIGNED_BYTE,
//        ftGlyph->bitmap.buffer
//      );
//    glTexImage2D(
//        GL_TEXTURE_2D,
//        0,
//        GL_ALPHA,
//        ftGlyph->bitmap.width,
//        ftGlyph->bitmap.rows,
//        0,
//        GL_ALPHA,
//        GL_UNSIGNED_BYTE,
//        ftGlyph->bitmap.buffer
//      );
    
//      float x2 = 0;
//      float y2 = fontRowHight*2;
//      float h = fontRowHight;
//      float w = fontRowWidht;
//
//      GLfloat box[4][4] = {
//          {x2,     -y2    , 0, 0},
//          {x2 + w, -y2    , 1, 0},
//          {x2,     -y2 - h, 0, 1},
//          {x2 + w, -y2 - h, 1, 1},
//      };
//
//      // give gl the Vertices via array
//        glVertexAttribPointer(
//            GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS       /* pass vertices to vertex Pos attribute of vertexShader */,
//            4                                               /* 3 Aruments: x,y,z */,
//            GL_FLOAT                                        /* Format: float     */,
//            GL_FALSE                                        /* take values as-is */,   
//            0                                               /* Entry lenght ?    */,
//            box                                             /* vertices Array    */ );
//
//        // draw with the vertices form the given Array
//        // make two connected triangle to create a rectangle
//        glDrawArrays(
//            GL_TRIANGLE_STRIP,
//            0 /* Array start pos   */,
//            4 /* how much vertices */);
    // -- Test ------------------------------------------------- END -
    
    //glBindTexture(GL_TEXTURE_2D, HANDEL_TEXTURE_ATLAS);
    // ---------------------------------------------------------------
    // -- draw characters ----------------------------cv----------------
    
//    glBindBuffer(GL_ARRAY_BUFFER, HANDEL_VERTEX_BUFFER);
//    glDrawArrays(GL_TRIANGLES, 0, charAmount);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    if (init)
//    {
//    
//        glGenBuffers(1, &HANDEL_VERTEX_BUFFER);
////        glEnableVertexAttribArray(GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS);
////        glBindBuffer(GL_ARRAY_BUFFER, GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS);
////        glVertexAttribPointer(GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS, 4, GL_FLOAT, GL_FALSE, 0, 0);
//        init = false;
//        cout << "[TEXT] gen Buffer [OK]" << endl;
//    }
//    glBindBuffer(GL_ARRAY_BUFFER, GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS);
//    glVertexAttribPointer(GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS, 4, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS);
    
    int x = 0;
    int y = -fontRowHight;
    const char *p; 
 
    // render lines -> for each character
    for(p = ((Text*)view)->text_str.c_str(); *p; p++) {
        
      // check for new line
      if (*p == '\n' || ((x + charInfo[*p].advanceX) > renderAttributes.width))
      {
          // new line 
          x = 0;                // to row start
          y-= fontRowHight;     // one line deeper
      }  
        

      float x2 = x +  charInfo[*p].marginLeft; //ftGlyph->bitmap_left
      float y2 = -y - charInfo[*p].marginTop;  //ftGlyph->bitmap_top
      float w =       charInfo[*p].texWidht;   //ftGlyph->bitmap.width
      float h =       charInfo[*p].texHight;   //ftGlyph->bitmap.rows

      GLfloat box[4][4] = {
          {x2,     -y2    ,  charInfo[*p].texPosX                               / fontRowWidht, 0},
          {x2 + w, -y2    ,  (charInfo[*p].texPosX + charInfo[*p].texWidht)     / fontRowWidht, 0},
          {x2,     -y2 - h,  charInfo[*p].texPosX                               / fontRowWidht, charInfo[*p].texHight / fontRowHight},
          {x2 + w, -y2 - h,  (charInfo[*p].texPosX + charInfo[*p].texWidht)     / fontRowWidht, charInfo[*p].texHight / fontRowHight},
      };

      
//      glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
//      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      // give gl the Vertices via array
        glVertexAttribPointer(
            GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS       /* pass vertices to vertex Pos attribute of vertexShader */,
            4                                               /* 3 Arguments: x,y,z */,
            GL_FLOAT                                        /* Format: float     */,
            GL_FALSE                                        /* take values as-is */,   
            0                                               /* Entry length ?    */,
            box                                             /* vertices Array    */ );

        // draw with the vertices form the given Array
        // make two connected triangle to create a rectangle
        glDrawArrays(
            GL_TRIANGLE_STRIP,
            0 /* Array start pos   */,
            4 /* how much vertices */);

      x += charInfo[*p].advanceX;
      y += charInfo[*p].advanceY;
    }
    

//    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // ---------------------------------------------------------------
    // ---------------------------------------------------------------
    
    
    // deactivate vertex Array mode
    // => end of operation
    glDisableVertexAttribArray(GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS);
    
    // unbind shader
    glUseProgram(0);
    
    

    
    
    /*
    // draw a rectangle ----------------------------------------------
    // activate for client (to draw, not calculate) vertex Array
    // => tell gl we want to draw a something with a vertex Array -> to vertexPos attribute of vertexShader
    // => kind  of glBeginn()
    glEnableVertexAttribArray(HANDEL_ATTRIBUTE_VERTEXPOS);
    
    // give gl the Vertices via array
    glVertexAttribPointer(
        HANDEL_ATTRIBUTE_VERTEXPOS   /* pass vertices to vertex Pos attribute of vertexShader *-/,
        3                            /* 3 Aruments: x,y,z *-/,
        GL_FLOAT                     /* Format: float     *-/,
        GL_FALSE                     /* take values as-is *-/,   
        0                            /* Entry lenght ?    *-/,
        renderAttributes.vertices    /* vertices Array    *-/ );
    
    /* set the color of vertices
    glColor4f(
        0.5f     /* Red   *-/,
        0.5f     /* Green *-/,
        0.0f    /* Blue  *-/,
        1.0f /* Alpha *-/); *-/ 
    
    // draw with the vertices form the given Array
    // make two connected triangle to create a rectangle
    glDrawArrays(
        GL_TRIANGLE_STRIP,
        0 /* Array start pos   *-/,
        4 /* how much vertices *-/);
    
    // deactivate vertex Array mode
    // => end of operation
    glDisableVertexAttribArray(HANDEL_ATTRIBUTE_VERTEXPOS);
    */
   
    
}



// ###########################################
// -- DESTROY OBJEKT -----------
TextRenderer::~TextRenderer() {
}

