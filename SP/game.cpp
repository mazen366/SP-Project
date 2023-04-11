#include <iostream>
using namespace std;
using namespace sf;
RenderWindow window( VideoMode(1920,1080),"game");
void backgoundDrawer();
int main()
{
    
    window.setFramerateLimit(60);
  
    
    while(window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if(event.type==Event::Closed||(event.KeyPressed&&event.key.code==Keyboard::Space))
                window.close();
            
        }
        //update
        
        //draw
        window.clear();
        backgoundDrawer();
        window.display();
        
      
    }
    return 0;
}
void backgoundDrawer()
{
    Texture background1Atex;
    background1Atex.loadFromFile("Level 1-A BG.png");
    //9063 × 1192
    Sprite background1A(background1Atex);
    background1A.setPosition(0, -53);
    
    window.draw(background1A);
    
}