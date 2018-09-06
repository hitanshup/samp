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
#include <utility>
#include <stdlib.h>

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
int FPS = 30;
int SPEED = 5;
const int width = 800;
const int height = 600;

/*
 * Splash screen variables
*/
const string AuthorName = "Name: Hitanshu Paruthi";
const string AuthorUserId = "userId: hparuthi";
const string GameDescription = "This is snake, play with a and d to turn left and right";


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
};



/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}

void pop_front(vector<pair <int, int> > &vec)
{
    if(!vec.empty())
    	vec.erase(vec.begin());
}

/*
 * An abstract class representing displayable things. 
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};       


class Fruit : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, 10);
        }

        Fruit() {
            // ** ADD YOUR LOGIC **
            // generate the x and y value for the fruit 
            x = 50;
            y = 50;
        }

        int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}

		void resetFruit(vector< pair <int, int> > &blocks) {
			x = (rand() % 80 + 1) * 10;
			y = (rand() % 60 + 1) * 10;
			for (int i = 0; i < blocks.size(); i++) {
				if (x == blocks[i].first) {
					x = (rand() % 60 + 1) * 10;
					i = 0;
				} else if (y == blocks[i].second) {
					y = (rand() % 60 + 1) * 10;
					i = 0;
				}	
			}			
			
		}

        // ** ADD YOUR LOGIC **
        /*
         * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
         */

    private:
        int x;
        int y;
};


class Snake : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			string score_str = "SCORE: " + to_string(score);
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0],
				30, 550, score_str.c_str(),
				score_str.length() );
			for(int i = 0; i < snake_blocks.size(); i++) {
				XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0],
						snake_blocks[i].first, snake_blocks[i].second, blockSize, blockSize);
			}
		
		}
		
		bool move(XInfo &xinfo, Fruit &fruit) {
			if (is_moving_x) {
				x = x + direction;
			} else {
				y = y + direction;
			}
			if (didHitObstacle()) {
				return false;
			}
			snake_blocks.push_back(make_pair(x, y));
			
			if (x != fruit.getX() || y != fruit.getY())
					pop_front(snake_blocks);
			else {
				fruit.resetFruit(snake_blocks);
				score += 10;
			}

            return true;
		}
		
		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}

		int getScore() {
			return score;
		}

        /*
         * ** ADD YOUR LOGIC **
         * Use these placeholder methods as guidance for implementing the snake behaviour. 
         * You do not have to use these methods, feel free to implement your own.
         */ 
        void didEatFruit() {
        }

        bool didHitObstacle() {
        	if (x < 0 || x >= width) {
				return true;
			} else if (y < 0 || y  >= height) {
				return true;
			}
        	for (int i = 0; i < snake_blocks.size(); i++) {
				if (x == snake_blocks[i].first && y == snake_blocks[i].second) {
					return true;
				}
			}
			return false;
        }

        void turnLeft() {
        	if (is_moving_x) {
        		direction = -direction;
        	}
        	is_moving_x = !is_moving_x;
        }

        void turnRight() {
        	if (!is_moving_x)
        		direction = -direction;
        	is_moving_x = !is_moving_x;
        }
		
		Snake(int x, int y): x(x), y(y) {
			snake_blocks.push_back(make_pair(x - 40, y));
			snake_blocks.push_back(make_pair(x - 30, y));
			snake_blocks.push_back(make_pair(x - 20, y));
			snake_blocks.push_back(make_pair(x - 10, y));
			snake_blocks.push_back(make_pair(x, y));
			direction = 10;
            blockSize = 10;
            is_moving_x = true;
            snake_width = 25;
            score = 0;
		}
	
	private:
		int x, y;
		vector<pair <int, int> > snake_blocks;
		int snake_width;

		int blockSize;
		int direction;
		bool is_moving_x;
		int score;
};


class Text : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[0],
				this->x, this->y, this->s.c_str(),
				this->s.length() );
		}
		// constructor
		Text(int x, int y, string s)
			: x(x), y(y), s(s)
		{}
	private:
		int x;
		int y;
		string s;
};




list<Displayable *> dList;           // list of Displayables
Snake snake(100, 450);
Fruit fruit;
Text fps_text(700, 550, "FPS: " + to_string(FPS));
Text speed_text(700, 570, "SPEED: " + to_string(SPEED));


/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xInfo) {
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
		"animation",		// window's title
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

	XSelectInput(xInfo.display, xInfo.window, 
		ButtonPressMask | KeyPressMask | 
		PointerMotionMask | 
		EnterWindowMask | LeaveWindowMask |
		StructureNotifyMask);  // for resize events

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xInfo.display, xInfo.window );
	XFlush(xInfo.display);
}

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	XClearWindow( xinfo.display, xinfo.window );
	
	// get height and width of window (might have changed since last repaint)

	XWindowAttributes windowInfo;
	// XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
	unsigned int height = windowInfo.height;
	unsigned int width = windowInfo.width;

	// big black rectangle to clear background
    
	// draw display list
	while( begin != end ) {
		Displayable *d = *begin;
		d->paint(xinfo);
		begin++;
	}
	XFlush( xinfo.display );
}

void handleKeyPressPause(XInfo &xinfo, XEvent &event) {
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
		if (text[0] == 'r') {
			snake = Snake(100, 450);
			fruit = Fruit();
			dList.pop_front();
			dList.pop_front();
			dList.pop_front();
			dList.pop_front();
			dList.push_back(&snake);
			dList.push_back(&fruit);
			dList.push_front(&fps_text);
		    dList.push_front(&speed_text);
		} else if (text[0] == 'e') {
			dList.pop_front();
			dList.pop_front();
			dList.pop_front();
			dList.pop_front();
			dList.push_back(&snake);
			dList.push_back(&fruit);
			dList.push_front(&fps_text);
		    dList.push_front(&speed_text);
		} else if (text[0] == 'q') {
			error("Terminating normally.");
		} else {
			XEvent event;
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case KeyPress:
					handleKeyPressPause(xinfo, event);
					break;
			}
		}
	}
}

void renderPauseScreen(XInfo &xinfo) {
	Text game_over(350, 120, "GAME OVER");
	Text game_over_restart(330, 140, "Press r to restart");
	Text game_over_quit(330, 160, "Press e to resume");
	Text game_over_quit2(330, 180, "Press q to quit.");
	dList.pop_front();
	dList.pop_front();
	dList.pop_front();
	dList.pop_front();
	dList.push_front(&game_over);
	dList.push_front(&game_over_restart);
	dList.push_front(&game_over_quit);
	dList.push_front(&game_over_quit2);
	usleep(1000000);
	repaint(xinfo);
	usleep(1000000);
	XEvent event;
	XNextEvent( xinfo.display, &event );
	cout << "event.type=" << event.type << "\n";
	switch( event.type ) {
		case KeyPress:
			handleKeyPressPause(xinfo, event);
			break;
	}
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
		if (text[0] == 'q') {
			error("Terminating normally.");
		} else if (text[0] == 'w') {
			//move up
		} else if (text[0] == 'a') {
			snake.turnLeft();
		} else if (text[0] == 'd') {
			snake.turnRight();
		} else if (text[0] == 'p') {
			renderPauseScreen(xinfo);
		} else if (text[0] == 'r') {
			snake = Snake(100, 450);
			fruit = Fruit();
			dList.pop_front();
			dList.pop_front();
			dList.pop_front();
			dList.pop_front();
			dList.push_back(&snake);
			dList.push_back(&fruit);
			dList.push_front(&fps_text);
		    dList.push_front(&speed_text);
		}
	}
}

void handleKeyPressGameOver(XInfo &xinfo, XEvent &event) {
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
		if (text[0] == 'r') {
			snake = Snake(100, 450);
			fruit = Fruit();
			dList.pop_front();
			dList.pop_front();
			dList.pop_front();
			dList.push_back(&snake);
			dList.push_back(&fruit);
			dList.push_front(&fps_text);
		    dList.push_front(&speed_text);
		} else if (text[0] == 'q') {
			error("Terminating normally.");
		} else {
			XEvent event;
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case KeyPress:
					handleKeyPressGameOver(xinfo, event);
					break;
			}
		}
	}
}

void renderGameOverScreen(XInfo &xinfo) {
	Text game_over(350, 120, "GAME OVER");
	Text game_over_restart(330, 140, "Press r to restart");
	Text game_over_quit(330, 160, "Press q to quit.");
	dList.pop_front();
	dList.pop_front();
	dList.pop_front();
	dList.pop_front();
	dList.push_front(&game_over);
	dList.push_front(&game_over_restart);
	dList.push_front(&game_over_quit);
	usleep(1000000);
	repaint(xinfo);
	usleep(1000000);
	XEvent event;
	XNextEvent( xinfo.display, &event );
	cout << "event.type=" << event.type << "\n";
	switch( event.type ) {
		case KeyPress:
			handleKeyPressGameOver(xinfo, event);
			break;
	}
}

void handleAnimation(XInfo &xinfo, int inside) {
    /*
     * ADD YOUR OWN LOGIC
     * This method handles animation for different objects on the screen and readies the next frame before the screen is re-painted.
     */


	if(!snake.move(xinfo, fruit)) {
		renderGameOverScreen(xinfo);
	}
}

// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void renderSplashScreen(XInfo &xinfo) {
	Text author_name(300, 120, AuthorName);
	Text author_user_id(300, 140, AuthorUserId);
	Text game_description(220, 180, GameDescription);
	Text game_description2(220, 200, "Press q to quit, r to restart and p to pause at any time");
	dList.push_front(&author_name);
	dList.push_front(&author_user_id);
	dList.push_front(&game_description);
	dList.push_front(&game_description2);
	usleep(1000000);
	repaint(xinfo);
	usleep(5000000);
	dList.pop_front();
	dList.pop_front();
	dList.pop_front();
	dList.pop_front();
}


void eventLoop(XInfo &xinfo) {
	// splash screen
	renderSplashScreen(xinfo);

	// Add stuff to paint to the display list
	dList.push_front(&snake);
    dList.push_front(&fruit);
	dList.push_front(&fps_text);
    dList.push_front(&speed_text);

	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;
	int x = 1;
	bool firstTime = true;
	while( true ) {
		/*
		 * This is NOT a performant event loop!  
		 * It needs help!
		 */
		
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			cout << "event.type=" << event.type << "\n";
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
		unsigned long speedCounter = 100000 * (40 - (SPEED * 4) + 1);
		// cout<<"speedCounter: "<<speedCounter<<endl;
		// cout << "end " << end - (lastRepaint)<< endl;
		// cout << 1000000/25 << endl;
		if (end - lastRepaint > speedCounter/(FPS)) {
			handleAnimation(xinfo, inside);
			repaint(xinfo);
			lastRepaint = now();
			// cout<<"SCORE: " << snake.getScore() << endl;
		}
		unsigned long tc = lastRepaint - end;
		// for the restart time issue
		if (tc > 1000)
			tc = 100;
		// cout<<"TIME :"<<(tc)<<endl;
		if(XPending(xinfo.display) == 0)
			usleep( 1000000/FPS - (tc));
	}
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {
	XInfo xInfo;
	initX(argc, argv, xInfo);
	for (int i = 1; i < argc; ++i) {
        if (i == 1) {
        	if (atoi(argv[1]) <= 100 &&  atoi(argv[1]) >= 25)
        		FPS = atoi(argv[1]);
        } else {
        	if (atoi(argv[2]) >= 1 && atoi(argv[2]) <= 10)
        		SPEED = atoi(argv[2]);
        }
    }
	eventLoop(xInfo);
	XCloseDisplay(xInfo.display);
}
