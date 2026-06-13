# LITTLE HELPER:
## DOCUMENTATION

## Basic Questions and Overview:

### What is Little Helper?

Little Helper is a track based 4wd bot that is capable of climbing stairs and moving around with a giant box on top of it so that teachers can carry books and papers between floors, although it could be used for other purposes as well! As of right now it is controlled via a PS3 controller (you could also use a phone to control it if you tweak the code). I plan to eventually add autonomous capabilities but that is not currently available.

### Why did I make this project?

A while back after a robotics competition called the National Robotics League, a teacher saw our robot moving blocks around and jokingly asked, could you make it move the textbooks from class to class? We laughed it off, but when I was preparing an idea for Fallout - a Hack Club program, I thought why not actually do that? I love building bots that can drive and it would actually be usefull.

### Use case:
The bot has an A3 sized box on top and a set of tank tracks that help move it, but the bot could very easily be retrofitted to other purposes as well. For example an all terrain bot would also be a valid use case due to the tank tracks. The tank tracks also make it easy to climb stairs or other unusual surfaces (keep in mind the stairs can't have too much height), the motors I've used are highly powerful and have really good torque as well, so it would be a good recreation/racing bot perhaps (bear in mind that the bot would be on the heavier side). It could also be used to pull or push objects should you want. (The only limit here is your imagination realistically, the bot is kind off a base to build your ideas!)

### How long would it take me to build this?
The main issue with the building process would be acquiring the parts especially the 3d printed parts, however as for the rest of it, if you're following this guide to the T, a week should be more than enough if you have your parts.

### Can I buy this?
In a way. While the bot isn't available for purchase you can buy the parts and build it yourself! It honestly isn't that difficult even if you have very basic skills in regards to this. Another thing to bear in mind is that this is obviously not a lisenced or approved project, so there's no warranty and should you need regulatory authorisation/permission , you would need to reach out to the appropriate body yourself.

### Is it easy to build this?
Absolutely! The bot is actually quite basic, even the tank tracks are just held on by guiderails and friction. I'm not the most skilled individual so I couldn't make the project too difficult to build, which gives the benefit of an easy to make product.

### Is this all free?
YES! All the materials I've provided are open source, you can use, modify, or distribute them however you like, however please credit me for this build. Infact if you need any help with building this just send an email to me at : alvashlok@gmail.com and I'll try to help you out.

### How expensive is this?
The cost of building it is completely and elaborately detailed in the BOM ( Bill of Materials ) showcased in this repo.



## Suggestions For Use:



1. Firstly if you plan for the robot to move up stairs, keeping CG in mind while loading stuff in the box would help (try to keep the books flat as otherwise not only would it move around but should they fall that may displace the bot).
   
2. The battery life won't be more than an hour (I've not tested this yet but it's an approximate). However installing a battery of higher amperage would be really easy ( Just don't change the voltage or if you do use a step down/step up converter).
  
3. The motor driver already has a very high current draw from the motors since they're 4*1000 RPM Johnson Grade A motors so be wary of installing components that would add to the current draw.

4. To conserve battery life removing components that are unnecessary for your use case is suggested. You can also change the code to tweak the speed of the bot (1000 RPM is the absolute max and the minimum would be whatever you want).

5. People should be aware of the bot's presence wherever you're using it so no one accidentally stays in its path or tampers with it.

6. If exam papers are being carried in the box, lock the box securely.

7. While you are free to use whatever parts you choose, avoid mecanum wheels as they won't fit onto the motors and they can't be used to their potential and as such are a waste of time, since it isn't possible to move sideways etc when using tracks which is the main point of mecanum wheels. Also the wheels may not grip the tracks well.

8. If you want to not use code and have a transmitter/receiver and want to save on parts, you could use an ESC to control your motors with a transmitter and receiver, however you would still need to keep an ESP for the other electronics, and there would be no scope for making the bot autonomous after this.



## The CAD File:

The CAD file can be found in this repository as both a STEP file and a f3d file ( I made the model in fusion ). 

### Basic Suggestions:

Wherever there are holes with fasteners like screws placed inside, I would recommend drilling the holes with a handhed drill if you have one available to you. If you don't have the access to one however, you can 3d print the part with the hole as a part of it, or as a DIY method that I sometimes use you could heat a sharp metal rod like a small poker used for melting marshmellows/making corn on the cob and then carefully make the hole where you need to (you may not get a tight fit or threading with this method so you may have to add nuts). All the screws are either M3 or M5 to make the process of building easier. IMPORTANT: This is designed to be changeable the motor driver holes I have provided should be adjusted to appropriate dimensions unless you're using the same motors. Similarly adjust all other mounts.

Something to note is that I did not add any spacers between the components and the plastic chassis in the CAD file, however, it's important to do that. Plastic spacers can be 3d printed or bought online as well.

While the CAD file does have 3d wiring, that is more to serve as a reference, there is no gaurantee that it will be accurate and as such please refer to the schematic diagram instead when you're doing the wiring.

Something else to note is that care should be taken while attempting to 3d print. If a part is too big for your 3d printer, I would suggest printing it in pieces and using the adhesive - Metlock 743 which practically makes it one component ( but be very careful with this adhesive as it is incredibly strong and even a drop bonds the plastic together permanently and you should wear a mask and safety goggles incase you see fumes from the plastic (also make sure that you aren't making some kind of toxic fume)). You could also CNC the chassis or make it from foamboard, the CNC version will be heavier however and the foamboard version would be weaker.

Also the tracks while modelled aren't designed to be 3d printed. Tracks should be made of a material like rubber for optimum grip, 3d printing it wouldn't work. I've also not modelled the linking mechanism well enough that you could take 1 link from the cad and just attach it to other links, you would have to glue it together.

Something very important is resizing the power button as per the dimensions of the one you purchase (if theres a large size difference) this has to be a 3d printed hole (you could make the hole yourself if you have the required power tools).

### General Info:

The CAD file has every component and body named for ease, some components/bodies are connecting wires and as such they are named with the origin and destination. This notation is important to note: BR- Back panel Right side, BL - Back panel Left side, FR - Front panel Right side, FL - Front panel Left side, LF - Left panel Front end, RF - Right panel Front end, LB- Left panel Back end, RB - Right panel Back side.

The hinges for the box are 3d modelled but they SHOULD NOT be printed, purchase those from a hardware shop, these hinges are not designed to actually work. I'd also suggest changing the text present on the chassis and box since they're highly customised to me, so it would make sense to change it to something that you want. You could use the timeline feature to find where I wrote the text and just change the text and the extrude should catch up after that operation is done. 

I know it sounds stupid to mention it but please don't try to 3D print any of the wires or electronic components either. 

There are a few joints that can be animated through the relationship page as well : The wheels rotate, the lid of the box opens and closes, the lock for the box opens and closes, and a few of the nuts also rotate.

The placement of most components in the CAD file is genuinely arbritary, you could change it as you please (Obviously would have to change the wiring and mounting points appropriately).

## Code:

The code can be tweaked however you want. Especially do change the configuration for the controls and how you connect your controller (you could keep the same controls if you wish but the controller connection won't transfer).

The code can be uploaded to the ESPs via Arduino IDE. Just make sure you select the appropriate port and board in the board manager, and ensure you have all the required modules installed and all the needed drivers. 

Its also possible to upload the code via VSC if you use PlatformIDE or other such extensions, so if that's something you want to do that is also possible.


## Building the Project: 



1. To start with refer to the BOM and ensure you have all the parts you require. ( You could start building with only some of the parts and adding parts as and when they come as well but workflow would become worse ).

2. Step one would be to take your side panels and attach the motors to them. The motors have 3 holes in an L shape which can be matched to the side Chassis. Orient it in a way such that the motor shaft passes through the hole in the same way that in passes through on the opposite side (avoid difference between he sides since that would make the tracks uneven). 

3. Once that is done, attach the guidelines as per the screw holes at the center of the side panel. ( For the guidlines there's only a small surface area where there is a contact between it and the panel. 

4. Next take the RFID reader, LCD screen and Numpad and place and secure them in the appropriate positions on the side panels. Note that the numpad isn't secured by screws as it has no scope for that due to its thickness and material. The numpad comes with adhesive on the back and it should be placed on the side panel.

5. Take the battery and place it in the battery holder (if you changed the battery obviously change the battery holder as well unless it's the same configuration). Secure the cover as well. In my CAD file there is a canvas showing a fire hazard, I would suggest making a marking similarly whether with a marker or a sticker, or 3d printing an imprint or sticking a sheet of paper. 

6. Take the bottom panel and attach the battery holder (and the buzzer holder if that was printed seperately).

7. Attach the side panels on both sides to the bottom plate.

8. Now take the wheels and place them on the motor shafts such that the tire treads all point in the same direction and the hole on the tire for the bolt overlaps with the hole on the motor. Place the m3 bolt inside and secure it tightly (the wheels should not shake or come lose). A nut isn't needed for this bolt as the motor shaft has threading.

9. Now comes the part that differentiates the bot from other regular 4wd bots. Take the purchased track chain and place it over the two tires on a side. These tracks have removable links, so add or remove the links to tension the track as tightly as you can. Remember that there is nothing holding the chain there except friction. So tighten it as much as you can without risking the chain snapping.

10.  Great! Now you have your hollow shell, the next step would be wiring etc, before you close the top or front and back plates so that you can still easily access components. Bring the wiring for the components placed on the side panels through their respective holes. Then place the motor driver, ESP8266, and ESP32 in their respective positions. Attach the ultraonic sensor to the front panel on the mounting plate. Plop the buzzer into its holder as well (Don't attach power button yet). Next refer to the wiring schematic and connect all the components together(except leds). If you don't want the wires coming off you can solder them to the terminals instead of just having the jumper cables. Do not connect the battery or powerswitch yet.

11.  Now would be the stage where I would suggest you upload the code, as you can still test and make sure everything is correct by fixing any issues before you close any panels. (NOTE: DO NOT UPLOAD THE CODE WITH THE BOT ON A TABLE. Sometimes the wheels can spin when you're uploading the code, yes , even if you've disconnected the battery.) Make sure to upload the codes for both the ESPs.

12.  Once that's all done and dusted you can go ahead connect the battery and the powerswitch, but leave the powerswitch dangling somewhere inside the chassis do not secure it in place yet as then you would have to close the top panel.

13.  Switch it on and configure your method of controlling the bot and then make sure everything is running fine, and if not debug.

14.  Now keep all the panels in close vicinity and attach the front and back plates to the side panels.

15.  Attach the led lights to the resistors and cables and wire it appropriately. Then insert the led light into the side plate by pushing it into the led holes till its securely fastened. Now test the led lights.

16.  Take the top plate and attach the box to it (if you are attaching one to carry stuff). Secure it tightly since it moving can affect the CG and make the bot topple down the stairs if it shifts.

17.  Detach the powerswitch and attach it to the top panel and then wire it backk to where it was. Attach the top plate to the side panels.

18.  Paint/add stickers as required.

19.  Drive it around and test it!

### CONGRATS!!!! YOU HAVE NOW BUILT THE BOT!!!


#### Now use your bot as you please. Modify your design as per your testing diagnostics. If you find an easier way to do something or find an issue or have modified your bot, you should try to comment on the repo so that others also benefit from your knowledge.


## NOW GO AND SHOW OFF YOUR BOT!




# Maintained and developed by : SHLOK ALVA 




