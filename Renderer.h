/* 
 * File:   Renderer.h
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * RENDERER CLASS
 * render VIEW
 * ---------------------------------------
 */


#ifndef RENDERER_H
#define	RENDERER_H

#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "GLES/gl.h"

//#include "View.h" // -> in StyleAttribute
class View; // => problems with circular including

#include "const.h"
#include "ColorValue.h"
#include "IntValue.h"
#include "FloatValue.h"
#include "StyleAttribute.h"
#include "StyleRule.h"

using namespace std;


class Renderer
{
    public:
      Renderer (View *view);
      Renderer ();
      virtual ~Renderer ();
      
      // -- var -----------------------------
      
      // projection * translate Matix
      // glm::mat4 modelMatrix;
        
      // calc Tasks
      bool calcTasks[UI_CALCTASK__SIZE];
      
      // render Attriubtes                      \n
      // - includes clalculatet values          \n
      // - -> used by render() with openGL
      struct RenderAttributes 
      {
        GLfloat positionX;
        GLfloat positionY;
        
        // 4 vertices -> *(x,y,z) = 12
        GLfloat vertices[12]; 
        
        ColorValue *background_color;
        FloatValue *opacity;
        
        ColorValue  *text_color;
        IntValue    *text_size;
        StringValue *text_family;
      } renderAttributes;
      //RenderAttributes *renderAttributesPtr;
      

      // layout Attributes                      \n
      // - need layout calculation              \n
      // - -> then into render Attriubtes
      struct LayoutAttributes 
      {
        IntValue *top, *left, *right, *bottom;
        IntValue *height, *width;
      } layoutAttributes;
      //LayoutAttributes *layoutAttributesPtr;
      
      
      // -- calculate -> layout Attributes into render Attriubtes
              void calcLayout(); // calls clacLayoutPosition() and calcLayoutSize()
      virtual void calcLayoutPosition();
      virtual void calcLayoutSize();
      
      // add calculation Task
      virtual void  addCalcTask(int type);
      
      // execute all calc Tasks
      virtual int exeCaclTasks();
      
      
      // render itself
      // - openGl uses render Attributes
      virtual void render();
      virtual void renderGl_1(); // render in openGl ES 1.x context
      
      // have to be called after each render()  \n
      // - set Cursor to pos before render()
      void resetCursor();  
      
      
      
      // -- bindings ---------------------------
      
      /* check if StyleRule fits to view
       * 0 = false
       * 1 = fits
       * 2 = is own Id rule
       */
      int  checkRuleBinding(StyleRule *rule);
      
      // bind StyleRule to view
      void bindRule(StyleRule *rule);
      void unbindRule(StyleRule *rule);
      
      // seaches in Style::rules for fitting one and bind
      void bindRuleAutomatic();
      
      
      // return calculated StyleAttribute(Base) that have to be used
      StyleAttributeBase* checkAttributeBinding(StyleAttributeBase::Type type);
      
      // bind StyleAttribute to view
      void bindAttribute(StyleAttributeBase *attribute);
      void unbindAttribute(StyleAttributeBase *attribute);
      
      // searches in View rules for fitting Attributes and bind them
      void bindAttributeAutomatic();      
      
      
      // -- help var ----------------------
      struct Vertex 
      {
        int x, y;
      };
      
      // -- The view
      View *view;
      
    private:
      
      
};

#endif	/* RENDERER_H */

