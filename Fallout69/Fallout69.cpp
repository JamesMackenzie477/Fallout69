#include "Fallout69.h"

int main()
{
	// Used to calculate the player movement offset.
	Fallout76::Vector3 Offset, Position;
	// Stores the player's pitch and yaw.
	float Yaw, Pitch;
	// True if the hack is enabled.
	bool Enabled = false, Sprint = false, KeyDown = false;
	// Connects to the Fallout process.
	Fallout76::Fallout69 F76;
	// Gets the local player.
	auto Player = F76.GetLocalPlayer();
	// Enters the input loop.
	while (true)
	{
		// Enables or disables the hack.
		if (!KeyDown && (GetKeyState(VK_NUMPAD5) & 0x8000)) KeyDown = true;
		if (KeyDown && !(GetKeyState(VK_NUMPAD5) & 0x8000))
		{
			KeyDown = false;

			Enabled = !Enabled;

			if (Enabled)
			{
				// Gets the player's position.
				Position = Player.GetPosition();
			}
		}
		// Checks if the hack is enabled.
		if (Enabled)
		{
			// Increases the player speed if true.
			Sprint = false; // GetKeyState(VK_SHIFT) & 0x8000;

			// Gets the player's pitch and yaw values.
			Pitch = Player.GetPitch();
			Yaw = Player.GetYaw();

			// Calculates the movement offset.
			Offset = {
				(Sprint ? PLAYER_SPRINT_SPEED : PLAYER_SPEED) * sinf(Pitch) * cosf(Yaw),
				(Sprint ? PLAYER_SPRINT_SPEED : PLAYER_SPEED) * cosf(Pitch) * cosf(Yaw),
				(Sprint ? PLAYER_SPRINT_SPEED : PLAYER_SPEED) * -sinf(Yaw)
			};

			// Moves the player forward or backward depending on the input.
			if (GetKeyState(VK_NUMPAD8) & 0x8000) Position += Offset;
			if (GetKeyState(VK_NUMPAD2) & 0x8000) Position -= Offset;

			// Updates the player position.
			Player.SetPosition(Position);
		}
	}
	// Waits for the user to exit.
	std::cin.get();
}