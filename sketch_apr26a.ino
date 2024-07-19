 #include <BitBool.h>
#include <OnewireKeypad.h>

#include <OnePinKeypad.h>

#include <LiquidCrystal_I2C.h>

#include <BitBool.h>
#include <SPI.h>
#include <MFRC522.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define buzzerPin 3
int card1Balance = 4000;
int card2Balance = 200;

#define num 7
char Data[num];
byte data_count = 0;

String num1, num2, card, card2;
int a, b;
char Key;
#define SS_PIN 10 // Define the pin connected to the SS (Slave Select) pin of the MFRC522 module
#define RST_PIN 9 // Define the pin connected to the RST (Reset) pin of the MFRC522 module

bool recharge = true;

MFRC522 mfrc522(SS_PIN, RST_PIN);

int state = 0;

char KEYS[] = {
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  '*', '0', '#', 'D'
};

OnewireKeypad <Print, 16 > KP2(Serial, KEYS, 4, 4, A0, 4700, 1000);

int selectedLocation = -1; // Variable to store selected location index

void setup() {

  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);

  KP2.SetKeypadVoltage(5.0);

  SPI.begin();
  mfrc522.PCD_Init();

  lcd.setCursor(0, 0);
  lcd.print(" Automatic Toll");
  lcd.setCursor(0, 1);
  lcd.print("Collection System");
  delay(3000);
  lcd.clear();
}

void loop()
{

  if (recharge == 0)
  {
    reCharge();
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("    Welcome");
    rfid();
    KeyPad();
  }
}

void rfid()
{
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  if (content.substring(1) == "E3 09 4F E4")
  {
    if (card1Balance >= 500)
    {
      if (selectedLocation != -1) {
        lcdPrint(selectedLocation); // Display the selected location
      }
      card1Balance = card1Balance - 500;
      lcd.setCursor(9, 1);
      lcd.print(card1Balance);
      delay(2000);
      lcd.clear();
      state = 1;
    }
    else
    {
      card = content.substring(1);
      LcdPrint();
      lcd.setCursor(9, 1);
      lcd.print(card1Balance);
      lcd.print(" Tk");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please Recharge");
      delay(1000);
      lcd.clear();
      state = 0;
    }
  }
  else if (content.substring(1) == "F0 C3 6D 10")
  {
    if (card2Balance >= 500)
    {
      if (selectedLocation != -1) {
        lcdPrint(selectedLocation); // Display the selected location
      }
      card2Balance = card2Balance - 500;
      lcd.setCursor(9, 1);
      lcd.print(card2Balance);
      delay(2000);
      lcd.clear();
      state = 1;
    }
    else
    {
      card = content.substring(1);
      LcdPrint();
      lcd.setCursor(9, 1);
      lcd.print(card2Balance);
      lcd.print(" Tk");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please Recharge");
      lcd.clear();
      delay(1000);
      state = 0;
    }
  }
  else   {
    digitalWrite(buzzerPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Unknown Vehicle");
    lcd.setCursor(0, 1);
    lcd.print("Access denied");
    delay(1500);
    lcd.clear();
    digitalWrite(buzzerPin, LOW);
  }
}

void KeyPad()
{
  byte KState = KP2.Key_State();

  if (KState == PRESSED)
  {
    Key = KP2.Getkey();
    if (Key)
    {
      if (Key == 'A')
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Recharging Mode.");
        lcd.setCursor(0, 1);
        lcd.print("Enter amount:");
        int rechargeAmount = 0;
        while (true)
        {
          byte KState = KP2.Key_State();
          if (KState == PRESSED)
          {
            Key = KP2.Getkey();
            if (Key >= '0' && Key <= '9')
            {
              lcd.print(Key);
              rechargeAmount = rechargeAmount * 10 + (Key - '0');
            }
            else if (Key == 'D')
            {
              if (card == "63 5B 9F 17")
              {
                card1Balance += rechargeAmount;
                lcd.clear();
                lcd.print("Recharge successful");
                lcd.setCursor(0, 1);
                lcd.print("New balance:");
                lcd.print(card1Balance);
                delay(2000);
                recharge = 1;
                break;
              }
              else if (card == "9D B9 BE 09")
              {
                card2Balance += rechargeAmount;
                lcd.clear();
                lcd.print("Recharge successful");
                lcd.setCursor(0, 1);
                lcd.print("New balance:");
                lcd.print(card2Balance);
                delay(2000);
                recharge = 1;
                break;
              }
            }
          }
        }
      }
      else if (Key == 'B')
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Select location:");
        lcd.setCursor(0, 1);
        lcd.print("1 2 3 4 5");
        delay(1500);
        lcd.clear();
        Key = 0; // Reset Key variable
        while (true)
        {
          byte KState = KP2.Key_State();
          if (KState == PRESSED)
          {
            Key = KP2.Getkey();
            if (Key >= '1' && Key <= '5')
            {
              selectedLocation = Key - '1'; // Convert key to index (0-4)
              lcdPrint(selectedLocation); // Display the selected location
              // Deduct payment logic here
              state = 1;
              recharge = 0;
              break;
            }
          }
        }
      }
    }
  }
}

void clearData()
{
  while (data_count != 0)
  {
    Data[data_count--] = 0;
  }
  return;
}

void reCharge()
{

  lcd.setCursor(0, 0);
  lcd.print ("Enter the amount");

  byte KState = KP2.Key_State();

  if (KState == PRESSED)
  {
    Key = KP2.Getkey();
    if (Key)
    {
      if (Key == 'D')
      {
        if (card == "63 5B 9F 17")
        {
          num1 = Data;
          card1Balance = num1.toInt() + card1Balance;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Your current");
          lcd.setCursor(0, 1);
          lcd.print("balance: ");
          lcd.setCursor(9, 1);
          lcd.print (card1Balance);
          lcd.print(" Tk");
          delay(3000);
          clearData();
          lcd.clear();
          recharge = 1;
        }
        else if (card == "9D B9 BE 09")
        {
          num2 = Data;
          card2Balance = num2.toInt() + card2Balance;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Your current");
          lcd.setCursor(0, 1);
          lcd.print("balance: ");
          lcd.setCursor(9, 1);
          lcd.print (card2Balance);
          lcd.print(" Tk");
          delay(3000);
          clearData();
          lcd.clear();
          recharge = 1;
        }
      }
      else
      {
        Data[data_count] = Key;
        lcd.setCursor(data_count, 1);
        lcd.print(Data[data_count]);
        data_count++;
      }
    }
  }
}

void lcdPrint(int location)
{
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Payment made at:");
  lcd.setCursor(0, 1);
  lcd.print("Location ");
  lcd.print(location + 1); // Display location number
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Your Remaining");
  lcd.setCursor(0, 1);
  lcd.print("balance: ");
}

void LcdPrint()
{
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Your balance");
  lcd.setCursor(0, 1);
  lcd.print(" is insufficient");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Your Remaining");
  lcd.setCursor(0, 1);
  lcd.print("balance: ");
}
