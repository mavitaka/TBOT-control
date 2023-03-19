#include "fonctionMoteur.h"	
#include "inc/hw_can.h"
#include "driverlib/can.h"


/*
 *Note Importante le bitRate du mcu doit etre 2*plus grande que celle du moteur
*/


/* de quoi j'ai besoin pour envoyer une trame ?
-de l'identifiant
-la taille de donner est fixe
-pas de flag particulier
-un pointeur sur la trame de donne

struct composanteMoteur
{
	tCANMsgObject msgObjEnvoyer;
	unsigned char trameData[7];
	char identifiantMsgBox;

};

*/
tCANMsgObject MsgObjRetour;
unsigned char TrameRetour[7];



/**
 * @brief      { Initialise l'objet moteur}
 *
 * @param[in]  idCAN     l'identifiant can
 * @param      moteur    Structure moteur
 * @param[in]  idMsgBox  The identifier message box
 */ 
void ConfigurationMoteur(int idCAN , composanteMoteur * moteur, int idMsgBox)
{
	int i;

	moteur->msgObjEnvoyer.ui32MsgID=idCAN;
	moteur->msgObjEnvoyer.ui32MsgIDMask=0;
	moteur->msgObjEnvoyer.ui32Flags=MSG_OBJ_NO_FLAGS;
	moteur->msgObjEnvoyer.pucMsgData=moteur->trameData;
	moteur->msgObjEnvoyer.ui32MsgLen=TAILLE_DONNE_OCTETS;

	moteur->identifiantMsgBox=idMsgBox;

	for(i=0;i<7;i++)
		moteur->trameData[i]=0;

}

/**
 * @brief      { initMsgRetour() }
 * initialise le message objet permettant le contrôle de validiter des fonctions
 */

void initMsgRetour()
{
	MsgObjRetour.ui32MsgID=IDmicroControleur;
	MsgObjRetour.ui32MsgIDMask=0;
	MsgObjRetour.pucMsgData=TrameRetour;
	MsgObjRetour.ui32MsgLen=7;

	CANMessageSet(CANA_BASE,ID_MSGBOX_RET,&(MsgObjRetour),MSG_OBJ_TYPE_RX);
}



/**
 * @brief      { Verifie le bon traitement d'une commande }
 *
 * @param[in]  id_MsgBox  l'identifiant de la message Box du microcontroleur ( != identifiant CAN)
 *
 * @return     { 1 commande c est bien passer
 * 				-1 une erreur commande 	 }
 */
int VerificationCommande(char id_MsgBox)
{
	
	int32 status=0;
	int32 Location= ID_MSGBOX_RET-1;
	int32 res=Puissance(2,Location);
	
	do
	{
		status=CANStatusGet(CANA_BASE,CAN_STS_NEWDAT);
	}
	while(((status & 1<<Location )!= res));
		

		CANMessageGet(CANA_BASE,ID_MSGBOX_RET,&MsgObjRetour,true);
	if(TrameRetour[1]==STATUS_OK)
	{
		return 1;
	}
	else
	{
		return -1;
	}


}

void CANRTdata(composanteMoteur * motor, int commande, int type, int motorNumber, int32 value)
{
    motor->trameData[0]=(commande&0xFF);
    motor->trameData[1]=(type&0xFF);
    motor->trameData[2]=(motorNumber&0xFF);
    motor->trameData[3]=(value & 0xff000000)>>24;
    motor->trameData[4]=(value&0x00FF0000)>>16;
    motor->trameData[5]=(value&0x0000FF00)>>8;
    motor->trameData[6]=(value&0x000000FF);

    CANMessageSet(CANA_BASE,motor->identifiantMsgBox,&(motor->msgObjEnvoyer),MSG_OBJ_TYPE_TX);
}

void TQL(composanteMoteur * moteur, int commande , int32 vitesse)
 {

     moteur->trameData[0]=(commande&0xFF);
     moteur->trameData[1]=0x00;
     moteur->trameData[2]=0x00;
     moteur->trameData[3]=(vitesse & 0xff000000)>>24;
     moteur->trameData[4]=(vitesse&0x00FF0000)>>16;
     moteur->trameData[5]=(vitesse&0x0000FF00)>>8;
     moteur->trameData[6]=(vitesse&0x000000FF);

     // possibilité de l'initialiser qu'une seul fois au début du programme

     //moteur->msgObjEnvoyer.pucMsgData=moteur->trameData;

     CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);

     /*
     section de code à tester
     verification de la bonne recepetion de la trame

     VerificationCommande(ID_MSGBOX_RET)
     */

 }

/**
 * @brief      { Rotate Right : rotation du moteur à droite}
 *
 * @param      moteur   The moteur
 * @param[in]  vitesse  The vitesse
 */
void  ROR(composanteMoteur * moteur, int32 vitesse)
{

	moteur->trameData[0]=1;
	moteur->trameData[1]=0x00;
	moteur->trameData[2]=0x00;
	moteur->trameData[3]=(vitesse & 0xff000000)>>24;
	moteur->trameData[4]=(vitesse&0x00FF0000)>>16;
	moteur->trameData[5]=(vitesse&0x0000FF00)>>8;
	moteur->trameData[6]=(vitesse&0x000000FF);
	
	// possibilité de l'initialiser qu'une seul fois au début du programme 
	
	//moteur->msgObjEnvoyer.pucMsgData=moteur->trameData;

	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);
	
	/*
	section de code à tester 
	verification de la bonne recepetion de la trame

	VerificationCommande(ID_MSGBOX_RET)
	*/
	
}




/**
 * @brief      { Rotate Left : rotation du moteur à gauche }
 *
 * @param      moteur   The moteur
 * @param[in]  vitesse  The vitesse
 */
void  ROL(composanteMoteur * moteur, int32 vitesse)
{

	moteur->trameData[0]=0x02;
	moteur->trameData[1]=0x00;
	moteur->trameData[2]=0x00;
	moteur->trameData[3]=(vitesse & 0xff000000)>>24;
	moteur->trameData[4]=(vitesse&0x00FF0000)>>16;
	moteur->trameData[5]=(vitesse&0x0000FF00)>>8;
	moteur->trameData[6]=(vitesse&0x000000FF);
	// possibilité de l'initialiser qu'une seul fois au début du programme 

	//envoi de la trame
	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);
	
	/*
	section de code à tester 
	verification de la bonne recepetion de la trame

	VerificationCommande(ID_MSGBOX_RET)
	*/
	
}

/**
 * @brief      { Moteur Stop : stop le moteur }
 *
 * @param      moteur  The moteur
 */
void MST(composanteMoteur * moteur)
{

	moteur->trameData[0]=0x03;
	moteur->trameData[1]=0;
	moteur->trameData[2]=0;
	moteur->trameData[3]=0;
	moteur->trameData[4]=0;
	moteur->trameData[5]=0;
	moteur->trameData[6]=0;

	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);

	/*
	section de code à tester 
	verification de la bonne recepetion de la trame

	VerificationCommande(ID_MSGBOX_RET)
	*/

}

/**
 * @brief      { Move to position : se déplace à une position en utilisant un type de mouvement }
 *
 * @param      moteur          The moteur
 * @param[in]  Valdeplacement  la valeur de déplacement 
 * @param[in]  Mouvement       le type de mouvement 
 */
void MVP(composanteMoteur *moteur,int32  Valdeplacement, TypeMouvement Mouvement)
{
	switch(Mouvement)
	{
		case MOVE_ABSOLUTE:
		moteur->trameData[1]=0;
		break;

		case MOVE_RELATIVE:
		moteur->trameData[1]=1;
		break;

		case MOVE_COORDINATE:
		moteur->trameData[1]=2;
		break;

		default:
		moteur->trameData[1]=0xff;
		break;	
	}
	moteur->trameData[0]=4;
	moteur->trameData[2]=0;
	moteur->trameData[3]=(Valdeplacement&0xff000000)>>24;
	moteur->trameData[4]=(Valdeplacement&0x00FF0000)>>16;
	moteur->trameData[5]=(Valdeplacement&0x0000FF00)>>8;
	moteur->trameData[6]=(Valdeplacement&0x000000FF);

	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);
	/*
	section de code à tester 
	verification de la bonne recepetion de la trame

	VerificationCommande(ID_MSGBOX_RET)
	*/
}

/**
 * @brief      { SAP : Set Axis Parameter  permet de fixer (set) la valeur d un parametre du moteur}
 *
 * @param      moteur       The moteur
 * @param[in]  valCommande  The value commande
 * @param[in]  valeur       The valeur
 */
void SAP(composanteMoteur *moteur,NumCommande valCommande, int32 valeur)
{
	moteur->trameData[0]=5;
	moteur->trameData[1]=valCommande;
	moteur->trameData[2]=0;
	moteur->trameData[3]=(valeur&0xff000000)>>24;
	moteur->trameData[4]=(valeur&0x00FF0000)>>16;
	moteur->trameData[5]=(valeur&0x0000FF00)>>8;
	moteur->trameData[6]=(valeur&0x000000FF);

	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);
	/*
	section de code à tester 
	verification de la bonne recepetion de la trame

	VerificationCommande(ID_MSGBOX_RET)
	*/	


}
/**
 * @brief      { SGP : Set Global Parameter met à jour (set) les parametre instraseque du microcontroleur du moteur }
 *
 * @param      moteur       The moteur
 * @param[in]  valCommande  The value commande
 * @param[in]  valeur       The valeur
 */
void SGP(composanteMoteur *moteur,NumCommande valCommande,int32 valeur)
{
	moteur->trameData[0]=9;
	moteur->trameData[1]=valCommande;
	moteur->trameData[2]=0;
	moteur->trameData[3]=(valeur&0xff000000)>>24;
	moteur->trameData[4]=(valeur&0x00FF0000)>>16;
	moteur->trameData[5]=(valeur&0x0000FF00)>>8;
	moteur->trameData[6]=(valeur&0x000000FF);

	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);
	/*
	section de code à tester 
	verification de la bonne recepetion de la trame

	VerificationCommande(ID_MSGBOX_RET)
	*/	


}
void SC0(composanteMoteur *moteur,Bank numBank, int32 valeur)
{
	moteur->trameData[0]=0x1e;
	moteur->trameData[1]=numBank;
	moteur->trameData[2]=0;
	moteur->trameData[3]=(valeur&0xff000000)>>24;
	moteur->trameData[4]=(valeur&0x00FF0000)>>16;
	moteur->trameData[5]=(valeur&0x0000FF00)>>8;
	moteur->trameData[6]=(valeur&0x000000FF);	

	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);
	/*
	section de code à tester 
	verification de la bonne recepetion de la trame

	VerificationCommande(ID_MSGBOX_RET)
	*/	
}

/**
 * @brief      { CC0 Capture Coordinate 0 : capture les coordonnées actuel du moteur et les stock à la bank numBank }
 *
 * @param      moteur   The moteur
 * @param[in]  numBank  The number bank
 */
void CC0(composanteMoteur *moteur,Bank numBank)
{
	moteur->trameData[0]=0x20;
	moteur->trameData[1]=numBank;
	moteur->trameData[2]=0;
	moteur->trameData[3]=0;
	moteur->trameData[4]=0;
	moteur->trameData[5]=0;
	moteur->trameData[6]=0;

	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);
	/*
	section de code à tester 
	verification de la bonne recepetion de la trame

	VerificationCommande(ID_MSGBOX_RET)
	*/	
}

/**
 * @brief      { Get Axis Parameter permet d'obtenir les valeurs de l'axe  }
 *
 * @param      moteur       The moteur
 * @param[in]  valCommande  The value commande
 *
 * @return     { - La valeur demande 
 * 				 - -1 en cas d'erreur}
 */
int32 GAP(composanteMoteur *moteur,NumCommande valCommande)
{

	
	int32 retourfct=0;
	int32 status=0;
	int32 Location= ID_MSGBOX_RET-1;
	int32 res=Puissance(2,ID_MSGBOX_RET-1);
	int32 inter=0;


	moteur->trameData[0]=6;
	moteur->trameData[1]=valCommande;
	moteur->trameData[2]=0;
	moteur->trameData[3]=0;
	moteur->trameData[4]=0;
	moteur->trameData[5]=0;
	moteur->trameData[6]=0;

	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);

	do
	{
		status=CANStatusGet(CANA_BASE,CAN_STS_NEWDAT);
	}
	while(((status & 1<<Location )!= res));
		

	//ne pas oublier de mettre une fonction init qui initaliser le MsgObjRetour
		CANMessageGet(CANA_BASE,ID_MSGBOX_RET,&MsgObjRetour,true);
	if(TrameRetour[1]==STATUS_OK)
	{
		//tout est ok 
		//pas souci de comptatibilité il faut déclarer une variable supplémentaire de 4 octets pour faire le traitement 
		inter=TrameRetour[3];
		retourfct=inter<<24;
		inter=0;
		inter=TrameRetour[4];
		retourfct|=inter<<16;
		inter=0;
		inter=TrameRetour[5];
		retourfct|=inter<<8;
		retourfct|=TrameRetour[6];
	}
	else
	{
		retourfct=-1;
	}

	return retourfct;
}

/**
 * @brief      { Get Global parameter permet d'obtenir les valeurs intrasèque du microcontroleur du moteur }
 *
 * @param      moteur       The moteur
 * @param[in]  valCommande  The value commande
 *
 * @return     { - La valeur demande 
 * 				 - -1 en cas d'erreur}
 */
int32 GGP(composanteMoteur *moteur,NumCommande valCommande)
{

	int32 retourfct=0;
	int32 status=0;
	int32 Location= ID_MSGBOX_RET-1;
	int32 res=Puissance(2,Location);
	int32 inter=0;

	moteur->trameData[0]=10;
	moteur->trameData[1]=valCommande;
	moteur->trameData[2]=0;
	moteur->trameData[3]=0;
	moteur->trameData[4]=0;
	moteur->trameData[5]=0;
	moteur->trameData[6]=0;

	//envoi de la trame
	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);

	do
	{
		status=CANStatusGet(CANA_BASE,CAN_STS_NEWDAT);
	}
	while(((status & 1<<Location )!= res));
		

	//ne pas oublier de mettre une fonction init qui initaliser le MsgObjRetour
		CANMessageGet(CANA_BASE,ID_MSGBOX_RET,&MsgObjRetour,true);
	if(TrameRetour[1]==STATUS_OK)
	{
		//tout est ok 
		//pas souci de comptatibilité il faut déclarer une variable supplémentaire de 4 octets pour faire le traitement 
		inter=TrameRetour[3];
		retourfct=inter<<24;
		inter=0;
		inter=TrameRetour[4];
		retourfct|=inter<<16;
		inter=0;
		inter=TrameRetour[5];
		retourfct|=inter<<8;
		retourfct|=TrameRetour[6];
	}
	else
	{
		retourfct=-1;
	}

	return retourfct;

}

/**
 * @brief      { Get Capture 0  retourne les coordonnée enregistrer  }
 *
 * @param      moteur   The moteur
 * @param[in]  numBank  The number bank
 *
 * @return      { - La valeur demande 
 * 				 - -1 en cas d'erreur} 
 */
int32 GC0(composanteMoteur *moteur,Bank numBank)
{

	int32 retourfct=0;
	int32 status=0;
	int32 Location= ID_MSGBOX_RET-1;
	int32 res=Puissance(2,Location);
	int32 inter=0;

	moteur->trameData[0]=0x1f;
	moteur->trameData[1]=numBank;
	moteur->trameData[2]=0;
	moteur->trameData[3]=0;
	moteur->trameData[4]=0;
	moteur->trameData[5]=0;
	moteur->trameData[6]=0;

	//envoi de la trame
	CANMessageSet(CANA_BASE,moteur->identifiantMsgBox,&(moteur->msgObjEnvoyer),MSG_OBJ_TYPE_TX);

	do
	{
		status=CANStatusGet(CANA_BASE,CAN_STS_NEWDAT);
	}
	while(((status & 1<<Location )!= res));
		

	//ne pas oublier de mettre une fonction init qui initaliser le MsgObjRetour
		CANMessageGet(CANA_BASE,ID_MSGBOX_RET,&MsgObjRetour,true);
	if(TrameRetour[1]==STATUS_OK)
	{
		//tout est ok 
		//pas souci de comptatibilité il faut déclarer une variable supplémentaire de 4 octets pour faire le traitement 
		inter=TrameRetour[3];
		retourfct=inter<<24;
		inter=0;
		inter=TrameRetour[4];
		retourfct|=inter<<16;
		inter=0;
		inter=TrameRetour[5];
		retourfct|=inter<<8;
		retourfct|=TrameRetour[6];
	}
	else
	{
		retourfct=-1;
	}

	return retourfct;
}



int32 Puissance(int32 base, int32 exposant)
{
	int i;
	int32 res=1;

	for(i=0;i<exposant;i++)
		res*=base;

	return res;
}


