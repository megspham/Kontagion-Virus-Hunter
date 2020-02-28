#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"

class StudentWorld;
class Socrates;

class Actor : public GraphObject
{
public:
    Actor(int imageID, double x, double y, int dir, int depth, StudentWorld* w);
    virtual ~Actor ();
    // Action to perform for each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    bool isAlive() const;
    
    // Mark this actor as dead.
    void setDead();
    
    // Get this actor's world
    StudentWorld* getWorld() const;
    
    // If this actor can suffer damage, make it do so and return true;
    // otherwise, return false.
    virtual bool takeDamage(int damage);
    
    // Does this object block bacterium movement?
    virtual bool canBlock() const;
    
    // Is this object edible?
    virtual bool isEdible() const;
    
    // Does the existence of this object prevent a level from being completed?
    virtual bool preventsLevelCompleting() const;
    
private:
    int m_life;
    StudentWorld* m_world;
    
};




class Dirt : public Actor
{
public:
    Dirt(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual bool takeDamage(int);
    virtual bool canBlock() const;
};

class Food : public Actor
{
public:
    Food (double x, double y, StudentWorld* w);
    virtual void doSomething();
    virtual bool isEdible() const;
};

class Pit : public Actor
{
public:
    Pit(double x, double y, int regsal, int aggsal, int ecoli, StudentWorld *w);
    virtual void doSomething();
    virtual bool preventsLevelCompleting() const;
private:
    int m_regsal;
    int m_aggsal;
    int m_ecoli;
};

class Projectile : public Actor
{
public:
    Projectile(int imageID, double x, double y, int dir, int maxDis, int damage, StudentWorld *w);
    virtual void doSomething();
    
private:
    int m_maxDis;
    int m_damage;
};

class Spray : public Projectile
{
public:
    Spray(double x, double y, Direction dir, StudentWorld *w);
 
};

class Flame : public Projectile
{
public:
    Flame(double x, double y, Direction dir, StudentWorld *w);
    
};

class Goodie : public Actor
{
public:
    Goodie(int imageID, double x, double y, int lifetime, StudentWorld *w);
    virtual bool takeDamage(int);
    virtual void doSomething();
    // Have s pick up this goodie.
    virtual void pickUp(Socrates* s) = 0;
    
private:
    int m_lifetime;
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(double x, double y, int lifetime, StudentWorld *w);
    virtual void pickUp(Socrates* s);
};

class FlamethrowerGoodie : public Goodie
{
public:
    FlamethrowerGoodie( double x, double y, int lifetime, StudentWorld *w);
    virtual void pickUp(Socrates* s);
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie( double x, double y, int lifetime, StudentWorld *w);
    virtual void pickUp(Socrates* s);
};

class Fungus : public Goodie
{
public:
    Fungus( double x, double y, int lifetime, StudentWorld *w);
    virtual void pickUp(Socrates* s);
};

class Agent : public Actor
{
public:
    Agent(int imageID, double x, double y, int hp, Direction dir, StudentWorld *w);
    virtual bool takeDamage(int damage);
    
    // How many hit points does this agent currently have?
    int numHitPoints() const;
    
    // Restore this agent's hit points to their original level
    void restoreHealth();
    
    
    // What sound should play when this agent is damaged but does not die?
    virtual int soundWhenHurt() const = 0;
    
    // What sound should play when this agent is damaged and dies?
    virtual int soundWhenDie() const = 0;
private:
    int m_hp;
};

class Socrates : public Agent
{
public:
    Socrates(double x, double y, int spray, int flame, int hp, StudentWorld *w);
    virtual void doSomething();
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
    
    // Increase the number of flamethrower charges the object has.
    void addFlames();
    
    // How many flamethrower charges does the object have?
    int numFlames() const;
    
    // How many spray charges does the object have?
    int numSprays() const;
    
private:
    int m_spray;
    int m_flame;
};

class Bacteria : public Agent
{
public:
    Bacteria( int imageID, double x, double y, int move, int hitPoints, StudentWorld *w);
    virtual bool takeDamage(int damage);
    virtual bool preventsLevelCompleting() const;
    bool eat(int &x, int &y);
    
private:
    int m_hp;
    int m_foodCount;
    
};

class EColi : public Bacteria
{
public:
    EColi(double startX, double startY, StudentWorld *w);
    virtual void doSomething();
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
private:
    int theta;
};

class Salmonella : public Bacteria
{
public:
    Salmonella(double x, double y, int hitPoints, StudentWorld *w);
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
    void move();
private:
    int m_move;
};

class RegularSalmonella : public Salmonella
{
public:
    RegularSalmonella( double x, double y, StudentWorld* w);
    virtual void doSomething();
    
};


class AggSalmonella : public Salmonella
{
public:
    AggSalmonella(double x, double y, StudentWorld* w);
    virtual void doSomething();
    
};



#endif // ACTOR_INCLUDED
