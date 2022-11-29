# Adv-CG HW3

### Results
The following figure is the result of speed testing.
I test whether the tracing depth of ray affect the rendering speed.

```
C:\Users\jason\Desktop\RT-HW3> Hw3.bat
==============================================================================
This is a speed testing for codes in Advanced Computer Graphics HW3.
It runs multiple settings for rendering.

The usage of HW3.exe is:
    $ cd Iwherever_ you_keep_itiRT-HW3
    $ RT-HW3.exe trace_depth figure

positional arguments:
    trace_depth         the tracing depth of ray
    figure              the name of .txt file which you want to render
                        do not enter postfix ".txt"
                        just the file name is fine

==============================================================================
Input file : Assignment 3 Input\Input_Suzanne.txt
Render duration: 10 s
Output file : Output\Input_Suzanne_TD_2.ppm
Input file : Assignment 3 Input\Input_Suzanne.txt
Render duration: 12 s
Output file : Output\Input_Suzanne_TD_4.ppm
Input file : Assignment 3 Input\Input_Suzanne.txt
Render duration: 15 s
Output file : Output\Input_Suzanne_TD_6.ppm
Input file : Assignment 3 Input\Input_Suzanne.txt
Render duration: 15 s
Output file : Output Input_Suzanne_TD_8.ppm
Input file : Assignment 3 Input\Input_Bunny.txt
Render duration: 1098 s
Output file : Output\Input_Bunny_TD_2.ppm
Input file : Assignment 3 Input\Input_Bunny.txt
Render duration: 1405 s
Output file : OutputlInput_Bunny_TD_4.ppm
Input file : Assignment 3 Input\Input_Bunny.txt
Render duration: 1576 s
Output file : Output\Input_Bunny_TD_6.ppm
Input file : Assignment 3 Input\Input_Bunny.txt
Render duration: 1684 s
Output file : Output\Input_Bunny_TD_8.ppm
Press any key to continue...
```

The following table summarize the results.
As we can see when the tracing depth gets bigger, the rendering process takes more time.

| trace depth\filename | Input_Suzanne | Input_Bunny |
| :--------: | :--------: | :--------: |
| 2 | 10 sec | 1098 sec |
| 4 | 12 sec | 1405 sec |
| 6 | 15 sec | 1576 sec |
| 8 | 15 sec | 1684 sec |

The computer info 
```
C:\Users\jason>systeminfo

Host Name:                 DESKTOP-HLRRO0P
OS Name:                   Microsoft Windows 10 企業版
OS Version:                10.0.19044 N/A Build 19044
OS Manufacturer:           Microsoft Corporation
OS Configuration:          Standalone Workstation
OS Build Type:             Multiprocessor Free
System Manufacturer:       ASUSTeK COMPUTER INC.
System Model:              D830MT
System Type:               x64-based PC
Processor(s):              1 Processor(s) Installed.
                           [01]: Intel64 Family 6 Model 158 Stepping 9 GenuineIntel ~3601 Mhz
BIOS Version:              ASUSTeK COMPUTER INC. 0701, 2017/8/16
Total Physical Memory:     8,042 MB
Available Physical Memory: 3,189 MB
Virtual Memory: Max Size:  13,628 MB
Virtual Memory: Available: 4,779 MB
Virtual Memory: In Use:    8,849 MB
```

### Reproduce
To reproduce the speed test, you can simply do
```bash
$ cd \path\to\RT-HW3
$ start HW3.bat
```

Or if you want to test other figure, please put the text file under `RT-HW3\Assignment 3 Input\` directory, then run 
```bash
$ start RT-HW3.exe trace_depth file_name
```

e.g., There is a `figure.txt` you want to render with trace depth of 4, put it under `RT-HW3\Assignment 3 Input\` folder , then run
```bash
$ start RT-HW3.exe 4 figure
```
The output image will be `RT-HW3\Output\figure_TD_4.ppm`.

Note that the execution time may vary with my implementation since our platform is possibly different.
If your windows system is x32-based, then you have to recompile `RT-HW3.exe` to whatever release your system can run.