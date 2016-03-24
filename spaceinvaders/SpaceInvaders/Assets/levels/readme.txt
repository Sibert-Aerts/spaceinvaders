
Level format:

	File Name:
		The file name has to be "levelX.txt" with X a positive integer, and every level from 0 through (X-1) has to already exist.

	Entities:
		The entities in a level are stored as 15 lines of 20 characters each, split by newlines.
		The following are allowed "tiles":
		
			'.'		Represents a lack of any entity on that tile, and is required
			'X'		With X an integer between 1 and 9, represents a barrier with X health
			'eX'	With X an integer between 1 and 9, represents a small enemy with X health
			'EX'	With X an integer between 1 and 9, represents a large enemy with 2*X health
			
			Enemies take up 2 characters since their sprites are 2 tiles wide, as such there's no possible ambiguity.			

	Fields:
		A level file ends with a series of fields configuring settings of that specific level.
		All fields are required.
		
		'Speed: X Y'	With X and Y floating point numbers, 
						indicates that enemies will start the level moving X pixels per second,
						and will accellerate an additional Y pixels per second for each enemy destroyed.
						
		'Name:X'		With X a series of characters terminated by the EOF or newline,
						indicates the level's name as "X".