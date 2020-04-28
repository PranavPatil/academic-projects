
import create
from create import *
import time

class robo:
 def robo():   
    return

def level1(r):

 distance = 0
 flag = 0
 minlev1dist = 2600 # initially was 2100, pranav changed it to 2200 for tuning
 maxlev1dist = 3900
 record = 0
 
 while 1==1 :

     d = r.sensors()
     distance += d[create.DISTANCE]
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]

#     if distance < maxlev1dist:
#        print 'distance < maxlev1dist'
#     elif distance > maxlev1dist:
#        print 'distance > maxlev1dist'

     if (left == 1 or right == 1) and distance < maxlev1dist and distance > minlev1dist:
       distance -= 25

     if left == 1 and right == 0 and distance > minlev1dist:
        r.go(0,35)   # changed from -35 to 35
        time.sleep(1.05)
        r.go(15, 0)
        time.sleep(0.5)
        print 'Lev3: Left bump'
        record += 35
     elif left == 0 and right == 1 and distance > minlev1dist:
        print 'distance..............'
        r.go(0,25)
        time.sleep(1.05)
        r.go(15, 0)
        time.sleep(0.5)
        print 'Lev3: Right bump'
        record += 25
     elif left == 1 and right == 1 and distance > maxlev1dist:
        r.go(0,35)
        time.sleep(1.05)
        r.go(15,0)
        time.sleep(0.5)
        record += 35
     elif left == 1 and right == 1 and distance > minlev1dist and distance < maxlev1dist:
        distance = distance - 685
        r.go(-10, 0)
        time.sleep(0.5)
        r.go(0,90)
        time.sleep(1.05)
        r.go(30,0)
        time.sleep(1.2)
        r.go(0,-90)
        time.sleep(1.05)
        r.go(30,0)
        time.sleep(1.8)
        r.go(0,0)
        time.sleep(0.5)
        r.go(0,-90)
        time.sleep(1.05)
        r.go(30,0)
        time.sleep(1.26)
        r.go(0,90)
        time.sleep(1.05)
        r.go(20,0)

     elif left == 1 and right == 0 and distance < minlev1dist:
        r.go(0,0)
        time.sleep(1.2)
        r.go(10, 0)
        time.sleep(0.5)
        flag = 1
        print 'Lev1: Left bump'
     elif left == 0 and right == 1 and distance < minlev1dist:
        r.go(0,20)
        time.sleep(1.05)
        r.go(10, 0)
        time.sleep(0.5)
        flag = 1
        print 'Lev1: Right bump'
     elif left == 1 and right == 1 and distance < minlev1dist:
        r.go(0,30)
        time.sleep(1.05)
        r.go(10, 0)
        time.sleep(0.5)
        
     elif flag == 1 and left ==0 and right ==0:
        if distance < maxlev1dist:
           record = 0
        if wals >= 59 :
           r.go(20,15)
        if wals <= 40 :
           r.go(20,-15)
        if wals < 59 and wals > 40 :
           r.go(20,0)
        if wals == 0 and distance < minlev1dist :
           print 'Lev1: distance < minlev1dist'
           print distance
           r.go(10,-27)
           time.sleep(1)
           r.go(10, 0)
           time.sleep(0.5)
        elif wals == 0 and distance > minlev1dist and distance < maxlev1dist :
           print 'Lev1: distance > minlev1dist and distance < maxlev1dist'
           print distance
           r.go(10,-35)
           time.sleep(1)
           r.go(10, 0)
           time.sleep(0.5)
           print distance
           distance = distance - 20
        elif wals == 0 and distance > maxlev1dist :
           print 'Lev1: (distance > maxlev1dist) End '
           r.go(20,0)
           time.sleep(2)
           r.go(0, -record)
           time.sleep(1)
           record = 0
           print distance
           r.go(20,0)
           break
            
#           r.go(5,15)
#           time.sleep(2)
#           r.go(0,15)
#           time.sleep(1)
     
 return

def transit1(r):

 distance = 0
 rtbumpflag = 0
 maxdist = 1400 # value tested.
 deviation = 0
 
 while 1==1 :

     d = r.sensors()
     distance += d[create.DISTANCE]
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]
     print 'Trans1_to_2: distance = '; print distance

     if distance < maxdist:
       if left == 1 and rtbumpflag==0 or right == 1 and rtbumpflag==0: # bow bow's code to handle shoe
          r.go(-10, 0)    # -45 distance tested
          time.sleep(0.9)
          r.go(0,-90)
          time.sleep(1.05)
          r.go(20,0)    # 505 (507max) distance tested
          time.sleep(2.6)
          r.go(0,90)
          time.sleep(1.05)
          r.go(10,0)  # 45 distance tested
          time.sleep(0.9)
          rtbumpflag=1
          deviation = deviation + 505
          distance = distance - 437   # was 487 tested
       elif left == 1 and rtbumpflag==1 or right == 1 and rtbumpflag==1: # bow bow's code to handle shoe
          r.go(-10, 0)    # -45 distance tested
          time.sleep(0.9)
          r.go(0,90)
          time.sleep(1.05)
          r.go(20,0)    # 505 (507max) distance tested
          time.sleep(2.6)
          r.go(0,-90)
          time.sleep(1.05)
          r.go(10,0)  # 45 distance tested
          time.sleep(0.9)
          rtbumpflag=0
          deviation = deviation - 505
          distance = distance - 437    # was 487 tested
     else:
       print 'ALERT ALERT ALERT ALERT ALERT ALERT ALERT WALLLLLLLLLLLLLLLLLL'
       if left == 1 and right == 0:
          r.go(0,120)
          print 'turned 120 on left bump-------------------'
          time.sleep(1)
          r.go(0,0)
          print ' sleeping 4 sleeping 4--------------------'
          time.sleep(4)
          print 'Trans1: Wall Reached by left bump'
          break
        
       #elif left == 0 and right == 1:
           
       elif left == 1 or right == 1:
          r.go(-10,0)
          time.sleep(0.4)
          if rtbumpflag==1:
            r.go(0,95)
          else:
            r.go(0,90)
          time.sleep(1.2)
          r.go(0,0)
          time.sleep(2)
          #turn 90 degrees left
          print 'Trans1: Wall Reached by centre bump sleeping for 4 secs'
          break
       else:
          r.go(20, 0)
          time.sleep(0.5)        
            
 return deviation

def transit2(r):

  print 'entered transit 2_3'
  distance = 0
  rtbumpflag = 0
  r.go(30, 0)
  deviation = 0
    
  while 1 == 1 :
      d = r.sensors()
      distance += d[create.DISTANCE]
      left = d[create.LEFT_BUMP]
      right = d[create.RIGHT_BUMP]
      wals = d[create.WALL_SIGNAL]
      print 'distance = %d' % distance
       
      if distance < 1219 :  #Rosayln door 1240 tested values 1237, 1244, 1277 etc  # 1228,1228, 1219
#        print distance
#        r.go(0,90)
#        time.sleep(1.05)
        if (left == 1 or right == 1) and rtbumpflag==0: # bow bow's code to handle shoe
           r.go(-10, 0)    # -45 distance tested
           time.sleep(0.9)
           r.go(0,-90)
           time.sleep(1.05)
           r.go(20,0)    # 290 (288max) distance tested
           time.sleep(1.5)
           r.go(0,90)
           time.sleep(1.05)
           r.go(10,0)  # 45 distance tested
           time.sleep(0.9)
           rtbumpflag=1
           deviation = deviation + 288
           print 'distance = %d' % distance
           distance = distance - 278   # was 487 tested
        elif (left == 1 or right == 1) and rtbumpflag==1: # bow bow's code to handle shoe
           r.go(-10, 0)    # -45 distance tested
           time.sleep(0.9)
           r.go(0,90)
           time.sleep(1.05)
           r.go(20,0)    # 290 (288max) distance tested
           time.sleep(1.5)
           r.go(0,-90)
           time.sleep(1.05)
           r.go(10,0)  # 45 distance tested
           time.sleep(0.9)
           rtbumpflag=0
           deviation = deviation - 288
           print 'distance = %d' % distance
           distance = distance - 278    # was 278 tested
        else:
           r.go(30, 0)

      elif distance > 1219:
           if left == 1 or right == 1:    #Rosayln door 1240 tested values 1237, 1244, 1277 etc
             print 'rosalyn door detected level 3 reached'
             print distance
#            r.go(-10,0)
#            time.sleep(1)
             if rtbumpflag==1:
               r.go(0,100)
             else:
               r.go(0,90)
             time.sleep(1.05)
             r.go(0,0)
             time.sleep(2)
             break
           else:
             r.go(20, 0)
             time.sleep(0.5)        
  return


def alignwall(r):

   while 1==1 :
     r.go(10, 0)
     time.sleep(0.2)
     d = r.sensors()
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]

     if left == 1 and right == 0:
            r.go(0,35)
            time.sleep(1.05)
            r.go(10, 0)
            time.sleep(0.2)
     elif left == 0 and right == 1:
            r.go(0,25)
            time.sleep(1.05)
            r.go(10, 0)
            time.sleep(0.2)
     elif left == 1 and right == 1:
            r.go(0,35)
            time.sleep(1.05)
            r.go(10,0)
            time.sleep(0.2)
     elif left == 0 and right == 0:
        if wals >= 59 :
          break
        if wals <= 40 :
          break
        if wals < 59 and wals > 40 :
          break
        elif wals == 0 :
          r.go(0,-25)
          time.sleep(1)
          r.go(10, 0)
          time.sleep(0.2)

   return

def level3(r, finaldist, levelno):   # here the final distance is 5700

   distance = 0
   prevdistance = 0
   print 'levelno = %d' % levelno

   while 1==1 :

     d = r.sensors()
     distance += d[create.DISTANCE]
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]
     prevdistance = distance

     r.go(20,0)
     print 'distance = %d ' % distance
     
     if (left == 1 or right == 1) and distance < finaldist:
       distance -= 5

       if left == 1 and right == 0:
            r.go(0,35)   # changed from -35 to 35
            time.sleep(1.05)
            r.go(15, 0)
            time.sleep(0.5)
            print 'Lev3: Left bump'
       elif left == 0 and right == 1:
            print 'distance..............'
            r.go(0,25)
            time.sleep(1.05)
            r.go(15, 0)
            time.sleep(0.5)
            print 'Lev3: Right bump'
       elif left == 1 and right == 1:
            r.go(0,35)
            time.sleep(1.05)
            r.go(15,0)
            time.sleep(0.5)
     elif left == 0 and right == 0 and levelno == 2 and distance > finaldist:
          print distance
          r.go(-10, 0)
          time.sleep(0.5)
          r.go(0,90)
          time.sleep(1)
          break     
     elif (left == 1 or right == 1) and levelno != 2 and distance > finaldist:
          print distance
          r.go(-10, 0)
          time.sleep(0.5)
          r.go(0,90)
          time.sleep(1)
          break     
     elif (left == 1 or right == 1) and levelno == 2 and distance > finaldist:  # align to wall
          print distance
          alignwall(r)
          r.go(-10, 0)
          time.sleep(0.5)
          r.go(0,90)
          time.sleep(1)
          break     
     elif left == 0 and right == 0:
        if wals >= 59 :
          print 'Lev3: No Bump NO Bump   wals >= 59'           
          r.go(20,15)
        if wals <= 40 :
          print 'Lev3: No Bump NO Bump   wals <= 40'           
          r.go(20,-15)
        if wals < 59 and wals > 40 :
          print 'Lev3: No Bump NO Bump   wals < 59 and wals > 40'
          r.go(20,0)
        elif wals == 0 :
          print 'Lev3:'
          print distance
          r.go(10,-35)
          time.sleep(1)
          r.go(10, 0)
          time.sleep(0.5)
          print distance
          distance = distance - 20

        if prevdistance == distance:  # This is not a useful code but sometimes the robot stops in between
		                      # so I gave a little sleep time for it to move forward and keep doin that.
          r.go(20,0)
          time.sleep(0.5)            
            
   return

def transit3(r):

  print 'entered transit3'
  distance = 0
  dist = 0
  wallzero = 0
  angletkn = 0
  rtbumpflag = 0
  deviation = 0
  benchbmp = 0
  maxdist = 1850
  r.go(20, 0)
    
  while 1 == 1 :
      d = r.sensors()
      distance += d[create.DISTANCE]
#      dist += d[create.DISTANCE]
      left = d[create.LEFT_BUMP]
      right = d[create.RIGHT_BUMP]
      wals = d[create.WALL_SIGNAL]
      print 'distance = %d ' % distance
#      print 'dist = %d ' % dist
      print 'wallzero = %d ' % wallzero

      if rtbumpflag == 1:
        maxdist = 2770
      else:
        maxdist = 1850
               
      if distance > maxdist and angletkn==1:  # robot reaches the wall (1850 if no bumps given)
        if left == 1 and right == 1:
            print 'transit 3 covered wall hit'
            r.go(0, 90)
            time.sleep(1.05)
            r.go(0,0)
            time.sleep(2)
            break 
        elif left == 1 and right == 0:
            r.go(0,110)
            time.sleep(1.2)
            r.go(0,0)
            time.sleep(2)
            print 'Transit3: Left bump wall hit'
            break
        
      elif (distance < 640 and wallzero < 11) or (distance > 640 and wallzero < 11):   # robot going beside the bench
        if left == 1 and right == 0: 
            r.go(0,-20)
            time.sleep(1.05)
            r.go(20, 0)
            time.sleep(0.5)
            distance -= 80     # distacne travelled 95 value tested
            benchbmp += 1
            print 'Transit3: Left bump'            
        elif (left == 0 or left == 1) and right == 1:
            r.go(0,20)
            time.sleep(1.05)
            r.go(20, 0)
            time.sleep(0.5)
            distance -= 80
            benchbmp += 1
            print 'Transit3: Right bump'     
        elif left == 0 and right == 0:
            if wals >= 59:
              r.go(20,15)
            if wals <= 40:
              r.go(20,-15)
            if wals < 59 and wals > 40:
              r.go(20,0)
            if wals==0:
              r.go(10, 0)
              time.sleep(0.5)
              wallzero += 1
              
      elif distance > 639 and wallzero > 10:
        if left == 0 and right == 0 and wals == 0:
            if angletkn==0:
              if benchbmp == 0:
                r.go(0,-40)
              else:
                r.go(0,-45)
              time.sleep(1)
              r.go(20,0)
              angletkn = 1
#              dist = 0     dist var used to reset dist after anglular turn to calc dist after that
            elif angletkn==1:
              r.go(20,0)
        elif left == 1 and rtbumpflag==0 or right == 1 and rtbumpflag==0: # bow bow's code to handle shoe
            r.go(-10, 0)    # -45 distance tested
            time.sleep(0.9)
            r.go(0,-90)
            time.sleep(1.05)
            r.go(20,0)    # 505 (507max) distance tested
            time.sleep(2.6)
            r.go(0,90)
            time.sleep(1.05)
            r.go(10,0)  # 45 distance tested
            time.sleep(0.9)
            rtbumpflag=1
            deviation = deviation + 505
            distance = distance - 437   # was 487 tested
        elif left == 1 and rtbumpflag==1 or right == 1 and rtbumpflag==1: # bow bow's code to handle shoe
            r.go(-10, 0)    # -45 distance tested
            time.sleep(0.9)
            r.go(0,90)
            time.sleep(1.05)
            r.go(20,0)    # 505 (507max) distance tested
            time.sleep(2.6)
            r.go(0,-90)
            time.sleep(1.05)
            r.go(10,0)  # 45 distance tested
            time.sleep(0.9)
            rtbumpflag=0
            deviation = deviation - 505
            distance = distance - 437    # was 487 tested          
           
#   elif distance  < 2000:          
#   code to handle the bumps during transit3 here       -------------------------HANDLE IT PODA  
                    
  return

def level5(r):

 distance = 0
 maxlev1dist = 1000
 flag = 0
 edge = 0
 turn = 0      # total 2 turns
 levellimitStansifer = 13510
 headbump = 0  # total 2 bumps
 door = 0      # total 4 + 1/2 stansifer

 while distance < levellimitStansifer :
     
     d = r.sensors()
     distance += d[create.DISTANCE]
     print 'Distance: '
     print distance
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]
#     r.go(20,0);
     
     print 'level5entered'
     if left == 1 and right == 0:
          r.go(0,25)   # changed from -15 to 25
          time.sleep(1.05)
          r.go(10, 0)
          time.sleep(0.5)
          print 'Lev5: Left bump'
     elif left == 1 and right == 1 and headbump == 0:   # initial wall
          headbump = 1
          distance = 0
          r.go(0,90)
          time.sleep(1.05)
          r.go(15, 0)
          time.sleep(0.5)
          print 'Lev5: CENTER BUMP'

     elif left  == 0 and right == 1 and headbump == 1:
          r.go(0,25)
          time.sleep(1.05)
          r.go(10, 0)
          time.sleep(0.5)
          print 'Lev3: Right bump'
    
     elif left  == 1 and right == 1  and headbump == 1:
          r.go(0,35)
          time.sleep(1.05)
          r.go(10,0)
          time.sleep(0.5)

     elif left ==0 and right ==0:
          if wals >= 59 :
             print 'Lev5: No Bump NO Bump   wals >= 59'           
             r.go(20,15)
          if wals <= 40 :
             print 'Lev5: No Bump NO Bump   wals <= 40'           
             r.go(20,-15)
          if wals < 59 and wals > 40 :
             print 'Lev5: No Bump NO Bump   wals < 59 and wals > 40'
             r.go(20,0)
          if wals == 0 and turn == 0:             # first 45 degree turn
             print 'Lev5: distance < 3000 '           
             print distance
             turn = 1
             r.go(10,0)
             time.sleep(1.5)
             r.go(0,-90)
             time.sleep(0.5)
             print 'XXXXXXXXXXXXXXXXXXXX level5 wall = 0 covered XXXXXXXXXXXXXXXXXXXXXXXX'
             r.go(20,0)
             time.sleep(3)
             flag = 1
          if wals == 0 and turn == 1 and edge == 0 and headbump == 0:  # between first turn until first headbump
             print 'WALLLLLLLLLLL 0' 
             r.go(10,0)
             time.sleep(1.5)
             r.go(0,10)
             time.sleep(0.5)
             r.go(20,0)
             time.sleep(2)
          if wals == 0 and turn == 1 and edge == 0 and headbump == 1:  # start running through the door
             r.go(10,-25)
             time.sleep(1)
             r.go(10, 0)
             time.sleep(0.5)

 r.go(0,90)
 time.sleep(1.02)
 r.go(0,0)
 time.sleep(1)           
     
 return
 
def aimain() :
    r = create.Create(4)
    r.toFullMode()
    deviation = 0
    print 'to full mode'
#    r.go(0,-90)
#    time.sleep(1.05)
#    r.go(20,0)
#    level1(r)
#    deviation = transit1(r)
    deviation = 10150 + deviation   # Total calculated distance for level 2 is 980 + 3300 + 1050 + 20 extra  (5350)
    level3(r, deviation, 2)       # Actual level 2
    transit2(r)
    level3(r, 8200, 3)            # Actual level 3  dist = 5700   prev = 6700
    transit3(r)
    level3(r, 5500, 4)            # Actual level 4                prev = 7500
    level5(r)
    r.stop()
    return

aimain()
