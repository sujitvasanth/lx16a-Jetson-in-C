#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

void send_command(int fd, unsigned char *command, int len){
    unsigned char tx_buffer[20];unsigned char sum = 0;
    tx_buffer[0] = tx_buffer[1] = 0x55;
    for (int i = 0; i < len+1; i++){
        tx_buffer[i + 2] = command[i];
        sum += tx_buffer[i+2];}
    tx_buffer[len + 2] = (~sum) & 0xFF;
    for (int i = 0; i < len+3; i++) {printf("%02x ", tx_buffer[i]);}
    write(fd, tx_buffer, len + 3);}

void move_servo(int fd, int id, int position, int time_ms){
    unsigned char command[7] = {0x06, 0x07, 0x01,
        (unsigned char)((position >> 0) & 0xff),
        (unsigned char)((position >> 8) & 0xff),0x00, 0x00};
    send_command(fd, command, 7);}

int main(){
    int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {printf("Error opening UART.\n");return 1;}
    struct termios settings = {};
    if (tcgetattr(fd, &settings) < 0 || cfsetospeed(&settings, B115200) < 0 ||
        (settings.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS), settings.c_cflag |= CS8 | CREAD | CLOCAL) < 0 ||
        (settings.c_iflag &= ~(IXON | IXOFF | IXANY | ICANON | ECHO | ECHOE | ISIG), settings.c_cc[VMIN] = 3, settings.c_cc[VTIME] = 0) < 0 ||
        tcsetattr(fd, TCSANOW, &settings) < 0) {printf("Error configuring UART.\n");close(fd);return 1;}
    while(true) {
        move_servo(fd, 6, 500, 0);
        printf("Press enter to move\n");getchar();
        move_servo(fd, 6, 800, 0);
        printf("Press enter to move\n");getchar();}
    close(fd);return 0;}