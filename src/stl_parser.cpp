#include <glm/glm.hpp>
#include <stl_parser.h>


//#define OUTPUT_TO_FILE

output_data StlParser::ParseToTriangleMesh(const char *stlFileName, unsigned int &number_of_triangles, float xoffset, float yoffset, float zoffset, float size_coef = 1.0)
{
	std::ifstream stlFile;

#ifdef OUTPUT_TO_FILE
	std::ofstream dataFile;
#endif

	stlFile.open(stlFileName, std::ios::binary);
#ifdef OUTPUT_TO_FILE
	dataFile.open("stlDataFileDebug.txt", std::ios::trunc);
	dataFile.setf(std::ios_base::fixed);
#endif

	output_data data;

	data.output = 0;
	data.normal = 0;
	number_of_triangles = 0;
	unsigned int numberTriangles;

	STL_HEADER	partHolderHeader;

	if (stlFile.is_open() /*&& dataFile.is_open()*/)
	{
		stlFile.seekg(0, std::ios::beg);
		stlFile.read((char *)&partHolderHeader, sizeof(STL_HEADER));
		stlFile.read((char *)&numberTriangles, sizeof(unsigned int));

#ifdef OUTPUT_TO_FILE
		dataFile << "Number of Triangles:	" << numberTriangles << std::endl;
		dataFile << "Size of Triangle:	" << sizeof(triangle) << std::endl;
		dataFile << "Header size" << sizeof(STL_HEADER) << std::endl;

#endif

		data.output = new glm::vec4[numberTriangles * 3];
		data.normal = new glm::vec3[numberTriangles * 3];
		number_of_triangles = numberTriangles;

		char *c = strstr((char*)partHolderHeader.data, "COLOR");

		if (c)
		{
#ifdef OUTPUT_TO_FILE_COLOR
			dataFile << "COLOR FOUND" << std::endl;
#endif
		}

		c = strstr((char*)partHolderHeader.data, "MATERIAL");

		if (c)
		{
#ifdef OUTPUT_TO_FILE_COLOR
			dataFile << "MATERIAL FOUND" << std::endl;
#endif
		}

		for (unsigned int ii = 0; ii < numberTriangles; ii++)
		{
			unsigned int index = ii * 3;
			triangle next;
			size_t sizeofTriangle = sizeof(triangle);
			stlFile.read((char*)&next, 50);

#ifdef OUTPUT_TO_FILE
			//dataFile << ii + 1;
			dataFile << next.norm.data[0] << ',' << next.norm.data[1] << ',' << next.norm.data[2] << ',';
#endif

			data.normal[index] =
				data.normal[index + 1] =
				data.normal[index + 2] = glm::vec3(next.norm.data[0], next.norm.data[1], next.norm.data[2]);

#ifdef OUTPUT_TO_FILE
			dataFile << next.points[0].data[0] << ',' << next.points[0].data[1] << ',' << next.points[0].data[2] << ',';
#endif	

			data.output[index] = glm::vec4(next.points[0].data[0] * size_coef + xoffset, next.points[0].data[1] * size_coef + yoffset, next.points[0].data[2] * size_coef + zoffset, 1.0f);

#ifdef OUTPUT_TO_FILE
			dataFile << next.points[1].data[0] << ',' << next.points[1].data[1] << ',' << next.points[1].data[2] << ',';
#endif		

			data.output[index + 1] = glm::vec4(next.points[1].data[0] * size_coef + xoffset, next.points[1].data[1] * size_coef + yoffset, next.points[1].data[2] * size_coef + zoffset, 1.0f);

#ifdef OUTPUT_TO_FILE
			dataFile << next.points[2].data[0] << ',' << next.points[2].data[1] << ',' << next.points[2].data[2] << ",\n";
#endif

			data.output[index + 2] = glm::vec4(next.points[2].data[0] * size_coef + xoffset, next.points[2].data[1] * size_coef + yoffset, next.points[2].data[2] * size_coef + zoffset, 1.0f);

#ifdef OUTPUT_TO_FILE_COLOR
			int blue = (next.atribute_byte_count & 0x001f);
			int green = (next.atribute_byte_count & 0x03e0) >> 5;
			int red = (next.atribute_byte_count & 0x7c00) >> 10;
			int valid = (next.atribute_byte_count & 0x8000) >> 15;

			dataFile << "Blue:	" << blue << std::endl;
			dataFile << "Green	" << green << std::endl;
			dataFile << "Red:	" << red << std::endl;
			dataFile << "Valid:	" << valid << std::endl;

			int iRedWrite = 20 << 10;
			int iGreenWrite = 20 << 5;

			next.atribute_byte_count |= iRedWrite;
			next.atribute_byte_count |= iGreenWrite;
			next.atribute_byte_count |= 1 << 15;

			blue = (next.atribute_byte_count & 0x001f);
			green = (next.atribute_byte_count & 0x03e0) >> 5;
			red = (next.atribute_byte_count & 0x7c00) >> 10;
			valid = (next.atribute_byte_count & 0x8000) >> 15;

			dataFile << "Blue:	" << blue << std::endl;
			dataFile << "Green	" << green << std::endl;
			dataFile << "Red:	" << red << std::endl;
			dataFile << "Valid:	" << valid << std::endl;
#endif
		}
	}

	return data;
}