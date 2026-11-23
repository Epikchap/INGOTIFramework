#include <Arduino.h>
#include <IngotFrameWork.hpp>


IN_Image Jim(16,4,  
		(uint16_t[4]){
		0b0000000110000000,
		0b0100111111111100,
		0b0011111111101111,
		0b0100111111111000}
		 );


IN_AudioSound sound = {700, 3000};

IN_AudioSong song;
IN_AudioSound source[5] = {{700, 500}, {500, 1000},{800, 800},{100, 300},{300, 1000},};
float PlayerX = 0;
float PlayerY = 0;
bool FacingRight = true;
void setup()
{
	song.SongSource = source;
	song.TotalSoundCount = 5;

	Serial.begin(9600);
	IN_IngotInitialize();

	IN_SetLineText("AAAAAAAAAAAAAAAAAAAAA", 1);
	IN_SetLineText("fisch", 1);

//	IN_AddSoundToQueue(&sound, 0);
//	IN_AddSongToQueue(&song, 3000);
}


bool buttondownprev = false;
bool buttondownprev2 = false;
void loop()
{
	IN_ClearScreenBuffer();

	PlayerX += IN_JoyStickHorizontalPosition();
	PlayerY -= IN_JoyStickVerticalPosition();
	if(IN_JoyStickHorizontalPosition() > 0)
	{
		FacingRight = true;

	}
	if(IN_JoyStickHorizontalPosition() < 0)
	{
		FacingRight = false;

	}

	if(FacingRight)
	{
		IN_DrawImage(&Jim, PlayerX, PlayerY );
	}
	else
	{
		IN_DrawImage(&Jim, PlayerX, PlayerY, IN_RF_MIRRORX );
	}
	//IN_DrawRect(1, 7, 8, PlayerX, PlayerY);
	
	if(0 && !IN_ConsoleUpright())
	{

		IN_GetLCDObject()->setCursor(5, 0);
		IN_GetLCDObject()->print("UpsideDown");

	}
	if(IN_MainButtonDown())
	{
		if (buttondownprev == false)
		{
			IN_AddSoundToQueue(&sound, 0);
		}
		buttondownprev = true;
	
	}
	else
	{
		buttondownprev = false;

	}


	if(IN_SecondaryButtonDown())
	{
		if (buttondownprev2 == false)
		{
			IN_AddSongToQueue(&song, 0);
		}
		buttondownprev2 = true;
	
	}
	else
	{
		buttondownprev2 = false;
	}


	IN_DisplayScreenBuffer();
	IN_AudioIterate(30);
	delay(30);
	Serial.println("Iteration");
	
}
