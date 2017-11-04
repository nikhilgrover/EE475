#include <stdio.h>
#include <fcntl.h> // File control
#include <termios.h> // POSIX terminal control
#include <unistd.h>
#include <errno.h>

void main(void)
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

  //cfmakeraw(&SerialPortSettings);

  // Set time outs
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

}
