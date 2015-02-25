#include "Arduino.h"
#include "SL015M.h"

SL015M::SL015M()
{
  card = 0;
  rfcmdsent = false;
  rf_i = 0;
  pinset = false;
  readcmd = false;
  lcd_enabled = false;
}

void SL015M::setPins(int sensor, HardwareSerial* serialid)
{
    rfSENSOR = sensor;
    rfSerial = serialid;
	
    pinMode(rfSENSOR,INPUT);                            // Rfid sensor
    rfSerial->begin(RFID_BAUD);
    
    pinset=true;

}

void SL015M::update()
{
  char lcdline[17];
  char hex[2];
  
  if(!pinset)
    return;
  if(!readcmd)
    return;
  
    if(digitalRead(rfSENSOR)==LOW)             // card presence
    {
        
        if(!rfcmdsent)
        {
        
          rfSerial->write(0xBA);                  // pream
          rfSerial->write(0x02);                  // size
          rfSerial->write(0x31);                  // rfidcommand: get tag info
          rfSerial->write(0xBA ^ 0x02 ^ 0x31);    // checksum
          
          rf_i = 0;
          rfcmdsent= true;
          timesent = millis();
        }
        else // if command has been sent, let's read answer
        {

          if(rfSerial->available()>0)
          {
            
            rfidp[rf_i] = rfSerial->read();
            rf_i++;
            
            if(rfidp[0] != 0xBD)                 // restart filling buffer from begining until received byte is 0xBD (preamb)
              rf_i = 0;
            
            if(rf_i>2)                           // if we know paquet length
            {
              
              byte length = rfidp[1];
              if(rf_i >= length + 2)             // if paquet complete
              {
          
            // checksum check
                byte chk = 0x00; 
                for(int i=0;i< length+1;i++)
                  chk ^= rfidp[i];
                
                if(chk != rfidp[length+1])
                {
                  rfcmdsent = false;             // checksum error, send cmd again
                }
                else
                {
                  if(rfidp[3]!=0x00)             // Operation not succeeded
                  {
                    rfcmdsent = false;           // error, send again
                  }
                  else
                  {
            
            //success, let's copy the uid
                    for(int i=0;i<8;i++)
                    {
                      uid[i] = rfidp[11-i];
                      if (i == 0) {
                          sprintf(lcdline,"%02X",uid[i]);
                       } else {
                          sprintf(hex,"%02X",uid[i]);
                          strcat(lcdline,hex);
                       }
                    }
            
                    if(uid[0] == 0xE0 && uid[1] == 0x04)  // if correct konami card
                      card = 1;
                    else
                      card = 0;
                      
                    readcmd = false;//reading finished (card found)
                    
                    if (lcd_enabled) {
                       if (lcd_rows == 4) {
                         lcd->setCursor(0,1);
                         char line2[17];
                         sprintf(line2,"P%d ISO15693     ",uid);
                         lcd->print(line2);
             
                         lcd->setCursor(-4,2);
                         lcd->print("e-AMUSEMENT PASS");
                       }
          
                     if (LCD_STATUSLINE > 1) {
                       lcd->setCursor(-4,LCD_STATUSLINE);
                     } else {
                       lcd->setCursor(0,LCD_STATUSLINE);
                     }
                     lcd->print(lcdline);
                    }

                  }
                }
              }
            }
          }
          else 
          {
            // command sent, but nothing to receive
            if(millis() - timesent >500)         // if last command was sent more than 500ms ago, send a new one
              rfcmdsent = false;
          }
        }
    }
    else // if no tag in range
    {
        card = 0;
        rfcmdsent = false;
        readcmd = false;//reading finished (no card found)
    }
}

byte SL015M::isCardPresent()
{
	return card;
}

void SL015M::read()
{
  readcmd=true;
}

void SL015M::getUID(byte* uida)
{
	memcpy(uida,uid,8);
}

void SL015M::setReaderNumber(int reader)
{
    readerNumber = reader;
}

void SL015M::setLcd(LiquidCrystal *passed_lcd, int passed_lcd_rows, int passed_lcd_statusline)
{
    lcd = passed_lcd;
    lcd_rows = passed_lcd_rows;
    lcd_statusline = passed_lcd_statusline;
    lcd_enabled = true;
}

