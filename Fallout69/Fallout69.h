#pragma once

#include <iostream>
#include <string>
#include <math.h>

// Uses the exploit tools library.
#include "ExploitTools/ExploitTools.h"
#pragma comment(lib, "ExploitTools/ExploitTools.lib")

// The speed of the player.
#define PLAYER_SPEED .001f
#define PLAYER_SPRINT_SPEED .01f

// Encapsulates the Fallout 76 tools.
namespace Fallout76
{
	// Allows manipulation of a Vector 3.
	class Vector3
	{
	public:
		float x, z, y;

		Vector3& operator+=(const Vector3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector3& operator-=(const Vector3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
	};

	// Allows manipulation of the local player.
	class Player
	{

		HANDLE procHandle;

		char * addr;

	public:
		Player(HANDLE procHandle, char * addr)
			: procHandle(procHandle), addr(addr)
		{

		}

		char * GetAddress() { return addr; }

		float GetPitch()
		{
			float Pitch;
			ReadProcessMemory(procHandle, addr + 0xD8, &Pitch, sizeof(Pitch), 0);
			return Pitch;
		}

		float GetYaw()
		{
			float Yaw;
			ReadProcessMemory(procHandle, addr + 0xD0, &Yaw, sizeof(Yaw), 0);
			return Yaw;
		}

		Vector3 GetPosition()
		{
			// Stores the player position.
			Vector3 playerPos;
			// Stores the external address.
			char * externAddr;
			// Follows the pointer chain.
			ReadProcessMemory(procHandle, addr + 0x790, &externAddr, sizeof(externAddr), 0);
			ReadProcessMemory(procHandle, externAddr + 0x8, &externAddr, sizeof(externAddr), 0);
			ReadProcessMemory(procHandle, externAddr + 0x438, &externAddr, sizeof(externAddr), 0);
			ReadProcessMemory(procHandle, externAddr + 0x4A0, &externAddr, sizeof(externAddr), 0);
			ReadProcessMemory(procHandle, externAddr + 0x70, &playerPos, sizeof(playerPos), 0);
			// Returns the player position.
			return playerPos;
		}

		bool SetPosition(Vector3 position)
		{
			// Stores the external address.
			char * externAddr;
			// Follows the pointer chain.
			ReadProcessMemory(procHandle, addr + 0x790, &externAddr, sizeof(externAddr), 0);
			ReadProcessMemory(procHandle, externAddr + 0x8, &externAddr, sizeof(externAddr), 0);
			ReadProcessMemory(procHandle, externAddr + 0x438, &externAddr, sizeof(externAddr), 0);
			ReadProcessMemory(procHandle, externAddr + 0x4A0, &externAddr, sizeof(externAddr), 0);
			WriteProcessMemory(procHandle, externAddr + 0x70, &position, sizeof(position), 0);
			// Player's position has been changed.
			return true;
		}
	};

	// Attaches to the game process.
	class Fallout69
	{
		HANDLE procHandle;

	public:
		Fallout69(std::string imgName = "Fallout76.exe")
		{
			DWORD procId = Process::FindProcess(imgName.c_str());
			procHandle = OpenProcess(PROCESS_ALL_ACCESS, false, procId);
		}

		Player GetLocalPlayer()
		{
			// Stores the player address.
			char *playerAddr;
			// Gets the module base address.
			HMODULE modHandle = Module::GetRemoteModuleHandle(procHandle, "Fallout76.exe");
			// Calculates the local player address and reads the address.
			ReadProcessMemory(procHandle, reinterpret_cast<char *>(modHandle) + 0x6A09CC0, &playerAddr, sizeof(playerAddr), 0);
			// Returns the player address.
			return Player(procHandle, playerAddr);
		}
	};
}