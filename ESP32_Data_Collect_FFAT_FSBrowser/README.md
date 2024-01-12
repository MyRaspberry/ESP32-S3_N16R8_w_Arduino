# ESP32_Data_Collect
## board: ESP32-S3 F16N8
incl:<br/>
## use FFAT ( on 10MB partition )
## use PSRAM 8MB for long array definition
## use FSBrowser use FFAT via webserver 
### ESP32-S3 Class lib for RGB_LED pin 48 after soldering

### 1 sec : ".", rainbow.run(), wifi.run()

### 15 sec : Analog In with counter and timestamp

### 1 min : save / append to FFAT /data/readings.csv

<CODE>,rec,datetimes,A0,A1,A2,
,191,2024-01-07 16:09:29,58.46,30.28,1.00,</CODE>



##v0.2.A+

++ FSBrowser HOME PAGE

*with show dir and files of FFAT<br/>
*data file print ( html ) ( can be saved as xxx.csv and used by office )<br/>
*Upload and Download<br/>
*favicon.ico upload to FFAT and served<br/>

##v0.2.A++
* show esp_info, FFAT_drive data, last DirList in HOME PAGE

##v0.2.B
*PSRAM has 5 arrays 3600 long ( 1h of 1sec samples )<br/>
*show PSRAM 1min AVG MIN MAX data collection<br/>
