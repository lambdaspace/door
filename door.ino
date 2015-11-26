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
//members card
long keys[] = { 1234567, 1234568, 1234569 };

int unknown_card = 0;
bool success = false;

void loop()
{
	//keep the door locked
	digitalWrite(5, LOW);

	//if a card is read
	if(wg.available())
	{
		long card = wg.getCode();
		int i;
		//if the code matches to any members card
		for(i = 0; i < sizeof(keys); i++)
		{
			if(keys[i] == card)
			{
				Serial.print("Welcome: ");
				Serial.println(card);
				digitalWrite(5, HIGH);
				//wait 2 seconds before lock the door
				delay(2000);
				success=true;
				unknown_card=0;
				break;
			}
		}
		// if card is unknown
		if(success == false)
		{
			unknown_card++;
			Serial.print("Unknown:");
			Serial.println(card);
			//delay depends on the number of times the card is swiped
			switch (unknown_card)
			{
				case 1:
					delay(1000);
					break;
				case 2:
					delay(1000);
					break;
				case 3:
					Serial.print("WARNING! Intrusion detected: 3\n");
					delay(1000);
					break;
				case 4:
					Serial.print("WARNING! Intrusion detected: 4\n");
					delay(2000);
					break;
				case 5:
					Serial.print("WARNING! Intrusion detected: 5\n");
					delay(2000);
					break;
				case 6:
					Serial.print("WARNING! Intrusion detected: 6\n");
					delay(3000);
					break;
				case 7:
					Serial.print("WARNING! Intrusion detected: 7\n");
					break;
					delay(5000);
				default:
					Serial.print("WARNING! Intrusion detected: default\n");
					delay(10000);
					break;
			}
		}
	}
}
