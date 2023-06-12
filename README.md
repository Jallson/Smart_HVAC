# Smart_HVAC_by_Jallson_Suryo
Problem:
The problem often found in HVAC system is the waste of energy dollars because the system cannot adjust to what is needed. Cooling or heating power if it is too small or too large will waste of energy. 
Need a system that can collect data on energy intensity needs at each zone, for example based on activities, number of people and duration.

Our Solution:
Smart HVAC System that can optimize energy consumption by adjusting the energy intensity in different zones inside an office or residential space (zones with more people, more activity and longer time durations will need more cooling/heating and vice versa).
The zone heat mapping will be generated using data obtained from Nicla Vision (with Edge Impulse's FOMO ML model embedded) that’s mounted like a surveillance camera inside the space.

Description:
The project uses Edge Impulse’s FOMO to detect multiple objects and its coordinates using a compact micro-controller with on-board camera (Nicla Vision).
The object detection ML model will use the top view of miniature figures with standing and sitting positions as objects. The data captured will be divided into training and test data.
Then the Impulse with Image and Object Detection as learning blocks and grayscale color blocks will be created.
The accuracy result for this training and test model is above 90% so there is confidence when distinguishing activities, counting the number of objects and location tracking.
The ML model deployed into the Nicla Vision, and by having a live output of number of objects, and the coordinate of each, then the information send via I2C to other microcontroller (Arduino Nano) with fans (motors) and Mini LED display attached.
This system will increase fan intensity on areas/zone that need more cooling/heating, which means more activity/people on a certain zone will increase fan intensity on that zone.
The total HVAC power output can also be adjusted based on the total number of people in the space. The project is a proof of concept (PoC) using a 1:50 scaled model with an office interior with several partitions and furniture and miniature figures.
The space divided into 4 zones, each zone will have a small fan installed. A small LED display can be used in the PoC to show the output of this simulation.
