/* 
 * File:   TextRenderer.h
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * TEXTRENDERER CLASS
 * render TEXT(VIEW)
 * extends RENDERER
 * ---------------------------------------
 */


#ifndef TEXTRENDERER_H
#define	TEXTRENDERER_H

#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//#include "GLES2/gl2.h"

//#include "GL.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Renderer.h"
class Text;
using namespace std;



class TextRenderer : public Renderer
{
    public:
        TextRenderer (Text *view);
        ~TextRenderer ();
        
        // calculate Text
        void calcText();
        void calcTextFamily();
        void calcTextSize();
        void calcTextTexAtlas(); // call when change text-family or size \n -> recalculate texture atlas that contains all character bitmaps
        
        virtual void addCalcTask (int type);
        virtual int  exeCaclTasks ();
        
        // render
        virtual void render ();
         
        // charakter position information
        struct CharacterInfo 
        {
          float texPosX;                // offset in textureAtlas
          float texHight, texWidht;     // hight, widht
          float marginTop,marginLeft;   // to position character
          float advanceX, advanceY;
        };
        CharacterInfo charInfo[128];
        int charAmount;
        
      
    private:
      
      // create vertexBuffer
//      void createBuffer();
//      bool bufferCreated;

      // freetype - text
      FT_GlyphSlot ftGlyph;
      FT_Face      ftFace;
      float        fontRowHeight,
                   fontRowWidht;
      
      GLuint       HANDEL_TEXTURE,
                   HANDEL_TEXTURE_ATLAS,
                   HANDEL_VERTEX_BUFFER;
};

#endif	/* TEXTRENDERER_H */

