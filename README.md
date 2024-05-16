# Implementasi Sistem IOT untuk Peringatan Dini Kebocoran Gas dengan Sensor, Alarm dan Baterai

Kelompok A1 :
- 2109106024 - Ananta Yusra P. A.
- 2109106026 - Bayu Setiawan
- 2109106011 - Ilham Ramadhan

## Deskripsi :
1. Terdapat platform shiftr.io untuk memantau komunikasi anta node
2. DHT11 pada node 1 digunakan untuk memonitoring suhu dengan ambang batas tertentu jika melebihi ambang batas maka akan menyalakan riley pada node ke 2 dan akan mengirimkan pesan peringatan ke telegram
3. MQ2 pada node 1 digunakan untuk memonitoring gas dengan ambang batas PPM tertentu jika melebihi ambang batas maka akan menyalakan riley pada node ke 2 dan akan mengirimkan pesan peringatan ke telegram
4. pada platform telegram digunakan untuk menerima pesan dan melakukan on off secara manual pada riley di node ke 2
5. pada platform MQTTX di gunakan untuk membuat topic sebagai subscriber, topic ini yang akan di gunakan pada pemrograman nantinya dengan tipe QOS 0  dan pesan plaintext.

## Pembagian Tugas :
- Ananta = Pembuatan rangkaian dan menyiapkan platform shiftr dan MQTTX
- Bayu = Pemrograman dan Bot telegram
- Ilham = Schemantic dan laporan

## Komponen Yang Digunakan :
1.	ESP8266 x 2
2.	Kabel Jumper Male - Male x 21
3.	Kabel Jumper Male - Female x 2
4.	Sensor Gas MQ2 x 1
5.	Sensor suhu DHT11 x 1
6.	Layar Monitor Led x 1
7.	L2C x 1
8.	Indikator Baterai x 1
9.	Relay x 1
10.	Baterai Holder  x 1
11.	BreadBoard x 2 

## Board Schematic
Schematic :
![image](https://github.com/anantaYSR/pa-praktikum-iot-unmul-a1/assets/93465182/fe6b2a5c-c409-415f-b1e2-fbdf5714486d)
![image](https://github.com/anantaYSR/pa-praktikum-iot-unmul-a1/assets/93465182/6c890983-1c4b-4cb8-b42f-db8880112d56)



## Link Video : 
