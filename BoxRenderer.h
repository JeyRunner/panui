/* 
 * File:   BoxRenderer.h
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * BOXRENDERER CLASS
 * render BOX(VIEW)
 * extends RENDERER
 * ---------------------------------------
 */


#ifndef BOXRENDERER_H
#define	BOXRENDERER_H

#include <stdlib.h>
#include "GLES/gl.h"
#include "Renderer.h"
class Box;
using namespace std;



class BoxRenderer : public Renderer
{
    public:
      BoxRenderer (Box *view);
      virtual ~BoxRenderer ();
      
      
      
      // -- calc layout of children
      void calcLayoutChildrenPos();
      
      // -- position a child
      void calcLayoutChildRelative(View *v);
      void calcLayoutChildAbsolute(View *v);
      
      
      // -- position itself -> update children
      virtual void calcLayoutSize ();
      virtual void addCalcTask (int type);
      virtual int  exeCaclTasks ();



      // -- render itself and children
      virtual void render ();

      
      
      // -- help var ---------
      // children cursor stores x, y
      struct ChildrenCursor
      {
        float x, y;
        float hightesHight; // for nex row
        
        void X(float x);
        void Y(float y);
        
        void XY(float x, float y);
      } chCur;
     

};

#endif	/* BOXRENDERER_H */

