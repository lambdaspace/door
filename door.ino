#include <Wiegand.h>

//Initializing wiegand protocol for card reading
WIEGAND wg;

//Variables for pins
int doorlock = 5;
int doorsensor = 6; //if sensor not present, connect to ground
int doorbutton = 7; //if sensor not present, connect to ground

//Global Variables
//members card - hardcoded
long keys[] = { 1234567, 1234568, 1234569 };
//counter for invalid card reads
int unknown_card_counter = 0; 
boolean dooropenorclosed = LOW;

void setup()
{
	//intializing the serial port
	Serial.begin(9600);
	// the relay where the door's electric strike is connected
	pinMode(doorlock, OUTPUT);
	pinMode(doorsensor, INPUT);
	pinMode(doorbutton, INPUT);
	//initializing the wg class
	wg.begin();
}


void unlockdoor()
{
	digitalWrite(doorlock, HIGH);
	//wait 2 seconds before lock the door
	delay(2000);
	digitalWrite(doorlock, LOW);
	return;
}

void checkmanualopenclose()
{
	boolean temp=digitalRead(doorsensor);
	if (dooropenorclosed!=temp){
		if (temp) Serial.println("Manual Open"); //might need to switch these
		else Serial.println("Manual Close");
		dooropenorclosed=temp;
		delay(150); //sort of debounce
	}
	return;
}

void checkbutton()
{
	while (doorbutton) unlockdoor();
	return;
}

void loop()
{
	//keep the door locked - also a failsafe
	digitalWrite(doorlock, LOW);
	
	checkmanualopenclose();
	checkbutton();
	
	//small delay per loop, so that the wiegand check for card will not occur continuously while on standby
	//this is for power consumption purposes
	delay(100);
	//if a card is read
	if(wg.available())
	{
		//get code of card that was read
		long card = wg.getCode();
		int i;
		//if the code matches to any members card
		for(i = 0; i < sizeof(keys); i++) // for each key in the table of keys
		{
			if(keys[i] == card) //if this key matches 
			{
				Serial.println("\nOpened:"); // record keeping
				Serial.println(card);
				unlockdoor();
				unknown_card_counter=0; 
				//since we found a valid key, return to the beginning of the loop()
				return;
			}
		}
		
		// if there was no "return", the card we read was unknown.
		// from here on, execute the intrusion detection code
		
		unknown_card_counter++;
		//record keeping
		Serial.println("Bad Key:");
		Serial.println(card);
		Serial.println("Consecutive:");
		Serial.println(unknown_card_counter);
		
		//delay depends on the number of times the card is swiped.
		//exponential increase of delay for each wrong swipe. 
		//delaytime = 1000+66*2^unknown_card_counter - this returns values close to the old switch case
		int delaytime=1<<unknown_card_counter; // delaytime = 2^unknown_card_counter
		delaytime*=66;	//delaytime = delaytime * 66
		delaytime+=1000; //delaytime = delaytime + 1000
		delay(delaytime);
	}
}
