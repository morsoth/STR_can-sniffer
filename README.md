# CAN Sniffer
## Project structure
```
...
```

## Set-up
#### Arduino IDE
* Install `Arduino Mbed OS Giga Boards` from the Board Manager.
* Install the `Arduino_GigaDisplay` library pack from the Library Manager.

#### Connections
![connections_image]()

## Use Precompiled Libraries (Windows)
#### Get Precompiled Library File
1. Go to `Documents\Arduino\libraries\<YourLibrary>` folder.

2. Add this line in the `library.properties` file:
```
dot_a_linkage=true
```

3. In your Arduino IDE, go to **Preferences** and check the _Show verbose output during compile_ box.

4. Compile a sketch in where you include the library and look for the final command in the output, something like this should appear:
```
"C:\\Users\\XXXXX\\AppData\\Local\\Arduino15\\packages\\arduino\\tools\\arm-none-eabi-gcc\\7-2017q4/bin/arm-none-eabi-size" -A "C:\\Users\\XXXXX\\AppData\\Local\\Temp\\arduino\\sketches\\55BB565C88C1ADC02EEB74A4C9C2D41C/sketch_may3b.ino.elf"
```
&nbsp; &nbsp; This means that our `.a` file is in:
```
C:\\Users\\XXXXX\\AppData\\Local\\Temp\\arduino\\sketches\\55BB565C88C1ADC02EEB74A4C9C2D41C/
```
5. Delete the `dot_a_linkage=true` line in the `library.properties` file.

#### Use Precompiled Library
1. Copy the `.a` file into `Documents\Arduino\libraries\<YourLibrary>\src` folder.

> **Note:** In some cases a library could have different subdirectories in the `src` folder for each board. In that case you must copy the `.a` file into the corresponding subfolder according to the board you compiled to.

2. Rename the `.a` file, adding `lib` to the start: `lib<YourLibrary>.a`.

3. Return to the `library.properties` file and add:
```
precompiled=full
ldflags=-l<YourLibrary>
```
&nbsp; &nbsp; Where `<YourLibrary>` is the library name.

4. Now you can delete the `.cpp` and `.h` files from your library and compile using the precompiled file!!

> **Note:** It's not mandatory to delete the source and header files. 
