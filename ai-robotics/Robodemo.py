
import create
from create import *
import time

class robo:
 def robo():   
    return

def level1(r):

 distance = 0
 flag = 0
 minlev1dist = 2200 # initially was 2100, pranav changed it to 2200 for tuning
 maxlev1dist = 3900
 
 while 1==1 :

     d = r.sensors()
     distance += d[create.DISTANCE]
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]

     if left == 1 and right == 0:
        r.go(0,0)
        time.sleep(1.2)
        r.go(20, 0)
        time.sleep(0.5)
        flag = 1
        print 'Lev1: Left bump'
     elif left == 0 and right == 1:
        r.go(0,15)
        time.sleep(1.05)
        r.go(15, 0)
        time.sleep(0.5)
        flag = 1
        print 'Lev1: Right bump'
     elif left == 1 and right == 1 and distance < maxlev1dist and distance > minlev1dist:  # bow bow's code to handle shoe
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

     elif left == 1 and right == 1 and distance < minlev1dist:
        r.go(0,30)
        time.sleep(1.05)
        r.go(20, 0)
        time.sleep(0.5)
        
     elif flag == 1 and left ==0 and right ==0:                
        if wals >= 59 :
           r.go(20,15)
        if wals <= 40 :
           r.go(20,-15)
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
           r.go(0,-20)
           time.sleep(1)
           r.go(20,0)
           
        elif wals == 0 and distance > maxlev1dist :
           print 'Lev1: (distance > maxlev1dist) End '           
           print distance
           r.go(20,0)
           break
            
        if wals < 59 and wals > 40 :
           r.go(20,0)
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


def levelx(r):

 distance = 0
 maxlev1dist = 1000
 flag = 0
 level2corner = 3000
 
 while distance < level2corner :
     
     d = r.sensors()
     distance += d[create.DISTANCE]
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]
     
     print 'level2 entered'
     if left == 1 and right == 0:
          r.go(0,-15)
          time.sleep(1.05)
          r.go(20, 0)
          time.sleep(0.5)
          print 'Lev2: Left bump'
     elif left == 0 and right == 1:
          r.go(0,15)
          time.sleep(1.05)
          r.go(15, 0)
          time.sleep(0.5)
          print 'Lev2: Right bump'
     elif left == 1 and right == 1:
          r.go(0,90)
          time.sleep(1.05)
          r.go(20, 0)
          time.sleep(0.5)
     elif left ==0 and right ==0:
          if wals >= 59 :
             print 'Lev2: No Bump NO Bump   wals >= 59'           
             r.go(20,15)
          if wals <= 40 :
             print 'Lev2: No Bump NO Bump   wals <= 40'           
             r.go(20,-15)
          if wals < 59 and wals > 40 :
             print 'Lev2: No Bump NO Bump   wals < 59 and wals > 40'
             r.go(20,0)
          if wals == 0 and distance > maxlev1dist and distance < 3000 :
             print 'Lev2: distance > maxlev1dist End '           
             print distance
             r.go(10,0)
             time.sleep(0.4)
             r.go(10,-90)
             time.sleep(1)
#             r.go(10,0)
#             time.sleep(0.5)
             r.go(0, 15)
             time.sleep(0.5)
             r.go(20,0)
             flag = 1
          
          elif wals == 0 and distance < maxlev1dist :
             print 'Lev2: distance < maxlev1dist'
             print distance
             r.go(10,-15)
             time.sleep(1)
             r.go(10, 0)
             time.sleep(0.5)
# r.go(0,90)
 
 r.go(0,0)
 time.sleep(1)           
#        r.go(-5,15)
#        time.sleep(2)
#        r.go(0,15)
#        time.sleep(1)
     
 return

def level2(r, deviation):

 distance = 0
 edge1dist = 980 + deviation
 edge2dist = 3300 + edge1dist
 frntbmp = 0
 edge3limitRosalyn = 1050       # need to change this depending on edge1dist and edge2dist
 edge = 0
 
 while (frntbmp < 3 or (frntbmp == 3 and distance < edge3limitRosalyn)) :
     
     d = r.sensors()
     distance += d[create.DISTANCE]
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]
     r.go(20,0);
     
#    if flag == 1 and left == 1 or flag == 1 and right == 1:
#        print "Lev2: Ending Level 2"
#        break
     print 'level2_5entered'
     if left == 1 and right == 0:
          r.go(0,-15)
          time.sleep(1.05)
          r.go(20, 0)
          time.sleep(0.5)
          print 'Lev2_5: Left bump'
          print 'THOOOOOOOOOOOOOOOOOOOOOOOKAA'
     elif left == 0 and right == 1:
          r.go(0,15)
          time.sleep(1.05)
          r.go(15, 0)
          time.sleep(0.5)
          print 'Lev2_5: Right bump'
          print 'THOOOOOOOOOOOOOOOOOOOOOOOKAA'

     elif left == 1 and right == 1 and (distance < edge1dist or (distance > edge1dist and distance < edge2dist)) :  # bow bow's code to handle shoe
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

     elif left == 1 and right == 1 and distance > edge2dist:
          if frntbmp==1:
            r.go(0,95)
          else:
            r.go(0,90)
          time.sleep(1.05)
          r.go(20, 0)
          time.sleep(0.5)
          frntbmp = frntbmp + 1
          print 'THOOOOOOOOOOOOOOOOOOOOOOOKAA'
          
     elif left ==0 and right ==0:
          if wals >= 59 :
             print 'Lev2_5: No Bump NO Bump   wals >= 59'           
             r.go(20,15)
          if wals <= 40 :
             print 'Lev2_5: No Bump NO Bump   wals <= 40'           
             r.go(20,-10)
          if wals < 59 and wals > 40 :
             print 'Lev2_5: No Bump NO Bump   wals < 59 and wals > 40'
             r.go(20,0)
          if wals == 0 :             
             print 'Lev2_5: distance < 3000 -------------------------------------------------------------> '           
             print distance
             r.go(10,0)
             if edge==0:         # change in bow's bow's perfect cut for 1st edge
               time.sleep(1.7)
             else:
               time.sleep(1.5)
             r.go(0,-90)
             time.sleep(0.5)
             r.go(10,0)
             time.sleep(0.1)
             r.go(0, -10)
             time.sleep(0.5)
             print 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
             r.go(20,0)
             flag = 1
             edge = edge + 1
             if frntbmp==2:
               distance = 0
               frntbmp += 1

     print 'frntbmp = %d' % frntbmp
     print 'edge = %d' % edge
          
 r.go(0,95)
 time.sleep(1.02)
 print 'distance = %d' % distance
 r.go(0,0)
 time.sleep(1)           
#        r.go(-5,15)
#        time.sleep(2)
#        r.go(0,15)
#        time.sleep(1)
     
 return

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

def level3(r):

   print 'entered level 3'
   distance = 0
   wallsmooth = 0     
     
   while 1==1 :
     
     d = r.sensors()
     distance += d[create.DISTANCE]
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]

     print 'level3 entered'
     
     r.go(20,0)
     print 'distance = %d ' % distance
     print 'wallsmooth = %d ' % wallsmooth
     
     if (left == 1 or right == 1) and (distance < 2800 or wallsmooth < 6):
       if left == 1 and right == 0:
            r.go(0,-35)
            time.sleep(1.05)
            r.go(20, 0)
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
            r.go(0,25)
            time.sleep(1.05)
            r.go(15,0)
            time.sleep(0.5)

     elif (left == 1 or right == 1) and distance > 2800 and wallsmooth > 5 and distance < 5700:
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
        time.sleep(1.5)
        r.go(0,90)
        time.sleep(1.05)
        r.go(20,0)
            
     elif left == 1 and right == 1 and distance > 5700:
          print distance
          r.go(-10, 0)
          time.sleep(0.5)
          r.go(0,90)
          time.sleep(1)
          break     
     elif left ==0 and right == 0:
        if wals >= 59 :
          print 'Lev3: No Bump NO Bump   wals >= 59'           
          r.go(20,15)
        if wals <= 40 :
          print 'Lev3: No Bump NO Bump   wals <= 40'           
          r.go(20,-15)
        if wals < 59 and wals > 40 :
          print 'Lev3: No Bump NO Bump   wals < 59 and wals > 40'
          r.go(20,0)
          wallsmooth += 1
        elif wals == 0 :
          print 'Lev3:'
          print distance
          r.go(10,-25)
          time.sleep(1)
          r.go(10, 0)
          time.sleep(0.5)
            
#        r.go(-5,15)
#        time.sleep(2)
#        r.go(0,15)
#        time.sleep(1)
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
            time.sleep(1.3)
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

def level4(r):

 distance = 0
 maxlev1dist = 1000
 flag = 0
 edge = 0
 edgecount = 0
 headon_count = 0
 level2limitRosalyn = 7500

 # temp settings
# edge = 0
# headon_count = 2
# distance = 3000
 
 while distance < level2limitRosalyn :
     
     d = r.sensors()
     distance += d[create.DISTANCE]
     left = d[create.LEFT_BUMP]
     right = d[create.RIGHT_BUMP]
     wals = d[create.WALL_SIGNAL]
     r.go(20,0)
     print 'level4 entered'
     print 'distance = %d ' % distance
     print 'edgecount = %d ' % edgecount
     
     if left == 1 and right == 0:
          r.go(0,-15)
          time.sleep(1.05)
          r.go(10, 0)
          time.sleep(0.5)
          print 'Lev4: Left bump'
     elif (left == 0 or left == 1) and right == 1 and edge == 2 and headon_count == 2:
          print ' right bump edge 1'
          print 'RIGHT BUMP !!!!!!!!!!! EDGE 11111111111111111111111111111111111111111111'
          edge  = 0     # was commented before 
          headon_count = 3
          r.go(0,35)
          time.sleep(1.3)
          r.go(15, 0)
          time.sleep(0.5)
          r.go(0,-35)
          time.sleep(1.2)
          r.go(20, 0)
          time.sleep(0.5)
          
     elif (left  == 0 or left  == 1) and right == 1 and edge == 1 and headon_count == 2:
          print 'RIGHT BUMP !!!!!!!!!!! EDGE 000000000000000'
          edge = 2
          r.go(0,35)
          time.sleep(1.2)
          r.go(15,0)
          time.sleep(0.7)
#          r.go(0,-15)        # lean this little away from wall so that there is never center bump (always right)
#          time.sleep(1)

     elif left  == 1 and right == 1 and edge == 0 and headon_count == 2:
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
    
     elif left == 1 and headon_count == 4 or right == 1 and headon_count == 4:   # By accident if women's bathroom corner didn't center bump, then after 90turn bump, exe this
          print 'RIGHT BUMP !!!!!!!!!!! HEADCOUNT = 3'
          r.go(0,25)
          time.sleep(1.05)
          r.go(15,0)
          time.sleep(0.5)

     elif left == 1 and right == 1 and headon_count == 0 and edgecount > 3:
           print 'CENTER BUMP !!!!!!!!!!! HEADCOUNT 0'
           headon_count = 2
           r.go(0,90)
           time.sleep(1.05)
           r.go(10,0)
           time.sleep(1)

     elif (left == 1 and right == 1 and edgecount < 4) or (left == 0 and right == 1) and headon_count == 0:
           r.go(0,15)
           time.sleep(1)
           r.go(10,0)
           time.sleep(0.5)

     elif left == 1 and right == 1 and headon_count == 3 and edgecount > 4:
           print 'CENTER BUMP !!!!!!!!!!! HEADCOUNT 2'
           print 'WHY ARE YOU NOT RUNNING !!!!!!!!!!!!!!!!!!!'
           headon_count = 4
           r.playSong( [(55,16),(55,16),(55,16),(51,64)] ) #Play Tone
           time.sleep(0.5)
#           headon_count = 3;
           r.go(0,90)
           time.sleep(1.15)
           r.go(0,0)
           time.sleep(1)
           r.go(15,0)
           time.sleep(1.5)
           
     elif (left == 1 and right == 1 and edgecount < 5) or (left == 0 and right == 1) and headon_count == 3:
           r.go(0,15)
           time.sleep(1)
           r.go(10,0)
           time.sleep(0.5)

     elif left ==0 and right ==0 and distance < 3000:
          if wals >= 59 :
             print 'Lev4: No Bump NO Bump   wals >= 59'           
             r.go(20,15)
          if wals <= 40 :
             print 'Lev4: No Bump NO Bump   wals <= 40'           
             r.go(20,-15)
          if wals < 59 and wals > 40 :
             print 'Lev4: No Bump NO Bump   wals < 59 and wals > 40'
             r.go(20,0)
          if wals == 0:
             print 'Lev4: distance < 3000 '           
             print distance
             r.go(10,0)
             time.sleep(1.5)
             r.go(0,-90)
             time.sleep(0.5)
             r.go(0, 10)
             time.sleep(0.5)
             r.go(20,0)
             flag = 1
             edgecount += 1
             
     elif left == 0 and right == 0 and distance > 3000 and distance < 7500 and headon_count == 4 :
           print 'HEADCOUNT IS 33333333333333333'
           r.go(0,-90)
           time.sleep(1.05)
#           r.go(20,0)
#           time.sleep(1.2)
#           r.go(0,0)
           r.playSong( [(55,16),(55,16),(55,16),(51,64)] ) #Play Tone
           edge = 0
           break
              
     elif left == 0 and right == 0 and distance > 3000 and distance < 7500 and headon_count != 4 :
              
             if wals >= 59 :
               print 'Lev4: No Bump NO Bump   wals >= 59'           
               r.go(20,15)
             if wals <= 40 :
               print 'Lev4: No Bump NO Bump   wals <= 40'           
               r.go(20,-15)
             if wals < 59 and wals > 40 :
               print 'Lev4: No Bump NO Bump   wals < 59 and wals > 40'
               r.go(20,0)
             if wals == 0 and edge == 0:
               print " level 4 ist dooredge"
               print 'Lev4: distance > 3000 and < 7500 '           
               print distance
               edge = 1       # previously was commented LDay
               r.go(10,0)
               time.sleep(1.5)
               r.go(0,-60)
               time.sleep(0.5)
               r.go(20,0)
               time.sleep(1.5)
               r.go(0,0)
               time.sleep(2)
               edgecount += 1
          
 print 'ENDING WHILE LOOP .......................'
 r.go(20,0)
 time.sleep(2)
 print 'LEVEL 4 ENDING .......................'
 r.go(0,0)
 time.sleep(1)           
#        r.go(-5,15)
#        time.sleep(2)
#        r.go(0,15)
#        time.sleep(1)
 return


def level5(r):

 distance = 0
 maxlev1dist = 1000
 flag = 0
 edge = 0
 turn = 0      # total 2 turns
 levellimitStansifer = 14000
 headbump = 0  # total 2 bumps
 door = 0      # total 4 + 1/2 stansifer
 initialAngletakn = 0

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
     if left == 1 and right == 0 and headbump == 0 and initialAngletakn== 1:
          r.go(0,25)   # changed from -15 to 25
          time.sleep(1.05)
          r.go(10, 0)
          time.sleep(0.5)
          print 'Lev5: Left bump'
     elif left == 1 and right == 1 and headbump == 0 and initialAngletakn== 1:   # initial wall
          headbump = 1
          distance = 0
          r.go(0,90)
          time.sleep(1.05)
          r.go(15, 0)
          time.sleep(0.5)
          print 'Lev5: CENTER BUMP'

     elif left == 1 and right == 1 and headbump == 0 and initialAngletakn== 0:   # initial wall
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
             time.sleep(1)
             r.go(0,15)
             time.sleep(0.5)
             r.go(20,0)
             time.sleep(1)
             flag = 1
          if wals == 0 and turn == 1 and edge == 0 and headbump == 0:  # between first turn until first headbump
             print 'WALLLLLLLLLLL 0'
             initialAngletakn = 1
             r.go(10,0)
             time.sleep(1.5)
             r.go(0,-10)
             time.sleep(0.5)
             r.go(20,0)
             time.sleep(1)
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

# #############################################################################################################################################
 
def aimain() :
    r = create.Create(4)
    r.toFullMode()
    deviation = 0
    print 'to full mode'
    r.go(0,-90)
    time.sleep(1.05)
    r.go(20,0)
    level1(r)
    deviation = transit1(r)
    level2(r, deviation)
    transit2(r)
    level3(r)
    transit3(r)
    level4(r)
    level5(r)
    r.stop()
    return

aimain()
