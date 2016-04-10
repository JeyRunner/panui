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

// include
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <list>
#include "Renderer.h"
namespace ui
{
class Text;

using namespace std;


/* TextRenderer Class
 */
class TextRenderer : public Renderer
{
    public:
        TextRenderer (Text *view);
        ~TextRenderer ();

        // calculate Text
        void calcText();
        void calcTextFamily();
        void calcTextSize();
        
        virtual void addCalcTask (int type);
        virtual int  exeCalcTasks();
        
        // render
        virtual void render ();
         
        // character position information
        struct CharacterInfo 
        {
          float texPosX;                // offset in textureAtlas
          float texHeight, texWidth;     // height, width
          float marginTop,marginLeft;   // to position character
          float advanceX, advanceY;
        };
        //CharacterInfo charInfo[128];
        int charAmount;

        // Font
        class Font : public Log
        {
            public:
            string name;
            float  size;
            GLuint HANDEL_TEXTURE_ATLAS;

            // freetype - text
            FT_GlyphSlot ftGlyph;
            FT_Face      ftFace;
            float        fontRowHeight,
                         fontRowWidth;

            Font(string name, float size);
            void calcTextTexAtlas(); // call when change text-family or size \n -> recalculate texture atlas that contains all character bitmaps
            void calcTextSize(float size);
            CharacterInfo charInfo[128];
        };

        // list of fonts
        static list<Font*> fonts;

        // own font
        Font *font;
        int   numVertices;


    private:
          GLuint  VERTEX_BUFFER_TEXT;
          GLuint  HANDEL_TEXTURE,
                  HANDEL_VERTEX_BUFFER;
};


};     /* END NAMESPACE */
#endif	/* TEXTRENDERER_H */

