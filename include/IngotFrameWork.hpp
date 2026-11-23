#ifndef INGOTFRAMEWORK
#define INGOTFRAMEWORK

#include <stdint.h>
#include <LiquidCrystal.h>


//IngotFrameWork
//Very minimal, and as a result, pretty lightweight. There is little structure that
//needs to be followed.








	//USER INPUT
	//
	//These are just inputs the console can recieve. They're pretty self explanatory.
		

	//Returns true if MAINBUTTON is currently held down
	bool IN_MainButtonDown(void);
	//Returns true if SECONDARYBUTTON is currently held down
	bool IN_SecondaryButtonDown(void);

	//Returns True if the console is upright. (based off tilt ball sensor)
	bool IN_ConsoleUpright(void);

	//Returns a float between -1.0 and 1.0, representing the joysticks vertical orientation
	float IN_JoyStickVerticalPosition(void);
	//Returns a float between -1.0 and 1.0, representing the joysticks horizontal orientation
	float IN_JoyStickHorizontalPosition(void);





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
		IN_AudioSound* SongSource;
		

	} IN_AudioSong;


	



	


	//Pretty self explanatory; pauses all currently playing audio
	void IN_PauseAllAudio(void);
	//Resumes all audio (sounds + songs)
	void IN_ResumeAllAudio(void);
	//Cancels all currently playing audio (sounds + songs)
	void IN_ClearAllAudio(void);


	//Play a sound. Second parameter is for a delay for how many ms it waits before playing (In Ms) 
	//Returns an ID to the sound, which can be used for specifically altering that one sound.
	//Requires the Sound to exist for the entirety of it being "played"
	uint16_t IN_AddSoundToQueue(IN_AudioSound* sound, const unsigned short delay);

	//Play a song (collection of sounds). Second parameter is for a delay for how many ms it waits before playing (in Ms)
	//Returns an ID to the sound, which can be used for specifically altering that one sound.
	//Requires the Song to exist for the entirety of it being "played"
	uint16_t IN_AddSongToQueue(IN_AudioSong* song, const unsigned short delay);

	//Cancel a specific sound using its ID
	bool IN_RemoveQueueSoundFromID(const uint16_t SoundID);
	//Pause a specific sound using its ID
	bool IN_PauseQueueSoundFromID(const uint16_t SoundID);
	//Resume a specific sound using its ID
	bool IN_ResumeQueueSoundFromID(const uint16_t SoundID);


	//Cancel a specific song using its ID
	bool IN_RemoveQueueSongFromID(const uint16_t SongID);
	//Pause a specific song using its ID
	bool IN_PauseQueueSongFromID(const uint16_t SongID);
	//Resume a specific song using its ID
	bool IN_ResumeQueueSongFromID(const uint16_t SongID);

	//Needs to be called for audio to start/end/work in general. Time step is how "far" forward in time you want the audio to go, in ms.
	void IN_AudioIterate(const unsigned short TimeStepMS);




	//VISUAL OUTPUT
	//Basically just the screen. Uses a 4x2 character grid at the console's left. 
	//each character is 5x8, and so the console has access to a 20x16 screen.
	//Its not a lot of realestate all things considered. The parts not taken up by the screen can be used for normal text.
	//Uses bit packing to be very quite optimized. Screen buffer takes up 40 bytes.

	//Invert the colors, making true false and vice versa
#define IN_RF_INVERTED 0b00000001
	//Make false pixels count as transparent, essentially not drawing anything.
	//Think of it like disabling a white background.
#define IN_RF_FALSETRANSPARENT 0b00000010
	//Mirrors the Image to be right to left
#define IN_RF_MIRRORX 0b00000100
	//Mirros the Image to be down to up
#define IN_RF_MIRRORY 0b00001000

	//Use "IN_DrawImage" to render.
	class IN_Image
	{
		uint8_t* ImageSource = nullptr;
		uint8_t SizeX = 0;
		uint8_t SizeY = 0;

	public:

		//Returns false if pixel is out of range
		bool GetImagePixel(uint8_t PosX, uint8_t PosY);
		//Returns true if setting was a success.
		bool SetImagePixel(bool PxVal, uint8_t PosX, uint8_t PosY);

		//Horizontal Size of image, vertical size, and the image source. Image source should be an array written in binary notation, up to 64 digits.
		IN_Image(uint8_t SizeX, uint8_t SizeY, uint8_t bitsource[]);
		//Horizontal Size of image, vertical size, and the image source. Image source should be an array written in binary notation, up to 64 digits.
		IN_Image(uint8_t SizeX, uint8_t SizeY, uint16_t bitsource[]);
		//Horizontal Size of image, vertical size, and the image source. Image source should be an array written in binary notation, up to 64 digits.
		IN_Image(uint8_t SizeX, uint8_t SizeY, uint32_t bitsource[]);
		//Horizontal Size of image, vertical size, and the image source. Image source should be an array written in binary notation, up to 64 digits.
		IN_Image(uint8_t SizeX, uint8_t SizeY, uint64_t bitsource[]);


		friend void IN_DrawImage(IN_Image*, short , short, uint8_t);

	};
	//Ignore spacing makes it so that the gap in between characters is not treated as a pixel.
	void IN_SetIgnoreGapSpacing(bool);
	
	//Empties the Buffer used for the screen (Not the screen itself!)
	//Used for rendering.
	void IN_ClearScreenBuffer(void);
	//Set a specific pixel in the screen buffer to be true/false
	//returns false if setting the pixel fails
	bool IN_SetBufferPixel(bool pixelval, uint8_t PosX, uint8_t PosY);
	//Get the value of a specific pixel in the screen buffer
	bool IN_GetBufferPixel( uint8_t PosX, uint8_t PosY);

	//A direct pointer to the LCD object used for the screen.
	//Make sure you know what your doing, chap
	LiquidCrystal* IN_GetLCDObject(void);

	//Set the text of a specific line of the LCD. 0 Indexed
	void IN_SetLineText(const char*, uint8_t);

	//Draw a sprite. Drawing is modified by ORing the IN_RF flags.
	void IN_DrawImage(IN_Image* Sprite,short ScreenX, short ScreenY, uint8_t RenderFlags = 0);

	//draw a rectangle of on or off pixels
	void IN_DrawRect(bool Val, uint8_t Width, uint8_t Height, short PosX, short PosY);
	//Invert the pixels of the entire screen buffer, turning true to false and vice versa
	void IN_InvertScreenBuffer();
	//Update the screen to look like the Screen Buffer data.
	void IN_DisplayScreenBuffer();
	


	 //MUST be run at startup; needed for all major functions
	 void IN_IngotInitialize(void);

	 
	 //Basically freezes the entire console until the program ends. Use for errors
	 void IN_IngotAbort(const char* ErrLine1, const char* ErrLine2); 

#endif

