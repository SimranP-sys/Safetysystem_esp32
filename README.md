# IoT-based-Safety-system-for-smart-building

A Hackathon project which implements a safety system - intruder detection, gas leakage, emergency key, temp monitoring using sensors and actuators.



This is my 5 hour hackathon project in which we worked on the problem statement "IoT based safety system for smart buildings."



Components we were provided with:

1. esp32
2. relays
3. SR04
4. MQ-2 gas sensor
5. DHT11

<img width="900" height="700" alt="Circ" src="https://github.com/user-attachments/assets/3c350890-4b69-48ca-ac63-d540f275e1aa" />


We used Blynk platform to generate dashboard.



Configured SR04 to detect any intruder presence at the main gate. Any intruder coming with the threshold (20cm) will cause an alert using buzzer and Blynk dashboard will be updated.



Similarly the Blynk dashboard updates temperature and humidity using DHT11 sensor. 



The MQ-2 gas sensor's value is continuously updated on the dashboard. If sensor value goes above 200 ppm the Blynk dashboard is updated and the relay representing exhaust fan ckt is turned on. 

Additionally, the exhaust fan relay can also be turned on through the online dashboard. 


<img width="1000" height="541" alt="image" src="https://github.com/user-attachments/assets/4db84f1c-2065-446c-b4ac-cf7040d5e5ce" />







