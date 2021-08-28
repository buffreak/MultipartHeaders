#ifndef MIME_TYPES
#include<iostream>
#include<map>

namespace MimeType {
	static std::map<std::string, std::string> MIME_LIST{
		{ "txt", "text/plain"},
		{ "exe", "application/octet-stream" },
		{ "bin", "application/octet-stream" },
		{ "pdf", "application/pdf" },
		{ "zip", "application/zip" },
		{ "docx", "application/msword" },
		{ "doc", "application/msword"},
		{ "png", "image/png"},
		{ "jpg", "image/jpg" },
		{ "jpeg", "image/jpeg" },
		{ "bmp", "image/bmp" },
		// You can define it by what you want too
	};
}

#endif // !MIME_TYPES

