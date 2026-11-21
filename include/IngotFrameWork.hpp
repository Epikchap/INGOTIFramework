#ifndef INGOTFRAMEWORK
#define INGOTFRAMEWORK

#include <stdint.h>
#include <LiquidCrystal.h>


//IngotFrameWork
//Very minimal, and as a result, pretty lightweight. There is very little structure that
//needs to be followed.








	//USER INPUT
	//
	//These are just inputs the console can recieve. They're pretty self explanatory.
		

	//Returns true if MAINBUTTON is currently held down
	bool IN_MainButtonDown();
	//Returns true if SECONDARYBUTTON is currently held down
	bool IN_SecondaryButtonDown();

	//Returns True if the console is upright. (based off tilt ball sensor)
	bool IN_ConsoleUpright();

	//Returns a float between -1.0 and 1.0, representing the joysticks vertical orientation
	float IN_JoyStickVerticalPosition();
	//Returns a float between -1.0 and 1.0, representing the joysticks horizontal orientation
	float IN_JoyStickHorizontalPosition();





	//SOUND OUTPUT. 
	//
	//Sound objects can be created. They must be added to the sound queue to "exist".
	//Only 255 different sounds can be played at the same time. Going over this limit will
	//There are also songs, which are really just sound arrays. They also have a limit of 255.
	//lead to an Ingot error.
	

	//Represents a singular sound.
	typedef struct AS 
	{
		unsigned short Frequency;
		unsigned short Duration;

	} IN_AudioSound;

	//Array of AudioSounds.
//YOU [finger point] are in charge of making sure YOU [finger point x2] have the right number of array elements.
	typedef struct ASo 
	{
		unsigned short TotalSoundCount;
		IN_AudioSound SongSource[];
		

	} IN_AudioSong;


	



	


	void IN_PauseAudioQueue();
	void IN_ResumeAudioQueue();
	void IN_ClearAudioQueue();


	//Play a sound. Second parameter is for a delay for how many ms it waits before playing (in ticks)
	//Returns an ID to the sound, which can be used for specifically altering that one sound.
	//Requires the Sound to exist for the entirety of it being "played"
	unsigned char IN_AddSoundToQueue(IN_AudioSound* sound, const unsigned short delay);
	//Requires the Song to exist for the entirety of it being "played"
	unsigned char IN_AddSongToQueue(IN_AudioSong* song, const unsigned short delay);

	bool IN_RemoveQueueSoundFromID(const unsigned char SoundID);

	bool IN_PauseQueueSoundFromID(const unsigned char ID);
	bool IN_ResumeQueueSoundFromID(const unsigned char ID);


	//Needs to be called for audio to start/end/work in general. Time step is how "far" forward in time you want the audio to go, in ms.
	void IN_AudioIterate(const unsigned int TimeStepMS);

	//VISUAL OUTPUT
	//Basically just the screen. Uses a 4x2 character grid at the console's left. 
	//each character is 5x8, and so the console has access to a 20x16 screen.
	//Its not a lot of realestate all things considered. The parts not taken up by the screen can be used for normal text.
	//Uses bit packing to be very quite optimized. Screen buffer takes up 40 bytes.

#define IN_RENDERFLAG_INVERTED 0b00000001
#define IN_RENDERFLAG_FALSE_IS_TRANSPARENT 0b00000010
#define IN_RENDERFLAG_MIRRORX 0b00000100
#define IN_RENDERFLAG_MIRRORY 0b00001000

	class IN_Image
	{
		uint8_t* ImageSource = nullptr;
		uint8_t sizeX = 0;
		uint8_t SizeY = 0;
		uint8_t ImgFlags = 0;

	public:

		//Returns false if pixel is out of range
		bool GetImagePixel(unsigned char PosX, unsigned char PosY);
		//Returns true if setting was a success.
		bool SetImagePixel(bool PxVal, unsigned char PosX, unsigned char PosY);


		IN_Image(uint8_t ColCount, uint8_t RowCount, uint8_t bitsource[], uint8_t ImageFlags );

	};
	
	void IN_ClearScreenBuffer();
	void IN_SetBufferPixel(bool pixelval, unsigned char PosX, unsigned char PosY);
	void IN_GetBufferPixel( unsigned char PosX, unsigned char PosY);
	LiquidCrystal* IN_GetLCDObject();

	//Draw a sprite. Can be drawn inverted.
	void IN_DrawImage(IN_Image* Sprite,unsigned char ScreenX, unsigned char ScreenY, uint8_t RenderFlags);

	//draw a rectangle of on or off pixels
	void IN_DrawRect(bool Val, unsigned char Width, unsigned char Height, unsigned char PosX, unsigned char PosY);
	void IN_InvertScreenBuffer();
	//Set the console screen to look like that data for the screen.
	void IN_DisplayScreenBuffer();
	


	 //MUST be run at startup
	 void IN_IngotInitialize();

	 
	 //Basically freezes the entire console until the program ends
	 void IN_IngotAbort(const char* ErrLine1, const char* ErrLine2); 

#endif

