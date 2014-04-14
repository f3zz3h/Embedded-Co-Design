/* ********************************************************
 * Project Title: EMU Pick & Place
 * File: replay.c
 * Description: Currently unfinished implementation of the
 * replay path functionality
 * Authors: Luke Hart, Joe Ellis, Kerrim Morris & Lukasz Matczak
 * Last edited: 14/04/2014
 * ******************************************************* */

#include "global.h"
/* ********************************************************
 * 	Description: Reads the replay path file back and send the
 * 	values to motors in the sequence they were written to it.
 * 	Params: The current X,Y,Z co-ords
 * 	Return: Succes or Failure
 * ******************************************************* */
int replay(float* xyz_pos)
{
	/* Makes sure the file can be opened before trying to read */
	if ((path = fopen(REPLAY_FILE, "r")) == NULL)
	{
		printf("Cannot open replay path file, please create a path first");
		return (0);
	}

	/* Keep going until end o the file is reached */
	while (! (feof (path) ) )
	{
		fscanf(path, "%f:%f:%f:\n", &xyz_pos[X],&xyz_pos[Y],&xyz_pos[Z]);
		/* TODO: Call IK with read values and update PWM's */
	}
		
	/* Close the file and exit function */
	fclose(path);
	return (0);

}
/* ********************************************************
 * 	Description: Write Path Function to write to file one
 * 	position at a time.
 * 	Params: Current XYZ position
 * 	Return: Success or failure
 * ******************************************************* */
int write_path(float* xyz_pos)
{
	int i;

	/* Opens file for writing too */
	if ((path = fopen(REPLAY_FILE, "a+")) == NULL)
	{
		printf("Cannot open replay path file");
		return (0);
	}

	for (i = 0; i < 3; i++)
	{
		/* Writes values to the file */
		fprintf(path, "%f:", xyz_pos[i]);
	}
	fprintf(path, "\n");
	/* Close file and exit function */
	fclose (path);	
	return (0);
}
/* ********************************************************
 * Description: Only function of this file is to wipe /
 * create new replay path file
 * Params:
 * Return: Success or Fail
 * ********************************************************/
int new_file()
{

	if ((path = fopen(REPLAY_FILE, "w+")) == NULL)
	{
		printf("Cannot create replay path file");
		return -1;
	}

	return 0;
}
