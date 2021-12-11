//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Header.
#include "FontLoader.h"
// Used for dictionary.
#include <map>
// Resources
#include "Utilities/Resources/ResourceHandler.h"
// JSON parsing.
#include "External/JSON/json.hpp"

//=============================================================================================================================================//
//  MSDFGen font loading.																													   //
//=============================================================================================================================================//

Font msdfLoadFont(int fontID) 
{
	// Generate a font struct to load the data to.
	Font font;

	// Load the layout from resource.
	using JSON			 = nlohmann::json;
	std::string jsonFile = loadTextFromResource(fontID);
	JSON fontInfo		 = JSON::parse(jsonFile);

	// Load atlas information.
	JSON atlas			= fontInfo["atlas"];
	font.textureSize[0] = atlas["height"];
	font.textureSize[1] = atlas["width"];
	font.yOrigin		= atlas["yOrigin"];
	font.pixelsPerEM	= atlas["size"];

	// Load metrics.
	JSON metrics			= fontInfo["metrics"];
	font.sizeInEMs			= metrics["emSize"];
	font.lineHeight			= metrics["lineHeight"];
	font.ascender			= metrics["ascender"];
	font.descender			= metrics["descender"];
	font.underLineY			= metrics["underlineY"];
	font.underLineThickness = metrics["underlineThickness"];

	// Load glyphs (characters).
	JSON glyphs = fontInfo["glyphs"];
	for (unsigned i = 0; i < glyphs.size(); i++)
	{
		// Load character data from the json file..
		Character character;
		character.id				= glyphs[i]["unicode"];
		character.xAdvance			= glyphs[i]["advance"];
		// The space does not have all of the data.
		// If the character is a space, stop loading data.
		if (character.id == 32) 
		{
			// Insert the character into the dictionary.
			font.characterDictionary.insert({ character.id, character });
			continue;
		}
		// Load character position in atlas.
		float xAtlas[2]				= { glyphs[i]["atlasBounds"]["left"]   , glyphs[i]["atlasBounds"]["right"] };
		float yAtlas[2]				= { glyphs[i]["atlasBounds"]["bottom"] , glyphs[i]["atlasBounds"]["top"]   };
		// Calculate texture coords for OpenGL.
		character.xTextureCoords[0] = xAtlas[0] / font.textureSize[0];
		character.xTextureCoords[1] = xAtlas[1] / font.textureSize[0];
		character.yTextureCoords[0] = yAtlas[0] / font.textureSize[1];
		character.yTextureCoords[1] = yAtlas[1] / font.textureSize[1];
		// Load the local offsets, in pixels.
		character.xPlaneBounds[0]	= glyphs[i]["planeBounds"]["left"];
		character.xPlaneBounds[1]	= glyphs[i]["planeBounds"]["right"];
		character.yPlaneBounds[0]	= glyphs[i]["planeBounds"]["bottom"];
		character.yPlaneBounds[1]	= glyphs[i]["planeBounds"]["top"];  
		// Calculte character dimensions, in pixels.
		character.width				= (character.xPlaneBounds[1] - character.xPlaneBounds[0]);
		character.height			= (character.yPlaneBounds[1] - character.yPlaneBounds[0]);
		// Insert the character into the dictionary.
		font.characterDictionary.insert({ character.id, character });
	}

	// Load kerning data.
	JSON kerning = fontInfo["kerning"];
	for (unsigned i = 0; i < kerning.size(); i++)
	{
		unsigned unicode1  = kerning[i]["unicode1"];
		unsigned unicode2  = kerning[i]["unicode2"];
		float kerningValue = kerning[i]["advance"];
		font.kerningDictionary.insert({std::make_pair(unicode1, unicode2), kerningValue});
	}

	// Done.
	return font;
}

Font msdfLoadFont(int fontID, int atlasID)
{
	Font font = msdfLoadFont(fontID);									// Load layout information.
	font.textureID = loadBitmapToGL(loadImageFromResource(atlasID));	// Load the atlas into memory and load the bitmap to OpenGL.
	return font;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
