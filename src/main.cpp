#include <Arduino.h>
#include <IngotFrameWork.hpp>

IN_AudioSound sound = {100, 3000};

void setup()
{

	Serial.begin(9600);
	IN_IngotInitialize();

	IN_AddSoundToQueue(&sound, 2000);
}



void loop()
{

	delay(40);
	IN_AudioIterate(40);
	//Serial.println("Iteration");
	
}
