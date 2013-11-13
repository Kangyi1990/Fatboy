

//#include <windows.h>				
#include <stdio.h>					
#include "ImageIO.h"

#define	MAX_PARTICLES	2000		
#define MAX_FLASH_PARTICLES 500


				
bool	active=TRUE;				
					
//flame emitter variable
float	slowdown=2.0f;				
//float	xspeed;						
//float	yspeed;						
float	zoom=-20.0f;				
GLuint	loop;						
GLuint	col;						
GLuint	delay;						
GLuint	texture[1];	


//flash emitter variables
float	flash_slowdown=1000.0f;				
//float	xspeed;						
//float	yspeed;						
float	flash_zoom=-40.0f;				

GLuint	flash_loop;						
GLuint	flash_col;						
GLuint	flash_delay;						
GLuint	flash_texture[1];	

float win_width = 768;
float win_height = 512;
typedef struct						
{
	bool	active;					
	float	life;					
	float	fade;					
	float	r;						
	float	g;						
	float	b;						
	float	x;						
	float	y;						
	float	z;						
	float	xi;						
	float	yi;						
	float	zi;						
	float	xg;						
	float	yg;						
	float	zg;						
}
particles;							// Particles Structure

particles particle[MAX_PARTICLES];	// Particle Array 
particles flash_particle[MAX_FLASH_PARTICLES];

int LoadGLTextures(ImageIO* the_image,GLuint* texture )									
{
        int Status=FALSE;								
        
			Status=TRUE;								
			glGenTextures(1, texture);				

			glBindTexture(GL_TEXTURE_2D, *texture);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,the_image->getWidth(), the_image->getHeight(), 0, GL_RGB, GL_FLOAT, the_image->getImageDisplayArray());
        

        
        return Status;									
}

GLvoid reshape(GLsizei width, GLsizei height)		
{
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);					

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									

	
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}


void flameEmitterInit(){

	ImageIO* the_image = new ImageIO("C:/Users/Lanceton/Dropbox/compsci344_code/Final Project/Particle System Code/falme.ppm");
	LoadGLTextures(the_image, &texture[0]);							
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f,0.0f,0.0f,0.0f);					
	glClearDepth(1.0f);								
	glDisable(GL_DEPTH_TEST);							
	glEnable(GL_BLEND);									
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				
	glEnable(GL_TEXTURE_2D);							
	glBindTexture(GL_TEXTURE_2D,texture[0]);			

	for (loop=0;loop<MAX_PARTICLES;loop++)				
	{
		particle[loop].active=true;								
		particle[loop].life=1.0f;								
		particle[loop].fade=float(rand()%100)/1000.0f+0.01f;	
		particle[loop].r=1;	
		particle[loop].g=103/255.0;	
		particle[loop].b=0;	
		particle[loop].xi=float((rand()%50)-26.0f)*3;		
		particle[loop].yi=float((rand()%50)-25.0f)*3;		
		particle[loop].zi=float((rand()%50)-25.0f)*3;	
		particle[loop].xg=0.0f;									
		particle[loop].yg=0.0f;								
		particle[loop].zg=0.0f;									
	}


}

void flashEmitterInit(){
ImageIO* theflash_image = new ImageIO("C:/Users/Lanceton/Dropbox/compsci344_code/Final Project/Particle System Code/flash.ppm");
	LoadGLTextures(theflash_image, &flash_texture[0]);							
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f,0.0f,0.0f,0.0f);					
	glClearDepth(1.0f);								
	glDisable(GL_DEPTH_TEST);							
	glEnable(GL_BLEND);									
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				
	glEnable(GL_TEXTURE_2D);							
	glBindTexture(GL_TEXTURE_2D,flash_texture[0]);			

	for (flash_loop=0;flash_loop<MAX_FLASH_PARTICLES;flash_loop++)				
	{
		flash_particle[flash_loop].active=true;								
		flash_particle[flash_loop].life=1.0f;								
		flash_particle[flash_loop].fade=float(rand()%100)/10000.0f +0.01;	
		flash_particle[flash_loop].r=227/255.0;	
		flash_particle[flash_loop].g=140.0/255.0;	;	
		flash_particle[flash_loop].b=45/255.0;	
		flash_particle[flash_loop].xi=float((rand()%50)-26.0f)*2;		
		flash_particle[flash_loop].yi=float((rand()%50)-25.0f)*2;			
		flash_particle[flash_loop].zi=float((rand()%50)-25.0f)*2;		
		flash_particle[flash_loop].xg=0.0f;									
		flash_particle[flash_loop].yg=0.0f;								
		flash_particle[flash_loop].zg=0.0f;									
	}



}


int InitGL(GLvoid)										
{
	
	flashEmitterInit();
	flameEmitterInit();
	return TRUE;										
}


void flameEmitterDisplay(){
for (loop=0;loop<MAX_PARTICLES;loop++)					
	{
		if (particle[loop].active)							
		{
			float x=particle[loop].x;						
			float y=particle[loop].y;						
			float z=particle[loop].z+zoom;					

			
			glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life);
			glBindTexture(GL_TEXTURE_2D,texture[0]);	
			glBegin(GL_TRIANGLE_STRIP);						
			    glTexCoord2d(1,1); glVertex3f(x+1.0f,y+1.0f,z); 
				glTexCoord2d(0,1); glVertex3f(x-1.0f,y+1.0f,z); 
				glTexCoord2d(1,0); glVertex3f(x+1.0f,y-1.0f,z); 
				glTexCoord2d(0,0); glVertex3f(x-1.0f,y-1.0f,z); 
			glEnd();			
			
			particle[loop].x+=particle[loop].xi/(slowdown*500);
			particle[loop].y+=particle[loop].yi/(slowdown*500);
			particle[loop].z+=particle[loop].zi/(slowdown*500);

			particle[loop].xi+=particle[loop].xg;			
			particle[loop].yi+=particle[loop].yg;			
			particle[loop].zi+=particle[loop].zg;			
			particle[loop].life-=particle[loop].fade;		

			if (particle[loop].life<0.0f)					
			{
				/*particle[loop].life=1.0f;					
				particle[loop].fade=float(rand()%100)/1000.0f+0.003f;	
				particle[loop].x=0.0f;						
				particle[loop].y=0.0f;						
				particle[loop].z=0.0f;						
				particle[loop].xi=xspeed+float((rand()%60)-32.0f);	
				particle[loop].yi=yspeed+float((rand()%60)-30.0f);	
				particle[loop].zi=float((rand()%60)-30.0f);	
				particle[loop].r=1;			
				particle[loop].g=103/255.0;			
				particle[loop].b=0;			*/
			}

			

			
		}
}


}


void flashEmitterDisplay(){
for (flash_loop=0;flash_loop<MAX_FLASH_PARTICLES;flash_loop++)					
	{
		if (flash_particle[flash_loop].active)							
		{
			float x=flash_particle[flash_loop].x;						
			float y=flash_particle[flash_loop].y;						
			float z=flash_particle[flash_loop].z+flash_zoom;					

			
			glColor4f(flash_particle[flash_loop].r,flash_particle[flash_loop].g,flash_particle[flash_loop].b,flash_particle[flash_loop].life);
			glBindTexture(GL_TEXTURE_2D,flash_texture[0]);	
			glBegin(GL_TRIANGLE_STRIP);						
			    glTexCoord2d(1,1); glVertex3f(x+2.0f,y+2.0f,z); 
				glTexCoord2d(0,1); glVertex3f(x-2.0f,y+2.0f,z); 
				glTexCoord2d(1,0); glVertex3f(x+2.0f,y-2.0f,z); 
				glTexCoord2d(0,0); glVertex3f(x-2.0f,y-2.0f,z); 
			glEnd();			
			
			flash_particle[flash_loop].x+=	flash_particle[flash_loop].xi/(flash_slowdown);
			flash_particle[flash_loop].y+=	flash_particle[flash_loop].yi/(flash_slowdown);
			flash_particle[flash_loop].z+=	flash_particle[flash_loop].zi/(flash_slowdown);

			flash_particle[flash_loop].xi+=	flash_particle[flash_loop].xg;			
			flash_particle[flash_loop].yi+=	flash_particle[flash_loop].yg;			
			flash_particle[flash_loop].zi+=	flash_particle[flash_loop].zg;			
			flash_particle[flash_loop].life-=	flash_particle[flash_loop].fade;		

			if (flash_particle[flash_loop].life<0.0f)					
			{
				/*particle[loop].life=1.0f;					
				particle[loop].fade=float(rand()%100)/1000.0f+0.003f;	
				particle[loop].x=0.0f;						
				particle[loop].y=0.0f;						
				particle[loop].z=0.0f;						
				particle[loop].xi=xspeed+float((rand()%60)-32.0f);	
				particle[loop].yi=yspeed+float((rand()%60)-30.0f);	
				particle[loop].zi=float((rand()%60)-30.0f);	
				particle[loop].r=1;			
				particle[loop].g=103/255.0;			
				particle[loop].b=0;			*/
			}

			

			
		}
}


}


void  display(GLvoid)										
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	glLoadIdentity();										
	
	flashEmitterDisplay();
	flameEmitterDisplay();
    
										
}



void idle( void ){
    
    glutPostRedisplay();
}
int main(int argc, char * argv[])
{
    
    glutInit( &argc, argv );
    
    glutInitDisplayMode( GLUT_RGB );
    glutInitWindowSize( win_width, win_height );
	
    glutCreateWindow( "Image" );
    
    InitGL();
	
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );

    glutIdleFunc( idle );
    
        
    glutMainLoop();
    
    
   
    return 0;
}
