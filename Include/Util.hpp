#pragma once

namespace util {

void fatalErrorMessage(const char* message);

void* getDllExport(const char* symbol, const char* dllName);

}
