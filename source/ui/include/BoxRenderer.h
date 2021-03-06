/* 
 * File:   BoxRenderer.h
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * BOXRENDERER CLASS
 * render BOX(VIEW)
 * extends RENDERER
 *
 *
 * layout calculation:
 * [if self height = AUTO]
 * 1. self width
 * 2. children (for each child)
 * 2.1 width, height
 * 2.2 position
 * 3. self height
 *
 * layout calculation:
 * * [if self height = VALUE, PERCENT]
 * 1. self width, height
 * 2. children (for each child)
 * 2.1 width, height
 * 2.2 position
 *
 *
 * if use height = AUTO
 * * own height only dependent on
 *   children with non percent height
 * ---------------------------------------
 */


#ifndef BOXRENDERER_H
#define	BOXRENDERER_H

// include
#include <stdlib.h>
#include "GL.h"
#include "Renderer.h"
namespace ui
{
class Box;

using namespace std;


/* BoxRenderer Class
 */
class BoxRenderer : public Renderer
{
    public:
      BoxRenderer (Box *view);
      virtual ~BoxRenderer ();
      
      
      
      // -- calc layout of children
      void calcLayoutChildrenPos();
      
      // -- clac position of child
      void calcLayoutChildRelative(View *v);
      void calcLayoutChildAbsolute(View *v);
      
      
      // -- position itself -> update children
      virtual bool calcLayoutSizeAutoContend();
      virtual void addCalcTask (int type);
      virtual int  exeCalcTasks();



      // -- render itself and children
      virtual void render ();

      
      virtual View *isOver(float x, float y);
      
      
      // -- help var ---------
      // children cursor stores x, y
      struct ChildrenCursor
      {
        float x, y;
        float hightesHight; // for nex row
        float contendHeight;
        
        void X(float x);
        void Y(float y);
        
        void XY(float x, float y);
      } chCur;
};


};     /* END NAMESPACE */
#endif	/* BOXRENDERER_H */

