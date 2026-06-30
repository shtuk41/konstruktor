#ifndef _OPTIONSREADER_H
#define _OPTIONSREADER_H

#include <string>
#include <map>

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

	struct FovsGeometry
	{
	public:
		FovsGeometry(std::string file) : fileName(file) {}
		std::string GetFileName() const { return fileName;};
	private:
		std::string		fileName;
	};

	struct Segments
	{
	public:
		Segments(std::string file) : fileName(file) {}
		std::string GetFileName() const { return fileName;}
	private:
		std::string		fileName;
	};

	class Acquired
	{
	public:
		Acquired(std::string path): path(path) { }
		std::string GetPath() const { return path; }
	private:
		std::string		path;
	};

	class OptionsReader
	{
	public:
		static OptionsReader* Instance() { if (optionsReader) return optionsReader; else return optionsReader =  new OptionsReader();}
		~OptionsReader();

		static const ApplicationWindow* GetApplicationWindowOptions() { return Instance()->applicationWindow;}
	private:
		OptionsReader();
		void Init();
		static OptionsReader *optionsReader;

		ApplicationWindow *applicationWindow;
	};

}

#endif