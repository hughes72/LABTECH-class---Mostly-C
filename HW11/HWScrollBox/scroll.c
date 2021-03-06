  /* scroll.c -- scrolling display
 *
 * Kyle Hughes
 * 
 */

#include <time.h>
#include <stdio.h>

// sleep needs this
#include <unistd.h>

#include "scroll.h"

// debug flag - changed via options
int debug = 0;
int SPEED = 0;
int TIMER = 0;
int TEST = 0;
char *timer;
static char rcsid[] ="$Id: $";

static char bugaddress[]="hughes72@elvis.rowan.edu";

// version -- say which version this is and exit
// (note simple output; you could also print the rcsid too)
void version()
{
    fprintf(stderr, "scroll version 1\n");
    exit(0);
}

// usage -- print brief summary and instructions
void usage(char *progname)
{
    fprintf(stderr, "This program shows a scrolling box.\n");
    fprintf(stderr, "Usage: %s [-dDvh] text\n", progname);
    fprintf(stderr, "  -d # : set scroll delay to # milliseconds (default=1000)\n");
    fprintf(stderr, "  -D   : turn on debug messages (more Ds = more debugging)\n");
    fprintf(stderr, "  -v   : show version information\n");
    fprintf(stderr, "  -h   : this help message\n");
    fprintf(stderr, "Use quotes for multi-word messages, 'like this'.\n");
    fprintf(stderr, "report bugs to %s \n", bugaddress);
    exit (0);
}



void exit_scroll()
{
    end_display();
    exit(0);
}

 

char  title[81] =
   "                                  Hughes' Scroller                             ";

// keyboard and mouse handling
void process_key(keybits KeyCode)
{
    int row, col;
    if ( KeyCode & 0x80 ) { // mouse click
        row = (KeyCode & 0x70) >> 4;
        col = (KeyCode & 0x0f);

	if(row == 0 && col == 4){
	  exit_scroll();
	  TIMER = 0;
	}
	if(row ==0 && col == 0){
	  TIMER =0;
	  SPEED = 0;
	  start_timer(SPEED);
	  
	}
	if(row==0 && col == 1){
	  TIMER =0;
	  SPEED = 1;
	  start_timer(SPEED);
      }

	if(row ==0 && col ==2){

	  TIMER = 1;

	}
	if(row ==0 && col ==3){

	  TEST = 1;

	}

    } else { // keyboard press
        switch(KeyCode) {
            case 'q': exit_scroll(); break;
	
	}
    }
}

int main(int argc, char *argv[])
{
    int   letter; // option char, if any
    int   delay = 1000; // milliseconds of delay

    // loop through all the options; getopt() can handle together or apart
    while ( ( letter = getopt(argc, argv, "d:Dvh")) != -1 ) {
        switch (letter) {
            case 'd':  delay = atoi(optarg);   break;

            case 'D':  debug++;                break;
            case 'v':  version();              break;
            case 'h':  usage(argv[0]);         break;

            case '?':  // unknown flag; fall through to next case
            default:   // shouldn't happen, but Just In Case
                       // note that getopt() warns about the unknown flag
                  fprintf(stderr, "run \"%s -h\" for help\n", argv[0]);
                  exit(1);
        }
    }

    // optind is the first argument after options are processed
    // if there aren't any, the user didn't give a message to scroll
    if (optind == argc) {
        usage(argv[0]);
    }

    // put the information from the command line into the module
    setup(argv[optind]);

    // set up the view
    register_keyhandler(process_key);

 
      set_title_bar(title);
 

    // turn on some keys in row 2
 

 

    start_display();
    start_timer(delay);    // normal timer

    // wait for the model to signal controller
    while ( 1 ) {
        get_key();
    }

    end_display();

    return 0;
}

/* This is the function the model uses to signal the controller
 * that there's work to do.
 */
int settime =0;
char *tock;
void tick(int sig)
{  // char *tock ;
    char   *segment;
    

    /* get the information from model about what chars to show
     * and how far over to slide them */ 
    segment = display_string(TEST);
    tock = segment;     //store the scroll string    
    if (debug >= 3) {
        fprintf(stderr, "display_string() returned |%s|\r\n", segment);
        sleep(2);
    }


    if(TIMER ==1 && settime<5){
      start_timer(4);
      settime++;
      time_t now;
      struct tm *dateinfo;
      (void) time(&now);
      dateinfo = localtime(&now);
      sprintf(segment,"%02d:%02d:%02d",dateinfo->tm_hour,
	      dateinfo->tm_min,
	      dateinfo->tm_sec);
      show(segment);

    }
    else{
      TIMER = 0;
      settime =0;
    /* Send those chars to the view. */
    show(tock);
    }
    get_key();
}

