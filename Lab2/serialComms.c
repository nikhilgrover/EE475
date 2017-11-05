#include <stdio.h>
#include <fcntl.h> 
#include <termios.h> 
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  char commandKey = '?';
  char data[2];
  while(commandKey != '0') {
    printf("Type in one of the following commands:\n 0: Quit\n 1: Initiate a scan\n 2: Terminate scan\n 3: Grab the last reading from a particular system\n 4: Grab the last 16 readings from a particular system\n");
    commandKey = '?';

    while(commandKey < '0' || commandKey > '5') {
      scanf("%c", &commandKey);
    }

    if(commandKey == '1'){
      printf("Enter the address of the system you'd like to scan:\n");
      data[0] = "0x0";
      scanf("%c", data[1]);
      serialWrite(data);
    }
    else if(commandKey == '2') {
      printf("Enter the address of the system:\n");
      data[0] = "0x1";
      scanf("%c", data[1]);
      serialWrite(data);
    }
    else if(commandKey == '3') {
      printf("Enter the address of the system you'd like to read:\n");
      data[0] = "0x2";
      scanf("%c", data[1]);
      serialWrite(data);
      //add some delay here
      char[] returnData;
      returnData = serialRead();
      // Process data into floats and print
    }
    else if(commandKey == '4') {
      printf("Enter the address of the system you'd like to read:\n");
      data[0] = "0x3";
      scanf("%c", data[1]);
      serialWrite(data);
      //delay
      char[] returnData;
      returnData = serialRead();
      //Process data into floats and print
    }
  }
}

char[] serialRead()
{
  int fd; // File descriptor
  printf("\nOpening serial port \n");

  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);

  if(fd == -1)
    printf("\nError in opening serial port \n");
  else
    printf("\nSerial port opened \n");

  struct termios SerialPortSettings;
  tcgetattr(fd, &SerialPortSettings);

  // Set baud rate
  cfsetispeed(&SerialPortSettings, B9600); // Read
  cfsetospeed(&SerialPortSettings, B9600); // Write

  // 8N1 Mode
  SerialPortSettings.c_cflag &= ~PARENB;
  SerialPortSettings.c_cflag &= ~CSTOPB;
  SerialPortSettings.c_cflag &= ~CSIZE;
  SerialPortSettings.c_cflag |= CS8;

  SerialPortSettings.c_cflag &= ~CRTSCTS;
  SerialPortSettings.c_cflag |= CREAD | CLOCAL;

  SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);
  SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

  SerialPortSettings.c_oflag &= ~OPOST;

  // Set time outs SEE LINK ON VMIN AND VTIME
  SerialPortSettings.c_cc[VMIN] = 0; // Read this many characters
  SerialPortSettings.c_cc[VTIME] = 10; // Wait indefinitely 

  if((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0)
    printf("ERROR in setting attributes\n");
  else
    printf("Baude rate = 9600, StopBits = 1, Parity = none\n");

  // Read data from serial port

  tcflush(fd, TCIFLUSH);
  char read_buffer[3];
  int bytes_read = 0;
  int i = 0;

  bytes_read = read(fd, &read_buffer, 3);

  printf("\n\n Bytes read: %d", bytes_read);
  printf("\n\n");

  for(i = 0; i < bytes_read; i++)
    printf("%c", read_buffer[i]);

  close(fd);
  printf("Serial port closed\n");

  return read_buffer;

}

void serialWrite(char[] write_buffer)
{
  int fd;
  
  printf("Opening serial port\n");

  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  
  if(fd == -1)
    printf("Error in opening serial port\n");
  else
    printf("Serial port opened\n");

  // Set attributes of serial port
  
  struct termios SerialPortSettings;

  tcgetattr(fd, &SerialPortSettings);

  cfsetispeed(&SerialPortSettings, B9600);
  cfsetospeed(&SerialPortSettings, B9600);
  
  SerialPortSettings.c_cflag &= ~PARENB;
  SerialPortSettings.c_cflag &= ~CSTOPB;
  SerialPortSettings.c_cflag &= ~CSIZE;
  SerialPortSettings.c_cflag |= CS8;

  SerialPortSettings.c_cflag &= ~CRTSCTS;
  SerialPortSettings.c_cflag |= CREAD | CLOCAL;

  SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);
  SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);

  SerialPortSettings.c_oflag &= ~OPOST;

  if((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0)
    printf("Error in setting attributes\n");
  else
    printf("Baude rate = 9600\n Stop bits = 1\n Parity = none\n");

  // char write_buffer[] = "111";
  int bytes_written = 0;

  bytes_written = write(fd, write_buffer, sizeof(write_buffer));

  printf("%s written to serial port\n", write_buffer);
  printf("%d bytes written to serial port\n", bytes_written);

  close(fd);

}
