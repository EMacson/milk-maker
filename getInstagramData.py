# information for instaloader was obtained from https://www.youtube.com/watch?v=Kr1RyK6WENQ  

import instaloader
import requests
import time

from serial import Serial

mySerial = Serial(port='COM5', baudrate=9600)

# page username
soyUsername = "soy_milk_is_the_best_milk"
oatUsername = "oat_milk_is_the_best_milk"
almondUsername = "almond_milk_is_the_best_milk"
coconutUsername = "coconut_milk_is_the_best_milk"

instagram = instaloader.Instaloader()

# load password for each page from session file
# this isn't fully needed if only running a few times
instagram.load_session_from_file(soyUsername)
instagram.load_session_from_file(oatUsername)
instagram.load_session_from_file(almondUsername)
instagram.load_session_from_file(coconutUsername)

soyProfile = instaloader.Profile.from_username(instagram.context, soyUsername)
oatProfile = instaloader.Profile.from_username(instagram.context, oatUsername)
almondProfile = instaloader.Profile.from_username(instagram.context, almondUsername)
coconutProfile = instaloader.Profile.from_username(instagram.context, coconutUsername)

while True:
    value = mySerial.readline()
    valueInString = str(value, 'UTF-8').strip()
    if valueInString == "1":
        # print follower counts to cmd
        print(f"Soy Followers: {soyProfile.followers}")
        print(f"Oat Followers: {oatProfile.followers}")
        print(f"Almond Followers: {almondProfile.followers}")
        print(f"Coconut Followers: {coconutProfile.followers}")

        # cast follower counts to strings
        soyFollowers = str(soyProfile.followers)
        oatFollowers = str(oatProfile.followers)
        almondFollowers = str(almondProfile.followers)
        coconutFollowers = str(coconutProfile.followers)
        deliminatorS = "S"
        deliminatorO = "O"
        deliminatorA = "A"
        deliminatorC = "C"

        # write follower counts to serial for arduino to read        
        mySerial.write(bytes(soyFollowers,'UTF-8'))
        mySerial.write(bytes(deliminatorS,'UTF-8'))

        mySerial.write(bytes(oatFollowers,'UTF-8'))
        mySerial.write(bytes(deliminatorO,'UTF-8'))

        mySerial.write(bytes(almondFollowers,'UTF-8'))
        mySerial.write(bytes(deliminatorA,'UTF-8'))

        mySerial.write(bytes(coconutFollowers,'UTF-8'))
        mySerial.write(bytes(deliminatorC,'UTF-8'))