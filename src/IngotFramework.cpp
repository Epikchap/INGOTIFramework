#include <IngotFrameWork.hpp>
#include <Arduino.h>
#include <EzBits.h>

#define LCD_DATA4 4
#define LCD_DATA5 5
#define LCD_DATA6 6
#define LCD_DATA7 7
#define LCD_RSPIN 10
#define LCD_READWRITEPIN 11
#define LCD_ENABLEPIN 12

#define BUTTONMAIN A1
#define BUTTONSECONDARY A2
#define UPRIGHTINPUT A0

#define JOYSTICKXOUT A7
#define JOYSTICKYOUT A6

#define SOUNDOUTPUT 2


static bool SystemInitialized = false;
static LiquidCrystal* LCDDisplay = nullptr;

#define INGOT_SYSTEMCHECK if (!SystemInitialized) { IN_IngotAbort("InitError", "");}





//input

bool IN_MainButtonDown()
{
	INGOT_SYSTEMCHECK
	return !digitalRead(BUTTONMAIN);

}
bool IN_SecondaryButtonDown()
{
	INGOT_SYSTEMCHECK
	return !digitalRead(BUTTONSECONDARY);

}
bool IN_ConsoleUpright()
{

	INGOT_SYSTEMCHECK
	return !digitalRead(UPRIGHTINPUT);
}

float IN_JoyStickVerticalPosition()
{

	INGOT_SYSTEMCHECK
	float val = (analogRead(JOYSTICKYOUT)/512.f)  -1;
	if (val < 0.025 && val > -0.025)
	{
		val = 0.f;
	}
	return val;
}

float IN_JoyStickHorizontalPosition()
{

	INGOT_SYSTEMCHECK
	float val = (analogRead(JOYSTICKXOUT)/512.f)  -1;
	if (val < 0.025 && val > -0.025)
	{
		val = 0.f;
	}
	return val;
}



//sound output


typedef struct IS
{
	unsigned char AudioID;
	long int TimestampMS;
	IN_AudioSound* source;
} INTERNALSOUND;

typedef struct ISONG
{
	unsigned char AudioID;
	long int TimestampMS;
	IN_AudioSong* source;
} INTERNALSONG;

static bool AudioPaused = false;

static uint8_t AudioSoundNum = 0;
static uint8_t AudioSongNum = 0;
static INTERNALSOUND* SoundsArray = nullptr;
static INTERNALSONG* SongArray = nullptr;

unsigned char IN_AddSoundToQueue(IN_AudioSound* sound,const unsigned short delay = 0)
{
	INGOT_SYSTEMCHECK
	if (AudioSoundNum == 255) 
	{
		IN_IngotAbort("INGOT error", "Sound OverfL");
	}


	AudioSoundNum++;
	SoundsArray = (INTERNALSOUND*)realloc(SoundsArray, sizeof(INTERNALSOUND) * AudioSoundNum );

	SoundsArray[AudioSoundNum -1].AudioID = AudioSoundNum;
	SoundsArray[AudioSoundNum -1].TimestampMS = 0 - (int)delay;
	SoundsArray[AudioSoundNum -1].source = sound;




	return AudioSoundNum;
}
unsigned char IN_AddSongToQueue(IN_AudioSong* song, const unsigned short delay = 0)
{
	INGOT_SYSTEMCHECK
	if (AudioSongNum == 255) 
	{
		IN_IngotAbort("INGOT error", " Song OverfL");
	}


	AudioSongNum++;
	SongArray = (INTERNALSONG*)realloc(SoundsArray, sizeof(INTERNALSONG) * AudioSongNum );

	SongArray[AudioSongNum-1].AudioID = AudioSongNum;
	SongArray[AudioSongNum-1].TimestampMS = 0 - (int)delay;
	SongArray[AudioSongNum-1].source = song;

	return AudioSongNum;
}



void IN_AudioIterate(const unsigned int TimeStepMS)
{
	INGOT_SYSTEMCHECK
	if(!AudioPaused)
	{
		unsigned long int OutFreq = 0;
		for(unsigned char SoundNum = 0; SoundNum < AudioSoundNum; SoundNum++)
		{
			SoundIterationStart:
			if(AudioSoundNum == 0)
			{
				break;
			}
			

			SoundsArray[SoundNum].TimestampMS += TimeStepMS;
			Serial.println(SoundsArray[SoundNum].TimestampMS);

			if(SoundsArray[SoundNum].TimestampMS > SoundsArray[SoundNum].source->Duration)
			{
				Serial.println("removed");
				//removal
				SoundsArray[SoundNum] = SoundsArray[AudioSoundNum-1]; 
				AudioSoundNum--;
				SoundsArray = (INTERNALSOUND*)realloc(SoundsArray, sizeof(INTERNALSOUND) * AudioSoundNum );
				goto SoundIterationStart;

			}

			if(SoundsArray[SoundNum].TimestampMS > 0)
			{
				OutFreq += SoundsArray[SoundNum].source->Frequency;
			}

		}

		for(unsigned char SongNum = 0; SongNum < AudioSongNum; SongNum++)
		{

		}


		if(OutFreq == 0)
		{
			noTone(SOUNDOUTPUT);
		}
		else
		{
			tone(SOUNDOUTPUT, OutFreq);
		}

	}

}





void  IN_IngotInitialize()
{
	//Thats a pretty long declaration
	LCDDisplay = new LiquidCrystal(LCD_RSPIN, LCD_READWRITEPIN, LCD_ENABLEPIN, LCD_DATA4, LCD_DATA5, LCD_DATA6, LCD_DATA7);
	LCDDisplay->begin(16, 2);

	//Initialize da pins!!
	pinMode(BUTTONMAIN, INPUT_PULLUP);
	pinMode(BUTTONSECONDARY, INPUT_PULLUP);
	pinMode(UPRIGHTINPUT, INPUT_PULLUP);

	pinMode(SOUNDOUTPUT, OUTPUT);

	SystemInitialized = true;
}



void IN_IngotAbort(const char* ErrLine1, const char* ErrLine2)
{
	if(strcmp(ErrLine1, "InitError") == 0)
	{
		LCDDisplay = new LiquidCrystal(LCD_RSPIN, LCD_READWRITEPIN, LCD_ENABLEPIN, LCD_DATA4, LCD_DATA5, LCD_DATA6, LCD_DATA7);
		LCDDisplay->begin(16, 2);
		pinMode(SOUNDOUTPUT, OUTPUT);

		LCDDisplay->setCursor(0, 0);
		LCDDisplay->print("Fatal Error: ");
		LCDDisplay->setCursor(0, 1);
		LCDDisplay->print(" Not initialized");
		tone(SOUNDOUTPUT, 150, 2200);
		while (true)
		{
			delay(500);
		}

	}
	else
	{
		free(SoundsArray);
		free(SongArray);
		LCDDisplay->setCursor(4, 0);
		LCDDisplay->print(ErrLine1);
		LCDDisplay->setCursor(4, 1);
		LCDDisplay->print(ErrLine2);

		tone(SOUNDOUTPUT, 500, 500);
		noTone(SOUNDOUTPUT);
		tone(SOUNDOUTPUT, 300, 500);
		

		
		while (true)
		{
			delay(500);
		}
	}


}
