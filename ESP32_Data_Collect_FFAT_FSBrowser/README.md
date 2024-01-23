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



## v0.2.A+

++ FSBrowser HOME PAGE

*with show dir and files of FFAT<br/>
*data file print ( html ) ( can be saved as xxx.csv and used by office )<br/>
*Upload and Download<br/>
*favicon.ico upload to FFAT and served<br/>

## v0.2.A++
* show esp_info, FFAT_drive data, last DirList in HOME PAGE

## v0.2.B
### 1 sec : Analog In with counter and timestamp to PSRAM Array
*5 arrays 3600 long ( 1h of 1sec samples )<br/>
*show from this a 1min AVG MIN MAX data evaluation<br/>

## v0.3.0
*FFat file list to show ROOT and /data/ at every refresh<br/>
*make a Select Option drop down list for PRINT<br/>
*use that selected to make a Download Link for it<br/>
