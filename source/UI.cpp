#include <graphics\UI.h>
#include <fstream>
#include <string>

UI::UI()
{
}


UI::~UI()
{
}

void checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}

// in ->  -> attribute
// out -> varying

GLuint UI::CreateShaderProgram()
{
	// Shadereiden lataaminen tiedostoista:
	// vertexshader.vert
	// fragmentshader.frag

	std::string vertex_shader_string;
	// Aukaistaan shader filu
	std::ifstream file("vertexshader.vert");

	// Jos tiedosto voi hyvin
	if(file.good())
	{
		// Korvataan stringi filussa olevalla kamalla
		vertex_shader_string.assign((std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>()));
	}
	else
	{
		// Heitet‰‰n erroria consoleen
		std::cout << "Failed to open file: vertexshader.vert" << std::endl;
	}

	// Jos filu auki -> filu kiinni
	if (file.is_open())
		file.close();

	
	// T‰h‰n voitas helposti lis‰t‰ kamaa esim. vertex_shader_string.c_str() + fragment_shader_string.c_str()
	// Muutetaan stringi muotoon miss‰ glShaderSource() haluaa sen
	GLchar const* vertex_shader[] = { vertex_shader_string.c_str() };

	std::string fragment_shader_string;
	// Avataan toinen shader filu
	file.open("fragmentshader.frag", std::fstream::in);

	// Jos tiedosto voi hyvin
	if (file.good())
	{
		// Stringi korvataan shader filu kamalla
		fragment_shader_string.assign((std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>()));
	}
	else
	{
		// Muuten erroria konsoleen
		std::cout << "Failed to open file: fragmentshader.frag" << std::endl;
	}

	// Jos filu auki -> filu kiinni
	if (file.is_open())
		file.close();

	// Muutetaan stringi muotoon miss‰ glShaderSource() haluaa sen
	GLchar const* fragment_shader[] = { fragment_shader_string.c_str() };
		
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vertex_shader, NULL);
	glCompileShader(vs);
	checkCompileErrors(vs, "VERTEX");
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fragment_shader, NULL);
	glCompileShader(fs);
	checkCompileErrors(fs, "FRAGMENT");

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	checkCompileErrors(fs, "PROGRAM");

	return shader_programme;
}
