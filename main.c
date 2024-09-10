#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h> //for IOCTL defs
#include <fcntl.h>


#define I2C_FICHIER "/dev/i2c-1" // fichier Linux representant le BUS #2
#define I2C_ADRESSE 0x29 // adresse du Device I2C MPU-9250 (motion tracking)
#define NOMBRE_MAX 22

//définitions privée
typedef struct
{
	uint16_t adresse;
	uint8_t valeur;
} TABLEAU_REGISTRE;


int main()
{


	uint8_t DataRegistre;

	TABLEAU_REGISTRE ucTabRegister[NOMBRE_MAX] = {
	{0x0207, 0x01}, {0x0208, 0x01}, {0x0133, 0x01}, {0x0096, 0x00}, {0x0097, 0xFD},
	{0x00e3, 0x00}, {0x00e4, 0x04}, {0x00e5, 0x02}, {0x00e6, 0x01}, {0x00e7, 0x03},
	{0x00f5, 0x02}, {0x00D9, 0x05}, {0x00DB, 0xCE}, {0x00DC, 0x03}, {0x00DD, 0xF8},
	{0x009f, 0x00}, {0x00a3, 0x3c}, {0x00b7, 0x00}, {0x00bb, 0x3c}, {0x00b2, 0x09},
	{0x00ca, 0x09},	{0x0198, 0x01}};

	int fdPortI2C;  // file descriptor I2C

	// Initialisation du port I2C, 
	fdPortI2C = open(I2C_FICHIER, O_RDWR); // ouverture du 'fichier', création d'un 'file descriptor' vers le port I2C
	if(fdPortI2C == -1)
	{
		printf("erreur: I2C initialisation step 1\n");
		return -1;
	}
	if(ioctl(fdPortI2C, I2C_SLAVE_FORCE, I2C_ADRESSE) < 0)  // I2C_SLAVE_FORCE if it is already in use by a driver (i2cdetect : UU)
	{
		printf("erreur: I2C initialisation step 2\n");
		close(fdPortI2C);
		return -1;
	}
	
	// Écriture et Lecture sur le port I2C
	uint8_t Message[3];

	uint8_t Registre[3];
	
	uint16_t Source = 0x0018; // registre d'ID du chip I2C VL6180X
	uint8_t Destination;
	uint8_t NombreDOctetsALire = 1;
	uint8_t NombreDOctetsAEcrire = 2;


	// Registre 16 bits
	Registre[0] = (Source >> 8);
	Registre[1] = (Source & 0xFF);
	DataRegistre = 0x01;
	Registre[2] = DataRegistre;
	// Settings pour la lecture distance
	write(fdPortI2C,&Registre,3);


	for (int index = 0; index < NOMBRE_MAX; index++)
	{
		Message[0] = (ucTabRegister[index].adresse >> 8); 
		Message[1] = (ucTabRegister[index].adresse & 0xFF); 
		Message[2] = ucTabRegister[index].valeur;
		write(fdPortI2C,&Message,3);
	}













/*

	if(write(fdPortI2C, &Registre, NombreDOctetsAEcrire) != NombreDOctetsAEcrire)
	{
		printf("erreur: Écriture I2C\n");
		close(fdPortI2C);
		return -1;
	}
	if(read(fdPortI2C, &Destination, NombreDOctetsALire) != NombreDOctetsALire)
	{
		printf("erreur: Lecture I2C\n");
		close(fdPortI2C);
		return -1;
	}
*/

	//
	Source = 0x062;
	Registre[0] = (Source >> 8);
	Registre[1] = (Source & 0xFF);

	write(fdPortI2C,&Registre,2);
	while(1)
	{
	read(fdPortI2C,&Destination,1);


	printf("Distance: %#04x\n\r", Destination);
	}
}








