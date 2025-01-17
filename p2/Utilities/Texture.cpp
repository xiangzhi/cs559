/************************************************************************
     File:        Texture.cpp
				  see the header file for more info

				  $Header: /p/course/cs559-gleicher/private/CVS/Utilities/Texture.cpp,v 1.1 2009/11/10 22:31:59 gleicher Exp $

     Author:     
                  Michael L. Gleicher, gleicher@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     Simple texture manager
	              (c) 2000 Michael L. Gleicher
                  This code may be modified and re-used providing that proper credit
                  is given to the author. 

				  Management of textures
                  probably more complicated than it needs to be, but it makes some
                  other things simple

                  The trick is to only have to load each texture once, no matter how
                  many things use it

 *************************************************************************/
#include "Texture.H"
#include "libtarga.h"

#pragma warning (push)              // Disable the warning between the block of push and pop
// The list of numbers of warning we will like to disable 
#pragma warning(disable : 4204)
#pragma warning(disable : 4244)
#pragma warning(disable : 4312)
#pragma warning(disable : 4311)
#pragma warning(disable : 4083)
#pragma warning(disable : 4193)
// List of all fltk include file for example #include <Fl/Fl_Window.h>
#include <Fl/Fl.h>
#include <Fl/Fl_Widget.h>
#include <FL/Fl_Image.h>
#include <FL/Fl_JPEG_Image.h>
#include <FL/Fl_PNG_Image.h>
#include <FL/Fl_BMP_Image.h>
#include <FL/Fl_GIF_Image.h>
#include <Fl/filename.h>

#pragma warning (pop) // Enable the warning again

using std::vector;
using std::string;

// the list of all textures loaded in already
vector<Texture*> theTextures;

//**********************************************************************************
// The places to look for textures...
//    this should be modified to look where you might find your textures.
//    ultimately, there should be a better way than just sticking this in
//    the code.
//**********************************************************************************
std::vector<const char*> texturePaths;

// the kinds of images we can load
#define MAX_IMAGEFORMAT_NUM 7
char* S_ImgFormatArray[MAX_IMAGEFORMAT_NUM]= {
  ".TGA", 
  ".BMP", 
  ".GIF", 
  ".PNG", 
  ".JPEG", 
  ".JPG",
  NULL
};

//*************************************************************************************
//
// Utility routine
//
//*************************************************************************************

//*************************************************************************************
//
// * Copy and create a new string from the s
//=====================================================================================
static char* newString(char* s)
//=====================================================================================
{
  char* n = new char [strlen(s)+1];
  strcpy(n,s);
  return n;
}

//*************************************************************************************
//
// * Return the file format of the file
//=====================================================================================
int ImageType(char *imgName)
//=====================================================================================
{
	if(!imgName) 
    return -1;

	// Get the input directory and the file extension
  char *imgNameWODirName = (char *) fl_filename_name(imgName); 
  char* ext=(char*) fl_filename_ext(imgName);
	char* capital_ext = new char[strlen(ext)+1];
	for(int i = 0; i < (int)strlen(ext); i++){
		capital_ext[i] = toupper(ext[i]);
	}
	capital_ext[strlen(ext)] = 0;

  //*****************************************************************
  // Scan the image format array to see which image format it is
  //*****************************************************************
  int imgFormatIdx = -1;
  for(int i = 0;i < MAX_IMAGEFORMAT_NUM && S_ImgFormatArray[i] != NULL;i++){
    if(strcmp(capital_ext, S_ImgFormatArray[i]) == 0)	{
      imgFormatIdx = i;
      break;
    } // end of if
  } // end of for
  if(imgFormatIdx == -1){ // Not a valid image format
    printf("Not supported image format...\n");	
    return -1;		
  }

	return imgFormatIdx;
}
	
//*****************************************************************************
//
// * Copy this into a new image, reversing the rows as it goes. A pointer
//   to the new image is returned.
//=============================================================================
unsigned char* Reverse_Rows(unsigned char * data, int width, int height)
//=============================================================================
{
  unsigned char   *dest = new unsigned char[width * height * 4];
  if (! data)
    return NULL;

  for (int i = 0 ; i < height ; i++) {
		int in_offset = (height - i - 1) * width * 4;
		int out_offset = i * width * 4;

		for (int j = 0 ; j < width ; j++) {
			dest[out_offset + j * 4] = data[in_offset + j * 4];
			dest[out_offset + j * 4 + 1] = data[in_offset + j * 4 + 1];
			dest[out_offset + j * 4 + 2] = data[in_offset + j * 4 + 2];
			dest[out_offset + j * 4 + 3] = data[in_offset + j * 4 + 3];
		}
  }
	return dest;
}// Reverse_Rows


//*****************************************************************************
//
// * Read a JPG image from a file.  Return a character array
//   Return 0 on failure.
//=============================================================================
void* read_JPG(char *filename, int *width, int *height)
//=============================================================================
{
	if (!filename) {
    printf("No filename given.\n");
    return 0;
  }// if

 	Fl_JPEG_Image *jpg_image = new Fl_JPEG_Image(filename);
	*width  = jpg_image->w();
	*height = jpg_image->h();
	int depth  = jpg_image->d();

	if(*width <= 0 || *height <= 0){
		// printf("Fail to load in image\n");
		return NULL;
	}
	if(depth != 3) {
		printf("The image is not in RGB\n");
		return NULL;
	}

	int total = *width * *height * (depth + 1);
	unsigned char *temp_data = new unsigned char[total];

	for(int i = 0; i < *width; i++){
		int raw_index = i * *height;
		for(int j = 0; j < *height; j++){
			int org_raw_height = (raw_index + j) * 3;
			int dst_raw_height = (raw_index + j) * 4;
			for(int k = 0; k < depth; k++){
				int org_index = org_raw_height + k;
				int dst_index = dst_raw_height + k;
				temp_data[dst_index] =  jpg_image->data()[0][org_index];
			}
			temp_data[i * *height * 4 + j * 4 + 3] = 255;
		}
	}
	unsigned char *result = Reverse_Rows(temp_data, *width, *height);
	delete [] temp_data;
	return result;
}// read_JPG

//*****************************************************************************
//
// * Read a BMP image from a file.  Return a character array
//   Return 0 on failure.
//=============================================================================
void* read_BMP(char *filename, int *width, int *height)
//=============================================================================
{
  if (!filename) {
		printf("No filename given.\n");
    return 0;
  }// if

 	Fl_BMP_Image *bmp_image = new Fl_BMP_Image(filename);
	*width  = bmp_image->w();
	*height = bmp_image->h();
	int depth  = bmp_image->d();

	if(*width <= 0 || *height <= 0){
		// printf("Fail to load in image\n");
		return NULL;
	}
	if(depth != 3) {
		printf("The image is not in RGB\n");
		return NULL;
	}

	int total = *width * *height * (depth + 1);
	unsigned char *temp_data = new unsigned char[total];

	for(int i = 0; i < *width; i++){
		int raw_index = i * *height;
		for(int j = 0; j < *height; j++){
			int org_raw_height = (raw_index + j) * 3;
			int dst_raw_height = (raw_index + j) * 4;
			for(int k = 0; k < depth; k++){
				int org_index = org_raw_height + k;
				int dst_index = dst_raw_height + k;
				temp_data[dst_index] =  bmp_image->data()[0][org_index];
			}
			temp_data[i * *height * 4 + j * 4 + 3] = 255;
		}
	}
	unsigned char *result = Reverse_Rows(temp_data, *width, *height);
	delete [] temp_data;
	return result;
}// read_BMP

//*****************************************************************************
//
// * Read a BMP image from a file.  Return a character array
//   Return 0 on failure.
//=============================================================================
void* read_GIF(char *filename, int *width, int *height)
//=============================================================================
{
  if (!filename) {
    printf("No filename given.\n");
    return 0;
  }// if

 	Fl_GIF_Image *gif_image = new Fl_GIF_Image(filename);
	*width  = gif_image->w();
	*height = gif_image->h();
	int depth  = gif_image->d();

	if(*width <= 0 || *height <= 0){
		// printf("Fail to load in image\n");
		return NULL;
	}
	if(depth != 3) {
		printf("The image is not in RGB\n");
		return NULL;
	}

	int total = *width * *height * (depth + 1);
	unsigned char *temp_data = new unsigned char[total];

	for(int i = 0; i < *width; i++){
		int raw_index = i * *height;
		for(int j = 0; j < *height; j++){
			int org_raw_height = (raw_index + j) * 3;
			int dst_raw_height = (raw_index + j) * 4;
			for(int k = 0; k < depth; k++){
				int org_index = org_raw_height + k;
				int dst_index = dst_raw_height + k;
				temp_data[dst_index] =  gif_image->data()[0][org_index];
			}
			temp_data[i * *height * 4 + j * 4 + 3] = 255;
		}
	}
	unsigned char *result = Reverse_Rows(temp_data, *width, *height);
	delete [] temp_data;
	return result;
}// read_GIP

//*****************************************************************************
//
// * Read a PNG image from a file.  Return a character array
//   Return 0 on failure.
//=============================================================================
void* read_PNG(char *filename, int *width, int *height)
//=============================================================================
{
  if (!filename) {
    printf("No filename given.\n");
    return 0;
  }// if

 	Fl_PNG_Image *png_image = new Fl_PNG_Image(filename);
	*width  = png_image->w();
	*height = png_image->h();
	int depth  = png_image->d();

	if(*width <= 0 || *height <= 0){
		// printf("Fail to load in image\n");
		return NULL;
	}
	if(depth != 3) {
		printf("The image is not in RGB\n");
		return NULL;
	}

	int total = *width * *height * (depth + 1);
	unsigned char *temp_data = new unsigned char[total];

	for(int i = 0; i < *width; i++){
		int raw_index = i * *height;
		for(int j = 0; j < *height; j++){
			int org_raw_height = (raw_index + j) * 3;
			int dst_raw_height = (raw_index + j) * 4;
			for(int k = 0; k < depth; k++){
				int org_index = org_raw_height + k;
				int dst_index = dst_raw_height + k;
				temp_data[dst_index] =  png_image->data()[0][org_index];
			}
			temp_data[i * *height * 4 + j * 4 + 3] = 255;
		}
	}
	unsigned char *result = Reverse_Rows(temp_data, *width, *height);
	delete [] temp_data;
	return result;
}// read_BMP

//method that check whether the file exist of not
//based on the answer given at
//http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
inline bool fileExistsTest2(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

//loop through the list of paths and find the  texture,
//return empty if cannot find.
std::string findCorrectPath2(std::string& name){

	std::string path = "";

	for (int i = 0; i < texturePaths.size(); i++){
		std::string temp = std::string(texturePaths[i]) + "/" + name;
		if (fileExistsTest2(temp)){
			path = temp;
			break;
		}
	}
	return path;
}



#include "GrTown\lodePNG.h"
#include  <iostream>
Texture* loadPNG(string _filename)
{
	std::string filename = findCorrectPath2(_filename);
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	//looad it into openGL
	glEnable(GL_TEXTURE_2D);
	
	Texture* t = new Texture();
	GLuint textureID;
	glGenTextures(1, &textureID);
	t->texName = textureID;
	t->width = width;
	t->height = height;
	t->textureUnit = 0;

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	//smooth the images;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);

	return t;
}

//*************************************************************************************
//
// * The core guts of the thing...
//=====================================================================================
void* read_image(char* filename, int type, int *width, int *height)
//=====================================================================================
{
	switch(type){
		case 0: // TGA
			return tga_load(filename, width, height, TGA_TRUECOLOR_32);
		case 1: // BMP
			return read_BMP(filename, width, height);
		case 2: // GIF
			return read_GIF(filename, width, height);
		case 3: // PNG
			return read_PNG(filename, width, height);
		case 4:
		case 5:
				return read_JPG(filename, width, height);
		default:
			printf("Unknown image file format\n");
			return 0;
	}
	return 0;
}

//*************************************************************************************
//
// * The core guts of the thing...
//=====================================================================================
Texture* fetchTexture(char* name, bool wrapS, bool wrapT, GLenum textureUnit)
//=====================================================================================
{
  char buf[200]; // String buffer for all possible filenames

  int   w, h;    // Width and height of the loading in image
  void* b=0;     // The buffer data for the image

  //************************************************************
  // Check to see if we've loaded the texture already
  //************************************************************
  for(vector<Texture*>::iterator t=theTextures.begin(); t != theTextures.end(); ++t) {
	  if (!(*t)->name.compare(name)) {
      glActiveTexture((*t)->textureUnit);
			(*t)->bind();
      glActiveTexture(GL_TEXTURE0);
			return *t;
		}	  
  }

	//************************************************************
	// Find the type of the textures
	//************************************************************
	int type = ImageType(name);
	if(type < 0 || type > 6){
		printf("!! Unrecognized type of image `%s'~\n",name);
		return 0;
	}

  //************************************************************
  // We need to load the file
  // Search everywhere on the path until we find it
  //************************************************************
	for(std::vector<const char *>::iterator i=texturePaths.begin(); (i != texturePaths.end()) && !b; i++) {
		_snprintf(buf, 200,"%s/%s", *i, name);
		// printf("Trying: `%s'\n", buf);
		b = read_image(buf, type, &w, &h);
	}

  // If we found an image, load it into Texture data structure and put it into the
  // link list
  if (b) {
    // Create the structure
		Texture* t  = new Texture;

		// Set up the field in the data structure
		t->name     = name;
		t->fname    = buf;
		t->bits     = b;
		t->width    = w;
		t->height   = h;
        t->textureUnit = textureUnit;
		theTextures.push_back(t);
	
    //make sure its in the correct texture unit
    glActiveTexture(t->textureUnit);

		// Generate a unique ID inside opengl
		glGenTextures(1,&t->texName);
		
		// Print out the creating message
		printf("Created texture %u for %s\n",t->texName,t->name.c_str());

		// Why bind twice, this need to be corrected
    
		glBindTexture(GL_TEXTURE_2D, t->texName);
   
	
		// Set up the texture clamping information in s direction
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, 
										wrapS ? GL_REPEAT : GL_CLAMP);
		// Set up the texture clamping information in t direction
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, 
										wrapT ? GL_REPEAT : GL_CLAMP);
		// Set up the alignment inside the texture
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		// Generate the mip maps
		int e = gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE,b);

    //go back to the default map
    glActiveTexture(GL_TEXTURE0);

		printf("Build MipMap(%d %d) returns %d = %s (%d)\n", w, h, e, gluErrorString(e), e);
		return t;
  } else {
		printf("!! Couldn't load texture `%s'~\n",name);
		return 0;
  }
}




void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D,texName);
}


#include <stdio.h>
#include <direct.h>

GLuint loadBMP(const char * _imagePath){
  // Data read from the header of the BMP file
  unsigned char header[54]; // Each BMP file begins by a 54-bytes header
  unsigned int dataPos;     // Position in the file where the actual data begins
  unsigned int width, height;
  unsigned int imageSize;   // = width*height*3
  // Actual RGB data
  unsigned char * data;



  char cCurrentPath[FILENAME_MAX];

  if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
  {
    return errno;
  }

  cCurrentPath[sizeof(cCurrentPath)-1] = '\0'; /* not really required */

  //add the path to it...
  std::string path = ((std::string)cCurrentPath + "\\Textures\\" + (std::string)_imagePath);
  const char* imagePath = path.c_str();


  // Open the file
  FILE * file;
  fopen_s(&file, imagePath, "rb");
  if (!file){
    printf("Image could not be opened\n");
    return 0;
  }

  if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
    printf("Not a correct BMP file\n");
    return false;
  }

  if (header[0] != 'B' || header[1] != 'M'){
    printf("Not a correct BMP file\n");
    return 0;
  }

  // Read ints from the byte array
  dataPos = *(int*)&(header[0x0A]);
  imageSize = *(int*)&(header[0x22]);
  width = *(int*)&(header[0x12]);
  height = *(int*)&(header[0x16]);

  // Some BMP files are misformatted, guess missing information
  if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
  if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

  // Create a buffer
  data = new unsigned char[imageSize];

  // Read the actual data from the file into the buffer
  fread(data, 1, imageSize, file);

  //Everything is in memory now, the file can be closed
  fclose(file);

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);

  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  // Generate mipmaps, by the way.
  glGenerateMipmap(GL_TEXTURE_2D);


  return textureID;
}



// $Header: /p/course/cs559-gleicher/private/CVS/Utilities/Texture.cpp,v 1.1 2009/11/10 22:31:59 gleicher Exp $
