#include "OutMod.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "OutFlags.h"
#include "OutHoi4World.h"
#include <string>



void createOutputFolder(const std::string& outputName);
void createModFiles(const std::string& outputName);



void clearOutputFolder(const std::string& outputName)
{
	const auto outputFolder = Utils::getCurrentDirectory() + "/output/" + outputName;
	if (Utils::doesFolderExist(outputFolder))
	{
		if (!Utils::deleteFolder(outputFolder))
		{
			throw std::runtime_error("Could not remove pre-existing output folder " + outputFolder +
											 ". Please delete folder and try converting again.");
		}
	}
}


void output(const HoI4::World& destWorld,
	 const std::string& outputName,
	 const bool debugEnabled,
	 const std::vector<std::string>& vic2Mods,
	 const std::string& vic2Path,
	 const Configuration& theConfiguration)
{
	LOG(LogLevel::Info) << "Outputting mod";

	createOutputFolder(outputName);
	createModFiles(outputName);
	copyFlags(destWorld.getCountries(), outputName, vic2Mods, vic2Path);
	OutputWorld(destWorld, outputName, debugEnabled, theConfiguration);
}


void createOutputFolder(const std::string& outputName)
{
	LOG(LogLevel::Info) << "\tCopying blank mod";
	if (!Utils::copyFolder("blankMod/output", "output/" + outputName))
	{
		throw std::runtime_error("Could not copy blankMod");
	}
}


void createModFiles(const std::string& outputName)
{
	LOG(LogLevel::Info) << "\tCreating .mod files";

	std::ofstream modFile("output/" + outputName + ".mod");
	if (!modFile.is_open())
	{
		throw std::runtime_error("Could not create .mod file");
	}
	modFile << "name = \"Converted - " << outputName << "\"\n";
	modFile << "path = \"mod/" << outputName << "/\"\n";
	modFile << "user_dir = \"" << outputName << "_user_dir\"\n";
	modFile << "replace_path=\"common/ideologies\"\n";
	modFile << "replace_path=\"history/countries\"\n";
	modFile << "replace_path=\"history/states\"\n";
	modFile << "supported_version=\"1.9.*\"";
	modFile.close();

	std::ofstream descriptorFile("output/" + outputName + "/descriptor.mod");
	if (!descriptorFile.is_open())
	{
		throw std::runtime_error("Could not create descriptor.mod");
	}
	descriptorFile << "name = \"Converted - " << outputName << "\"\n";
	descriptorFile << "user_dir = \"" << outputName << "_user_dir\"\n";
	descriptorFile << "replace_path=\"common/ideologies\"\n";
	descriptorFile << "replace_path=\"history/countries\"\n";
	descriptorFile << "replace_path=\"history/states\"\n";
	descriptorFile << "supported_version=\"1.9.*\"";
	descriptorFile.close();
}