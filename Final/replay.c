#include "global.h"
/*
 * 	Replay Function
 * 	Reads the replay path file back and send the values to motors in
 * 	the sequence they were written to it.
 */
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
/*
 * 	Write Path Function
 */
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
/*
 * 	New File Function
 *	Only fucntion of this file is to wipe / create new replay path file
 */
int new_file()
{

	if ((path = fopen(REPLAY_FILE, "w+")) == NULL)
	{
		printf("Cannot create replay path file");
		return -1;
	}

	return 0;
}
