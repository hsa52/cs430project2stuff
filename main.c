#include "parse.h"
#include "raycaster.h"

int main(int c, char** argv)
{
    if(!isnumber(argv[2])==0||argv[2]==NULL)
    {
        fprintf(stderr,"Sorry, the width value that you have passed is not of numerical value. :("
                "Please enter in a numerical value and try again.");
        exit(1);
    }
    else if(!isnumber(argv[1])==0||argv[1]==NULL)
    {
        fprintf(stderr,"Sorry, the value you passed in for the height was not of numerical value. :("
                "Please enter in a numerical value and try again.");
        exit(1);

    }
    int len = strlen(argv[4]);
    char* str = &argv[4][len-3];
    if(strcmp(str,"ppm")!=0||argv[4]==NULL)
    {
        fprintf(stderr,"Sorry, the output file is not in the proper form. :( "
                "Please choose an output file with the proper , which is ppm format and try again.");
    }
    Object objects[129];
    read_scene(argv[3],objects);
    raycast(objects,argv[1],argv[2],argv[4]);
    return 0;
}