/*
CS 349 A1 Skeleton Code - Snake

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o snake snake.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./snake

Note: the -L option and -lstdc++ may not be needed on some machines.
*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include <vector>
#include <sstream>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
 
/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 10;
const int width = 800;
const int height = 600;

int glospeed = 5;
int FPS = 30;
int score = 0;


/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	int		width;		// size of window
	int		height;

	Pixmap   pixmap;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}


/*
 * An abstract class representing displayable things. 
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};       
	

////////////////// PAUSE ////////////////////////////
bool isPause=false;
void pauseGame (){
	if(isPause == false){
		isPause = true;

	}
	else {
		isPause = false;
	}

}

////////////////// START ////////////////////////////
	
bool isStart =false;

class startPage : public Displayable {
	public:
		 virtual void paint(XInfo &xinfo) {
	 		if(isStart == false){
	 			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						330,200,"WELCOME TO SNAKE GAME",21);

	 			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						400,270,"Designed by Yang Wen (20608279)",31);

	 			XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					280, 350, 530, 350);
	 			XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					280, 510, 530, 510);
	 			XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					280, 350, 280, 510);
	 			XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					530, 350, 530, 510);

	 			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						300,370,"Press 's' to Start Game!",24);
	 			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						300,400,"Press 'q' to Quit Game!",23);
	 			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						300,430,"Use 'p' to Pause/Resume Game",28);
	 			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						300,460,"Use 'r' to Restart Game",23);

	 			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						300,490,"Use 'Arror Keys' to Control the Snake",37);
	 			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						380,540,"Challenge Mode after 7 Scores",29);

	 		}
	 	}
};

startPage startpage;

void startGame(XInfo &xinfo) {
	isStart = true;
	startpage.paint(xinfo);
}


///////////////////// Level 2 StartPage ///////////////////////
bool isLevel1Done = false;
bool isLevel2 = false;
bool isGameOver = false;
class Level2Page : public Displayable {

public:

	virtual void paint(XInfo &xinfo) {
		if (isLevel1Done == true && isLevel2==false){
			XDrawString(xinfo.display, xinfo.pixmap,xinfo.gc[0],
		 					350, 230,"MORE CHALLENGE ? ", 17);
			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						350,350,"Press 'Y' to ACCEPT",19);
			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						350,380,"Press 'N' to REJECT",19);
		}
	}
};

Level2Page levelpage;


//////////////////////////// LEVEL 2 obstancle //////////////////////////////////

void startLevel2();

class LevelUp : public Displayable {
public:
	virtual void paint(XInfo &xinfo) {
		if(isLevel2 == true && isGameOver== false) {
			// left up
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 80, 100, 120, 20);
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 80, 120, 20, 40);

			// left  down
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 80, 490, 120, 20);
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 80, 450, 20, 40);

			// right up

			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 550, 100, 120, 20);
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 650, 120, 20, 60);

			//right down
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 550, 490, 130, 20);
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 660, 450, 20, 40);

			// middle
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 150, 280, 450, 20);
	
		}
	}
};
LevelUp levelup;

/////////////////// GAME OVER Page ////////////////////////////////////////

class Gameover : public Displayable {
	public:
		 virtual void paint(XInfo &xinfo) {
		 	if(isGameOver==true){
		 		XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						360,180,"GAME  OVER!",11);

		 		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					330, 260, 470, 260);
		 		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					330, 300, 470, 300);
		 		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					330, 260, 330, 300);
		 		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					470, 260, 470, 300);


		 		XDrawString(xinfo.display, xinfo.pixmap,xinfo.gc[0],
		 					350, 280,"Your Score is: ", 15);

		 		string result;
				const char* sco;
				ostringstream convert;
				convert << score;
				result = convert.str();
				sco = result.c_str();
				int length = result.length();

				XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
							450, 280, sco, length);


		 		XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						350,350,"Press 'r' to RESTART",20);
		 		XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						350,380,"Press 'q' to QUIT",17);
		 	}
		 }
};
Gameover gameover;



///////////////////////////// Menu Bar //////////////////////////////////////
class Text: public Displayable {

public:
	virtual void paint (XInfo &xinfo) {

		if(isStart==true && isGameOver==false){

			if(isLevel2==true){
				XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					30, 560, "CHALLENGE MODE" , 14);
			}


		// menu bar
		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					0, 530, 800, 530);
		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					160, 530, 160, 600);
		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					380, 530, 380, 600);
		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					380, 530, 380, 600);
		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					580, 530, 580, 600);
		XDrawLine (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					0, 510, 800, 510);

		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					370, 525, "MENU BAR", 8);

		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					50, 525, "(DON'T HIT HERE)", 16);

		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					650, 525, "(DON'T HIT HERE)", 16);


		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					190, 550, "Press 'p' to Pause/Resume", 25);

		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					430, 550, "Press 'q' to Quit", 17);
		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					190, 580, "Press 'f' to Finish", 19);

		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					430, 580, "Press 'r' to Restart", 20);


		// score
		string result;
		const char* sco;
		ostringstream convert;
		convert << score;
		result = convert.str();
		sco = result.c_str();
		int length = result.length();
		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					50, 580, "Score: " , 7);

		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					100, 580, sco, length);

		// FPS

		ostringstream Fps;
		Fps << FPS;
		string fps;
		fps = Fps.str();
		int lenfps = fps.length();
		const char *FPs = fps.c_str();
		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					650, 550, "FPS: " , 4);
		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					700, 550, FPs, lenfps);

		//speed
		ostringstream sppd;
		sppd << glospeed;
		string spd = sppd.str();

		int spdlen = spd.length();
		const char *spdint = spd.c_str();

		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					650, 580, "Speed: " , 7);
		XDrawString (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					700, 580, spdint, spdlen);

	}
}

};
Text text;

//////////////////////////// Snake ///////////////////////////////////

class Snake : public Displayable {
	public:
		
		enum DIRECT {UP,DOWN, LEFT, RIGHT};
		virtual void paint(XInfo &xinfo) {
			if(isStart==true && isGameOver==false && (isLevel1Done == false || isLevel2==true)){
				XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0],x,y, blockSize, blockSize);
				for(int i=0; i<tail.size(); i++){
					XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], tail[i].first, tail[i].second, blockSize, blockSize);
				}
			}
		}

		bool isHitWall(){
			if(x < 0 || y < 0 || x > 799 || y > 509){
				return true;
			}
			else {
				return false;
			}
		}

		bool isHitWall2() {
			if(x <0  || y< 0 || x>= 799 || y>=509){
				return true;
			}
			// middle
			if( x>=150 && x <=599 && y>=280 && y<=299) {
				return true;
			}
			// left up
			if(x>=80 && x<=199 && y>=100 && y<=119 ) {
				return true;
			}
			if(x>=80 && x<=99 && y>=120 && y<=159){
				return true;
			}
			// right up
			if(x>=550 && x<=669 && y>=100 && y<=119){
				return true;
			}
			if(x>=650 && x<=669 && y>=120 && y<=179){
				return true;
			}

			// left down
			if(x>=80 && x<=199 && y>=490 && y<=519){
				return true;
			}
			if(x>=80 && x<=99 && y>=450 && y<=489){
				return true;
			}

			// right down
			if(x>=550 && x<=679 && y>=490 && y<=509){
				return true;
			}
			if(x>=660 && x<=679 && y>=450 && y<=489){
				return true;
			}

			return false;
		}

		int distance(int x1, int y1, int x2, int y2){
			int xsqur = (x2-x1) * (x2-x1);
			int ysqur = (y2-y1) * (y2-y1);
			int d = xsqur + ysqur;
			d = sqrt(d);
			return d;
		}


		bool isHitSelf(){
			for (int i =0; i<tail.size();i++){
				pair<int,int> tmp = tail[i];
				int dist = distance(x,y,tmp.first,tmp.second);
				if(dist < 1){
					return true;
				}
			}
			return false;
		}



		void move(XInfo &xinfo) {

			if(total==0){
				x = x + xspeed; // * blockSize
				y = y + yspeed;  // * blockSize	
			}
			
			else if(isEat == false ){
				for(int i=0 ; i< tail.size()-1; i++){
					tail[i].first= tail[i+1].first;
					tail[i].second= tail[i+1].second;
				}
				int total = tail.size();
				tail[total-1].first =x;
				tail[total-1].second = y;
				x = x + xspeed;  // * blockSize
				y = y + yspeed;  // * blockSize
			}

			else {
				score++;
				//// level2 
				if(total ==7){
					startLevel2();
				}

				isEat = false;
				if(total == 1){
					pair<int, int > tmp = make_pair(x ,y);
					tail.push_back(tmp);
				}
				else {
					int oldx =tail[total-1].first;
					int oldy= tail[total-1].second;
					pair<int, int > tmp = make_pair(x,y);
					tail.push_back(tmp);
				}
				x = x + xspeed;  // * blockSize
				y = y + yspeed;  // * blockSize
			}


			if(isHitWall()==true && isLevel2==false && isLevel1Done==false){
				isGameOver=true;
				gameover.paint(xinfo);
			
			}	

			if(isHitWall2()==true && isLevel1Done==true && isLevel2==true){
				isGameOver=true;
				gameover.paint(xinfo);
			}

			if(isHitSelf()==true){
				isGameOver=true;
				gameover.paint(xinfo);
			}

		}



		int getScore(){
			return total;
		}
		
		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}

		void setTotal (int x) {
			total = x;
		}

	
		void makeTurn (int x, int y){
			xspeed = x;
			yspeed = y;

		}

		void turnLeft(){
			if(direct == RIGHT){
				return;
			}

			makeTurn(-speed,0);
			direct = LEFT;
		}

		void turnRight(){
			if(direct ==LEFT){
				return;
			}
			makeTurn(speed,0);
			direct = RIGHT;
		}

		void turnUp(){
			if(direct ==DOWN){
				return;
			}
			makeTurn(0,-speed);
			direct = UP;
		}

		void turnDown(){
			if(direct ==UP){
				return;
			}
			makeTurn(0,speed);
			direct = DOWN;
		}

		int getSpeed(){
			return speed;
		}



        /*
         * ** ADD YOUR LOGIC **
         * Use these placeholder methods as guidance for implementing the snake behaviour. 
         * You do not have to use these methods, feel free to implement your own.
         */ 
        void didEatFruit();

        void didHitObstacle() {
        }

		
		Snake(int x, int y): x(x), y(y) {
		
            blockSize = 10;
            xspeed = 10;
            yspeed = 0;
            speed = 10;
            total = 0;
            isEat= false;
            direct = RIGHT;

           
		}
	
	private:
		int x;
		int  y;
		int xspeed;
		int  yspeed;
		int speed;
		int total;
		bool isEat;
		int blockSize;
		DIRECT  direct;
		vector<pair<int,int> > tail;

		

};
Snake snake(100, 450);



///////////////////////////////// Fruit ////////////////////////////////////////////////

class Fruit : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			if(isStart==true && isGameOver == false && (isLevel1Done==false || isLevel2==true)){
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], x, y, 10, 10);
			}
        }

        void genrateFruit() {

        	x = rand() % 780 + 10;
        	y = rand() % 480 + 10;

        }

        bool isHitWall3() {
			if(x < 0 || y< 0 || x>= 799 || y>=509){
				return true;
			}
			// middle
			if( x>=140 && x <=599 && y>=270 && y<=299) {
				return true;
			}
			// left up
			if(x>=70 && x<=199 && y>=90 && y<=119 ) {
				return true;
			}
			if(x>=70 && x<=99 && y>=110 && y<=159){
				return true;
			}
			// right up
			if(x>=540 && x<=669 && y>=90 && y<=119){
				return true;
			}
			if(x>=640 && x<=669 && y>=110 && y<=179){
				return true;
			}

			// left down
			if(x>=70 && x<=199 && y>=480 && y<=519){
				return true;
			}
			if(x>=70 && x<=99 && y>=440 && y<=489){
				return true;
			}

			// right down
			if(x>=540 && x<=679 && y>=480 && y<=509){
				return true;
			}
			if(x>=650 && x<=679 && y>=440 && y<=489){
				return true;
			}
			return false;
	}


        void genrateFruit2() {
        	x = rand() % 780 + 10;
        	y = rand() % 480 + 10;
        	if (isHitWall3()==true) {
        		genrateFruit2();
        	}
        }

        int getX(){
        	return x;
        }

        int getY(){
        	return y;
        }


        Fruit() {
            // ** ADD YOUR LOGIC **
            // generate the x and y value for the fruit 
      
            blockSize=10;
            genrateFruit();
        }

        // ** ADD YOUR LOGIC **
        /*
         * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
         */

    private:
        int x;
        int y;
        int blockSize;
};

Fruit fruit;



////////////////////////////////////////////////////////////////////////
int abs(int x, int y){
	int z= x-y;
	if(z<0){
		return -z;
	}
	else {
		return z;
	}
}

void Snake::didEatFruit(){

	if( (abs(fruit.getX() - x) <=7)  && (abs(fruit.getY() - y)) <=7) {
		isEat=true;
		total++;

		if(isLevel1Done==false && isLevel2==false){
			fruit.genrateFruit();
		}
		else {
			fruit.genrateFruit2();
		}
		
	}
}



/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xInfo) {

	if (argc == 3){
		FPS = atoi(argv[1]);
		glospeed = atoi(argv[2]);
	}

	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xInfo.display = XOpenDisplay( "" );
	if ( !xInfo.display )	{
		error( "Can't open display." );
	}
	
   /*
	* Find out some things about the display you're using.
	*/
	xInfo.screen = DefaultScreen( xInfo.display );

	white = XWhitePixel( xInfo.display, xInfo.screen );
	black = XBlackPixel( xInfo.display, xInfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 800;
	hints.height = 600;
	hints.flags = PPosition | PSize;

	xInfo.window = XCreateSimpleWindow( 
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xInfo.display,		// display containing the window
		xInfo.window,		// window whose properties are set
		"Snake Game",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);



	// DOUBLE BUFFER
	// creat bimap (pixmap) to us a other buffer
	int depth = DefaultDepth(xInfo.display, DefaultScreen(xInfo.display));
	xInfo.pixmap = XCreatePixmap(xInfo.display, xInfo.window, hints.width, hints.height, depth);
	xInfo.width = hints.width;
	xInfo.height = hints.height;

	XSetForeground (xInfo.display, xInfo.gc[0], WhitePixel (xInfo.display, xInfo.screen));
	XFillRectangle (xInfo.display, xInfo.pixmap, xInfo.gc[0],
					0, 0, width, height);
	XSetForeground (xInfo.display, xInfo.gc[0], BlackPixel (xInfo.display, xInfo.screen));


	XSelectInput(xInfo.display, xInfo.window, 
		ButtonPressMask | KeyPressMask | 
		PointerMotionMask | 
		EnterWindowMask | LeaveWindowMask |
		StructureNotifyMask);  // for resize events

	
	XSetWindowBackgroundPixmap(xInfo.display, xInfo.window, None);

	//
	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xInfo.display, xInfo.window );
	XFlush(xInfo.display);

	// ADDDDDDDDD:  give serve time to setup
	sleep(1);
}



list<Displayable *> dList;           // list of Displayables

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	XClearWindow( xinfo.display, xinfo.window );
	
	// get height and width of window (might have changed since last repaint)

	XWindowAttributes windowInfo;
	XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
	unsigned int height = windowInfo.height;
	unsigned int width = windowInfo.width;

	// big black rectangle to clear background
	XSetForeground (xinfo.display, xinfo.gc[0], WhitePixel (xinfo.display, xinfo.screen));
	XFillRectangle (xinfo.display, xinfo.pixmap, xinfo.gc[0],
					0, 0, width, height);
	XSetForeground (xinfo.display, xinfo.gc[0], BlackPixel (xinfo.display, xinfo.screen));

	
	// draw display list
	while( begin != end ) {
		Displayable *d = *begin;
		d->paint(xinfo);
		begin++;
	}

	XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height,  0, 0); 
	XFlush( xinfo.display );
}



////////////////// RESTART ////////////////////////////

void restartGame() {
	score = 0;
	snake.setTotal(0);
	snake = Snake(100,450);
	fruit.genrateFruit();
	isGameOver=false;
	isStart = true; 
	isPause = false;
	isLevel1Done =false;
	isLevel2 = false;
}
//////////////// PAUSE /////////////////

class Challenging : public Displayable {
public:
	virtual void paint(XInfo &xinfo) {
		if(isPause == true && (isLevel1Done==false || isLevel2==true)){
			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
	 						350,270,"Pause 'P' to Resume Game",24);
		}	
	}
};
Challenging challenging;

///////////  Level 2 ////////////////////////////

void startLevel2() {
		isLevel1Done = true;
		isGameOver =false;
		isStart = true;
		isPause = true;

}

void startnext(XInfo &xinfo) {

	isLevel2 = true;
	challenging.paint(xinfo);
	fruit.genrateFruit2();

}

void continousNow(XInfo &xinfo) {
	isLevel1Done = false;
	challenging.paint(xinfo);
}

void finshGame() {
	isGameOver = true;
}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)

	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);
		if ((text[0] == 'q') || (text[0]=='Q')) {
			error("Terminating normally.");
		}
		else if((text[0] == 'p') || (text[0]=='P')){
			pauseGame();
		}
		else if ((text[0] == 's') || (text[0]=='S')) {
			startGame(xinfo);

		} 
		else if((text[0] == 'r') || (text[0]=='R')) {
			restartGame();
		}
		else if((text[0] == 'y') || (text[0]=='Y')){
			startnext(xinfo);
		}
		else if((text[0] == 'N') || (text[0]=='n')){
			continousNow(xinfo);
		}
		else if((text[0] == 'f') || (text[0]=='F')){
			finshGame();
		}
	}

	if(isPause == false){
		switch(key) {
		case XK_Left:	
		snake.turnLeft();
			break;
		case XK_Right:
			snake.turnRight();
			break;
		case XK_Up:
			snake.turnUp();
			break;
		case XK_Down:
			snake.turnDown();	
			break;
			}
	}			
}


void handleAnimation(XInfo &xinfo, int inside) {
   
	if(isStart ==true && isPause==false && isGameOver==false){
			snake.move(xinfo);
			snake.didEatFruit();
	}	
}

// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}



void eventLoop(XInfo &xinfo) {

	dList.push_front(&challenging);
	dList.push_front(&startpage);
	dList.push_front(&text);
	dList.push_front(&snake);
    dList.push_front(&fruit);
    dList.push_front(&gameover);
    dList.push_front(&levelpage);
    dList.push_front(&levelup);

	
	XEvent event;
	unsigned long lastRepaint = 0;
	unsigned long lastMove = 0;

	int inside = 0;
	while( true ) {

		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
			}
		} 
		
		unsigned long end = now();
		if(end - lastRepaint > 1000000 / FPS) {

			if(end - lastMove > 1000000 / (glospeed*5)){
				handleAnimation(xinfo,inside);	
				lastMove = now();
			}
			
			repaint(xinfo);
			lastRepaint = now();	
		}

		if (XPending(xinfo.display) == 0){
			usleep(1000000/FPS - (end - lastRepaint));
		}
	}
}




/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {

	// check command line paramenters
	if(argc == 2){
		error("You Have to pass two arguments");
	}
	if(argc >= 4){
		error("Too many arguments");
	}
	if(argc == 3){
		int f = atoi(argv[1]);
		int s = atoi(argv[2]);
		if(f>100 || f<1){
			error("first argument is invalid");
		}
		if(s <1 || s>10){
			error("second argument is invalid");
		}
	}

	XInfo xInfo;
	initX(argc, argv, xInfo);
	eventLoop(xInfo);
	XCloseDisplay(xInfo.display);
}
