#pragma once

#include <iostream>
#include <string>

namespace tofu
{
namespace opengl
{
inline void print_context_info()
{
	using std::cout;
	using std::endl;
	using std::string;

	GLint contextProfileMask;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &contextProfileMask);
	string contextProfileMaskStr;
	if(contextProfileMask & GL_CONTEXT_CORE_PROFILE_BIT)
		contextProfileMaskStr += "GL_CONTEXT_CORE_PROFILE_BIT ";
	if(contextProfileMask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
		contextProfileMaskStr += "CONTEXT_COMPATIBILITY_PROFILE_BIT ";

	GLint contextFlags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);
	string contextFlagsStr;
	if(contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
		contextFlagsStr += "GL_CONTEXT_FLAG_DEBUG_BIT ";
	if(contextFlags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
		contextFlagsStr += "GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT ";

	cout <<
		"\nGL_VERSION:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_VERSION)) <<
		"\nGL_SHADING_LANGUAGE_VERSION:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)) <<
		"\nGL_RENDERER:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_RENDERER)) <<
		"\nGL_VENDOR:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_VENDOR)) <<
		"\nGL_CONTEXT_PROFILE_MASK:\n\t" <<
			contextProfileMaskStr	<<
		"\nGL_CONTEXT_FLAGS:\n\t" <<
			contextFlagsStr <<
		endl
		;

	GLint major_ver;
	GLint minor_ver;
	glGetIntegerv(GL_MAJOR_VERSION, &major_ver);
	glGetIntegerv(GL_MINOR_VERSION, &minor_ver);
	cout << "OpenGL version:" << major_ver << "." << minor_ver << endl;
}
}
}
