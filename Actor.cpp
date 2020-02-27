#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


const double PI = 4 * atan(1);

//*************************************
// ACTOR FUNCTIONS
//*************************************
Actor:: Actor (int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world): GraphObject(imageID, startX, startY, dir, depth){
    m_life= true;
    m_world = world;
}

bool Actor:: isAlive() const {
    return m_life;
}

void Actor :: setDead() {
    m_life=false;
}


StudentWorld* Actor::getWorld() const {
    return m_world;
}





// If this actor can suffer damage, make it do so and return true;
// otherwise, return false.
bool Actor:: takeDamage(int damage){
    return false;
}

bool Actor:: canBlock() const{
    return false;
}

bool Actor:: isEdible() const{
    return false;
}

// Does the existence of this object prevent a level from being completed?
bool Actor:: preventsLevelCompleting() const{
    return false;
}

Actor:: ~Actor(){
    
}



//*************************************
// DIRT FUNCTIONS
//*************************************
Dirt:: Dirt (StudentWorld *w, double startX, double startY) : Actor(IID_DIRT, startX, startY, 0, 1, w){
    
}

void Dirt:: doSomething (){
    return;
}

bool Dirt:: takeDamage(int){
    setDead();
    return true;
}

bool Dirt:: canBlock() const{
    return true;
}



//*************************************
// FOOD FUNCTIONS
//*************************************
Food:: Food (double startX, double startY, StudentWorld *w): Actor (IID_FOOD, startX, startY, 90, 1, w){
    
}

void Food:: doSomething(){
    return;
}

bool Food::isEdible() const{
    return true;
}



//*************************************
// PIT FUNCTIONS
//*************************************
Pit :: Pit (double startX, double startY, int regsal, int aggsal, int ecoli, StudentWorld *w): Actor(IID_PIT, startX, startY, 0, 1, w){
    m_regsal = regsal;
    m_aggsal = aggsal;
    m_ecoli = ecoli;
    
    
}

void Pit :: doSomething(){
    if (m_regsal==0 && m_aggsal==0 && m_ecoli==0){
        //inform student world object that it has emitted all bacteria
        setDead();
    }
    else {
        int chance = randInt(1, 50);
        if (chance ==1 ) {
            int bacChance = randInt(1, 3);
            if (bacChance==1 && m_regsal!=0) {
                //add regsal
                Actor *r = new RegularSalmonella(getX(), getY(), getWorld());
                getWorld()->addActor(r);
                m_regsal--;
                getWorld()->playSound(SOUND_BACTERIUM_BORN);
            }
            if (bacChance==2 && m_aggsal!=0){
                //add aggsal
                Actor *a = new AggSalmonella(getX(), getY(), getWorld());
                getWorld()->addActor(a);
                m_aggsal--;
                getWorld()->playSound(SOUND_BACTERIUM_BORN);
            }
            if (bacChance==3 && m_ecoli!=0){
                //add ecoli
                Actor *e = new EColi(getX(), getY(), getWorld());
                getWorld()->addActor(e);
                m_ecoli--;
                getWorld()->playSound(SOUND_BACTERIUM_BORN);
            }
        }
    }
    
}

bool Pit:: preventsLevelCompleting() const {
    if (m_regsal ==0 && m_aggsal==0 && m_ecoli ==0)
        return false;
    return true;
}



//*************************************
// PROJECTILES FUNCTIONS
//*************************************
Projectile:: Projectile (int imageID, double startX, double startY, Direction dir, int maxDis, int damage, StudentWorld *w): Actor(imageID, startX, startY, dir, 1, w){
    m_maxDis= maxDis;
    m_damage = damage;
}

void Projectile:: doSomething (){
    
    if (!isAlive())
        return;
    
    if (m_maxDis>0){
        moveForward(SPRITE_WIDTH);
        m_maxDis-=SPRITE_WIDTH;
        if (getWorld()->damageOneActor(this, m_damage)) {
            setDead();
            return;
        }
        
    }
    if (m_maxDis==0) {
        setDead();
    }
    
    
    
}


//*************************************
// SPRAY FUNCTIONS
//*************************************
Spray:: Spray (double startX, double startY, Direction dir, StudentWorld *w) : Projectile(IID_SPRAY, startX, startY, dir, 112, -2,  w) {
    
}




//*************************************
// FLAMES FUNCTIONS
//*************************************
Flame:: Flame (double startX, double startY, Direction dir, StudentWorld *w): Projectile(IID_FLAME, startX, startY, dir, 32, -5, w){
    
}




//*************************************
// GOODIE FUNCTIONS
//*************************************
Goodie:: Goodie (int imageID, double startX, double startY, int lifetime, StudentWorld *w):Actor(imageID, startX, startY, 0, 1, w){
    m_lifetime=lifetime;
}
bool Goodie:: takeDamage(int){
    setDead();
    return true;
}

void Goodie:: doSomething(){
    m_lifetime --;
    
    if (!isAlive())
        return;
    
    Socrates *s =getWorld()->getOverlappingSocrates(this);
    if (s!=nullptr){
        pickUp(s);
        return;
    }
    if (m_lifetime==0) setDead();
}


//*************************************
// RESTORE HEALTH GOODIE FUNCTIONS
//*************************************
RestoreHealthGoodie:: RestoreHealthGoodie (double startX, double startY, int lifetime, StudentWorld *w): Goodie(IID_RESTORE_HEALTH_GOODIE, startX, startY, lifetime, w){
    
}

void RestoreHealthGoodie::pickUp(Socrates *s){
    getWorld()->increaseScore(250);
    setDead();
    s->restoreHealth();
    getWorld()->playSound(SOUND_GOT_GOODIE);
}



//*************************************
// FLAME THROWER GOODIE FUNCTIONS
//*************************************
FlamethrowerGoodie:: FlamethrowerGoodie(double startX, double startY, int lifetime, StudentWorld *w):Goodie(IID_FLAME_THROWER_GOODIE, startX, startY, lifetime, w) {
    
}


void FlamethrowerGoodie::pickUp(Socrates *s){
    getWorld()->increaseScore(300);
    setDead();
    s->addFlames();
    getWorld()->playSound(SOUND_GOT_GOODIE);
}



//*************************************
// EXTRA LIFE GOODIE FUNCTIONS
//*************************************
ExtraLifeGoodie:: ExtraLifeGoodie(double startX, double startY, int lifetime, StudentWorld *w):Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, lifetime, w){
    
}




void ExtraLifeGoodie::pickUp(Socrates *s){
    getWorld()->increaseScore(500);
    setDead();
    getWorld()->incLives();
    getWorld()->playSound(SOUND_GOT_GOODIE);
}




//*************************************
// FUNGUS FUNCTIONS
//*************************************
Fungus::Fungus (double startX, double startY,  int lifetime, StudentWorld *w): Goodie(IID_FUNGUS, startX, startY, lifetime, w){
    
}



void Fungus::pickUp(Socrates *s){
    getWorld()->increaseScore(-50);
    setDead();
    s->takeDamage(-20);
}



//*************************************
// AGENT FUNCTIONS
//*************************************
Agent:: Agent (int imageID, double startX, double startY, int hp, Direction dir, StudentWorld *w): Actor (imageID, startX, startY, dir, 0, w){
    m_hp=hp;
}

bool Agent::takeDamage(int damage){
    m_hp+=damage;
    getWorld()->playSound(soundWhenHurt());
    
    if (m_hp<=0) {
        setDead();
        getWorld()->playSound(soundWhenDie());
    }
    return true;
    
}

int Agent::numHitPoints() const{
    return m_hp;
}


void Agent::restoreHealth(){
    m_hp=100;
}





//*************************************
// SOCRATES FUNCTIONS
//*************************************
Socrates:: Socrates  (double startX, double startY, int spray, int flame, int hp, StudentWorld* w): Agent(IID_PLAYER, startX, startY, hp, 0, w){
    m_spray=spray;
    m_flame=flame;
}


void Socrates:: doSomething (){
    
    if (!isAlive()) return;
    
    
    int ch;
    
    if (getWorld()->getKey(ch)){
        switch (ch) {
            case KEY_PRESS_LEFT:
            case 'a': moveTo(VIEW_RADIUS+VIEW_RADIUS*cos((getDirection()+180+5)*(PI/180)), VIEW_RADIUS+VIEW_RADIUS*sin((getDirection()+180+5)*(PI/180)));
                setDirection(getDirection()+5);
                break;
                
            case KEY_PRESS_RIGHT:
            case 'd':    moveTo(VIEW_RADIUS+VIEW_RADIUS*cos((getDirection()+180-5)*(PI/180)), VIEW_RADIUS+VIEW_RADIUS*sin((getDirection()+180-5)*(PI/180)));
                setDirection(getDirection()-5);
                
                break;
                
            case KEY_PRESS_SPACE:
                if (m_spray>=1){
                    double newX = (getX() + 2*SPRITE_RADIUS * cos(getDirection()*1.0 / 360 * 2 * PI));
                    double newY = (getY() + 2*SPRITE_RADIUS * sin(getDirection()*1.0 / 360 * 2 * PI));
                    Actor* s= new Spray(newX, newY, getDirection(), getWorld());
                    
                    getWorld()->addActor(s);
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                    m_spray--;
                }
                break;
            case KEY_PRESS_ENTER:
                if (m_flame>=1){
                    
                    for (int i=0;i<16;i++){
                        double dir = getDirection()+i*22;
                        double newX = (getX() + 2*SPRITE_RADIUS * cos(dir*1.0 / 360 * 2 * PI));
                        double newY = (getY() + 2*SPRITE_RADIUS * sin(dir*1.0 / 360 * 2 * PI));
                        
                        Actor* f= new Flame(newX, newY, dir, getWorld());
                        getWorld()->addActor(f);
                    }
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    m_flame--;
                }
                break;
        }
        
    }
    else if (m_spray<20){
        m_spray++;
    }
}


int Socrates::soundWhenHurt() const {
    return SOUND_PLAYER_HURT;
}

int Socrates::soundWhenDie() const{
    return SOUND_PLAYER_DIE;
}


void Socrates:: addFlames(){
    m_flame+=5;
}


int Socrates:: numFlames() const {
    return m_flame;
}

int Socrates:: numSprays() const {
    return m_spray;
}




//*************************************
// BACTERIA FUNCTIONS
//*************************************
Bacteria:: Bacteria (int imageID, double startX, double startY, int move, int hp, StudentWorld *w): Agent(imageID, startX, startY, hp, 90, w) {
    m_hp = hp;
    m_foodCount=0;
}


bool Bacteria:: takeDamage(int damage){
    
    m_hp+=damage;
    getWorld()->playSound(soundWhenHurt());
    
    if (m_hp<=0) {
        setDead();
        getWorld()->playSound(soundWhenDie());
        getWorld()->increaseScore(100);
        int r = randInt(1, 2);
        if (r==1){
            Food *f = new Food(getX(), getY(), getWorld());
            getWorld()->addActor(f);
        }
    }
    
    return true;
    
}

bool Bacteria::preventsLevelCompleting() const {
    if (!isAlive())
        return false;
    return true;
}

int Bacteria::getFoodCount () const{
    return m_foodCount;
}

bool Bacteria::helper(int &x, int &y){
    
    Actor *a = getWorld()->getOverlappingEdible(this);
    if (a!=nullptr){
        a->setDead();
        m_foodCount++;
    }
    
    if (m_foodCount==3){
        x = getX();
        if (getX()<VIEW_WIDTH/2)
            x+=SPRITE_WIDTH/2;
        if (getX()>VIEW_WIDTH/2)
            x+=SPRITE_WIDTH/2;
        y = getY();
        if (getY()<VIEW_HEIGHT/2)
            y+=SPRITE_WIDTH/2;
        if (getY()>VIEW_HEIGHT/2)
            y+=SPRITE_WIDTH/2;
        m_foodCount=0;
        return true;
    }
    return false;
}

//*************************************
// E. COLI FUNCTIONS
//*************************************
EColi:: EColi (double startX, double startY, StudentWorld *w): Bacteria(IID_ECOLI, startX, startY, 0, 5, w) {
    
}

void EColi:: doSomething(){
    int x,y;
    
    if (!isAlive())
        return;
    
    //step 2
    Socrates *p = getWorld()->getOverlappingSocrates(this);
    if (p!=nullptr){
        p->takeDamage(-4);
        if(getWorld()->getAngleToNearbySocrates(this, 256, theta)){
            
            for(int i=0; i< 10; i++){
                setDirection(theta);
                double newX = (getX() + 1 * cos(theta*1.0 / 360 * 2 * PI));
                double newY = (getY() + 1 * sin(theta*1.0 / 360 * 2 * PI));
                
                if (!getWorld()->isBacteriaMovementBlockedAt(newX, newY) && (getWorld()->distance(VIEW_WIDTH/2, VIEW_HEIGHT/2, newX, newY) < VIEW_RADIUS)){
                    moveTo(newX, newY);
                    return;
                }
                
                else {
                    theta+=10;
                    setDirection(theta);
                }
            }
            return;
            
            
        }
    }
    
    //step 3
    if (Bacteria::helper(x,y)){
        Bacteria *b = new EColi (x, y, getWorld() );
        getWorld()->addActor(b);
        
        if(getWorld()->getAngleToNearbySocrates(this, 256, theta)){
            
            for(int i=0; i< 10; i++){
                setDirection(theta);
                double newX = (getX() + 1 * cos(theta*1.0 / 360 * 2 * PI));
                double newY = (getY() + 1 * sin(theta*1.0 / 360 * 2 * PI));
                
                if (!getWorld()->isBacteriaMovementBlockedAt(newX, newY) && (getWorld()->distance(VIEW_WIDTH/2, VIEW_HEIGHT/2, newX, newY) < VIEW_RADIUS)){
                    moveTo(newX, newY);
                    return;
                }
                
                else {
                    theta+=10;
                    setDirection(theta);
                }
            }
            return;
            
            
        }
    }
    
    //step 5
    if(getWorld()->getAngleToNearbySocrates(this, 256, theta)){
        
        for(int i=0; i< 10; i++){
            setDirection(theta);
            double newX = (getX() + 1 * cos(theta*1.0 / 360 * 2 * PI));
            double newY = (getY() + 1 * sin(theta*1.0 / 360 * 2 * PI));
            
            if (!getWorld()->isBacteriaMovementBlockedAt(newX, newY) && (getWorld()->distance(VIEW_WIDTH/2, VIEW_HEIGHT/2, newX, newY) < VIEW_RADIUS)){
                moveTo(newX, newY);
                return;
            }
            
            else {
                theta+=10;
                setDirection(theta);
            }
        }
        return;
        
        
    }
}

int EColi::soundWhenHurt() const{
    return SOUND_ECOLI_HURT;
}

int EColi::soundWhenDie() const{
    return SOUND_ECOLI_DIE;
}



//*************************************
// SALMONELLA FUNCTIONS
//*************************************
Salmonella:: Salmonella (double startX, double startY, int hp,  StudentWorld*w): Bacteria(IID_SALMONELLA, startX, startY, 0, hp, w){
    m_move=0;
}

int Salmonella::soundWhenHurt() const{
    return SOUND_SALMONELLA_HURT;
}


int Salmonella::soundWhenDie() const{
    return SOUND_SALMONELLA_DIE;
}

void Salmonella::move(){
    if (m_move>0){
        m_move--;
        double newX = (getX() + 3 * cos(getDirection()*1.0 / 360 * 2 * PI));
        double newY = (getY() + 3 * sin(getDirection()*1.0 / 360 * 2 * PI));
        if (!getWorld()->isBacteriaMovementBlockedAt(newX, newY) && (getWorld()->distance(VIEW_WIDTH/2, VIEW_HEIGHT/2, newX, newY) < VIEW_RADIUS)){
            moveTo(newX, newY);
        }
        else {
            int d= randInt(0, 359);
            setDirection(d);
            m_move=10;
        }
        return;
    }
    
    else {
        
        int ang;
        if (getWorld()->getAngleToNearestNearbyEdible(this, 128, ang)) {
            double newX = (getX() + 3 * cos(getDirection()*1.0 / 360 * 2 * PI));
            double newY = (getY() + 3 * sin(getDirection()*1.0 / 360 * 2 * PI));
            setDirection(ang);
            if (!getWorld()->isBacteriaMovementBlockedAt(newX, newY) && (getWorld()->distance(VIEW_WIDTH/2, VIEW_HEIGHT/2, newX, newY) < VIEW_RADIUS)){
                moveTo(newX, newY);
            }
            else{
                setDirection(randInt(0, 359));
                m_move=10;
                return;
            }
        }
        else {
            setDirection(randInt(0, 359));
            m_move=10;
            return;
        }
    }
    
    
}



//*************************************
// REGULAR SALMONELLA FUNCTIONS
//*************************************
RegularSalmonella:: RegularSalmonella (double startX, double startY, StudentWorld *w) : Salmonella(startX, startY, 4, w){
    
}


void RegularSalmonella::doSomething() {
    int x,y;
    
    if (!isAlive())
        return;
    
    Socrates *p = getWorld()->getOverlappingSocrates(this);
    if (p!=nullptr){
        p->takeDamage(-1);
        Salmonella::move();
        
    }
    
    if (Bacteria::helper(x,y)){
        Bacteria *b = new RegularSalmonella (x, y, getWorld() );
        getWorld()->addActor(b);
        Salmonella::move();
    }
    
    Salmonella::move();
    
}




//*************************************
// AGGRESSIVE SALMONELLA FUNCTIONS
//*************************************
AggSalmonella:: AggSalmonella (double startX, double startY, StudentWorld *w): Salmonella(startX, startY, 10, w){
    
}

void AggSalmonella::doSomething(){
    int x,y =0;
    
    //step 1
    if (!isAlive())
        return;
    
    //step 2
    int ang=0;
    if (getWorld()->getAngleToNearbySocrates(this, 72, ang)){
        setDirection(ang);
        double newX = (getX() + 3 * cos(ang*1.0 / 360 * 2 * PI));
        double newY = (getY() + 3 * sin(ang*1.0 / 360 * 2 * PI));
        if (!getWorld()->isBacteriaMovementBlockedAt(newX, newY) && (getWorld()->distance(VIEW_WIDTH/2, VIEW_HEIGHT/2, newX, newY) < VIEW_RADIUS)){
            moveTo(newX, newY);
        }
        
        Socrates *p = getWorld()->getOverlappingSocrates(this);
        if (p!=nullptr){
            p->takeDamage(-2);
        }
        
        
        if (Bacteria::helper(x,y)){
            Bacteria *b = new AggSalmonella (x, y, getWorld() );
            getWorld()->addActor(b);
        }
        return;
    }
    
    //step 3
    Socrates *p = getWorld()->getOverlappingSocrates(this);
    if (p!=nullptr){
        p->takeDamage(-2);
        Salmonella::move();
    }
    
    //step 4 & 5
    if (Bacteria::helper(x,y)){
        Bacteria *b = new AggSalmonella (x, y, getWorld() );
        getWorld()->addActor(b);
        Salmonella::move();
    }
    
    //step 6
    Salmonella::move();
    
    
}








