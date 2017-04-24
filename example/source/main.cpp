#include <Ui.h>
#include <Text.h>
#include <unistd.h>
#include <Box.h>
using namespace std;
using namespace ui;



// main
int main(int argc, char *argv[])
{
  // init ui
  Ui::init();

  // create window
  Ui *window = new Ui(400, 400, "Example");

  // root
  Box *root = new Box();
  root->style.paddingLeft(10);

  // add welcome text
  Text *welcomeText = new Text();
  welcomeText->text("Welcome to Panui :)");
  welcomeText->style.textSize(75)
                    .top(20);
  root->addChild(welcomeText);

  // progress box
  View *box = new View();
  root->addChild(box);
  box->style.height(300)
      .width(100)
      .backgroundColor("#00ffaaaa")
      .top(20);

  // set root element and start
  window->setRootView(root);
  window->frameRenderer->start();




  // animate box
  for(float i=0; ;i+= 0.01) {
    //box->style.width(sin(i)*(root->style.width.getFloat()/2) + (root->style.width.getFloat()/2));
    usleep(10000);
  }
}