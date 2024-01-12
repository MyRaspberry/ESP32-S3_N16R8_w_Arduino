/*

+++ Esp32_IO.setup()


================================
Chip Model: ESP32-S3
Chip Revision: 0
with 2 core
Flash Chip Size : 16777216 
Flash Chip Speed : 80000000 
Total Heap : 396652 
Free Heap  : 371476 
Total PSRAM : 8386279 
Free PSRAM  : 8386019 

Used PSRAM : 260

Features included:
 
 2.4GHz WiFi
 Bluetooth LE
 
 


Connecting to kll-wlan_2.4G
......WiFi connected
IP address: 
192.168.1.11
Contacting Time Server

Now is : 2024-01-07 00:16:09

Open http://esp32fs.local/edit to see the file browser

 FFAT 
Total space:   10240000
Free space:   10240000

//OLD
Listing directory: /
Writing file: /hello.txt
- file written
Appending to file: /hello.txt
- message appended
Reading file: /hello.txt
- read from file:
Hello World!
Renaming file /hello.txt to /foo.txt
- file renamed
Reading file: /foo.txt
- read from file:
Hello World!
Deleting file: /foo.txt
- file deleted
Testing file I/O with /test.txt
- writing................................................................
- 1048576 bytes written in 10244 ms
- reading................................................................
- 1048576 bytes read in 126 ms
Free space:    9191424
Deleting file: /test.txt
- file deleted
Test complete
// end OLD

Connecting to kll-wlan_2.4G
......WiFi connected
IP address: 
192.168.1.11
Contacting Time Server

Now is : 2024-01-07 00:16:09

Open http://esp32fs.local/edit to see the file browser
 FFAT 
Total space:   10240000
Free space:   10227712
Listing directory: /
  DIR : data
Listing directory: /data
  FILE: readings.csv	SIZE: 8128
Free space:   10227712
Reading file: /data/readings.csv
- read from file:
,datetimes,A0,A1,A2,

,2024-01-06 19:23:00,7.37,17.34,0.63,
...

HTTP server started

SETUP  complete
.
___ loop1min 2024-01-07 06:16:10
Appending to file: /data/readings.csv
- append failed
..............
,2024-01-07 06:16:24,95.80,2.47,96.26,

...............
,2024-01-07 06:16:39,48.03,37.29,1.15,

...............
,2024-01-07 06:16:54,31.97,31.97,1.10,

......
___ loop1min 2024-01-07 06:17:00
Appending to file: /data/readings.csv
- message appended
.........


/// setup esp32_array 

Sketch uses 815053 bytes (25%) of program storage space. Maximum is 3145728 bytes.
Global variables use 59704 bytes (18%) of dynamic memory, leaving 267976 bytes for local variables. Maximum is 327680 bytes.

The PSRAM is correctly initialized
after init
___ PSRam : 8386279, Free : 8386019, Used : 260
after setup() arrays 
___ PSRam : 8386199, Free : 8313939, Used : 72260



*/