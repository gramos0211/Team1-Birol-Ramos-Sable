# Quest 2: Tactile Internet
Authors: Ece Birol, Gabriel Ramos, Brendan Sable

Date: 2020-10-08
-----

## Summary

The task was to connect various analog sensors in our lab kit, which are the thermistor, the ultrasonic sensor and the IR sensor to the ESP32 using the analog input ports
at A2, A3 and A4 -- in our case. After connecting these sensors, we needed to independently test and later calidate each of these modules before integrating them together
so that we didn't face a lot of errors in the complete task. We started out by the module to display the calues on the console as text, later sontinued with reading the ultrasonic
sensor values. We continued with the thermistor, the IR sensor until they were all functioning as they should. We finished up with testing the module to save console
data to a file on our host in a text file. We finished up with testing reading data from our host file and plottin with CanvasJS and node.js. 
After testing all these modules independently, we were ready for sticking all the pieces together for the big project, the Tactile Internet that combined all these modules
into one. We dipslayed the measured values on our GUI.

The voltage and power requirements for the sensors were different from each other. The IR sensor needed 5.5V whereas the ultrasonic range finder only required 3.3V. With these
in mind, we designed out hardware circuit tediously, trying to not burn any of the pieces. We accomplished that, and seems like everything is still in once piece. 

Investigative question: 
    Tabulate and compare the accuracy and speed of the IR and ultrasonic sensors. Which one would you prefer to use to support driving a robotic car?

    > Ultrasonics sensors are usually insensitive to hindering objects such as vapor, dust, smoke, mist etc. Therefore, they aren’t as good as IR sensors at defining edges of an area.
    > Infrared sensors work on the principle of reflected light waves, therefore are much more sensitive. However, they are prone to fluctuations in variant light conditions.  
    > IR sensors are much more sensitive to objects, and detecting movements and the accuracy and speed is better than of a Ultrasonic sensor; therefore, using an IR sensor to suport a robotic car is the way to go. 



Solution Requirements
For each sensor you integrate, establish the ranges, offsets, and limitations (read the spec sheets)
Specify the pin assignments (table) for the ESP and each device
Develop formulae to convert measured units into common engineering units among the sensors
Plotting: your plotting part should generate real-time strip-chart display(s)


## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Periodic reporting of ultrasonic range in m| 1 |  1     |       1         |
| Periodic reporting of IR range in m | 1 |  1     |       1         |
| Periodic reporting of temperature in C | 1 |  1     |     1         |
| Results displayed at host as text | 1 |  1     |      1         |
| Results graphed at host continuously based on reporting period | 1 |  1     |      1         |
| On Time | n/a |  1     |       n/a       |
| Investigative question response | 1 |  1     |     1         |


### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | 3 |  5     |       3
| Quality of report.md including use of graphics | 3 |  3     |      3
| Quality of code reporting | 2 |  3     |       2
| Quality of video presentation | 1 |  3     |       1


## Solution Design

The solution design was aimed towards maintaining a clean, simple and efficient look. Aside from that the design followed the one instructed from the Alphanumeric I2C Display with the added servo.



## Sketches and Photos

## Supporting Artifacts

This video is presents the original concept of the Tactile Internet:
 

## References

The sources used for this project were those provided in our online textbook for our individual skill assessments as well as the example code provided by the ESP32, 14 seg display and the class materials.

-----

