/*
 *
 * Original author: ewaller @ Arch linux forums
 * Found on https://bbs.archlinux.org/viewtopic.php?id=197863
 *
 * Minor modification to handle Macbook kb backlight instead.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

typedef struct {
  int verbose, inc, dec, set;
} ProgramArguments;

static ProgramArguments arguments;

static struct argp_option options[] =
{
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"inc", 'i', "INT",0,"Increment"},
  {"dec", 'd', "INT",0,"Decrement"},
  {"set", 's', "INT",0,"Set"},
  {0}
};

const char *argp_program_version = "keylight 0.1";
const char *argp_program_bug_address = "<robin@lindstream.com>";
static char doc[] =
  "keylight -- Read, set, increment, or decrement the keyboard backlight on your Macbook (Pro)";

static char args_doc[] = "";
static error_t parse_opt (int key, char *arg, struct argp_state *state);

static struct argp argp = { options, parse_opt, args_doc, doc };

int ReadSysFile(char* theFileName)
{
  char* readBuffer = NULL;
  long unsigned int  bufferSize = 0;
  
  FILE *theFile = fopen(theFileName,"r");
  if (!theFile)
    {
      fprintf(stderr,"\nCould not open the file %s\n",theFileName);
      return -1;
    }
  
  getline(&readBuffer, &bufferSize, theFile);
  if (readBuffer)
    {
      int theIntValue=atoi(readBuffer);
      free(readBuffer);
      readBuffer=NULL;
      bufferSize=0;
      fclose(theFile);
      return (theIntValue);
    }
  fclose(theFile);
  return -1;
}

int WriteSysFile(char* theFileName, int theValue)
{
  FILE *theFile = fopen(theFileName,"w");
  if (!theFile)
    {
      fprintf(stderr,"\nCould not open the file %s\n",theFileName);
      return -1;
    }
  int returnValue;
  returnValue = fprintf(theFile,"%i\n",theValue);
  fclose(theFile);
  return returnValue;
}

int parseIntArgument(char* arg)
{
  char *endptr, *str;
  long val;
  errno = 0;    /* To distinguish success/failure after call */
  val = strtol(arg, &endptr, 10);
  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
      || (errno != 0 && val == 0))
    {
      perror("strtol");
      exit(EXIT_FAILURE);
    }
  
  if (endptr == str) {
    fprintf(stderr, "No digits were found\n");
    exit(EXIT_FAILURE);
  }
  if (*endptr)
    {+
      printf ("Non digit in decimal value\n");
      exit(EXIT_FAILURE);
    }
  /* If we got here, strtol() successfully parsed a number */
  // printf("Arg: %s\nVal:%i\nEnd: %s\n",arg,val,endptr);
  return (int)val;
}

void TooManyOptions(void)
{
  printf("Increment, Decrement and Set are mutually exclusive options\n");
  exit(EXIT_FAILURE);
}

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  /*
   *  Process the command line arguments and options.  Collect all 
   *  the options and remember their state.  
   */
  
  ProgramArguments* argumentPtr = state->input;
  
  switch (key)
    {
    case 'v':
      argumentPtr->verbose = 1;
      break;
    case 'i':
      if ((arguments.dec != -1) || (arguments.set != -1))
    TooManyOptions();
      arguments.inc=parseIntArgument(arg);
      break;
    case 'd':
      if ((arguments.inc != -1) || (arguments.set != -1))
    TooManyOptions();
      arguments.dec=parseIntArgument(arg);
      break;
    case 's':
      if ((arguments.dec != -1) || (arguments.inc != -1))
    TooManyOptions();
      arguments.set=parseIntArgument(arg);
      break;
    case ARGP_KEY_NO_ARGS:
      /* If there are no Arguments, that is good.  We don't want any */
      break;
    case ARGP_KEY_ARG:
      /* I am not expecting any arguments that are not options. */
      argp_usage (state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}


int main (int argc, char** argv)
{
  arguments.verbose = 0;
  arguments.set = -1;
  arguments.inc = -1;
  arguments.dec = -1;
  
  int max_brightness = 0;
  int brightness =0;
  max_brightness = ReadSysFile("/sys/class/leds/smc::kbd_backlight/max_brightness");
  if (max_brightness < 0)
    exit(EXIT_FAILURE);
  brightness = ReadSysFile("/sys/class/leds/smc::kbd_backlight/brightness");
  if (brightness < 0)
    exit(EXIT_FAILURE);
  argp_parse (&argp, argc, argv, 0, 0, &arguments);
  if (arguments.inc >= 0 ) brightness += arguments.inc;
  if (arguments.dec >= 0 ) brightness -= arguments.dec;
  if (arguments.set >= 0 ) brightness  = arguments.set;
  if (brightness<0) brightness = 0;
  if (brightness>max_brightness) brightness = max_brightness;
  if ((arguments.inc >= 0) || (arguments.dec >= 0) || (arguments.set >= 0))
    if (WriteSysFile("/sys/class/leds/smc::kbd_backlight/brightness",brightness) < 0)
      printf("Unable to set brightness.  Check permissions");
  printf("Max Brightness = %i\n",max_brightness);
  printf("Current Brightness = %i\n",brightness);
  if (arguments.verbose)
    {
      printf("Increment:%i; Decrement:%i, Set:%i\n",arguments.inc,arguments.dec,arguments.set);
    }
}