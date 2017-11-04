#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

void main(void)
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

  char write_buffer[] = "111";
  int bytes_written = 0;

  bytes_written = write(fd, write_buffer, sizeof(write_buffer));

  printf("%s written to serial port\n", write_buffer);
  printf("%d bytes written to serial port\n", bytes_written);

  close(fd);

}


