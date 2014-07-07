//Whether this code have err check code or not.
#ifndef _TOFU_OPENGL_GLSL_ENABLE_CHECK_ERROR
#	define _TOFU_OPENGL_GLSL_ENABLE_CHECK_ERROR 1
#endif

//Whether this code have file open code or not.
#ifndef _TOFU_OPENGL_GLSL_ENABLE_SOURCE_FILE
#	define _TOFU_OPENGL_GLSL_ENABLE_SOURCE_FILE 1
#endif

#if _TOFU_OPENGL_GLSL_ENABLE_CHECK_ERROR || _TOFU_OPENGL_GLSL_ENABLE_SOURCE_FILE
#	include <iostream>
#	include <vector>
#	include <string>
#	include "glsl_preprocessor.hpp"
#endif

namespace tofu
{
namespace glsl
{

inline bool check_link_error(GLuint prog)
{
#if	_TOFU_OPENGL_GLSL_ENABLE_CHECK_ERROR
	if(prog == 0)
	{
		std::cerr << "Failed to create program object." << std::endl;
		return false;
	}

	GLint log_len;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &log_len);

	if(log_len != 0)
	{
		std::vector<char> buf(log_len);
		char* pbuf = &buf[0];

		glGetProgramInfoLog(prog, log_len, NULL, pbuf);

		if(pbuf[0] != 0)
		{
			std::cerr << pbuf << std::endl;

			//ログに何か書かれていたら入力待ちになるので
			//デバッガで一時停止してcall stackを確認する等する.
			std::cout << "Push enter key" << std::endl;
			::getchar();
		}
	}

	GLint success;
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	return !(success == GL_FALSE);
#else
	return true;
#endif
}

inline bool check_compile_error(GLuint shader)
{
#if	_TOFU_OPENGL_GLSL_ENABLE_CHECK_ERROR
	if(shader == 0)
	{
		std::cerr << "Failed to create shader object." << std::endl;
		return false;
	}

	GLint log_len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

	if(log_len != 0)
	{
		std::vector<char> buf(log_len);
		char* pbuf = &buf[0];

		glGetShaderInfoLog(shader, log_len, NULL, pbuf);

		if(pbuf[0] != 0)
		{
			std::cerr << pbuf<< std::endl;

			//ログに何か書かれていたら入力待ちになるので
			//デバッガで一時停止してcall stackを確認する等する.
			std::cout << "Push enter key" << std::endl;
			::getchar();
		}
	}

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	return !(success == GL_FALSE);
#else
	assert(shader!=0);
	return true;
#endif
}

#if	_TOFU_OPENGL_GLSL_ENABLE_CHECK_ERROR
inline GLuint load_shader_wo_cmplchk(const std::string& source, GLenum shader_type)
{
	GLuint shader = glCreateShader(shader_type);
	if(shader==0)
	{
		std::cerr << "Failed to glCreateShader()" << std::endl;
		return 0;
	}

	const std::string ppsrc = glsl_preprocessor(source);
	const GLchar* psrc = ppsrc.c_str();
	glShaderSource(shader, 1, (const GLchar**)&psrc, NULL);
	glCompileShader(shader);

	return shader;
}

inline GLuint load_shader_wo_cmplchk
(
	GLenum shader_type,
	const std::string& source0,
	const std::string& source1=""
)
{
	GLuint shader = glCreateShader(shader_type);
	if(shader==0)
	{
		std::cerr << "Failed to glCreateShader()" << std::endl;
		return 0;
	}

	const std::string ppsrc0 = glsl_preprocessor(source0);
	const std::string ppsrc1 = glsl_preprocessor(source1);
	const GLchar* psrcs[] = {ppsrc0.c_str(), ppsrc1.c_str()};
	glShaderSource(shader, 2, psrcs, NULL);
	glCompileShader(shader);

	return shader;
}
#else
__forceinline GLuint load_shader_wo_cmplchk(const char* source, GLenum shader_type)
{
	GLuint shader = glCreateShader(shader_type);
	assert(shader!=0);

	glShaderSource(shader, 1, (const GLchar**)&source, NULL);
	glCompileShader(shader);

	return shader;
}
__forceinline GLuint load_shader_wo_cmplchk
(
	GLenum shader_type, const char* source0, const char* source1
)
{
	GLuint shader = glCreateShader(shader_type);
	assert(shader!=0);

	const GLchar* psrcs[] = {source0, source1};
	glShaderSource(shader, 2, psrcs, NULL);
	glCompileShader(shader);

	return shader;
}
#endif

__forceinline GLuint load_shader(const char* source, GLenum shader_type)
{
	GLuint shader = load_shader_wo_cmplchk(source, shader_type);

	return check_compile_error(shader) ? shader : 0;
}

__forceinline GLuint load_program_wo_link(const char* vs_src, const char* fs_src)
{
	GLuint program = glCreateProgram();
	assert(program!=0);

	const GLuint vs_shader = load_shader(vs_src, GL_VERTEX_SHADER);
	assert(vs_shader!=0);
	glAttachShader(program, vs_shader);

	const GLuint fs_shader = load_shader(fs_src, GL_FRAGMENT_SHADER);
	assert(fs_shader!=0);
	glAttachShader(program, fs_shader);

	return program;
}

__forceinline GLuint load_program_wo_link(const char* src, GLenum shader_type)
{
	GLuint program = glCreateProgram();
	assert(program!=0);

	const GLuint vs_shader = load_shader(src, shader_type);
	assert(vs_shader!=0);
	glAttachShader(program, vs_shader);

	return program;
}

__forceinline GLuint load_shader
(
	GLenum shader_type, const char* source0, const char* source1
)
{
	GLuint shader = load_shader_wo_cmplchk(shader_type, source0, source1);

	return check_compile_error(shader) ? shader : 0;
}

__forceinline GLuint load_program_wo_link_with_common
(
	const char* common_src,
	const char* vs_src, const char* fs_src
)
{
	GLuint program = glCreateProgram();
	assert(program!=0);

	const GLuint vs_shader = load_shader(GL_VERTEX_SHADER, common_src, vs_src);
	assert(vs_shader!=0);
	glAttachShader(program, vs_shader);

	const GLuint fs_shader = load_shader(GL_FRAGMENT_SHADER, common_src, fs_src);
	assert(fs_shader!=0);
	glAttachShader(program, fs_shader);

	return program;
}

__forceinline GLuint load_program_wo_link_with_common
(
	GLenum shader_type,
	const char* common_src,
	const char* src
)
{
	GLuint program = glCreateProgram();
	assert(program!=0);

	const GLuint vs_shader = load_shader(shader_type, common_src, src);
	assert(vs_shader!=0);
	glAttachShader(program, vs_shader);

	return program;
}

#if _TOFU_OPENGL_GLSL_ENABLE_SOURCE_FILE
inline GLuint load_shader_from_file_wo_cmplchk(
	const std::string& filename, GLenum shader_type)
{
	std::cerr << "Loading " << filename << std::endl;

	std::ifstream ifs(filename.c_str());

	if(!ifs.good())
	{
		std::cerr << "Failed to open file:" << filename << std::endl;
		return 0;
	}

	const std::string source(
		std::istreambuf_iterator<char>(ifs.rdbuf()),
		std::istreambuf_iterator<char>());

	return load_shader_wo_cmplchk(source, shader_type);
}

inline GLuint load_shader_from_file(
	const std::string& filename, GLenum shader_type)
{
	GLuint shader = load_shader_from_file_wo_cmplchk(filename, shader_type);

	if(!check_compile_error(shader))
	{
		return 0;
	}

	return shader;
}

inline GLuint load_program_from_file_wo_link(
	const std::string& vs_src_file, const std::string& fs_src_file)
{
	GLuint program = glCreateProgram();
	assert(program!=0);
	if(program == 0)
	{
		return 0;
	}

	{
		const GLuint shader = load_shader_from_file(vs_src_file, GL_VERTEX_SHADER);
		assert(shader!=0);
		if(shader == 0)
		{
			return 0;
		}
		glAttachShader(program, shader);
	}

	{
		const GLuint shader = load_shader_from_file(fs_src_file, GL_FRAGMENT_SHADER);
		assert(shader!=0);
		if(shader == 0)
		{
			return 0;
		}
		glAttachShader(program, shader);
	}

	return program;
}

inline GLuint load_program_from_file_wo_link(
	const std::string& src_file, GLenum shader_type)
{
	GLuint program = glCreateProgram();
	assert(program!=0);
	if(program == 0)
	{
		return 0;
	}

	{
		const GLuint shader = load_shader_from_file(src_file, shader_type);
		assert(shader!=0);
		if(shader == 0)
		{
			return 0;
		}
		glAttachShader(program, shader);
	}

	return program;
}

inline GLuint load_shader_from_file_wo_cmplchk
(
	GLenum shader_type,
	const std::string& filename0, const std::string& filename1=""
)
{
	std::cerr << "Loading " << filename0 << " & " << filename1 << std::endl;

	const std::string filenames[] = {filename0, filename1};
	std::string sources[2];

	for(int i=0; i<2; ++i)
	{
		const std::string& filename = filenames[i];
		std::ifstream ifs(filename.c_str());

		if(!ifs.good())
		{
			std::cerr << "Failed to open file:" << filename << std::endl;
			return 0;
		}

		sources[i] =
		std::string
		(
			std::istreambuf_iterator<char>(ifs.rdbuf()),
			std::istreambuf_iterator<char>()
		);
	}

	return load_shader_wo_cmplchk(shader_type, sources[0], sources[1]);
}

inline GLuint load_shader_from_file(
	GLenum shader_type, const std::string& filename0, const std::string& filename1)
{
	GLuint shader = load_shader_from_file_wo_cmplchk(shader_type, filename0, filename1);

	if(!check_compile_error(shader))
	{
		return 0;
	}

	return shader;
}

inline GLuint load_program_from_file_wo_link_with_common
(
	const std::string& common_src_file,
	const std::string& vs_src_file, const std::string& fs_src_file
)
{
	GLuint program = glCreateProgram();
	assert(program!=0);
	if(program == 0)
	{
		return 0;
	}

	{
		const GLuint shader
		=
		load_shader_from_file(GL_VERTEX_SHADER, common_src_file, vs_src_file);
		assert(shader!=0);
		if(shader == 0)
		{
			return 0;
		}
		glAttachShader(program, shader);
	}

	{
		const GLuint shader
		=
		load_shader_from_file(GL_FRAGMENT_SHADER, common_src_file, fs_src_file);
		assert(shader!=0);
		if(shader == 0)
		{
			return 0;
		}
		glAttachShader(program, shader);
	}

	return program;
}

inline GLuint load_program_from_file_wo_link_with_common
(
	GLenum shader_type,
	const std::string& common_src_file,
	const std::string& src_file
)
{
	GLuint program = glCreateProgram();
	assert(program!=0);
	if(program == 0)
	{
		return 0;
	}

	{
		const GLuint shader
		=
		load_shader_from_file(shader_type, common_src_file, src_file);
		assert(shader!=0);
		if(shader == 0)
		{
			return 0;
		}
		glAttachShader(program, shader);
	}

	return program;
}

#endif

//こんな書き方したらプログラムサイズが増えた...
#if 0
__forceinline GLuint load_program(const char* vs_src, const char* fs_src)
{
	GLuint program = glCreateProgram();
	assert(program!=0);

	const GLuint vs_shader = glCreateShader(GL_VERTEX_SHADER);
	assert(vs_shader!=0);
	const GLuint fs_shader = glCreateShader(GL_FRAGMENT_SHADER);
	assert(fs_shader!=0);

	glShaderSource(vs_shader, 1, (const GLchar**)&vs_src, NULL);
	glShaderSource(fs_shader, 1, (const GLchar**)&fs_src, NULL);

	glCompileShader(vs_shader);
	glCompileShader(fs_shader);

	glAttachShader(program, vs_shader);
	glAttachShader(program, fs_shader);

	return program;
}
#endif

}
}
