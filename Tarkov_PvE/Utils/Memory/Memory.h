#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>

class Memory
{
private:
	HWND TargetHwnd;
	char TargetWindow[32] = "EscapeFromTarkov";
	char TargetClass[32]  = "";

	uintptr_t GetModuleBase(const std::string module);
public:
	DWORD PID;
	HANDLE pHandle;
	uint64_t BaseAddress;

	bool Init();

	template <typename T>
	constexpr const T Read(const uintptr_t& address) noexcept
	{
		T value = { };
		ReadProcessMemory(pHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}
	template <typename T>
	constexpr void Write(const uintptr_t& address, const T& value) noexcept
	{
		WriteProcessMemory(pHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
	uintptr_t ReadChain(uintptr_t address, const std::vector<uint64_t>& offsets)
	{
		uintptr_t result = Read<uint64_t>(address + offsets.at(0));
		for (int i = 1; i < offsets.size(); i++)
			result = Read<uintptr_t>(result + offsets.at(i));

		return result;
	}
};

extern Memory m;