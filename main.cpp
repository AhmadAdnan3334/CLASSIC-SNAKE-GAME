#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
using namespace std;
enum  Gamestate { MENU,
PLAYING, 
GAMEOVER};
Color bgColor      = { 11, 15, 26, 255 };   // Black
Color borderColor  = { 31, 41, 55, 255 };   // dark greyish blue
Color snakeColor   = { 34, 211, 238, 255 }; // cyan
Color snakeBorder  = { 8, 145, 178, 255 };  // Ocean blue
//Color foodColor    = { 244, 63, 94, 255 };  


int cellsize =26;
int cellcount =25;
int border=50;
double lastUpdateTime=0;
 bool ElementInDeque(Vector2 element,  deque<Vector2> deque)
{ 
  for(unsigned int i=0; i< deque.size();i++)
  {
    if(Vector2Equals( deque[i],element))
    {  
      return true;
    }
   
  }
    return false;

}

 
 bool EventTriggered(double interval)
 {double currenttime=GetTime();
  if(currenttime-lastUpdateTime  >= interval)
   { lastUpdateTime = currenttime  ;
  return true ;}
  else return false;
};
  class Snake{
    public:
    deque<Vector2> body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
    Vector2 Direction ={1,0};
    bool addsegment =false;
    void Draw()
    {
        for (unsigned int i=0; i<body.size(); i++) { 
            float x=body[i].x;
            float y=body[i].y; 
            Rectangle outer = Rectangle { border+x*cellsize-1, border+y*cellsize-1,(float)cellsize+2, (float)cellsize+2};
          
            Rectangle inner = Rectangle{ border+x*cellsize, border+y*cellsize,(float)cellsize, (float)cellsize };
             DrawRectangleRounded(outer,0.5,6,snakeBorder);            
            DrawRectangleRounded(inner,0.5,6,snakeColor);

         }
    }; 
    void Update ()
    {  body.push_front(Vector2Add(body[0],Direction) );
      if(addsegment== true){
       addsegment=false;
      }
       else {body.pop_back();
      //body.push_front(Vector2Add(body[0],Direction));
      //body.push_front(body[0]+Direction)
      }
    }; 
    void Reset()
    { body={ Vector2{6,9},Vector2{5,9},Vector2{4,9}};
   Direction={1,0};
   }

     
  };

  class Food
   {
    
    public:
      Vector2 position;
      Texture2D texture;
     
     
      Food(deque<Vector2> snakeBody) 
     {
       Image image = LoadImage ("food3.png");
       texture = LoadTextureFromImage(image);
       UnloadImage(image);
       position = GenerateRandomPos(snakeBody);
      }
       ~Food(){
        UnloadTexture(texture);
      }
    
     void Draw()
    { 
     DrawTexture(texture,border+position.x*cellsize,border+position.y*cellsize,WHITE);
    }
     Vector2 GenerateRandomCell(){
       float x= GetRandomValue (0, cellcount -1);
         float  y= GetRandomValue (0, cellcount -1 );
         return Vector2{x,y};
     }
    Vector2 GenerateRandomPos(deque<Vector2> snakebody)
    {
         
         Vector2 position = GenerateRandomCell() ;
         while(ElementInDeque(position, snakebody)){
          position =GenerateRandomCell();
         }
         return position;
        //return Vector2{x,y};
     };
 };


 class Game
 {
   public:

   Snake snake=Snake();  
     Food food=Food(snake.body);
     bool running=true;
     int score=0;
     Sound eatSound;
     Sound wallSound;
     Gamestate state = MENU;
     
     
     Game(){ 
      InitAudioDevice();
      eatSound=LoadSound("eat.mp3");
      wallSound=LoadSound("wall.mp3");
     }
     ~Game(){ 
      UnloadSound(eatSound);
      UnloadSound(wallSound);
      CloseAudioDevice();
     }

    void Draw(){ 
      food.Draw();
      snake.Draw();
    }; 
    void Update()
    { if( state == PLAYING){
      snake.Update();
      CheckCollisionsWithFood();
      CollisionWithEdges();
    CheckCollisionWithTail();}
    }
    void CheckCollisionsWithFood()
    { if(Vector2Equals( snake.body[0], food.position)) 
    { food.position=food.GenerateRandomPos(snake.body);
      snake.addsegment=true;
      score=score+1;
      PlaySound(eatSound);
      //cout<<"Eating food........."<<endl;
    } 
    } 
    void CollisionWithEdges() 
    { if(snake.body[0].x== cellcount || snake.body[0].x== -1 ) 
    { 

      GameOver();
    } 
    if( snake.body[0].y==cellcount || snake.body[0].y==-1 )
    { 
      GameOver();
    }
  } 
  void GameOver()
  { 
    snake.Reset();
    food.position= food.GenerateRandomPos(snake.body);
    state = GAMEOVER ;
    running=false;
   score=0;
    PlaySound(wallSound);
  }
  void CheckCollisionWithTail()
  {  
    deque<Vector2> headlessbody=snake.body ;
    headlessbody.pop_front();
    if(ElementInDeque(snake.body[0],headlessbody))
      {GameOver();}
  }
  void DrawMenu() //Draws menu
{
    DrawText("MAIN MENU", 220, 150, 40, snakeColor);
    DrawText("Press ENTER to Start", 200, 220, 20, LIGHTGRAY);
    DrawText("Use Arrow Keys to Move", 190, 260, 18, GRAY);
}
void DrawGameOverScreen() //display game over screen
{
    DrawText("GAME OVER", 230, 180, 40, RED);
    DrawText("Press ENTER to Restart", 190, 240, 20, LIGHTGRAY);
}

 }; 
 
 int main () 
{
    //cout << "Hello World" << endl;

    InitWindow(2*border + cellsize*cellcount, 2*border + cellsize*cellcount, "SNAKE GAME");
    SetTargetFPS(60);
    Game game;
    
    
    while (WindowShouldClose()==false)
    { BeginDrawing();
      if (EventTriggered(0.2)){ game.Update();}
      if (IsKeyPressed(KEY_ENTER))
    {
    if (game.state == MENU)
    {
         game.state = PLAYING;
        game.running = true;
    }
    else if (game.state == GAMEOVER)
    {
           // reset everything
        game.state = PLAYING;
        game.running = true;
    }
   }
      if(IsKeyPressed(KEY_UP)    &&  game.snake.Direction.y !=1 )
      { game.snake.Direction ={0,-1};
     game.running=true;
    }
       if(IsKeyPressed(KEY_DOWN)  && game.snake.Direction.y !=-1)  
      { game.snake.Direction ={0,1};
      game.running=true;
    }
       if(IsKeyPressed(KEY_RIGHT) && game.snake.Direction.x !=-1)
       { game.snake.Direction ={1,0};
        game.running=true;}
       if(IsKeyPressed(KEY_LEFT)  && game.snake.Direction.x !=1)
       { game.snake.Direction ={-1,0};
        game.running=true;}

       //snake.Update();
        //drawing
        ClearBackground(bgColor);
         DrawText("Snake Game", 1, 1, 25, snakeColor);
        DrawRectangleLinesEx(Rectangle{(float)border-5,(float)border-5,(float)cellsize*cellcount+10,(float)cellsize*cellcount+10},6,borderColor);
        DrawText(TextFormat(" %i",game.score),25,700, 25, snakeColor);
         
         if (game.state == MENU)
           game.DrawMenu();
         else if (game.state == PLAYING)
          game.Draw();
        else if (game.state == GAMEOVER)
         game.DrawGameOverScreen();
         
         

        EndDrawing(); 
       }

    
  CloseWindow();
    return 0;
}