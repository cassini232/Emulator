// lp_gfx.cpp		light panel graphics 


/* Copyright (c) 2007-2008, John Kichury

   This software is freely distributable free of charge and without license fees with the 
   following conditions:

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
   JOHN KICHURY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
   IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

   The above copyright notice must be included in any copies of this software.

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include "lpanel.h"
#include "lp_utils.h"
#include "jpeg.h"


lpObject *
Lpanel::addObject(void)
{
 lpObject *obj;

  if(num_objects + 1 > max_objects)
     growObjects();
   obj = new lpObject;

 if(obj) 
  { 
    obj->color[0] = .1;
    obj->color[1] = .1;
    obj->color[2] = .1;
    obj->referenced = 0;
    objects[num_objects] = obj;
    num_objects++;
  }
 return obj;
}

lpObject *
Lpanel::findObjectByName(char *name)
{
 int i;

 for(i=0;i<num_objects;i++)
  if(objects[i]->name)
   if(!strcmp(name, objects[i]->name)) return(objects[i]);

 return NULL;
}

void
Lpanel::genGraphicsData(void)
{
 int i,j, first;
    
 // generate data such as geometric extents, texture coords etc.
 // for all graphics objects

 for(i=0;i<num_objects;i++)
   objects[i]->genGraphicsData();

 // get bounding box for all geometry

 first = 1;

 for(i=0;i<num_objects;i++)
  {  
    if(objects[i]->referenced) continue;

     if(first)
      {
	for(j=0;j<3;j++)
	 {  bbox.xyz_min[j] = objects[i]->bbox.xyz_min[j];
	    bbox.xyz_max[j] = objects[i]->bbox.xyz_max[j];
	 }
	first=0;
      }
     else
      {
	for(j=0;j<3;j++)
	 { bbox.xyz_min[j] = min(bbox.xyz_min[j], objects[i]->bbox.xyz_min[j]);
	   bbox.xyz_max[j] = max(bbox.xyz_max[j], objects[i]->bbox.xyz_max[j]);
	 }
      }
 }

  for(j=0;j<3;j++)
    bbox.center[j] = (bbox.xyz_min[j] + bbox.xyz_max[j]) / 2.0f;

}

void
Lpanel::growObjects(void)
{
  int i;
  lpObject **new_objects;

  new_objects = new lpObject * [num_objects + 1];
  for(i=0;i<num_objects;i++)
    new_objects[i] = objects[i];

  max_objects += 1;
  if(objects) delete objects;
  objects = new_objects;
}



// ---------------------
// object class 

lpObject::lpObject(void)
{
 num_elements = max_elements = 0;
 elements = NULL;
 name=NULL;
 texture_num = 0;
 textures = NULL;
 texture_scale[0] = texture_scale[1] = 1.0;
 texture_translate[0] = texture_translate[1] = 0.0;
}

lpObject::~lpObject(void)
{
 int i;

 if(elements)
  {
   for(i=0;i<num_elements;i++)
    if(elements[i]) delete elements[i];

   delete elements;
  } 
}


lpElement*
lpObject::addElement(void)
{
 lpElement *element;

  if(num_elements + 1 > max_elements)
     growElements();

 element = new lpElement;
 if(element) 
  {
   elements[num_elements] = element;
   num_elements++;
  }
 return element;
}

void
lpObject::draw(void)
{
  int i;

  if(referenced) return;
  glColor3fv(color);
  for(i=0;i<num_elements;i++)
    elements[i]->draw();

}

void
lpObject::draw(int referenced)
{
  int i;

  if(texture_num) 
  	textures->bindTexture(texture_num);
  if(referenced != 2) glColor3fv(color);
  for(i=0;i<num_elements;i++)
    elements[i]->draw();

}

void
lpObject::genGraphicsData(void)
{
 int i, j, first=1;
    
 // generate data such as geometric extents, texture coords etc.
 // for this object 

 for(i=0;i<num_elements;i++)
  {  
     elements[i]->genGraphicsData();

     if(first)
      {
	for(j=0;j<3;j++)
	 {  bbox.xyz_min[j] = elements[i]->bbox.xyz_min[j];
	    bbox.xyz_max[j] = elements[i]->bbox.xyz_max[j];
	 }
	first=0;
      }
     else
      {
	for(j=0;j<3;j++)
	 { bbox.xyz_min[j] = min(bbox.xyz_min[j], elements[i]->bbox.xyz_min[j]);
	   bbox.xyz_max[j] = max(bbox.xyz_max[j], elements[i]->bbox.xyz_max[j]);
	 }
      }

  }

  for(j=0;j<3;j++)
    bbox.center[j] = (bbox.xyz_min[j] + bbox.xyz_max[j]) / 2.0f;


  // if this object has a texture defined, generate texture coords for elements that need them

 if(texture_num != 0)
  for(i=0;i<num_elements;i++)
  {  
    elements[i]->genTextureCoords(this, &bbox);

  }
}

void
lpObject::growElements(void)
{
  int i;
  lpElement **new_elements;

  new_elements = new lpElement* [num_elements+ 1];
  for(i=0;i<num_elements;i++)
    new_elements[i] = elements[i];

  max_elements += 1;
  if(elements) delete elements;
  elements = new_elements;
}

void
lpObject::setName(char *s)
{
  if(name) delete name;
  name = new char[ strlen(s)+1];
  strcpy(name,s);
}


lpElement::lpElement(void)
{
  num_verts = max_verts = 0;
  have_tcoords = 0;
  verts = NULL;
}

lpElement::~lpElement(void)
{
  int i;

  if(verts)
   {
     for(i=0;i<num_verts;i++)
	if(verts[i]) delete verts[i];

     delete verts;
   }
}


vertex_t *
lpElement::addVertex(void)
{
 int i;
 vertex_t *vert;

  if(num_verts + 1 > max_verts )
     growVerts();

 vert = new vertex_t;
 if(vert)
  { for(i=0;i<3;i++)
     {  vert->xyz[i] = 0.;
        vert->norm[i] = 0.;
     }

    for(i=0;i<2;i++)
       vert->st[i] = 0.;
 
    verts[num_verts] = vert;
    num_verts++;
 }

 return vert;
}

void
lpElement::draw(void)
{
  int i;

 if(have_tcoords)
  glEnable(GL_TEXTURE_2D);
 else
  glDisable(GL_TEXTURE_2D);

  switch(type)
  { case LP_LINE:
	glBegin(GL_LINE_LOOP);
	break;
    case LP_POLYGON:
	glBegin(GL_POLYGON);
	break;
    default:
	break;
  }

  if(have_tcoords) 
   { 
     for(i=0;i<num_verts;i++)
      { 

//	glTexCoord2fv( verts[i]->st );
	textures->TexCoord2fv( verts[i]->st );
	glVertex3fv( verts[i]->xyz );
      }
   }
  else
   {
     for(i=0;i<num_verts;i++)
      { glVertex3fv( verts[i]->xyz );
      }
   }
  glEnd();
}

void
lpElement::genGraphicsData(void)
{
 int first=1,
     i,j;

  // calc bbox for this element


  for(i=0;i<num_verts;i++)
   {
     if(first)
      {
	for(j=0;j<3;j++)
	  bbox.xyz_min[j] = bbox.xyz_max[j] = verts[i]->xyz[j];

	first = 0;
      }
     else
      {
	for(j=0;j<3;j++)
	 {  
	   bbox.xyz_min[j] = min(bbox.xyz_min[j],verts[i]->xyz[j]);
	   bbox.xyz_max[j] = max(bbox.xyz_max[j],verts[i]->xyz[j]);
         }
      }
   }

}

void
lpElement::genTextureCoords(lpObject *obj, lpBBox *bbox)
{
 int i,j;

 if(have_tcoords) return;	// already have them from config file 

 // generate texture coordinates

 for(i=0;i<num_verts;i++)
    for(j=0;j<2;j++)
     {	verts[i]->st[j] =  (1.0 / (bbox->xyz_max[j] - bbox->xyz_min[j])) * (verts[i]->xyz[j] - bbox->xyz_min[j]);
        verts[i]->st[j] *= 1.0 / obj->texture_scale[j];
        verts[i]->st[j] -= obj->texture_translate[j];
     }
	//verts[i]->st[j] =  (1.0 / (bbox->xyz_max[j] - bbox->xyz_min[j])) * (verts[i]->xyz[j] - bbox->xyz_min[j]);

 have_tcoords = 1;

}

void
lpElement::growVerts(void)
{
  int i;
  vertex_t **new_verts;

  new_verts = new vertex_t * [num_verts + 4];
  if(new_verts)
  {
   for(i=0;i<num_verts;i++)
    new_verts[i] = verts[i];

   max_verts += 4;
   if(verts) delete verts;
   verts = new_verts;
  }
}



// texture class


lpTextures::lpTextures(void)
{
  tex = new texture_t * [1];
  tex[0] = new texture_t;
  num_textures = 1;	// start at 1, zero reserved for unbinding
  max_textures = 1;
  last_accessed = 0;
  
}

lpTextures::~lpTextures(void)
{
 int i;
 
 if(tex)
 {
   for(i=0;i<num_textures;i++)
	if(tex[i]) 
	 {
	   if(tex[i]->texels) delete tex[i]->texels;
	   delete tex[i]; 
	 }

  delete tex;
 }

}


int
lpTextures::addTexture(char *fname)
{
 int texnum;
 unsigned char *p;


 if(num_textures + 1 > max_textures) growTextures();
 texnum = num_textures;

 tex[texnum] = new texture_t;

#if 0
 if((tex[texnum]->pixels = read_jpeg(fname, &tex[texnum]->imgXsize,
			  &tex[texnum]->imgYsize,
			  &tex[texnum]->imgZsize)) == 0) return(0);
#endif

 p = read_jpeg(fname, &tex[texnum]->imgXsize,
			  &tex[texnum]->imgYsize,
			  &tex[texnum]->imgZsize);
 if(!p) return 0;
 tex[texnum]->pixels = p;
 num_textures++;

// printf("\n\nAddTextureFile: added %s %dx%dx%d as texnum %d \n",fname,tex[texnum]->imgXsize,
//       tex[texnum]->imgYsize, tex[texnum]->imgZsize,texnum);

 last_accessed = texnum;
 return(texnum);
}


int
lpTextures::downloadTextures(void)
{
 int texnum;
 int n;

for(texnum = 1; texnum < num_textures; texnum++)
 if(tex[texnum]->pixels)
 {
 // calc power of two S and T dimensions

 tex[texnum]->texSsize = GetPowerOf2i(tex[texnum]->imgXsize); 
 tex[texnum]->texTsize = GetPowerOf2i(tex[texnum]->imgYsize); 

 // set gl parms

 switch(tex[texnum]->imgZsize)
 { case 1:
	tex[texnum]->format = GL_LUMINANCE;
	break;
   case 2:
	tex[texnum]->format = GL_LUMINANCE_ALPHA;
	break;
   case 3:
	tex[texnum]->format = GL_RGB;
	break;
   case 4:
	tex[texnum]->format = GL_RGBA;
	break;
   default:
	fprintf(stderr,"addTexture: Invalid # texture components.\n");
	return(0);
	break;

 return 1;
 }

 // set type

  tex[texnum]->type = GL_UNSIGNED_BYTE;

 // calc S and T limits

 tex[texnum]->texSmin = 0; 
 tex[texnum]->texTmin = 0; 
 tex[texnum]->texSmax =  (float) tex[texnum]->imgXsize / 
			(float) tex[texnum]->texSsize; 

 tex[texnum]->texTmax =  (float) tex[texnum]->imgYsize / 
			(float) tex[texnum]->texTsize; 

 // copy image pixel data to texel data

 unsigned char *src, *dst;
 int x,y,z;

 src = tex[texnum]->pixels;

#if 0
 tex[texnum]->texels = (unsigned char *) malloc(tex[texnum]->texSsize *
				tex[texnum]->texTsize *
				tex[texnum]->imgZsize);
#endif

 tex[texnum]->texels = new unsigned char  [tex[texnum]->texSsize *
				tex[texnum]->texTsize *
				tex[texnum]->imgZsize];

 dst = tex[texnum]->texels;

#if 0
  for(y=0;y<tex[texnum]->texTsize;y++)
   for(x=0;x<tex[texnum]->texSsize;x++)
    for(z=0;z<tex[texnum]->imgZsize;z++)
        if(y < tex[texnum]->texTsize && x < tex[texnum]->texSsize) 
         { *dst++ = *src++;
         }
        else *dst++ = 0;
#endif

  for(y=0;y<tex[texnum]->texTsize;y++)
   for(x=0;x<tex[texnum]->texSsize;x++)
    for(z=0;z<tex[texnum]->imgZsize;z++)
        if(y < tex[texnum]->imgYsize && x < tex[texnum]->imgXsize) 
         { 
           *dst++ = *src++;
         }
        else *dst++ = 0;

 // get a bind id from OpenGL

 n = glGetError(); /* clear any gl errors */

 glGenTextures(1,(GLuint *)&tex[texnum]->bind_id);
 glBindTexture( GL_TEXTURE_2D, tex[texnum]->bind_id );
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

 glTexImage2D(GL_TEXTURE_2D, 0, tex[texnum]->imgZsize, tex[texnum]->texSsize,
               tex[texnum]->texTsize, 0, tex[texnum]->format, GL_UNSIGNED_BYTE,
               tex[texnum]->texels);

 glBindTexture( GL_TEXTURE_2D, 0 );
 check_glerror();

 } // end for(texnum... 

 return 1;
}

void
lpTextures::bindTexture(int n)
{
  if(n == 0)
    glBindTexture(GL_TEXTURE_2D, 0);
  else
   {
   if( n > num_textures )
     {
       fprintf(stderr,"bindTexture: Invalid texture number (%d)\n",n);
       return;
     }
   else 
    { 
    //   printf("bind %d %d\n", n, tex[n]->bind_id);
       glBindTexture(GL_TEXTURE_2D,tex[n]->bind_id);
    }
   }
 last_accessed = n;
}

void
lpTextures::growTextures(void)
{
  int i;
  texture_t **new_tex;

  new_tex = new texture_t * [num_textures + 1];

 if(new_tex)
  {
   for(i=0;i<num_textures;i++)
    new_tex[i] = tex[i];
   max_textures += 1;
   if(tex) delete tex;
  tex = new_tex;
  }

}

void
lpTextures::TexCoord2fv(float *st) 
{

  glTexCoord2f( st[0] * tex[last_accessed]->texSmax,
                st[1] * tex[last_accessed]->texTmax);

} 



// Bbox class

lpBBox::lpBBox(void)
{
  int i;
  for(i=0;i<3;i++)
   xyz_min[i] = xyz_max[i] = center[i] = 0.0f;

}
lpBBox::~lpBBox(void)
{
}



