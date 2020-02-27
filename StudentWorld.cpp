#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>

using namespace std;

const double PI = 4 * atan(1);


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{

    //add player
   player = new Socrates(0, VIEW_HEIGHT/2, 20, 5, 100, this);

    double r, theta, x, y;
    
    
    //add pits
    for (int i=0; i<getLevel();i++ ){
        r = randInt(-(VIEW_HEIGHT/2)+8, (VIEW_HEIGHT/2)-8);
        theta = randInt(0, 360);
        getPositionOnCircumference(theta, x, y);
        x= r*x + VIEW_RADIUS;
        y= r*y + VIEW_RADIUS;
        if (!overlap(x, y)){
             //m_actors.push_back(new Pit(x, y, 5, 3 ,2, this));
            m_actors.push_back(new Pit(x, y, 0, 0, 1, this));
        }
        else i--;
    }
    
    //add food
    int nOfFood = min(5*getLevel(), 25);
    for (int i=0; i<nOfFood; i++){
        
        r = randInt(-(VIEW_HEIGHT/2)+8, (VIEW_HEIGHT/2)-8);
        theta = randInt(0, 360);
        getPositionOnCircumference(theta, x, y);
               x= r*x + VIEW_RADIUS;
               y= r*y + VIEW_RADIUS;
        if (!overlap(x, y)){
            m_actors.push_back(new Food(x, y, this));
        }
        else i--;
    }
    
    
    //add dirt
    int nOfDirt = max(180-20*getLevel(),20);
    for (int i=0; i<nOfDirt; i++) {
        
        r = randInt(-(VIEW_HEIGHT/2)+8, (VIEW_HEIGHT/2)-8);
        theta = randInt(0, 360);
         getPositionOnCircumference(theta, x, y);
               x= r*x + VIEW_RADIUS;
               y= r*y + VIEW_RADIUS;
        if (!overlap(x, y)){
            m_actors.push_back(new Dirt(this, x, y));
        }
        else i--;
        
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
       
    //if player is dead decrease lives
    if (!player->isAlive()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
      }
    
    
    //if all bacteria is dead and all pits have disappeared then nEXT LEVEL
    
    
       player->doSomething();
     if (!m_actors.empty()){
         for (int i=0; i!=m_actors.size();){
             m_actors[i]->doSomething();
             if (!m_actors[i]->isAlive())
                 kill(m_actors[i]);
             else i++;
         }
     }
       
       double x,y;
       double ChanceFungus = randInt (0, max(510-getLevel()*10, 200));
       if (ChanceFungus==0){
           double angle = randInt(0, 2*PI);
           x=VIEW_RADIUS+VIEW_RADIUS*cos(angle);
           y=VIEW_RADIUS+VIEW_RADIUS*sin(angle);
           m_actors.push_back(new Fungus(x, y, max(randInt(0, 300-10*getLevel()-1), 50) , this));
       }
    
        double ChanceGoodie = randInt(0, max(510-getLevel()*10,250));
       if (ChanceGoodie==0){
           double chance = randInt(0, 100);
           double angle = randInt(0, 2*PI);
           x=VIEW_RADIUS+VIEW_RADIUS*cos(angle);
           y=VIEW_RADIUS+VIEW_RADIUS*sin(angle);
           if (chance>0 && chance<=60){
               m_actors.push_back(new RestoreHealthGoodie (x, y, max(randInt(0, 300-10*getLevel()-1), 50), this));
               
           }
           else if (chance>60 && chance<=90){
               m_actors.push_back(new FlamethrowerGoodie(x, y, max(randInt(0, 300-10 *getLevel() - 1), 50), this));
               
           }
           else if (chance>90 && chance <=100){
               m_actors.push_back(new ExtraLifeGoodie (x, y, max(randInt(0, 300- 10 *getLevel() - 1), 50), this));
           }
       }
       
 //set game text at the top of the window
  ostringstream oss;
    string str;
    oss.fill('0');
 if (getScore()<0)
    oss<< "Score: -" << setw(6)<< -1*getScore() <<" Level: " << getLevel()<< " Lives: " <<getLives() << " Health: " <<player->numHitPoints() << " Sprays: " << player->numSprays() << " Flames: " <<player->numFlames();
    if (getScore()>=0)
    oss<< "Score: " << setw(6)<< getScore() <<"  Level: " << getLevel()<< "  Lives: " <<getLives() << "  Health: " <<player->numHitPoints() << "  Sprays: " << player->numSprays() << "  Flames: " <<player->numFlames();
    
    str = oss.str();
    setGameStatText(str);
    
       return GWSTATUS_CONTINUE_GAME;
    
    
  
}

void StudentWorld::cleanUp()
{
    if (player!=nullptr) {
        delete player;
    }
    vector <Actor*> :: iterator it;
    if (!m_actors.empty()){
        for (it=m_actors.begin(); it!= m_actors.end();it++){
            delete *it;
            it-- = m_actors.erase(it);
        }
    }
}


StudentWorld:: ~StudentWorld(){
    cleanUp();
}


void StudentWorld::addActor(Actor *a) {
    m_actors.push_back(a);
}


bool StudentWorld::damageOneActor(Actor *a, int damage) {
    int dis=0;
    for (int i=0; i < m_actors.size();){
        dis = distance(a->getX(), a->getY(), m_actors[i]->getX(), m_actors[i]->getY());
        if (dis <=2*SPRITE_RADIUS && m_actors[i]->takeDamage(-1)){
            m_actors[i]->takeDamage(damage);
            return true;
        }
        else i++;
        
    }
    return false;
}


bool StudentWorld::isBacteriaMovementBlockedAt (double x, double y) {
    int dis=0;
      for (int i=0; i < m_actors.size();){
          dis = distance(x, y, m_actors[i]->getX(), m_actors[i]->getY());
          if (dis <=SPRITE_WIDTH/2 && m_actors[i]->canBlock()){
              return true;
          }
          else i++;
          
      }
      return false;
}

Socrates* StudentWorld:: getOverlappingSocrates(Actor *a) const {
    int dis=0;
    dis = distance(a->getX(), a->getY(), player->getX(), player->getY());
    if (dis <=2*SPRITE_RADIUS){
        return player;
    }
   
    return nullptr;
}


Actor* StudentWorld:: getOverlappingEdible(Actor *a) const{
    int dis=0;
    for (int i=0; i < m_actors.size();){
        dis = distance(a->getX(), a->getY(), m_actors[i]->getX(), m_actors[i]->getY());
        if (dis <=2*SPRITE_RADIUS && m_actors[i]->isEdible()){
           // m_actors[i]->takeDamage(1);
            return m_actors[i];
        }
        else i++;
        
    }
    return nullptr;
}


bool StudentWorld::getAngleToNearbySocrates(Actor *a, int dist, int& angle) const {
    
    int dis=0;
    dis = distance(a->getX(), a->getY(), player->getX(), player->getY());
    if (dis <=dist){
        angle = atan2(player->getY()-a->getY(), player->getX()-a->getX()) *(180/PI);
        return true;
    }
    
    return false;
}



bool StudentWorld::getAngleToNearestNearbyEdible(Actor *a, int dist, int &angle) const{
    int dis=0;
    for (int i=0; i < m_actors.size();){
         dis = distance(a->getX(), a->getY(), m_actors[i]->getX(), m_actors[i]->getY());
         if (dis <=dist && m_actors[i]->isEdible()){
             angle = atan2(m_actors[i]->getY()-a->getY(), m_actors[i]->getX()-a->getX()) * (180/PI);
             return true;
         }
         else i++;
         
     }
     return false;
    
}


void StudentWorld::getPositionOnCircumference(int angle, double &x, double &y) const{
    //add VIEW_RADIUS and multiple x by r
    x = cos(angle*(PI/180));
    y= sin(angle*(PI/180));
}



double StudentWorld:: distance (double x1, double y1, double x2, double y2) const{
    return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
}

bool StudentWorld:: overlap (double x, double y){
    int dis=0;
    for (int i=0; i < m_actors.size();){
        dis = distance(x, y, m_actors[i]->getX(), m_actors[i]->getY());
        if (dis <=2*SPRITE_RADIUS){
            return true;
        }
        else i++;
        
    }
    return false;
}
    
   void StudentWorld :: kill (Actor* a) {
        vector <Actor*> :: iterator it;
        if (!m_actors.empty()){
            for (it=m_actors.begin(); it!= m_actors.end();it++){
                if (*it == a){
                    delete *it;
                    it-- = m_actors.erase(it);
                }
                
            }
        }
    }


