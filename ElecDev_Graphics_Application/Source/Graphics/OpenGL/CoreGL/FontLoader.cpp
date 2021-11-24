//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "FontLoader.h"
// General.
#include <iostream>
// Used for dictionary.
#include <map>
// Resources
#include "Resources/ResourceHandler.h"
// File reading.
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
// MSDFGen
#include "External/MSDFGen/msdfgen.h"
#include "External/MSDFGen/msdfgen-ext.h"


//=============================================================================================================================================//
//  Font loading function.																													   //
//=============================================================================================================================================//

Font loadFont(int fontID, int atlasID)
{
	// Create a font that will be returned at the end.
	Font font;

	// Font information
	float textureDimensions[2] = { 512, 512 };	// Dimensions of the .png file.
	float padding = 3;							// Amount of padding around each character.

	// Load font text file from executable and convert to stream.
	std::istringstream source(loadTextFromResource(fontID));

	// Read the lines.
	int lineCount = 0;
	for (std::string line; getline(source, line); )
	{
		// Read the character data.
		if (lineCount >= 4 && lineCount <= 97 - 1 + 4)
		{
			// Create character struct.
			Character c;

			//-------------------------------------
			// Read the character ID.
			//-------------------------------------
			int cursor = 8;
			std::string id;
			while (!isspace(line[cursor]))
			{
				id.push_back(line[cursor]);
				cursor++;
			}
			c.id = (unsigned int)::atof(id.c_str());

			//-------------------------------------
			// Read the character x.
			//-------------------------------------
			cursor = 18;
			std::string x;
			while (!isspace(line[cursor]))
			{
				x.push_back(line[cursor]);
				cursor++;
			}
			c.x = (float)(::atof(x.c_str())) / textureDimensions[0];

			//-------------------------------------
			// Read the character y.
			//-------------------------------------
			cursor = 25;
			std::string y;
			while (!isspace(line[cursor]))
			{
				y.push_back(line[cursor]);
				cursor++;
			}
			c.y = (float)(::atof(y.c_str())) / textureDimensions[1];

			//-------------------------------------
			// Read the character width.
			//-------------------------------------
			cursor = 36;
			std::string width;
			while (!isspace(line[cursor]))
			{
				width.push_back(line[cursor]);
				cursor++;
			}
			c.width = (float)(::atof(width.c_str())) / textureDimensions[0];

			//-------------------------------------
			// Read the character height.
			//-------------------------------------
			cursor = 48;
			std::string height;
			while (!isspace(line[cursor]))
			{
				height.push_back(line[cursor]);
				cursor++;
			}
			c.height = (float)(::atof(height.c_str())) / textureDimensions[1];

			//-------------------------------------
			// Read the character x offset.
			//-------------------------------------
			cursor = 61;
			std::string xOffset;
			while (!isspace(line[cursor]))
			{
				xOffset.push_back(line[cursor]);
				cursor++;
			}
			c.xOffset = (float)::atof(xOffset.c_str()) / textureDimensions[0];

			//-------------------------------------
			// Read the character y offset.
			//-------------------------------------
			cursor = 74;
			std::string yOffset;
			while (!isspace(line[cursor]))
			{
				yOffset.push_back(line[cursor]);
				cursor++;
			}
			c.yOffset = (float)::atof(yOffset.c_str()) / textureDimensions[1];

			//-------------------------------------
			// Read the character x advance.
			//-------------------------------------
			cursor = 88;
			std::string xAdvance;
			while (!isspace(line[cursor]))
			{
				xAdvance.push_back(line[cursor]);
				cursor++;
			}
			c.xAdvance = (float)::atof(xAdvance.c_str()) / textureDimensions[0];

			// Add cahracter data to dictionary.
			font.characterDictionary.insert({ c.id, c });
		}
		// Increment line counter.
		lineCount++;
	}
	// Return the created font.
	return font;
}

//=============================================================================================================================================//
//  MSDFGen font generation.																												   //
//=============================================================================================================================================//

using namespace msdfgen;

void msdfgenLoadFont() 
{
	FreetypeHandle* ft = initializeFreetype();
	if (ft) 
	{
		//FontHandle* font = loadFont(ft, "Source\\Resources\\Fonts\\Ruda\\Ruda-Bold.ttf");
		FontHandle* font = loadFont(ft, "C:\\Windows\\Fonts\\arialbd.ttf");
		if (font) 
		{
			Shape shape;
			if (loadGlyph(shape, font, 'A')) 
			{
				shape.normalize();
				//                      max. angle
				edgeColoringSimple(shape, 3.0);
				//           image width, height
				Bitmap<float, 3> msdf(32, 32);
				//                     range, scale, translation
				generateMSDF(msdf, shape, 4.0, 1.0, Vector2(4.0, 4.0));
				savePng(msdf, "output.png");
			}
			destroyFont(font);
		}
		deinitializeFreetype(ft);
	}
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
