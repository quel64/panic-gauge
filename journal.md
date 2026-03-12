### Refining the idea and BOM
This project is going to be a compact, real-time small monitor that tracks the VIX index, showing a graph and the current value of the index.

Core Features:
It will be powered by an ESP32-C3, to make it compact but still powerful enough to get the value for the index by an API key.
The graph and value will be shown by a 3.2" SPI display.

Tech Stack:
ESP32-C3
3.2" ILI9341 TFT (SPI)
C++/Arduino with TFT_eSPI for hardware-accelerated rendering.
3D-printed case designed in fusion 360.
<img width="878" height="636" alt="image" src="https://github.com/user-attachments/assets/44ffef47-b956-46a8-adea-e04fc25b9c10" />
<img width="1536" height="2048" alt="image" src="https://github.com/user-attachments/assets/598b52bc-2e23-454d-a394-f88bf422a6f7" />

### Making the full bom and planing the prices
I have decided not to make a pcb for this project, because it would over-complicate it and i feel like it isn't needed for this simple one.

Full BOM:
Esp32-C3 - $2.04
3.2 SPI TFT LCD (ILI9341) - $9.32
Solderable Perfboard - $2.19
AMS1117-3.3V,Voltage Regulator - $1.26
TOTAL: $14.81
<img width="1818" height="1131" alt="image" src="https://github.com/user-attachments/assets/eccf7712-06e7-4053-a673-20e515e0c45f" />

### Designing the case
the design proces for the case was pretty straight foward. The part that tool the longest was to define the actual dimensions of the case, and ake everything fit inside. I have decided on these:
<img width="1919" height="1199" alt="image" src="https://github.com/user-attachments/assets/9f0f50ac-bccf-44fb-984f-324c20ec3dcc" />
<img width="1919" height="1174" alt="image" src="https://github.com/user-attachments/assets/641a1f2f-3547-46e9-a09c-971bc9bb32d2" />
<img width="1919" height="1180" alt="image" src="https://github.com/user-attachments/assets/f15215cf-2453-4ad1-8698-f60b423ef135" />
instead of screwing the cover down, i decided that its best to have this case with the small extrusion to secure it, and then when the electronics are all good, glue it to the rest of the case.
then i added the usb-c hole to power everything:
<img width="1919" height="1198" alt="image" src="https://github.com/user-attachments/assets/484cfb5b-a691-4241-9215-c9fe68e1e541" />
<img width="1900" height="1198" alt="image" src="https://github.com/user-attachments/assets/809f384f-2fe4-471b-a1ec-23e15d650242" />
and also some small holes to add some optional rubber feet so it doesnt slide:
<img width="1919" height="1199" alt="image" src="https://github.com/user-attachments/assets/2a90dc98-b1ef-449f-93b0-6d0b523dc166" />
now that i finished the CAD, i only have the coding part left.

### setting up tft_espi and the driver config
the biggest pain so far has been the tft_espi library because it doesn't let you define pins in the sketch. you have to go into the library folders and edit User_Setup.h manually.
<img width="1919" height="1199" alt="image" src="https://github.com/user-attachments/assets/3fe98b74-5557-4f8d-bd52-e5b8f2decaf1" />
i spent most of the time cross-referencing the esp32-c3 supermini pinout with the ili9341 datasheet. since the supermini is so small, the pin labels are different than a standard devkit. i decided to use gpio 4, 5, 6, 7 and 10 for the spi bus. it’s a bit of a "blind" setup but i'm pretty sure the mapping is right. i also set the spi frequency to 40mhz so the graph updates look smooth and don't flicker.
<img width="1919" height="1199" alt="image" src="https://github.com/user-attachments/assets/dff63c01-f00c-4830-b86e-538558aae2ed" />

### api logic and json
had to pivot the code to use alpha vantage since twelve data wanted money for the vix index. alpha vantage is free but their json is way more nested
<img width="1832" height="1144" alt="image" src="https://github.com/user-attachments/assets/bea70c29-437f-4c49-b2b8-66db15aeabed" />
coding this blind is tricky because i can't check the serial monitor to see if the json buffer is big enough. i set the DynamicJsonDocument to 8192 bytes just to be safe so it doesn't crash the c3. i also wrote the logic to reverse the data array, because the api sends the newest price first, but the graph needs to draw from left to right (oldest to newest). it took a while to get the loop right so the current price always ends up at the very edge of the screen.
<img width="1919" height="1199" alt="image" src="https://github.com/user-attachments/assets/6f756402-86af-44e0-8dc5-6035b7f943e6" />

### graph math and finishing the logic
finished the main graphing function today. the vix usually stays in a small range, so if i used a static scale the graph would just look like a flat line. i wrote a dynamic scaling loop that finds the highest and lowest price in the last 30 points and "zooms" the graph to fit the box.
also added the theme toggle for the second button. it basically just swaps the hex codes for the colors (background and text). since i'm skipping the pcb, i made sure the button pins (gpio 2 and 3) use internal pullups in the code so i don't have to solder extra resistors. the full code is done, now i just need the parts to arrive so i can flash it and see if my math actually works.
<img width="1919" height="1199" alt="image" src="https://github.com/user-attachments/assets/ff77fa5e-2924-4897-8271-76b3591fa539" />
<img width="1919" height="1199" alt="image" src="https://github.com/user-attachments/assets/37d8dae4-6b32-4c70-a67a-9e5f70906b30" />
<img width="1919" height="1199" alt="image" src="https://github.com/user-attachments/assets/d86aed8f-d328-42b6-8f8f-3a8130f090ec" />
