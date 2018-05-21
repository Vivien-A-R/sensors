<!--
waggle_topic=IGNORE
-->

# Test Scope and Coverage

This test evaluates the following aspects of Airsense design. </br>
* I2C communication through EXT RJ-45 Jack. </br>

# Test Setup Procedure

## Test Events Timeline
* Airsense is loaded with firmware that performs the test. 

## Electrical Connection
-   Connect Lightsense to Airsense using CAT6 cable between Lightsense's RJ45 jack and Airsense's RJ45 jack
    labeled __Light__.  </br>
    <img src="./resources/RJ45AirtoLight.jpg" width="320"><img src="./resources/RJ45_Light.jpg" width="209">
-   Set the mode slide switch on Lightsense toward the right (away from the R1 label on the board).</br>
    <img src="./resources/PowerSlideSwitchLight.jpg" width="320">
-   Connect the ACHEM test adapter to the CHEM RJ-45 of Airsense using a CAT6 cable. 
    <img src="./resources/achem_connected.jpg" width="320">
-   Connect the AEXT test adapter to the EXT RJ-45 of Airsense using a CAT6 cable. 
    <img src="./resources/achem_connected.jpg" width="320">
-   Connect a micro USB cable between Lightsense and QA computer.

__Please make sure only the Airsense-Lightsense pair under test is connected to the QA computer. No other similar Coresense devices or Arduino Due/derivatives can be connected at this time.__

## Testing Procedure
*  In the coresense folder, and issue command *make run* and follow the on-screen instructions. 

__To exit the screen session created for the test, press Control+a, followed by k and y.__

## Test Log
This test is evaluated and scored on the __Coresense test document__.

# Success Criteria
The test is considered __PASSED__ if all the sub-tests under the Coresense-External Communication Test pass.

