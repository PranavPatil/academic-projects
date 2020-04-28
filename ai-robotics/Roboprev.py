#Program for iRobot create to carry cookies from Professor X's door to Professor Y's door.
#This program requires Create.py Package from. - http://www.cs.hmc.edu/~dodds/erdos/pyCreate.zip
#Python 2.4.4 - http://www.python.org/download/releases/2.4.4/ (www.python.org)
#Pyserial � 2.4 for windows � www.sourceforge.net
#Pywin32 � (win32all package) � www.sourceforge.net

import create
from create import *
import time

class robo:
 def robo():
    return

def stopfbump(r):
    while 1==1 :
        val = getbump(r)
        if val == 3:
            r.go()
            break


    return

def aimain() :
    r = create.Create(10)
    r.toFullMode()
    print 'to full mode'
    r.go(30,0)
    stopfbump(r)
    r.go(0,90) ; time.sleep(1.05) ; r.go() #turn 90 degrees left
    rundist1(r, 800) # Wall to Turn
    rundist2(r , 7750) #Turn to Rsolyn
    r.go(-30,0)
    time.sleep(4)
    r.go(0,-90) ; time.sleep(1.05) ; r.go() #turn 90 degrees right
    r.go(0,-90) ; time.sleep(1.05) ; r.go() #turn 90 degrees right
    r.go(30,0)
    stopfbump(r)
    r.go(0,90) ; time.sleep(1.05) ; r.go() #turn 90 degrees left
    rundist3(r,7700) #Roslyn to Dr Kaner
    r.go(0,90) ; time.sleep(1.08) ; r.go() #turn 90 degrees left
    rundist4(r,19450) #Dr Kaner to Dr Shoaff
    r.playSong( [(55,16),(55,16),(55,16),(51,64)] ) #Play Tone


#function to run create robot till the distance mentioned correcting left and right bump



    r.go(10,0)
    distance = 0

    while (distance < limit) :
            door = 0
            flag = 0
            d = r.sensors()
            distance += d[create.DISTANCE]
            left = d[create.LEFT_BUMP]
            right = d[create.RIGHT_BUMP]
            wals = d[create.WALL_SIGNAL]

            if left == 1 and right == 0:
                r.go(20,-30)
            elif left == 0 and right == 1:
                r.go(20,30)
            elif left == 1 and right == 1:
                r.go(-5,10)
                time.sleep(2)
                r.go(20,5)

            elif left ==0 and right ==0:

                if wals >= 59 :
                    r.go(30,15)
                if wals <= 40 :
                    r.go(30,-15)
                if wals == 0 :
                    r.go(20,0)
                if wals >= 100:
                    print 'greate than 100'
                    flag = 1
                if wals < 100:
                    print 'less than 100'
                    if flag == 1:
                        print 'less than 100 with flag'
                        door += 1
                        flag = 0

            time.sleep(0.015)
    print door
    r.go()
#********************************************************************************************************************************************
    #FIRST RUNDIST FOR 90DEGREE TURN
def rundist1( r , limit):
    r.go(10,0)
    distance = 0
    while (distance < limit) :
            door = 0
            flag = 0
            d = r.sensors()
            distance += d[create.DISTANCE]
            left = d[create.LEFT_BUMP]
            right = d[create.RIGHT_BUMP]
            wals = d[create.WALL_SIGNAL]
            if left == 1 and right == 0: #left bump
                r.go(10,-60)
            elif left == 0 and right == 1: #right bump
                r.go(10,60)
            elif left == 1 and right == 1: #front bump
                r.go(-5,10)
                time.sleep(2)
                r.go(20,5)
            elif left ==0 and right ==0: #no bump
                if wals >= 59 :
                    r.go(30,15)
                if wals <= 40 and wals >0:
                    r.go(30,-15)
                if wals == 0 :
                        if distance >= 500:
                            time.sleep(1)
                            r.go(20,-30)
                            break
                        elif distance <500:
                            r.go(20,0)
            time.sleep(0.015)
    r.go(0,-90) ; time.sleep(1.1) ; r.go() #turn 90 degrees right

#**************************************************************************************************************************************
    # RUNDIST TWO FROM TURN TO ROSYLN
def rundist2( r , limit):
    r.go(10,0)
    distance = 0
    door = 0
    d2 = 0
    flag = 0
    tmp = 0
    work = 0
    while (distance < limit) :
            d = r.sensors()
            distance += d[create.DISTANCE]
            left = d[create.LEFT_BUMP]
            right = d[create.RIGHT_BUMP]
            wals = d[create.WALL_SIGNAL]
            if left == 1 and right == 0:
                r.go(0,-60)
            elif left == 0 and right == 1:
                r.go(0,60)
            elif left == 1 and right == 1:
                r.go(-5,10)
                time.sleep(2)
                r.go(20,5)
            elif left ==0 and right ==0:
                if wals >= 50 :
                    r.go(30,15)
                if wals <= 40 and wals > 0:
                    r.go(30,-15)
                if wals == 0 :
                    r.go(30,-30)
                if wals >= 100:
                    tmp +=1
                    flag = 1
                    if distance >2000 and distance < 3200:
                        d2=1 #Door 1 detected
                    elif distance >6900 and distance <7500:
                        d2=2 #Door 2 detected (Roslyn)
                        r.Go(20,0)
                        time.sleep(2)
                        r.stop()
                        r.go(0,-90) ; time.sleep(1.05) ; r.go() #turn 90 degrees right
                        r.go(30,0)
                        stopfbump(r)
                        r.playSong([(60,8),(64,8),(60,8)])
                        time.sleep(1)
                        work = 1
                        break
                if wals < 100:
                    if flag == 1:
                        door += 1
                        flag = 0
            time.sleep(0.015)
    if work == 0:
        r.go(0,-90) ; time.sleep(1.05) ; r.go() #turn 90 degrees right
        r.go(30,0)
        stopfbump(r)
        time.sleep(1)
        r.playSong([(60,8),(64,8)])
    print 'door'
    print door
    print 'spike'
    print tmp
    print 'door 2'
    print d2
    r.go()

#**************************************************************************************************************************************
    # RUNDIST THREE FROM ROSYLN BACK TO TURN
def rundist3( r , limit):
    r.go(10,0)
    distance = 0
    door = 0
    d2 = 0
    flag = 0
    tmp = 0
    work = 0
    while (distance < limit) :
        d = r.sensors()
        distance += d[create.DISTANCE]
        left = d[create.LEFT_BUMP]
        right = d[create.RIGHT_BUMP]
        wals = d[create.WALL_SIGNAL]
        if left == 1 and right == 0:
            r.go(0,-60)
        elif left == 0 and right == 1:
            r.go(0,60)
        elif left == 1 and right == 1:
            r.go(-5,10)
            time.sleep(2)
            r.go(20,5)
        elif left ==0 and right ==0:
            if wals >= 50 :
                r.go(30,15)
            if wals <= 40 and wals > 0:
                r.go(30,-15)
            if wals == 0 :
                r.go(30,-30)
        time.sleep(0.015)
    r.go()
#**************************************************************************************************************************************
    #RUNDIT FOUR FROM TURN TO SHOAFF
def rundist4( r , limit):
    distance = 0
    r.go(10,0)
    door = 0
    flag = 0
    work = 0
    while (distance < limit) :
            d = r.sensors()
            distance += d[create.DISTANCE]
            left = d[create.LEFT_BUMP]
            right = d[create.RIGHT_BUMP]
            wals = d[create.WALL_SIGNAL]
            print distance
            if distance <= 1350:
                if left == 1 and right == 0:
                    r.go(20,-30)
                elif left == 0 and right == 1:
                    r.go(20,30)
                elif left == 1 and right == 1:
                    r.go(-5,10)
                    time.sleep(2)
                    r.go(20,5)
                elif left ==0 and right ==0:
                    r.go(20,0)
                print 'with in first 1350k'
#---------------------------------------------------------------------------------------------------------------
            if distance > 1350 and distance <= 1600:
                if left == 1 and right == 0:
                    r.go(0,-90)
                elif left == 0 and right == 1:
                    r.go(0,90)
                elif left == 1 and right == 1:
                    r.go(-5,20)
                    time.sleep(2)
                    r.go(20,5)
                elif left ==0 and right ==0:
                    r.go(30,10)
                print 'with in first 1350k-1600k'
#---------------------------------------------------------------------------------------------------------------
            if distance > 1600 and distance <= 5000:
                if left == 1 and right == 0:
                    r.go(0,-90)
                elif left == 0 and right == 1:
                    r.go(0,90)
                elif left == 1 and right == 1:
                    r.go(-5,-20)
                    time.sleep(2)
                    r.go(20,-5)
                elif left ==0 and right ==0:
                    r.go(30,10)
                print 'with in 2nd first 1600k - 5k'
#--------------------------------------------------------------------------------------------------------------
            elif distance > 5000 and distance <= 10000:
                print 'crossed first 5k'
                if left == 1 and right == 0:
                    r.go(0,-60)
                elif left == 0 and right == 1:
                    r.go(0,60)
                elif left == 1 and right == 1:
                    r.go(-5,20)
                    time.sleep(2)
                    r.go(20,5)
                elif left ==0 and right ==0:
                    if wals >= 50 :
                        r.go(30,15)
                    if wals <= 40 and wals > 0:
                        r.go(30,-15)
                    if wals == 0 :
                        r.go(30,-8)
#----------------------------------------------------------------------------------------------------
            elif distance > 10000:
                print 'crossed first 10k'
                if left == 1 and right == 0:
                    r.go(0,-60)
                elif left == 0 and right == 1:
                    r.go(0,60)
                elif left == 1 and right == 1:
                    r.go(-5,10)
                    time.sleep(2)
                    r.go(20,5)
                elif left ==0 and right ==0:
                    if wals >= 50 :
                        r.go(30,15)
                    if wals <= 40 and wals > 0:
                        r.go(30,-15)
                    if wals == 0 :
                        r.go(30,-30)
                    if wals >= 100:
                        if distance >17000:
                            time.sleep(0.5)
                            r.stop()
                            r.playSong([(60,8),(64,8),(60,8),(64,8)])
                            print 'door'
                            work = 1
                            break
            time.sleep(0.015)
    if work == 0:
        print 'complete'
        r.go()
        r.playSong([(60,8),(64,8),(60,8),(64,8)])
    print door
    r.go()
