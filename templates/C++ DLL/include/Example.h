#pragma once

#ifdef DLL
#ifdef EXAMPLE_EXPORTS
#define EXAMPLE_API __declspec(dllexport)
#else
#define EXAMPLE_API __declspec(dllimport)
#endif // Easycript api

#else
// Define as nothing
#define EXAMPLE_API
#endif // DLL

int EXAMPLE_API example(int a, int b);

