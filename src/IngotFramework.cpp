#include <IngotFrameWork.hpp>
#include <Arduino.h>
#include <stdint.h>
#include <limits.h>

//Pretty simple; I guess you could say its...EZ

//Bit functions are made for unsigned 1 byte integer types.



//Works on reading bits 0-7 in a byte. 0 = bit #1(first bit place, =1 when on), 7 = bit #8.
/*inline bool ReadBit(uint8_t TargetByte, unsigned char BitPlace)
{
		return 1 & (TargetByte >> BitPlace);	
}

inline bool ReadBit(uint16_t TargetByte, unsigned char BitPlace)
{
		return 1 & (TargetByte >> BitPlace);	
}
inline bool ReadBit(uint32_t TargetByte, unsigned char BitPlace)
{
		return 1 & (TargetByte >> BitPlace);	
}*/
inline bool ReadBit(uint64_t TargetByte, unsigned char BitPlace)
{
		return 1 & (TargetByte >> BitPlace);	
}


inline void SetBit(uint8_t* ByteToWrite, uint8_t BitPlace, bool Value)
{
	//Ty GeeksForGeeks
	if(Value == true)
	{
		*ByteToWrite = (*ByteToWrite | (1 << BitPlace));
	}
	else
	{
		*ByteToWrite = (*ByteToWrite & (~(1 << BitPlace)));
	}

}
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

#define INGOT_SYSTEMCHECK if (!SystemInitialized) { IN_IngotAbort("InitError", "");}





//input

bool IN_MainButtonDown(void)
{
	INGOT_SYSTEMCHECK
	return !digitalRead(BUTTONMAIN);

}
bool IN_SecondaryButtonDown(void)
{
	INGOT_SYSTEMCHECK
	return !digitalRead(BUTTONSECONDARY);

}
bool IN_ConsoleUpright(void)
{

	INGOT_SYSTEMCHECK
	return !digitalRead(UPRIGHTINPUT);
}

float IN_JoyStickVerticalPosition(void)
{

	INGOT_SYSTEMCHECK
	float val = (analogRead(JOYSTICKYOUT)/512.f)  -1;
	if (val < 0.025 && val > -0.025)
	{
		val = 0.f;
	}
	return val;
}

float IN_JoyStickHorizontalPosition(void)
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
	unsigned short SoundAudioID;
	long int TimestampMS;
	IN_AudioSound* Source;
	bool Paused;
} INTERNALSOUND;

typedef struct ISONG
{
	unsigned short SongAudioID;
	long int TimestampMS;
	IN_AudioSong* Source;
	bool Paused;
} INTERNALSONG;




static uint8_t AudioSoundNum = 0;
static uint8_t AudioSongNum = 0;
static INTERNALSOUND* SoundsArray = nullptr;
static INTERNALSONG* SongsArray = nullptr;

short MinorFunc_SongIDFound(unsigned short ID)
{
	for(unsigned char i = 0; i < AudioSongNum; i++)
	{

		if(SongsArray[i].SongAudioID == ID)
		{
			return i;
		}

	}
	return -1;

}

short MinorFunc_SoundIDFound(unsigned short ID)
{
	for(unsigned char i = 0; i < AudioSoundNum; i++)
	{

		if(SoundsArray[i].SoundAudioID == ID)
		{
			return i;
		}

	}
	return -1;

}

uint16_t IN_AddSoundToQueue(IN_AudioSound* sound,const unsigned short delay = 0)
{
	INGOT_SYSTEMCHECK
	if (AudioSoundNum == 255) 
	{
		IN_IngotAbort("INGOT error", "Sound OverfL");
	}


	AudioSoundNum++;
	SoundsArray = (INTERNALSOUND*)realloc(SoundsArray, sizeof(INTERNALSOUND) * AudioSoundNum );

	unsigned short ProposedID = 0;

	while(MinorFunc_SoundIDFound(ProposedID) != -1)
	{
		ProposedID = random(0, USHRT_MAX);	
	}


	SoundsArray[AudioSoundNum -1].SoundAudioID = ProposedID;
	SoundsArray[AudioSoundNum -1].TimestampMS = 0 - (long int)delay;
	SoundsArray[AudioSoundNum -1].Source = sound;
	SoundsArray[AudioSoundNum -1].Paused = false;
	

	return ProposedID;
}
uint16_t IN_AddSongToQueue(IN_AudioSong* song, const unsigned short delay = 0)
{
	INGOT_SYSTEMCHECK
	if (AudioSongNum == 255) 
	{
		IN_IngotAbort("INGOT error", " Song OverfL");
	}

	unsigned short ProposedID = 0;

	while(MinorFunc_SongIDFound(ProposedID) != -1)
	{
		ProposedID = random(0, USHRT_MAX);	
	}

	AudioSongNum++;
	SongsArray = (INTERNALSONG*)realloc(SongsArray, sizeof(INTERNALSONG) * AudioSongNum );

	SongsArray[AudioSongNum-1].SongAudioID = ProposedID;
	SongsArray[AudioSongNum-1].TimestampMS = 0 - (long int)delay;
	SongsArray[AudioSongNum-1].Source = song;
	SongsArray[AudioSongNum-1].Paused = false;

	return ProposedID;
}


void IN_PauseAllAudio(void)
{
	for(unsigned char song = 0; song < AudioSongNum; song++)
	{
		SongsArray[song].Paused = true;

	}
	for(unsigned char sound = 0; sound < AudioSoundNum; sound++)
	{
		SoundsArray[sound].Paused = true;

	}
}

void IN_ResumeAllAudio(void)
{
	for(unsigned char song = 0; song < AudioSongNum; song++)
	{
		SongsArray[song].Paused = false;

	}
	for(unsigned char sound = 0; sound < AudioSoundNum; sound++)
	{
		SoundsArray[sound].Paused = false;

	}
}
void IN_ClearAllAudio(void)
{
	AudioSoundNum = 0;
	AudioSongNum = 0;
	free(SoundsArray);
	free(SongsArray);
	SongsArray = nullptr;
	SoundsArray = nullptr;

}

	bool IN_RemoveQueueSoundFromID(const uint16_t SoundID)
	{
		INGOT_SYSTEMCHECK
		for(unsigned char i = 0; i < AudioSoundNum; i++)
		{
			if(SoundsArray[i].SoundAudioID == SoundID)
			{
				
				SoundsArray[i] = SoundsArray[AudioSoundNum-1]; 
				AudioSoundNum--;
				SoundsArray = (INTERNALSOUND*)realloc(SoundsArray, sizeof(INTERNALSOUND) * AudioSoundNum );
				return true;

			}

		}

		return false;
	}

	bool IN_PauseQueueSoundFromID(const uint16_t SoundID)
	{
		INGOT_SYSTEMCHECK
		for(unsigned char i = 0; i < AudioSoundNum; i++)
		{
			if(SoundsArray[i].SoundAudioID == SoundID)
			{
				SoundsArray[i].Paused = true;
				return true;
			}

		}

		return false;
	}


	bool IN_ResumeQueueSoundFromID(const uint16_t SoundID)
	{
		INGOT_SYSTEMCHECK
		for(unsigned char i = 0; i < AudioSoundNum; i++)
		{
			if(SoundsArray[i].SoundAudioID == SoundID)
			{
				SoundsArray[i].Paused = false;
				return true;
			}

		}

		return false;
	}

	bool IN_RemoveQueueSongFromID(const uint16_t SongID)
	{
		INGOT_SYSTEMCHECK
		for(unsigned char i = 0; i < AudioSongNum; i++)
		{
			if(SongsArray[i].SongAudioID == SongID)
			{
				
				SongsArray[i] = SongsArray[AudioSongNum-1]; 
				AudioSongNum--;
				SongsArray = (INTERNALSONG*)realloc(SongsArray, sizeof(INTERNALSONG) * AudioSongNum );
				return true;

			}

		}

		return false;
	}

	bool IN_PauseQueueSongFromID(const uint16_t SongID)
	{
		INGOT_SYSTEMCHECK
		for(unsigned char i = 0; i < AudioSongNum; i++)
		{
			if(SongsArray[i].SongAudioID == SongID)
			{
				SongsArray[i].Paused = true;
				return true;
			}

		}

		return false;
	}


	bool IN_ResumeQueueSongFromID(const uint16_t SongID)
	{
		INGOT_SYSTEMCHECK
		for(unsigned char i = 0; i < AudioSongNum; i++)
		{
			if(SongsArray[i].SongAudioID == SongID)
			{
				SongsArray[i].Paused = false;
				return true;
			}

		}

		return false;
	}




void IN_AudioIterate(const unsigned short TimeStepMS)
{
	INGOT_SYSTEMCHECK
	unsigned short OutFreq = 0;
		for(unsigned char SoundNum = 0; SoundNum < AudioSoundNum; SoundNum++)
		{
		SoundIterationStart:
			if(AudioSoundNum == 0)
			{
				break;
			}
			


			if(SoundsArray[SoundNum].TimestampMS > SoundsArray[SoundNum].Source->Duration)
			{
				//removal
				SoundsArray[SoundNum] = SoundsArray[AudioSoundNum-1]; 
				AudioSoundNum--;
				SoundsArray = (INTERNALSOUND*)realloc(SoundsArray, sizeof(INTERNALSOUND) * AudioSoundNum );
				goto SoundIterationStart;

			}

			if(SoundsArray[SoundNum].TimestampMS > 0 && !SoundsArray[SoundNum].Paused)
			{
				OutFreq += SoundsArray[SoundNum].Source->Frequency;
			}

			if(!SoundsArray[SoundNum].Paused)
			{

				SoundsArray[SoundNum].TimestampMS += TimeStepMS;

			}
		}


		for(unsigned char SongNum = 0; SongNum < AudioSongNum; SongNum++)
		{
			SongIterationStart:
			if(AudioSongNum == 0)
			{
				break;
			}
			//Calculate which sound note we are currently at, delete song if needed.
			
			long int SoundFinderTimeStamp = SongsArray[SongNum].TimestampMS;
			long int TotalSongTime = 0;
			//get total song time
			for(unsigned short i = 0; i < SongsArray[SongNum].Source->TotalSoundCount; i++)
			{
				TotalSongTime += SongsArray[SongNum].Source->SongSource[i].Duration;
			}

			//Duration Check
			if(SoundFinderTimeStamp > TotalSongTime)
			{
				//removal
				SongsArray[SongNum] = SongsArray[AudioSongNum-1]; 
				AudioSongNum--;
				SongsArray = (INTERNALSONG*)realloc(SongsArray, sizeof(INTERNALSONG) * AudioSongNum);
				goto SongIterationStart;
			}
			


			if(SoundFinderTimeStamp > 0 && !SongsArray[SongNum].Paused)
			{
				unsigned char CurrentSound = 0;
				while (1)
				{
					SoundFinderTimeStamp -= SongsArray[SongNum].Source->SongSource[CurrentSound].Duration;
					if (SoundFinderTimeStamp >= 0)
					{
						CurrentSound++;
					}
					else
					{
						break;
					}

				}

				OutFreq += SongsArray[SongNum].Source->SongSource[CurrentSound].Frequency;
			}


			if (!SongsArray[SongNum].Paused)
			{
				SongsArray[SongNum].TimestampMS += TimeStepMS;
			}

			

		}



		if(OutFreq == 0)
		{
			noTone(SOUNDOUTPUT);
		}
		else
		{
			if (OutFreq > 5000){OutFreq = 5000;}
			tone(SOUNDOUTPUT, OutFreq);
		}
}










//*Yoinks code from past self respectfully*

#define BOOLG_ROWNUMBER 2 //number of rows
#define BOOLG_COLNUMBER 4 //number of columns
			  //


//This only supports 5x8 characters because we are kind of just a wrapper for liquidcrystal.h, and custom characters only support 5x8
#define BOOLG_CHARWIDTH 5 
#define BOOLG_CHARHEIGHT 8 


#define BOOLG_ROWLENGTH  (BOOLG_CHARWIDTH * BOOLG_COLNUMBER + 3) //Number of pixels in a row 
#define BOOLG_COLLENGTH  (BOOLG_CHARHEIGHT * BOOLG_ROWNUMBER + 1) ////Number of pixels in a column 



static LiquidCrystal* LCDDisplay = nullptr;
uint8_t* ScreenBuffer = NULL;
unsigned short BufferSize = 0;

bool IgnoreSpacing = false;





void IN_ClearScreenBuffer(void)
{
	INGOT_SYSTEMCHECK
	memset(ScreenBuffer, 0, BufferSize);//Reset the memory, clearing the screen
}

bool IN_GetBufferPixel( uint8_t PosX, uint8_t PosY)
{
	INGOT_SYSTEMCHECK
	if ((PosY < BOOLG_COLLENGTH && PosX < BOOLG_ROWLENGTH) )
	{
		// math formulas give us the byte where the row starts, and it's bit.
		uint8_t* DestByte = ScreenBuffer + int((PosY * BOOLG_ROWLENGTH) /8.f);
		unsigned char RowLeftover = (PosY * BOOLG_ROWLENGTH) % 8;
		DestByte +=  int((PosX + RowLeftover)/8.f);
		unsigned char FinalBitPosition = 7- (PosX + RowLeftover) % 8;

		return ReadBit(*DestByte, FinalBitPosition);
	}
	return false;

}

bool IN_SetBufferPixel(bool pixelval, uint8_t PosX, uint8_t PosY)
{
	INGOT_SYSTEMCHECK
	if ((PosY < BOOLG_COLLENGTH ) && (PosX < BOOLG_ROWLENGTH ))
	{

		
			uint8_t* DestByte = ScreenBuffer + int((PosY * BOOLG_ROWLENGTH) /8.f);//Get row start byte // BGRAPHICSSECRETS::ScreenBuffer +  ((Yposition * BOOLG_ROWLENGTH + Xposition)/8);
			unsigned char RowLeftover =  (PosY* BOOLG_ROWLENGTH) % 8;
			DestByte += int((PosX + RowLeftover)/8.f);
			unsigned char FinalBitPosition = 7- (PosX + RowLeftover) % 8;
			SetBit(DestByte, FinalBitPosition, pixelval);
		
		return true;

	}
	return false;

}

LiquidCrystal* IN_GetLCDObject()
{
	INGOT_SYSTEMCHECK
	return LCDDisplay;
}

void IN_SetIgnoreGapSpacing(bool set)
{
	IgnoreSpacing = set;
}

void IN_SetLineText(const char* message, uint8_t lineNum)
{
	INGOT_SYSTEMCHECK;
	LCDDisplay->setCursor(4, lineNum);
	LCDDisplay->print("             ");
	LCDDisplay->setCursor(4, lineNum);
	LCDDisplay->print(message);

}

bool IN_Image::GetImagePixel(uint8_t PosX, uint8_t PosY)
{
			uint8_t* DestByte = this->ImageSource + int((PosY * SizeX) /8.f);//Get row start byte // BGRAPHICSSECRETS::ScreenBuffer +  ((Yposition * BOOLG_ROWLENGTH + Xposition)/8);
			unsigned char RowLeftover =  (PosY* SizeX) % 8;
			DestByte += int((PosX + RowLeftover)/8.f);
			unsigned char FinalBitPosition = 7- (PosX + RowLeftover) % 8;
			return ReadBit(*DestByte, FinalBitPosition);

}
bool IN_Image::SetImagePixel(bool PxVal, uint8_t PosX, uint8_t PosY)
{
	//if(PosX < SizeX && PosY < SizeY)
	{
			uint8_t* DestByte = this->ImageSource + int((PosY * SizeX) /8.f);//Get row start byte // BGRAPHICSSECRETS::ScreenBuffer +  ((Yposition * BOOLG_ROWLENGTH + Xposition)/8);
			unsigned char RowLeftover =  (PosY* SizeX) % 8;
			DestByte += int((PosX + RowLeftover)/8.f);
			unsigned char FinalBitPosition = 7- (PosX + RowLeftover) % 8;
			SetBit(DestByte, FinalBitPosition, PxVal);
			return true;
	}
	//return false;
}


IN_Image::IN_Image(uint8_t width, uint8_t height, uint8_t bitsource[] )
{
	this->SizeX = width;
	this->SizeY = height;
	this->ImageSource = static_cast<uint8_t*>(malloc(ceil((SizeX * SizeY)/8.f)));
	
	for(uint8_t y = 0; y < height; y++)
	{
		for(uint8_t x = 0; x < width; x++)
		{
			this->SetImagePixel(ReadBit(bitsource[y] , (width-1)-x), x, y);


		}
	}

}
IN_Image::IN_Image(uint8_t width, uint8_t height, uint16_t bitsource[] )
{
	this->SizeX = width;
	this->SizeY = height;
	this->ImageSource = static_cast<uint8_t*>(malloc(ceil((SizeX * SizeY)/8.f)));
	
	for(uint8_t y = 0; y < height; y++)
	{
		for(uint8_t x = 0; x < width; x++)
		{
			this->SetImagePixel(ReadBit(bitsource[y] , (width-1)-x), x, y);


		}
	}

}
IN_Image::IN_Image(uint8_t width, uint8_t height, uint32_t bitsource[] )
{
	this->SizeX = width;
	this->SizeY = height;
	this->ImageSource = static_cast<uint8_t*>(malloc(ceil((SizeX * SizeY)/8.f)));
	
	for(uint8_t y = 0; y < height; y++)
	{
		for(uint8_t x = 0; x < width; x++)
		{
			this->SetImagePixel(ReadBit(bitsource[y] , (width-1)-x), x, y);


		}
	}

}
IN_Image::IN_Image(uint8_t width, uint8_t height, uint64_t bitsource[] )
{
	this->SizeX = width;
	this->SizeY = height;
	this->ImageSource = static_cast<uint8_t*>(malloc(ceil((SizeX * SizeY)/8.f)));
	
	for(uint8_t y = 0; y < height; y++)
	{
		for(uint8_t x = 0; x < width; x++)
		{
			this->SetImagePixel(ReadBit(bitsource[y] , (width-1)-x), x, y);


		}
	}

}

//Draw a sprite. Drawing is modified by ORing the IN_RF flags.
void IN_DrawImage(IN_Image* Sprite, short ScreenX, short ScreenY, uint8_t RenderFlags )
{
	INGOT_SYSTEMCHECK
		
		//Serial.println();
		for(uint8_t Y = 0; Y < Sprite->SizeY; Y++)
		{
			for(uint8_t X = 0; X < Sprite->SizeX; X++)
			{

				uint8_t PXDrawX = X;
				uint8_t PXDrawY = Y;
		/*		if(Drawval)
				{
					Serial.print("██");
				}
				else
				{
					Serial.print("  ");

				}
				delay(2);*/
				
				if((RenderFlags & IN_RF_MIRRORX) == IN_RF_MIRRORX)
				{
					PXDrawX = Sprite->SizeX - X -1;
				}
				if((RenderFlags & IN_RF_MIRRORY) == IN_RF_MIRRORY)
				{

					PXDrawY = Sprite->SizeY - Y -1;
				}
				bool Drawval = Sprite->GetImagePixel(PXDrawX, PXDrawY);

				if((RenderFlags & IN_RF_FALSETRANSPARENT) == IN_RF_FALSETRANSPARENT && !Drawval)
				{
					continue;
				}
				if((RenderFlags & IN_RF_INVERTED) == IN_RF_INVERTED)
				{
					Drawval = !Drawval;
				}
				
					IN_SetBufferPixel(Drawval, ScreenX + X,ScreenY + Y );
				


			}
			Serial.println();
		}


}

//draw a rectangle of on or off pixels
void IN_DrawRect(bool Val, uint8_t Width, uint8_t Height, short PosX, short PosY)
{
	INGOT_SYSTEMCHECK
	
	for (uint8_t y = 0; y < Height; y++)
	{
		for (uint8_t x = 0; x < Height; x++)
		{
			IN_SetBufferPixel(Val, PosX+x, PosY+y);

		}

	}

}
void IN_InvertScreenBuffer()
{
	INGOT_SYSTEMCHECK
	for (unsigned short i = 0; i < BufferSize; i++)
	{
		ScreenBuffer[i] = ~ScreenBuffer[i];	
	}

}
//Set the console screen to look like that data for the screen.
void IN_DisplayScreenBuffer()
{
	INGOT_SYSTEMCHECK
		unsigned char CharactersUsed = 0;

		for (unsigned short row = 0; row < BOOLG_ROWNUMBER; row++)
		{

			for (unsigned short col = 0; col < BOOLG_COLNUMBER; col++)
			{

				if(CharactersUsed > 7)//LCDs are evil =[
				{
					return;
				}
				 byte FiveByEightSource[8] = {0};
				//construct character from screen data chunk
				for(unsigned char charrow = 0; charrow < 8; charrow++)
				{
					for(unsigned char px = 0; px < 5 /*+ IGNORESPACING*/; px++)
					{
						short resultX = col * BOOLG_CHARWIDTH + px;
						if(!IgnoreSpacing)
						{
							resultX += col;
						}
						short resultY =  row * BOOLG_CHARHEIGHT + charrow;
						if(!IgnoreSpacing)
						{
							resultY += row;
						}
						
						
							bool Result = IN_GetBufferPixel(resultX, resultY);
							SetBit(&FiveByEightSource[charrow],4-px, Result);
						

					}


				}


				LCDDisplay->createChar(CharactersUsed, FiveByEightSource);
				LCDDisplay->setCursor(col, row);
				LCDDisplay->write(byte(CharactersUsed));
				CharactersUsed++;


			}

		}

}




void  IN_IngotInitialize(void)
{
	if(!SystemInitialized)
	{
		//Thats a pretty long declaration
		LCDDisplay = new LiquidCrystal(LCD_RSPIN, LCD_READWRITEPIN, LCD_ENABLEPIN, LCD_DATA4, LCD_DATA5, LCD_DATA6, LCD_DATA7);
		LCDDisplay->begin(16, 2);

		ScreenBuffer = static_cast<uint8_t*> ( malloc(ceil( ((BOOLG_ROWLENGTH+5) * (BOOLG_COLLENGTH))/8.f)) );

		//Serial.print("Number of bytes allocated:" );
		//Serial.println(ceil( (BOOLG_ROWLENGTH * BOOLG_COLLENGTH)/8.f)) ;
		BufferSize = ceil( ((BOOLG_ROWLENGTH) * (BOOLG_COLLENGTH))/8.f) ;

		memset(ScreenBuffer , 0, BufferSize);//initialize the memory



		randomSeed(analogRead(4)); 
		//Initialize da pins!!
		pinMode(BUTTONMAIN, INPUT_PULLUP);
		pinMode(BUTTONSECONDARY, INPUT_PULLUP);
		pinMode(UPRIGHTINPUT, INPUT_PULLUP);

		pinMode(SOUNDOUTPUT, OUTPUT);

		SystemInitialized = true;
	}
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
		free(SongsArray);
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
