@ECHO OFF
ECHO ==============================================================================
ECHO This is a speed testing for codes in Advanced Computer Graphics HW4.
ECHO It runs multiple settings for rendering. 
ECHO
ECHO The usage of HW4.exe is:
ECHO	$ cd \wherever_you_keep_it\RT-HW4
ECHO 	$ RT-HW4.exe trace_depth partition figure
ECHO
ECHO positional arguments:
ECHO     trace_depth		the tracing depth of ray
ECHO     partition              number of primitives to form a bounding box
ECHO     figure			the name of .txt file which you want to render
ECHO				do not enter postfix ".txt"
ECHO				just the file name is fine
ECHO
ECHO ==============================================================================

RT-HW4.exe 4 200   Input_Suzanne
RT-HW4.exe 4 220   Input_Suzanne
RT-HW4.exe 4 200   Input_Suzanne-HR
RT-HW4.exe 4 220   Input_Suzanne-HR

RT-HW4.exe 4 12800 Input_Bunny
RT-HW4.exe 4 12800 Input_Bunny-HR

PAUSE
EXIT