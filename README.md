# Smart_HVAC_by_Jallson_Suryo
### Problem:
The problem often found in HVAC systems is that energy got wasted because the system uses more energy than necessary or the system cannot quickly adjust to the changing needs in a dynamic environment. To tackle the problem, we need a system that manages its power intensity based on what is necessary for each zones in real-time for a given environment. The power intensity necessary for each zones can be derived from the following data: number of person, time duration spent inside, and/or the person’s activity.

### Our Solution:
Smart HVAC System that can optimize energy consumption by adjusting the power intensity in different zones inside an office or a residential space (zones with more people, more activity and longer time durations will need more cooling/heating and vice versa). The zone heat mapping will be generated using data obtained from Nicla Vision (with Edge Impulse's FOMO ML model embedded) that’s mounted like a surveillance camera inside the space.

### Description:
The project uses Edge Impulse’s FOMO to detect multiple objects and its coordinates using a compact micro-controller with on-board camera (Nicla Vision). The object detection ML model will use the top view of miniature figures with standing and sitting positions as objects. The data captured will be divided into training and test data. Then the Impulse with Image and Object Detection as learning blocks and grayscale color blocks will be created. The accuracy result for this training and test model is above 90% so there is confidence when counting the number of objects (person) and tracking its centroid coordinates. The ML model is then deployed to the Nicla Vision. The number of objects in each zone is displayed into the OLED display. The Nicla Vision also communicates to an Arduino Nano via I2C which we are using for the fan speed controller. This system will increase fan intensity on areas/zone that need more cooling/heating, which means more activity/people on a certain zone will increase fan intensity on that zone. The total HVAC power output can also be adjusted based on the total number of people in the space. The project is a proof of concept (PoC) using a 1:50 scale model with an office interior with several partitions and furniture and miniature figures. The space is divided into 4 zones, each zone has a small fan installed. The OLED display is used in this PoC to show the output of this simulation.
