#pragma once

/*
Handles the serialising and deserialising of data.  First use case will be for
storing circuit templates/diagrams in a YAML format. 
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//



//=============================================================================================================================================//
//  Forward decelrations.																													   //
//=============================================================================================================================================//

class Circuit;
class Design2DEngineGL;

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

// Store a circuit in a YAML file.
void saveToYAML(Circuit circuit, std::string folder= "");
// Store a circuit in a BIN file.
void saveToBIN(Circuit circuit, std::string folder = "");

//=============================================================================================================================================//
//  Deserialisation.																														   //
//=============================================================================================================================================//

// Load a circuit from YAML file.
void loadFromYAML(Design2DEngineGL& engine, std::string file);
// Load a circuit from BIN file.
void loadFromBIN(Design2DEngineGL& engine, std::string file);

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//