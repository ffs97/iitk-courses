#ifndef LEDA_GL_TEXTURING_H
#define LEDA_GL_TEXTURING_H

#include <LEDA/string.h>
#include <LEDA/tuple.h>
#include <LEDA/stream.h>
#include <LEDA/d_array.h>

#include <LEDA/d3_gl_col.h>


#if defined(LEDA_STD_IO_HEADERS)
using std::cout;

#ifndef _MSC_VER
#define MATH_STD std::
#else
#define MATH_STD ::
#endif

#else
#define MATH_STD ::
#endif

LEDA_BEGIN_NAMESPACE

/*{\Manpage {tc_texture} {} {RGB textures}}*/ 

class tc_texture {

/*{\Mdefinition
The data type |\Mname| is used for storing true color RGB(A) textures for usage with OpenGL.
}*/

   char* data;
   int width;
   int height;
   int bytes_per_texel;

public:

/*{\Mcreation tex}*/
   
   tc_texture() : data(NULL), width(0), height(0), bytes_per_texel(0)
/*{\Mcreate  creates a texture.}*/   
   { }
   
   tc_texture(int w, int h, int b_per_texel)
/*{\Mcreate  creates a texture |\Mvar| with width |w|, height |h| and |b_per_texel| Bytes per texel. }*/    
   {
     width  = w;
     height = h;
     bytes_per_texel = b_per_texel;
     
     data = new char[w*h*bytes_per_texel];   
   }
   
   ~tc_texture() 
   {
     if (data != NULL) delete [] data;
   }

/*{\Moperations 1.5 6}*/
   
   void init()
/*{\Mop makes |tex| the empty texture.}*/   
   {
     if (data != NULL) delete [] data;
     width  = 0;
     height = 0;
     bytes_per_texel = 0;     
   }
   
   void init(int w, int h, int b_per_texel)
/*{\Mop makes |tex| a texture with width |w|, height |h| and |b_per_texel| Bytes per texel.}*/
   {
     if (data != NULL) delete [] data;
     width  = w;
     height = h;
     bytes_per_texel = b_per_texel;  
     
     data = new char[w*h*bytes_per_texel];    
   }  
   
   // init texels with some rgb value ...
   // prec : bytes_per_texel > 2
   void init_texels(char v1, char v2, char v3)
/*{\Mop }*/   
   {
     if (bytes_per_texel < 3) return;
   
     int i, j;
     
     for(i=0;i<height;i++){ 
      for(j=0;j<width;j++){     
       data[(i*width + j)*bytes_per_texel + 0] = v1;
       data[(i*width + j)*bytes_per_texel + 1] = v2;
       data[(i*width + j)*bytes_per_texel + 2] = v3; 
      }
     }    
   }
   
   int get_width() const { return width; }
/*{\Mop returns the width of |\Mvar|.}*/  
   
   int get_height() const { return height; }
/*{\Mop returns the heigth of |\Mvar|.}*/     
   
   int get_bytes_per_texel() const { return bytes_per_texel; }
/*{\Mop returns the number of bytes per texel of texture |\Mvar|.}*/  
   
   char* get_data() const { return data; }
/*{\Mop returns a pointer to the start byte of the texture data of |\Mvar|.}*/
   
   // get/set functions for RGB data ...
   // add check (?)
   
   d3_gl_col get(int x, int y)
/*{\Mop }*/   
   {
     char* pos = data + (y*width + x)*bytes_per_texel;
     unsigned char r = pos[0];
     unsigned char g = pos[1];
     unsigned char b = pos[2];
     
     return d3_gl_col(r,g,b);
   }
   
   void set(int x, int y, const d3_gl_col& c)
/*{\Mop }*/     
   {  
     char* pos = (data + (y*width + x)*bytes_per_texel);
     char r = (char) c.r();
     char g = (char) c.g();
     char b = (char) c.b(); 

     pos[0] = r; pos[1] = g; pos[2] = b;  
   }
};


/*{\Mtext
\bigskip
Non-member functions:
}*/


// get rgb info from a pixrect color string ...
// (that is the substring after the LAST #)
string get_rgb_string(const string& data)
{
  int position = 0, act;
  
  string f("#");
  
  do {
    act = data.pos(f, position);
    
    if (act != -1) position=act+1;
  }
  while (act != -1);
  
  return data(position, data.length() -1);
}

// read BMP (24 bit uncompressed) file_name ...
// store it in texture T
// read little-endian data from file

unsigned short read_short(std::ifstream& F)
{ // read 2 byte ... 
  unsigned short s;
  char h;
  F.get(h); s = h; // low byte
  F.get(h); s |= ((unsigned short)h << 8); // high byte
  return s;
}

unsigned long read_long(std::ifstream& F)
{ // read 4 byte ...
  unsigned long l;
  char h;
  F.get(h); l = h; // low byte
  F.get(h); l |= ((unsigned short)h << 8); 
  F.get(h); l |= ((unsigned short)h << 16);
  F.get(h); l |= ((unsigned short)h << 24);  
  return l;  
}

bool handle_bmp(string file_name, tc_texture& T)
/*{\Mfuncl
|handle_bmp| reads an uncompressed 24-bit BMP file |file_name| into
texture object |T|. If the function is successful, |true| is returned, |false| otherwise.
}*/
{
  std::ifstream F;
  F.open(file_name, std::ios::in | std::ios::binary);

  // read header ...
  if (! F) return false;
  
  char h1,h2;
  F.get(h1);
  F.get(h2);
  
  // BM ...
#if defined(DEBUG)     
  std::cout << h1 << h2 << "\n";
#endif  
  if (h1 != 'B' || h2 != 'M') return false;
  
  //unsigned long file_size = 
  read_long(F);
  unsigned short h;
  h = read_short(F); h = read_short(F); // reserved
  unsigned long l = read_long(F);
  l = read_long(F);
  
  // Zeilen/Spalten
  unsigned long cols = read_long(F);
  unsigned long rows= read_long(F);
  h = read_short(F);
  
  h = read_short(F);
  
  if (h == 8) return false; // 8 bit image
#if defined(DEBUG)     
  else std::cout << "24 bit image ...\n";
#endif  
  
  l = read_long(F);
  
  if (l != 0) return false; // 0 means no compression ...
  
  l = read_long(F); // image size ...
  l = read_long(F); l= read_long(F); // 0 ...
  
  unsigned long table_colors = read_long(F);

#if defined(DEBUG)     
  std::cout << table_colors << " colors in table ...\n";
#endif  
  if (table_colors != 0) return false;
  
  l = read_long(F);
  
  // compute pad bytes ...
  int skip;
  int pad_bytes = (((3*cols + 3)/4)*4) - 3*cols; 
  
  // init texture ...
  T.init(cols,rows,3);
  char* tex_data = T.get_data();
  int offset = 0;
  
  // read data into texture ...
  unsigned int i,j;
  char act;
 
#if defined(DEBUG)     
  std::cout << "columns/rows:" << cols << " " << rows << "\n";
#endif
  
  for(i=0;i<rows;i++){
   for(j=0;j<cols;j++){
   
     // read color triple (bgr) from file;
     char r,g,b;
     F.get(b); F.get(g); F.get(r);
     // write to texture ...
     tex_data[offset] = r;
     tex_data[offset+1] = g;
     tex_data[offset+2] = b;
     offset = offset + 3;
   }
   // skip some pad bytes at line's end ...
   for(skip=0;skip<pad_bytes;skip++) F.get(act);
  }

  F.close();

  return true;
}

// read an xpm pixrect from data
// store it into texture T
// set the texture width and height to powers of two if tex_size_to_power_of_two is true ...

void handle_pixrect(const char ** data, 
                    tc_texture& T, 
		    bool tex_size_to_power_of_two = true)
/*{\Mfuncl 
|handle_pixrect| reads a LEDA pixrectangle |data| into texture object |T|.
}*/
{
  string info = data[0];
  string col,line;
    
  int width;    // width of the image
  int height;   // height of the image
  int num_col;  // number of color indices
  int chars_per_pixel;  // number of chars for every pixel 
  
  string_istream IS(info);
  
  IS >> width; IS >> height; IS >> num_col; IS >> chars_per_pixel;

#if defined(DEBUG)  
  cout << "width :" << width   << "\n";
  cout << "height:" << height  << "\n";
  cout << "#colors:" << num_col << "\n";
  cout << "chars_per_pixel:" << chars_per_pixel << "\n";
#endif
  
  int i,j;
  
  // color map ...
  // assoc. data with its rgb color info ...
  d_array<string, three_tuple<int, int, int> >  chars_to_color;
  
  // read in color table ...
  for(i=0;i<num_col;i++){
    col = data[i+1];
    
    // we map this character to a color information ...
    string cact = col(0,chars_per_pixel-1);   
    
    string rgb_str = get_rgb_string(col);
    
    string hex("0x");
    string rs = hex + rgb_str(0,1);
    string gs = hex + rgb_str(2,3);
    string bs = hex + rgb_str(4,5);
    
    //cout << cact << " " << strtol(rs,NULL,16) << " " << strtol(gs,NULL,16) << " " << strtol(bs,NULL,16) << "\n";
    
    chars_to_color[cact] = three_tuple<int, int, int>((int)strtol(rs,NULL,16),(int) strtol(gs,NULL,16),(int) strtol(bs,NULL,16));
  }
  
  // init texture object ...
  
  int tex_width  = width;
  int tex_height = height;
  
  // upscaling of width and height to powers of two ...
  if (tex_size_to_power_of_two){
     int exp_w, exp_h;
     
     double f_w =  frexp((double) tex_width, &exp_w);
     double f_h =  frexp((double) tex_height, &exp_h);
  
     // in cases of 0.5 we have already powers of two ...
     if (f_w != 0.5) tex_width  = (int) MATH_STD pow(2.0, exp_w);     
     if (f_h != 0.5) tex_height = (int) MATH_STD pow(2.0, exp_h);
     
     //cout << tex_width << " " << tex_height << "\n";
  }
  
  T.init(tex_width, tex_height, 3);
  T.init_texels(0,55,0);
   
  // get memory ...
  char* tex_data = T.get_data();
  
  // read in height lines ...
  for(i=0;i<height;i++){
    line = data[i+num_col+1];
    
    for(j=0;j<width;j++){
       string cact = line(j*chars_per_pixel,j*chars_per_pixel+chars_per_pixel-1); 
       
       // get assoc. color information ...
       three_tuple<int, int, int> rgb = chars_to_color[cact];

#if defined(DEBUG)         
       cout << cact << ":" << rgb.first() << " " <<  rgb.second() << " " <<  rgb.third() << " ";
#endif
       
       char rv = rgb.first();
       char gv = rgb.second();
       char bv = rgb.third();
       
       // set r/g/b values in the texture ...
       tex_data[(i* tex_width + j)*3 + 0] = rv;
       tex_data[(i* tex_width + j)*3 + 1] = gv;
       tex_data[(i* tex_width + j)*3 + 2] = bv;              
    }
#if defined(DEBUG)      
    cout << "\n\n\n";
#endif    
  }
}

LEDA_END_NAMESPACE


#endif
