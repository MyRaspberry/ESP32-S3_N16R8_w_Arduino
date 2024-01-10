# ESP32_Data_Collect
## board: ESP32-S3 F16N8
incl:<br/>
## use FFAT ( on 10MB partition )
## use FSBrowser use FFAT via webserver 
### ESP32-S3 Class lib for RGB_LED pin 48 after soldering

### 1 sec : ".", rainbow.run(), wifi.run()

### 15 sec : Analog In with counter and timestamp

### 1 min : save / append to FFAT /data/readings.csv

<CODE>,rec,datetimes,A0,A1,A2,
,191,2024-01-07 16:09:29,58.46,30.28,1.00,</CODE>



<H2>v0.2.A+</H2>

++ FSBrowser HOME PAGE

* with show dir and files of FFAT<br/>
* data file print ( html ) ( can be saved as xxx.csv and used by office )<br/>
* Upload and Download<br/>
* favicon.ico upload to FFAT and served<br/>

[root show](http://kll.byethost7.com/kllfusion01/downloads/ESP32_FSBrowser_App.png)
[data dir ](http://kll.byethost7.com/kllfusion01/downloads/ESP32_FSBrowser_App_dataDir.png)
