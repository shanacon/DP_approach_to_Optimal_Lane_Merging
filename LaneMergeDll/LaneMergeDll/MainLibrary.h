#pragma once

#ifdef LaneMergeDll_EXPORTS
#define EXPORT  __declspec(dllexport)
#else
#define LaneMergeDll_API __declspec(dllimport)
#endif
extern "C" LaneMergeDll_API void SetUpRandom(double lamba_, int CaseLen_, int CaseNum_, float Wequal_, float Wdiff_, float Tf_);
extern "C" LaneMergeDll_API int GetTimeList(int LaneNum, int* arr);
extern "C" LaneMergeDll_API int GetRealTime(int LaneNum, int* RealTime);
extern "C" LaneMergeDll_API void DoDP();
extern "C" LaneMergeDll_API void DoConDP();
extern "C" LaneMergeDll_API void DoFIFO();
extern "C" LaneMergeDll_API void DoConFIFO();
extern "C" LaneMergeDll_API int GetTimeOneAns(int LaneNum, float* TimeList);
extern "C" LaneMergeDll_API int GetTimeAns(int LaneNum, float* TimeList);
extern "C" LaneMergeDll_API float GetFinalTime();
extern "C" LaneMergeDll_API float GetTDelay();
extern "C" LaneMergeDll_API void ClearAll();
extern "C" LaneMergeDll_API void Test(int* arr);