#include "utils.h"

void itoa3(int num, char* buffer) {
  const char digits[10] = "0123456789";
  if(num > 999) {
    buffer[0] = '9';
    buffer[1] = '9';
    buffer[2] = '9';
    return;
  } 
  buffer[0] = digits[num / 100];
  buffer[1] = digits[(num % 100) / 10];
  buffer[2] = digits[num % 10];

}

void itoa2(int num, char* buffer) {
  const char digits[10] = "0123456789";
  if(num > 99) {
    buffer[0] = '9';
    buffer[1] = '9';
    return;
  }
  buffer[0] = digits[num / 10];
  buffer[1] = digits[num % 10];
}


void format_seconds(int seconds, char* display_buffer){
  static char buffer[2];
  if (seconds > 3600){
    itoa2(seconds/3600, buffer);
    display_buffer[0] = buffer[0];
    display_buffer[1] = buffer[1];
    seconds = seconds % 3600;//removing the hours;
  }else{
    display_buffer[0] = '0';
    display_buffer[1] = '0';
  }
  display_buffer[2] = ':';
  if (seconds > 60){
    itoa2(seconds/60, buffer);
    display_buffer[3] = buffer[0];
    display_buffer[4] = buffer[1];
    seconds = seconds % 60;//removing the minutes;
  }else{
    display_buffer[3] = '0';
    display_buffer[4] = '0';
  }
  display_buffer[5] = ':';

  itoa2(seconds, buffer);
  display_buffer[6] = buffer[0];
  display_buffer[7] = buffer[1];
}
