#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"
#include <unordered_map>
#include <filesystem>

//=======================================================================================================================================//
// Component Editor.																													 //
//=======================================================================================================================================//

class CableCreator : public GuiElementCore
{
public:

	// Constructor.
	CableCreator(std::string name, int windowFlags);

	// Rendering functions.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

	// Cable properties.
	glm::vec4 m_cableColor = {0.f, 0.f, 0.f, 1.f};
	std::string m_cableName = "";
	std::unordered_map<std::string, std::string> m_dataDict;

	// Set the data that is contained in the provided file.
	void setCable(const std::filesystem::path& path);

private:

	// Saves the cable to file.
	void serialiseCable(const std::filesystem::path& path);
};

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//