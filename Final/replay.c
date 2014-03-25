#include "marco.h"
/* ****************************************************************************	*/
/* 	Replay Function								*/
/* 										*/
/* 	Reads the replay path file back and send the values to motors in	*/
/* 	the sequence they were written to it.					*/
/* 										*/
/* ****************************************************************************	*/
int f_replay(comedi_t *device)
{
/* Local Vars */
unsigned int left, right;
	
	/* Makes sure the fiel can be opened before trying to read */
	if ((path = fopen (REPLAY_FILE, "r")) == NULL)
	{
		mvprintw(24, 20, "Cannot open replay path file, please create a path first");
		return (0);
	}

	/* Keep going until end o the file is reached */
	while (! (feof (path) ) )
	{
		fscanf(path, "%4u %4u\n", &left, &right);
		motors (device, &left, &right);
		usleep (100100);
	}
		
	left = STOP;
	right = STOP;
	motors (device, &left, &right);

	/* Close the file and exit function */
	fclose(path);
	return (0);

}
/* ****************************************************************************	*/
/* 	Write Path Function							*/
/* 										*/
/* 	Values passed from Joystick function are written to the replay text	*/
/* 	file ready to be read back in replay path fucntion.			*/
/* 										*/
/* ****************************************************************************	*/
int write_path(unsigned int *left_motor, unsigned int *right_motor)
{

	/* Opens file for writing too */
	if ((path = fopen (REPLAY_FILE, "a+")) == NULL)
	{
		mvprintw(24, 20, "Cannot open replay path file");
		return (0);
	}

	/* Writes values to the file */
	fprintf(path, "%4u %4u\n", *left_motor, *right_motor);
	
	/* Close file and exit function */
	fclose (path);	
	return (0);
}
/* ****************************************************************************	*/
/* 	New File Function 							*/
/* 										*/
/*	Only fucntion of this file is to wipe / create new replay path file	*/
/* 										*/
/* ****************************************************************************	*/
int new_file()
{

	if ((path = fopen (REPLAY_FILE, "w+")) == NULL)
	{
		mvprintw(24, 20, "Cannot create replay path file");
		return (0);
	}

	return (0);
}
