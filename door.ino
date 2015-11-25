#include <Wiegand.h>

// initializing wiegand protocol
WIEGAND wg;

void setup()
{
	//intializing the serial port
	Serial.begin(9600);
	// the relay where the electric strike is connected
	pinMode(5, OUTPUT);
	//initializing the wg class
	wg.begin();
	int i;
	long card;

}
//claire's card
long keys[] = { 1234567, 1234568, 1234569 };


void loop()
{
	//keep the door locked
	digitalWrite(5, LOW);

	//if a card is read
	if(wg.available())
	{
		long card = wg.getCode();
		int i;
		Serial.print(wg.getCode());
		//if the code matches to claires card
		for(i = 0; i < sizeof(keys); i++)
		{
			if(keys[i] == card)
			{
				Serial.print("Welcome: ");
        Serial.println(card);
				digitalWrite(5, HIGH);
				//wait 2 seconds before lock the door
				delay(2000);
				break;
			}
      Serial.print("Unknown:")
      Serial.println(card)
		}
	}
}
