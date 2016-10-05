#include "includingstuff.h"
//struct for storing pixels to write to ppm file
typedef struct
{
    int r; int g; int b;
} Pixel;

void writing_to_p3(Pixel* pixel,int w,int h,char* filename)
{
    //opens the file handle
    FILE *fh = fopen(filename,"w+");
    //writes to the header file
    fprintf(fh,"P3 %d %d 255 \n",w,h);
    for(int i = 0;i<w*h;i++)
    {
        //loops through the pixels
        //writes them to the file
        fprintf(fh,"%d ",pixel[i].r);
        fprintf(fh,"%d ",pixel[i].g);
        fprintf(fh,"%d \n",pixel[i].b);
    }
    //closes the file handle
    fclose(fh);
}

static inline double sqr(double v)
{
    return v*v;
}

static inline void normalize(double* v)
{
    double len =  sqr(v[0])+sqr(v[1])+sqr(v[2]);
}
static inline double v3_dot(double* a, double* b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

double sphereintersect(double* ro,double* rd,double rad,double* center )
{
    normalize(center);
    double a = sqr(rd[0]) + sqr(rd[1]) + sqr(rd[2]);
    double b = 2 * (rd[0] * (ro[0] - center[0]) + rd[1] * (ro[1] - center[1]) + rd[2] * (ro[2] - center[2]));
    double c = sqr(ro[0] - center[0]) + sqr(ro[1] - center[1]) + sqr(ro[2] - center[2]) - sqr(rad);
    //finds the value of the determinant
    double det = sqr(b) - 4 * a * c;

    if (det < 0)
        // if there is no intersection appearing
        return -1;

    det = sqrt(det);

    //Quadratic equation can be both positive and negative
    //we run through it twice because of this
    // If it is negative
    double t0 = (-b - det) / (2 * a);

    if (t0 > 0)
        return t0;
    // If it positive
    double t1 = (-b + det) / (2 * a);
    if (t1 > 0)
        //returns the value of t
        return t1;
    //if t value is not returned then an error is brought up
    return -1;
}

double planeintersect(double*ro,double*rd,double* normal,double* position)
{
    normalize(normal);
    normalize(position);
    //this is how much of a distance there is
    //from the origin to the plane
    double D = -(v3_dot(position,normal));
    double t = -(normal[0] * ro[0] + normal[1] * ro[1] + normal[2] * ro[2] + D) /
               (normal[0] * rd[0] + normal[1] * rd[1] + normal[2] * rd[2]);

    if (t > 0)
        return t;
    return -1;
}

void raycast(Object* objects,char* picture_height,char* picture_width,char* output_file)
{
    //loops
    int j=0,k=0;

    //positions the camera
    double cx=0;
    double cy=0;

    //finds camera in Objects
    for(j=0;j< sizeof(objects);j++)
    {
        if(objects[j].kind==0)
        {
            break;
        }
    }

    //sets height and width of camera and scene
    //finds the pixheight+width
    double h =objects[j].cam.height;
    double w=objects[j].cam.width;
    int m =atoi(picture_height);
    int n = atoi(picture_width);
    double pixheight =h/m;
    double pixwidth =w/m;

    //creates the pixel array
    Pixel p[m*n];

    //loops through x and y coords.
    //flips y coords to output image correctly
    for(int y=m;y>0;y--)
    {
        for (int x=0;x<n;x++)
        {
            //origin of ray
            double ro[3]={0,0,0};
            //direction of ray
            double rd[3] =
                    {
                    cx - (w/2) + pixwidth * (x + 0.5),
                    cy - (h/2) + pixheight * (y + 0.5),
                    1
            };
            //normalize ray direction
            normalize(rd);
            double best_t=INFINITY;
            //creates a temp object hold object
            //the object temp object is holding intersects with ray
            Object object;

            //loops through objects
            for(int i=0;i<sizeof(objects);i++)
            {
                double t = 0;
                //switches by object type
                //sends it to correct intersection
                switch (objects[i].kind)
                {
                    case 1:
                        t = sphereintersect(ro, rd, objects[i].sphere.radius, objects[i].sphere.center);
                        break;
                    case 2:
                        t = planeintersect(ro, rd, objects[i].plane.normal, objects[i].plane.position);
                        break;
                    case 0:
                        break;
                    default:
                        fprintf(stderr,"Unknown File Type Found. Sorry. Try Again :(");
                        exit(-1);
                }

                //finds best intersection
                //assigns it to an object
                //resets best_t
                if(t>0 &&t<best_t)
                {
                    best_t=t;
                    object=objects[i];
                }
            }
            //assigns a color of the object to pixel array
            //if best_t has an intersecting ray
            if(best_t > 0 && best_t != INFINITY)
            {
                //cast to int
                //sphere
                if(object.kind==1)
                {
                    p[k].r=(int)(object.sphere.color[0]*255);
                    p[k].g=(int)(object.sphere.color[1]*255);
                    p[k].b=(int)(object.sphere.color[2]*255);
                }
                //plane
                if(object.kind==2)
                {
                    p[k].r=(int)(object.plane.color[0]*255);
                    p[k].g=(int)(object.plane.color[1]*255);
                    p[k].b=(int)(object.plane.color[2]*255);
                }
            } else
            {
                //if there is no intersection
                //makes the image print out a black image
                p[k].r=0;
                p[k].g=0;
                p[k].b=0;
            }
            k++;
        }
    }
    //writes the image to the ppm file to finish the command
    writing_to_p3(p,atoi(picture_height),atoi(picture_width),output_file);
}