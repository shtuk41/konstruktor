#ifndef _OPTIONSREADER_H
#define _OPTIONSREADER_H

#include <filesystem>
#include <memory>
#include <string>

namespace fs = std::filesystem;

namespace options_reader
{
	class ApplicationWindow
	{
	public:
		ApplicationWindow():x(0), y(0), width(1920), height(1080) {}
		ApplicationWindow(int x_in, int y_in, int width_in, int height_in) : x(x_in), y(y_in), width(width_in), height(height_in) {}
		int GetX() const {return x;}
		int GetY() const { return y;}
		int GetWidth() const { return width;}
		int GetHeight() const { return height;}
	private:
		int x;
		int y;
		int width;
		int height;
	};

	class ConfigurationFile
	{
	public:
		explicit ConfigurationFile(const fs::path &filePath) : filePath(filePath) {}
		fs::path GetPath() const { return filePath; }

	private:
		fs::path filePath;
	};

	class OptionsReader
	{
	public:
		OptionsReader(const OptionsReader&) = delete;
		OptionsReader& operator=(const OptionsReader&) = delete;

		static OptionsReader& Instance() 
		{ 
			static OptionsReader instance;
			return instance;
		}
		
		static const ApplicationWindow* GetApplicationWindowOptions() 
		{ 
			return Instance().applicationWindow.get();
		}

		static const ConfigurationFile* GetConfigurationFileOptions()
		{
			return Instance().configurationFile.get();
		}

	private:
		OptionsReader() { Init(); }
		~OptionsReader() = default;

		void Init();
		std::unique_ptr<ApplicationWindow> applicationWindow;
		std::unique_ptr<ConfigurationFile> configurationFile;
	};

}

#endif