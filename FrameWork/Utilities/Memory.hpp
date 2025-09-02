#pragma once

#include <FrameWork/FrameWork.hpp>

#include <vector>
#include <psapi.h>

namespace FrameWork
{
	namespace Memory
	{
		HWND GetWindowHandleByPID(int Pid);
		HWND GetWindowHandleByName(std::wstring WindowName);
		DWORD GetProcessPidByName(std::wstring ProcessName);
		uint64_t GetModuleBaseByName(DWORD Pid, std::wstring ModuleName);

		uint64_t GetModuleBaseSizeByName(DWORD Pid, std::wstring ModuleName);

		void AttachProces(DWORD Pid);
		void DetachProcess();

		void ReadProcessMemoryImpl(uint64_t ReadAddress, LPVOID Read, SIZE_T Size);
		bool WriteProcessMemoryImpl(uint64_t WriteAddress, LPVOID Write, SIZE_T Size);

		uintptr_t FindSignature(std::vector<uint8_t> Signature, uintptr_t ModuleBase, uintptr_t ModuleBaseSize);

		BOOL WriteBytes(uintptr_t Addr, std::vector<uint8_t> Bytes);

		BOOL PatchFunc(uintptr_t Addr, int NopCount);

		std::string ReadProcessMemoryString(uint64_t ReadAddress, SIZE_T StringSize = 256);

		template <typename T, typename B>
		T ReadMemory(B ReadAddress)
		{
			T Read;
			ReadProcessMemoryImpl((uint64_t)ReadAddress, &Read, sizeof(T));
			return Read;
		}

		template <typename T, typename B>
		bool WriteMemory(B WriteAddress, T Value)
		{
			return WriteProcessMemoryImpl((uint64_t)WriteAddress, &Value, sizeof(T));
		}


	}
}