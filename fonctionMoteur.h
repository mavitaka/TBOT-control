
#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_can.h"
#include "driverlib/can.h"


#define STATUS_OK 100
#define IDMoteur1 1
#define IDMoteur2 2
#define IDMoteur1_Moteur2 3
#define IDmicroControleur 9

#define TAILLE_DONNE_OCTETS 7
#define ID_MSGBOX_RET	5




struct composanteMoteur
{
    tCANMsgObject msgObjEnvoyer;
	unsigned char trameData[7];
	char identifiantMsgBox;

};
typedef struct composanteMoteur composanteMoteur;

enum TypeMouvement
{
    MOVE_ABSOLUTE=0,MOVE_RELATIVE=1,MOVE_COORDINATE=2
};
typedef enum TypeMouvement TypeMouvement;



enum NumCommande
{
    NextPosition=0,ActualPosition=1,MaximumPositionSpeed=4,SerialAdresse=66,SerialSecondaryAdress=87,MicroStepsResolution=140,Actual_Speed=3,MaxAcceleration=5

};
typedef enum NumCommande NumCommande;

enum Bank
{
	Coordinate0=0,Coordinate1=1,Coordinate2=2,Coordinate3=3,Coordinate4=4
	
	};
typedef enum Bank Bank	;

int VerificationCommande(char id_MsgBox);
void initMsgRetour();
void ConfigurationMoteur(int idCAN , composanteMoteur * moteur, int idMsgBox);
int32 Puissance(int32 base, int32 exposant);

void  ROR(composanteMoteur * moteur, int32 vitesse);
void  ROL(composanteMoteur * moteur, int32 vitesse);
void MST(composanteMoteur * moteur);
void MVP(composanteMoteur *moteur,int32  Valdeplacement, TypeMouvement Mouvement);

void SAP(composanteMoteur *moteur,NumCommande valCommande, int32 valeur);
int32 GAP(composanteMoteur *moteur,NumCommande valCommande);
void SGP(composanteMoteur *moteur,NumCommande valCommande,int32 valeur);
int32 GGP(composanteMoteur *moteur,NumCommande valCommande);
void SC0(composanteMoteur *moteur,Bank numBank, int32 valeur);
int32 GC0(composanteMoteur *moteur,Bank numBank);
void CC0(composanteMoteur *moteur,Bank numBank);

