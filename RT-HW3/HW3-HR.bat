@ECHO OFF
ECHO ==============================================================================
ECHO This is a speed testing for codes in Advanced Computer Graphics HW3.
ECHO It runs multiple settings for rendering. 
ECHO
ECHO The usage of HW3.exe is:
ECHO	$ cd \wherever_you_keep_it\RT-HW3
ECHO 	$ RT-HW3.exe trace_depth figure
ECHO
ECHO positional arguments:
ECHO     trace_depth		the tracing depth of ray
ECHO     figure			the name of .txt file which you want to render
ECHO				do not enter postfix ".txt"
ECHO				just the file name is fine
ECHO
ECHO ==============================================================================

RT-HW3.exe 4 Input_Suzanne-HR

RT-HW3.exe 4 Input_Bunny-HR

PAUSE
EXIT