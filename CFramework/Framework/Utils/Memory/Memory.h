#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <string>
#include <vector>

/*
	[+] メモリやオーバーレイの初期化モードを設定します

	WINDOW_TITLE : ウィンドウのタイトルを使用します
	WINDOW_CLASS : ウィンドウのクラス名を使用します
	PROCESS      : 実行ファイル名を使用します                */
enum InitMode : int
{
	WINDOW_TITLE,
	WINDOW_CLASS,
	PROCESS
};

class Memory
{
private:
	DWORD PID;
	HANDLE pHandle;

	uintptr_t GetModuleBase(const std::string moduleName);
	PROCESSENTRY32 GetProcess(const std::string processName);
public:
	uintptr_t m_gBaseAddress;

	bool AttachProcess(const std::string targetName, int mode);
	void DetachProcess();

	// R/WPM Templates
	template <typename T>
	constexpr const T Read(const uintptr_t& address) const noexcept
	{
		T value{};
		ReadProcessMemory(pHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}
	template <typename T>
	constexpr void Write(const uintptr_t& address, const T& value) const noexcept
	{
		WriteProcessMemory(pHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
	uintptr_t ReadChain(uintptr_t address, const std::vector<uintptr_t>& offsets)
	{
		uintptr_t result = Read<uintptr_t>(address + offsets.at(0));
		for (int i = 1; i < offsets.size(); i++)
			result = Read<uintptr_t>(result + offsets.at(i));

		return result;
	}
	bool ReadString(uintptr_t address, LPVOID buffer, SIZE_T size) const
	{
		SIZE_T size_read;

		return !!::ReadProcessMemory(pHandle, LPCVOID(address), buffer, size, &size_read) && size_read > 0;
	}
	std::string ReadString(ULONG64 address, int length)
	{
		static char Buffer[256]{};
		ReadProcessMemory(pHandle, (void*)(address), Buffer, length, nullptr);
		
		return std::string(Buffer);
	};
};

extern Memory m;