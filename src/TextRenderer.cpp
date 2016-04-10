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

#ifdef pl_andr
#include <asset_manager.h>
#include <android/asset_manager_jni.h>
#include <jni.h>
#endif

#include "TextRenderer.h"
#include "Text.h"
#include "View.h"
#include "Screen.h"
#include "GL.h"
#include "Ui.h"
using namespace std;
using namespace ui;


// -- VAR ------
//TextRenderer::CharacterInfo charInfo[128];
list<TextRenderer::Font*> TextRenderer::fonts;




// ###########################################
// -- CREATE OBJEKT ------------------
TextRenderer::TextRenderer(Text *view) : Renderer(view) 
{
    setLogName("TEXT");
    this->font = NULL;
    VERTEX_BUFFER_TEXT = 0;
}


// ###########################################
// ## FONT ###################################
TextRenderer::Font::Font(string name, float size)
{
    setLogName("FONT");
    int error;

    this->name = name;
    this->size = size;

#if !defined pl_andr
    // load font face
    error = FT_New_Face( GL::ftLib, name.c_str(), 0, &ftFace );
#endif


    // @TODO other font loading on android
#ifdef pl_andr
    trace("going to find class ... ");

    // retrieve the JNI environment.
    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();

    // retrieve the Java instance of the SDLActivity
    jobject activity = (jobject)SDL_AndroidGetActivity();

    // find the Java class of the activity. It should be SDLActivity or a subclass of it.
    jclass sdlClass(env->GetObjectClass(activity));

    ok("found class");



    jfieldID assman = env->GetStaticFieldID(sdlClass,
            "mAssetMgr", "Landroid/content/res/AssetManager;");

    if (assman == 0)
        err("could not find mAssetMgr");


    jobject assets = env->GetStaticObjectField(sdlClass, assman);

    if (assets == 0)
        err("could not get mAssetMgr");

    AAssetManager* manager = AAssetManager_fromJava((env), assets);
    ok("get AAssetManager");

    AAsset* fontFile = AAssetManager_open(manager, name.c_str(), AASSET_MODE_BUFFER);
    // const void* fontData = AAsset_getBuffer(fontFile);
    off_t fontLen = AAsset_getLength(fontFile);
    unsigned char* buffer = (unsigned char*)malloc (sizeof(unsigned char)*fontLen);
    memset (buffer, 0, fontLen*sizeof(unsigned char));
    AAsset_read (fontFile, buffer, fontLen);
    AAsset_close (fontFile);

    ok("font file");

    error = FT_New_Memory_Face( GL::ftLib,
                                    buffer,    /* first byte in memory */
                                    fontLen,      /* size in bytes        */
                                    0,         /* face_index           */
                                    &ftFace );
#endif

    out("create '" + name + "' size: " + str(size), error, "");

    ftGlyph = ftFace->glyph;

    // recalc
    calcTextSize(size);
}

// -- CALC TEXT - TEXTURE ATLAS -> CONTAINS CHARACTER BITMAPS
void TextRenderer::Font::calcTextTexAtlas()
{
    bool error = false;
    int  textureHight = 0, textureWidht = 0;

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
            err("calcTextTexAtlas FT_Load_Char " + str(i));
            continue;
        }

        // textureHight, widht
        textureWidht += ftGlyph->bitmap.width;
        textureHight  = max(textureHight, (int)ftGlyph->bitmap.rows);
    }


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
            err("calcTextTexAtlas FT_Load_Char");
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
        charInfo[i].texHeight = ftGlyph->bitmap.rows;
        charInfo[i].texWidth = ftGlyph->bitmap.width;
        charInfo[i].marginLeft = ftGlyph->bitmap_left;
        charInfo[i].marginTop  = ftGlyph->bitmap_top;
        charInfo[i].advanceX   = ftGlyph->advance.x >> 6;
        charInfo[i].advanceY   = ftGlyph->advance.y >> 6;


        // move cursor to next position
        xCursor += ftGlyph->bitmap.width;
    }

    // print
    if (error)
        err("calcTextTexAtlas");

    // set row hight to max hight of font
    fontRowHeight = textureHight;
    fontRowWidth = textureWidht;
}

// -- CALC TEXT SIZE -------
void TextRenderer::Font::calcTextSize(float size)
{
    int error;

    // set font size
    error = FT_Set_Pixel_Sizes(ftFace, 0, size);
    if ( error )
        err("set textSize to " + str(size));

    // recalc TextAtlas
    calcTextTexAtlas();
}

// ###########################################


// -- CALC TEXT --------------------------------
void TextRenderer::calcText() 
{
    // @TODO make calc text working
    // var
    const char *text = ((Text*)view)->text_str.c_str(); 
    const char *p;
    float contendHeight = 0;
    int x = layoutAttributes.paddingLeft->floatValue;
    int y = -font->fontRowHeight - layoutAttributes.paddingTop->floatValue;
    int oldNumVertices = numVertices;


    // Vertex
    struct Vertex
    {
        GLfloat x, y,
                u, v;
    }coords[6 * strlen(text)];
    
    charAmount = strlen(text);
    numVertices = 0;
    
    // calc lines -> for each character
    for(p = text; *p; p++) {

        // check for new line
        if (*p == '\n' || ((x + font->charInfo[*p].advanceX) > (renderAttributes.width - layoutAttributes.paddingRight->floatValue - layoutAttributes.paddingLeft->floatValue)))
        {
            // new line
            x = layoutAttributes.paddingLeft->floatValue; // to row start
            y-= font->fontRowHeight;     // one line deeper

            // add line to contend height
            contendHeight += font->fontRowHeight;
        }


        float x2 = x +  font->charInfo[*p].marginLeft; //ftGlyph->bitmap_left
        float y2 = -y - font->charInfo[*p].marginTop;  //ftGlyph->bitmap_top
        float w =       font->charInfo[*p].texWidth;   //ftGlyph->bitmap.width
        float h =       font->charInfo[*p].texHeight;   //ftGlyph->bitmap.rows


        coords[numVertices++] = (Vertex){x2,       -y2    ,    font->charInfo[*p].texPosX /font->fontRowWidth,                                     0};
        coords[numVertices++] = (Vertex){x2 + w,   -y2    ,    (font->charInfo[*p].texPosX + font->charInfo[*p].texWidth) / font->fontRowWidth,    0};
        coords[numVertices++] = (Vertex){x2,       -y2 - h,    font->charInfo[*p].texPosX / font->fontRowWidth,                                    font->charInfo[*p].texHeight / font->fontRowHeight}; //each glyph occupies a different amount of vertical space
        coords[numVertices++] = (Vertex){x2 + w,   -y2    ,    (font->charInfo[*p].texPosX + font->charInfo[*p].texWidth) / font->fontRowWidth,    0};
        coords[numVertices++] = (Vertex){x2,       -y2 - h,    font->charInfo[*p].texPosX / font->fontRowWidth,                                    font->charInfo[*p].texHeight / font->fontRowHeight};
        coords[numVertices++] = (Vertex){x2 + w,   -y2 - h,    (font->charInfo[*p].texPosX + font->charInfo[*p].texWidth) / font->fontRowWidth,    font->charInfo[*p].texHeight / font->fontRowHeight};


        x += font->charInfo[*p].advanceX;
        y += font->charInfo[*p].advanceY;
    }

    // add last line to contend height
    contendHeight += font->fontRowHeight;

    // add padding to contend height
    contendHeight = contendHeight + layoutAttributes.paddingTop->floatValue + layoutAttributes.paddingBottom->floatValue;


    // -- into buffer --------------------------------
    // if vertex buffer not generated or size changed
    if ((VERTEX_BUFFER_TEXT <= 0) || (oldNumVertices != numVertices))
    {
        // delete old
        glDeleteBuffers(1, &VERTEX_BUFFER_TEXT);
        // debug("create new vertices buffer with size " + str(sizeof(coords)) + "bytes");

        // create vertex buffer
        glGenBuffers(1, &VERTEX_BUFFER_TEXT);
        glBindBuffer(GL_ARRAY_BUFFER,VERTEX_BUFFER_TEXT);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coords), NULL, GL_DYNAMIC_DRAW /* often changed and used for rendering */ );
        // @TODO: dynamic vertex buffer size for text
    }
    else
        glBindBuffer(GL_ARRAY_BUFFER,VERTEX_BUFFER_TEXT);

    // insert data
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), &coords);
    //debug("buffer "+str(sizeof(coords))+"  bytes");

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // done
    // cout << "[DONE] calcText of '"<< view->id << ", " << view->class_ <<"'" << endl;
    calcTasks[UI_CALCTASK_TEXT_TEXT] = false;
    renderAttributes.contendHeight = contendHeight;

}


// -- CALC TEXT FAMILY -----
void TextRenderer::calcTextFamily()
{
    bool found = false;

    // find font
    for (Font *font : fonts)
    {
        if ((font->name.compare(renderAttributes.text_family->stringValue) == 0) && (font->size == renderAttributes.text_size->floatValue))
        {
            this->font = font;
            //ok("found font!");
            found = true;
            break;
        }

        //debug("compare " +str((font->name.compare(renderAttributes.text_family->stringValue))));
    }

    if (!found)
    {
        // not found -> create
        this->font = new Font(renderAttributes.text_family->stringValue, renderAttributes.text_size->floatValue);
        fonts.push_back(this->font);
    }


    /*
    int error;
    
#if !defined pl_andr
    // load font face
    error = FT_New_Face( GL::ftLib, renderAttributes.text_family->stringValue.c_str(), 0, &ftFace );
#endif

    
    // @TODO other font loading on android
#ifdef pl_andr
    trace("going to find class ... ");

    // retrieve the JNI environment.
    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();

    // retrieve the Java instance of the SDLActivity
    jobject activity = (jobject)SDL_AndroidGetActivity();

    // find the Java class of the activity. It should be SDLActivity or a subclass of it.
    jclass sdlClass(env->GetObjectClass(activity));

    ok("found class");
    
    
    
    jfieldID assman = env->GetStaticFieldID(sdlClass,
            "mAssetMgr", "Landroid/content/res/AssetManager;");

    if (assman == 0)
        err("could not find mAssetMgr");


    jobject assets = env->GetStaticObjectField(sdlClass, assman);

    if (assets == 0)
        err("could not get mAssetMgr");

    AAssetManager* manager = AAssetManager_fromJava((env), assets);
    ok("get AAssetManager");
    
    AAsset* fontFile = AAssetManager_open(manager, renderAttributes.text_family->stringValue.c_str(), AASSET_MODE_BUFFER);
    // const void* fontData = AAsset_getBuffer(fontFile);
    off_t fontLen = AAsset_getLength(fontFile);
    unsigned char* buffer = (unsigned char*)malloc (sizeof(unsigned char)*fontLen);
    memset (buffer, 0, fontLen*sizeof(unsigned char));
    AAsset_read (fontFile, buffer, fontLen);
    AAsset_close (fontFile);

    ok("font file");
    
    error = FT_New_Memory_Face( GL::ftLib,
                                    buffer,    /* first byte in memory *-/
                                    fontLen,      /* size in bytes        *-/
                                    0,         /* face_index           *-/
                                    &ftFace );
#endif

    out("load font face", error, "");
      
    ftGlyph = ftFace->glyph;
    
    // recalc TextAtlas
    addCalcTask(UI_CALCTASK_TEXT_ATLAS);
*/
    // done
    calcTasks[UI_CALCTASK_TEXT_FAMILY] = false;

}

// -- CALC TEXT SIZE -------
void TextRenderer::calcTextSize() 
{
    bool found = false;

    // find font
    for (Font *font : fonts)
    {
        if ((font->name.compare(renderAttributes.text_family->stringValue) == 0) && (font->size == renderAttributes.text_size->floatValue))
        {
            this->font = font;
            //ok("found font!");
            found = true;
            break;
        }

        //debug("compare " +str((font->name.compare(renderAttributes.text_family->stringValue))));
    }

    if (!found)
    {
        // not found -> create
        this->font = new Font(renderAttributes.text_family->stringValue, renderAttributes.text_size->floatValue);
        fonts.push_back(this->font);
    }

    /*
    int error;
    
    // set font size
    error = FT_Set_Pixel_Sizes(ftFace, 0, renderAttributes.text_size->intValue);
//    if ( error )
//      cout << "[TEXT] set textSize to " << renderAttributes.text_size->intValue << " [ERR]" << endl;
//    else
//      cout << "[TEXT] set textSize to " << renderAttributes.text_size->intValue << " [OK]" << endl;
    
        
    // recalc TextAtlas
    addCalcTask(UI_CALCTASK_TEXT_ATLAS);
     */
    // done
    calcTasks[UI_CALCTASK_TEXT_SIZE] = false;
}


// -- ADD CALC TASK
void TextRenderer::addCalcTask(int type) 
{
    Renderer::addCalcTask(type);
    
    // check for own possible task types
    switch (type)
    {
        case UI_CALCTASK_LAYOUT_SIZE:
            calcTasks[UI_CALCTASK_TEXT_TEXT] = true;
            break;

        case UI_CALCTASK_TEXT_SIZE:
        case UI_CALCTASK_TEXT_FAMILY:
        //case UI_CALCTASK_TEXT_ATLAS:
        case UI_CALCTASK_TEXT_TEXT:
            calcTasks[type] = true;
            break;
    }
}


// -- EXE CALC TAKKS
int  TextRenderer::exeCalcTasks()
{
    if(calcTasks[UI_CALCTASK_LAYOUT_SIZE])
        calcLayoutSize();

    if (calcTasks[UI_CALCTASK_TEXT_FAMILY])
        calcTextFamily();

    if (calcTasks[UI_CALCTASK_TEXT_SIZE])
        calcTextSize();

    //if (calcTasks[UI_CALCTASK_TEXT_ATLAS])
        //calcTextTexAtlas();

    if (calcTasks[UI_CALCTASK_TEXT_TEXT])
        calcText();

    if (calcTasks[UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND])
        calcLayoutSizeAutoContend();

    if (calcTasks[UI_CALCTASK_LAYOUT_SIZE_VERTICES])
        calcLayoutSizeVertices();

    
    return 0;
}




// -- RENDER -----------------------------------------
void TextRenderer::render()
{
    // render self box
    Renderer::render();

    if (!font)
        return;

    // ------ DRAW TEXT -----------------------------------------
    //glBindTexture(GL_TEXTURE_2D, HANDEL_TEXTURE);
    
    
    
    // bind view background shader
    glUseProgram(GL::SHADER_TEXT_CHARACTER);

    glBindBuffer(GL_ARRAY_BUFFER, VERTEX_BUFFER_TEXT);

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
                                            *GL::transformMatrix;
    
    glm::mat4 model     = GL::projectionMatrix * transform;
    
    glUniformMatrix4fv(GL::SHADER_TEXT_CHARACTER_UNIF_TRANSFORM_MATIX,
                       1                /* amount of matrix */,
                       GL_FALSE         /* convert format -> NO */,
                       &model[0][0]);
    
    //GL::transformMatrix = transform;
    
    
    // set shader color
    glUniform4f(GL::SHADER_TEXT_CHARACTER_UNIF_COLOR,
                renderAttributes.text_color->r,
                renderAttributes.text_color->g,
                renderAttributes.text_color->b,
                renderAttributes.text_color->alpha);
    

    // give gl the Vertices via buffer
    glVertexAttribPointer(
            GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS       /* pass vertices to vertex Pos attribute of vertexShader */,
            4                                               /* 3 Arguments: x,y,z */,
            GL_FLOAT                                        /* Format: float     */,
            GL_FALSE                                        /* take values as-is */,
            0                                               /* Entry length ?    */,
            NULL                                            /* vertices Array    */ );

    glBindTexture(GL_TEXTURE_2D, font->HANDEL_TEXTURE_ATLAS);

    // draw with the vertices form the given Array
    // make two connected triangle to create a rectangle
    glDrawArrays(
            GL_TRIANGLES,
            0 /* Array start pos   */,
            numVertices /* how much vertices */);


    glBindTexture(GL_TEXTURE_2D, 0);

    // deactivate vertex Array mode
    // => end of operation
    glDisableVertexAttribArray(GL::SHADER_TEXT_CHARACTER_ATTR_VERTEX_POS);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind shader
    glUseProgram(0);
}



// ###########################################
// -- DESTROY OBJEKT -----------
TextRenderer::~TextRenderer() {
}

